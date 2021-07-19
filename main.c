/**************************************************************************
Copyright (C) 2021 SkibbleBip

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/
/***************************************************************************
* File:  main.c
* Author:  SkibbleBip
* Procedures:
* cmdHandle     -Handles the command that was parsed
* handleError   -Translates the error received into human readable content
* main          -The main function
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pocketsphinx.h>
//#include <pulse/pulseaudio.h>
#include <ad.h>
#include "main.h"
#include "Recording.h"
#include "Speech.h"
#include "Commands.h"

#include "Parse.h"

/*important notes: if defining MODELDIR in codeblocks preprocessor, put 2 '\'
*escapes instead of 1 for quotes because codeblocks is friggin stupid and likes
*to waste my time
*/

void cmdHandle(Cmd_response_t* cmd, cst_voice* s);
char* handleError(char* reply, int err);


/***************************************************************************
* int main(void)
* Author: SkibbleBip
* Date: 07/17/2021
* Description: The main function
*
* Parameters:
*        main   O/P     int     exit value of the function
**************************************************************************/
int main(void)
{

#ifdef SILENT
        err_set_logfp(NULL);
        /*if defined by the preprocessor, this will silence the output of
        *CMU-Sphinx
        */
#endif // SILENT

        ps_decoder_t *recognizer_ps     = NULL;
        ps_decoder_t *trigger_ps        = NULL;
        cmd_ln_t *config_en_us          = NULL;
        /*Decoder and config of PocketSphinx*/
        cmd_ln_t *triggerConfig         = NULL;
        /*config of the trigger*/

        ad_rec_t *record_stream         = NULL;
        /*audio device for recording*/
        ad_rec_t *trigger_stream        = NULL;
        /*audio device for triggering*/
        cst_voice* speaker              = NULL;
        /*The text to speech voice*/

        int8_t active_flag              = 0;
        /*flag for determining if the act of speaking a command is active*/

        fclose(stdin);
        /*close stdin*/


        config_en_us = cmd_ln_init(NULL, ps_args(), TRUE,
		         "-hmm", MODELDIR "/en-us/en-us",
	                 "-lm", MODELDIR "/en-us/en-us.lm.bin",
	                 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
	                 NULL);
        /*Generate the EN-US model*/

        triggerConfig = cmd_ln_init(NULL, ps_args(), TRUE,
		         "-hmm", MODELDIR "/en-us/en-us",
	                 "-lm", "TriggerModel/4696.lm",
	                 "-dict", "TriggerModel/4696.dic",
	                 NULL);
        /*Generate the trigger config*/

        if(config_en_us == NULL)
                failedShutdown("Failed to create English-US config");
        if(triggerConfig == NULL)
                failedShutdown("Failed to create Trigger config");


        if( (trigger_stream = setupRecordingStream(triggerConfig)) == NULL ){
                failedShutdown("Failed to start up the trigger stream");
        }

        if( (record_stream = setupRecordingStream(config_en_us)) == NULL){
                failedShutdown("Failed to start up audio recording stream");
        }


        recognizer_ps   = ps_init(config_en_us);
        /*Initialize the Speech recognizer*/
        trigger_ps      = ps_init(triggerConfig);
        /*Initialize the Trigger recognizer*/
        if(recognizer_ps == NULL)
                failedShutdown("Failed to create recognizer");
        if(trigger_ps == NULL)
                failedShutdown("Failed to create recognizer");

        speaker = speak_init();
        /*initialize the speaker engine*/

        if(speaker == NULL)
                failedShutdown("Failed to initialize the TTS Engine");

        /**Completed initializing all needed properties**/




        do{
                int8 trigResp = triggerRecord(trigger_ps, trigger_stream);
                /*block until trigger sentance is spoken*/
                if(trigResp != ERR_SUCCESS)
                        handleRecordingError(trigResp);



                //char* command = NULL;
                Cmd_response_t cmd = {NULL, 0};
                int8 error;
                error = obtainCommand(&cmd.reply, recognizer_ps, trigger_stream);
                //printf("%s\n",command);
                if(error != ERR_SUCCESS)
                        handleRecordingError(error);

                if(cmd.reply != NULL){

                        char* tmp = malloc(strlen(cmd.reply)+1);
                        strcpy(tmp, cmd.reply);

                        if(0 == getCommandInfo(&cmd)){
                                speak(speaker, "I could not find anything for");
                                speak(speaker, tmp);

                        }
                        else{
                                cmdHandle(&cmd, speaker);

                        }
                        //PRINT("before\n");
                        free(cmd.reply);
                        //PRINT("after\n");
                        free(tmp);

                }

        }while(1);
        /*run until told to stop*/

        ad_close(record_stream);
        ad_close(trigger_stream);
        ps_free(recognizer_ps);
        ps_free(trigger_ps);
        cmd_ln_free_r(config_en_us);
        cmd_ln_free_r(triggerConfig);
        speak_destroy(speaker);

        return 0;
}
/***************************************************************************
* void cmdHandle(Cmd_response_t* cmd, cst_voice* s)
* Author: SkibbleBip
* Date: 07/14/2021
* Description: Function to handle the command and it's response
*
* Parameters:
*        cmd    I/O     Cmd_response_t* struct to contain the command type and
*                                               response
*        s      I/P     cst_voice*      pointer to the speaker object
*        full   I/P     char*           pointer to the full command
**************************************************************************/
void cmdHandle(Cmd_response_t* cmd, cst_voice* s)
{

        /*NOTE: cmd->reply is dynamic, parent function frees it*/

        switch(cmd->cmd){
        /*execute commands based of the enum value of the command reply*/
                case Search: {
                        int errRep = cmd_search(cmd);
                        if(errRep != ERR_SUCCESS){
                        /*get the response of the request, handle any errors*/
                                char* out = handleError(cmd->reply, errRep);
                                speak(s, out);
                                free(out);
                                break;
                        }

                        speak(s, "I found this on the internet.");
                        speak(s, cmd->reply);
                        break;
                        /*if no errors, then execute the command*/
                }
                case Wiki: {
                        int errRep = cmd_wiki(cmd);
                        if(errRep != ERR_SUCCESS){
                        /*get the response of the request, handle any errors*/
                                char* out = handleError(cmd->reply, errRep);
                                speak(s,out);
                                free(out);
                                break;
                        }

                        speak(s, "I looked this up on wikipedia.");
                        speak(s, cmd->reply);
                        break;
                        /*if no errors, then execute the command*/
                }
                case Weather: {
                        int errRep = cmd_weather(cmd);
                        if(errRep != ERR_SUCCESS){
                        /*get the response of the request, handle any errors*/
                                char* out = handleError(cmd->reply, errRep);
                                speak(s, out);
                                free(out);
                                break;
                        }

                        speak(s, cmd->reply);
                        break;
                        /*if no errors, then execute the command*/
                }
                case Joke: {
                        int errRep = cmd_joke(cmd);
                        if(errRep != ERR_SUCCESS){
                        /*get the response of the request, handle any errors*/
                                char* out = handleError(cmd->reply, errRep);
                                speak(s, out);
                                free(out);
                                break;
                        }
                        speak(s, cmd->reply);
                        break;
                        /*if no errors, then execute the command*/
                }
                case Unpermittable:
                case Unknown:
                default:{
                        speak(s, "I didn't understand that request");
                        break;
                        /*default response if the command was unknown or not
                        *defined
                        */
                }
        }


}
/***************************************************************************
* char* handleError(char* reply, int err)
* Author: SkibbleBip
* Date: 07/14/2021
* Description: function to handle any error that occured when fetching a
*               command response
*
* Parameters:
*        reply  I/P     char*   string for the command that was not understood
*        err    I/P     int     error value that was occured
*        handleError    O/P     char*   dynamic string of the decoded error to
*                                       inform the user of the error occured
**************************************************************************/
char* handleError(char* reply, int err)
{
        char* out;
        char* tmp;
        switch(err){
        /*depending on the error code, generate a dynamic string of the
        *translated error code
        */
                case ERR_NOTFOUND:{
                        tmp = "I could not find anything online about %s";
                        size_t len = strlen(tmp)+strlen(reply)+1;
                        out = malloc(len);
                        sprintf(out, tmp, reply);
                        return out;
                }
                case ERR_CURL_INIT:{
                        tmp =
                        "I couldn't figure out how to use the internet";
                        break;
                }
                case CURLE_COULDNT_RESOLVE_HOST:{
                        tmp =
                        "There is no internet connectivity. Check your connection";
                        break;
                }
                case CURLE_COULDNT_CONNECT:{
                        tmp =
                        "I tried to look it up, but no one responded";
                        break;
                }
                case CURLE_WEIRD_SERVER_REPLY:{
                        tmp =
"I tried to look it up, but the internet just kinda screamed back at me";
                        break;
                }
                default:{
                        tmp =
                        "I had an unspecified error, and couldn't complete that request";
                        break;
                }

        }
        out = malloc(strlen(tmp)+1);
        strcpy(out, tmp);
        /*copy the non-dynamic temp string into the dynamic string and return
        *out
        */

        return out;
}



