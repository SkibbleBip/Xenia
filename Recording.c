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
* File:  Recording.c
* Author:  SkibbleBip
* Procedures:
* setupRecordingStream  -Prepares the inputted recording stream pointer
* triggerRecord -Blocks until the activation sentance is heard
* obtainCommand -decodes inputted text into a detected command and returns it's
*       response
* wasTriggered  -checks if the inputted text is the trigger sentance
* handleRecordingError  -Decodes recording error into human format before
*       handling the failure
***************************************************************************/


#include "Recording.h"
#include "main.h"
#include <sphinxbase/err.h>
#include <time.h>

/***************************************************************************
* ad_rec_t* setupRecordingStream(cmd_ln_t* c)
* Author: SkibbleBip
* Date: 06/24/2021
* Description: Set up the audio input device and return a pointer to the
*               recording stream
*
* Parameters:
*        c    I/P    cmd_ln_t*    configuration pointer that contains the
*                                       properties of the recording device and
*                                       stream
*        setupRecordingStream    O/P    ad_rec_t*    pointer to returned
*                                                       recording stream
**************************************************************************/
ad_rec_t* setupRecordingStream(cmd_ln_t* c)
{

        char const* str = cmd_ln_str_r(c, "-adcdev");
        /*No clue what this does. But I have to do it I GUESS*/
        int flt         = (int) cmd_ln_float32_r(c, "-samprate");
        /*Obtain the samplerate of the microphone stream*/

        ad_rec_t* tmp = ad_open_dev(str, flt);
        /*obtain the recording stream pointer*/

        return tmp;
}


/***************************************************************************
* int triggerRecord(ps_decoder_t *ps, ad_rec_t* a)
* Author: SkibbleBip
* Date: 06/24/2021
* Description: Blocks until the user speaks the trigger sentence (trigger
*               sentence is what the user speaks to get the device's attention)
*
* Parameters:
*        ps    I/P    ps_decoder_t*    pointer to decoder of trigger sentence
*        a    I/P    ad_rec_t*    pointer to audio stream device for recording
*        triggerRecord    O/P    int    return status, negative if error and 0
*                                       if successful. returns error codes
*                                       defined
**************************************************************************/
int triggerRecord(ps_decoder_t *ps, ad_rec_t* a)
{
        int16 buffer[2048];
        /*buffer for containing the audio data*/
        uint8 utterStarted              = FALSE;
        /*flag for whether an act of speaking is being performed by user*/
        uint8 speech                    = FALSE;
        /*flag for whether audio was heard which may be speech from a user*/
        uint8 triggered                 = FALSE;
        /*flag for whether "Excuse me xenia" was spoken by the user*/

        char* hypothesis                = "";
        /*Hypothesis of what the user was spoken. This is dynamically
        *allocated by PocketSphinx, and the programmer does not need to dealloc
        *it as PocketSphinx does it in the backend
        */


        if(ad_start_rec(a) < 0)
        /*If it failed to start the trigger recorder, return negative*/
                return ERR_START_AUD_REC;
        if(ps_start_utt(ps) < 0)
        /*If failed to start utterance, then return negtive*/
                return ERR_START_UTTER;

        PRINT("Ready to start scanning for trigger...\n");

        do{
                int32 ret = ad_read(a, buffer, 2048);
                /*read audio data into the buffer*/
                if(ret < 0)
                /*if the reading was invalid, return error*/
                        return ERR_READ_AUD;
                if(ps_process_raw(ps, buffer, ret, FALSE, FALSE) < 0)
                /*process the audio buffer. if failure, return error*/
                        return ERR_PROCCESS;

                speech = ps_get_in_speech(ps);
                /*obtain if audio was heard*/

                if(speech && !utterStarted){
                /*if speech was picked up and an instance of utterance wasnt
                *already started, then start the utterance and begin listening
                *for trigger
                */
                        utterStarted = TRUE;
                        PRINT("Listening...\n");
                }

                if(!speech && utterStarted){
                /*input just went from audible speech to silence, if audible
                *sound is no longer being picked up and the flag for an act of
                *utterance is active, then it means the user is no longer
                *speaking
                */
                        if(ps_end_utt(ps) < 0)
                                return ERR_END_UTTER;
                        /*stop scanning the utterance*/
                        hypothesis = ps_get_hyp(ps, NULL);
                        /*Obtain the hypothesis of what was spoken*/

                        if(hypothesis != NULL){
                        /*if there was a valid vocal words spoken, check if
                        * they match the trigger term
                        */
                                triggered = wasTriggered(hypothesis);
                                /*obtain the status of the hypothesis being
                                * a trigger
                                */
                        }
                        utterStarted = FALSE;
                        /*set utterance flag to false*/

                        if(ps_start_utt(ps) < 0)
                                return ERR_START_UTTER;
                        /*start utterance again*/

                }

        }
        while(0 == triggered);
        /*continue looping unless triggered*/
        if(ad_stop_rec(a) < 0)
                return ERR_END_AUD_REC;
        if(ps_end_utt(ps) < 0)
                return ERR_END_UTTER;


        return ERR_SUCCESS;


}


/***************************************************************************
* int obtainCommand(char* command, ps_decoder_t *ps, ad_rec_t* a)
* Author: SkibbleBip
* Date: 06/26/2021      v1: Initial
* Date: 07/11/2021      v1: Replaced "command" with a double pointer so it
*                               could be dynamically allocated
* Description: function that takes a char array pointer as an argument and
*       copies command information that was obtained through speech recognition
*
* Parameters:
*        command    I/O    char**    the pointer to the char array that will
*                                       contain the command text
*        ps         I/P    ps_decoder_t*    The decoder pointer
*        a          I/P    ad_rec_t*    pointer to audio stream recorder
*        obtainCommand    O/P    int    Error code return
**************************************************************************/
int obtainCommand(char** command, ps_decoder_t *ps, ad_rec_t* a)
{
        int16 buffer[2048];
        /*buffer for containing the audio data*/
        uint8 utterStarted              = FALSE;
        /*flag for whether an act of speaking is being performed by user*/
        uint8 speech                    = FALSE;
        /*flag for whether audio was heard which may be speech from a user*/
        uint8 speaking                 = FALSE;
        /*flag for whether the user is no longer speaking for 3+ seconds*/
        time_t startTime, endTime;
        /*Time structures*/


        if(ad_start_rec(a) < 0)
        /*If it failed to start the trigger recorder, return negative*/
                return ERR_START_AUD_REC;
        if(ps_start_utt(ps) < 0)
        /*If failed to start utterance, then return negtive*/
                return ERR_START_UTTER;



        PRINT("Listening for command...\n");



        do{
                int32 ret = ad_read(a, buffer, 2048);
                /*read audio data into the buffer*/
                if(ret < 0)
                /*if the reading was invalid, return error*/
                        return ERR_READ_AUD;
                if(ps_process_raw(ps, buffer, ret, FALSE, FALSE) < 0)
                /*process the audio buffer. if failure, return error*/
                        return ERR_PROCCESS;

                speech = ps_get_in_speech(ps);
                /*obtain if audio was heard*/

                if(speech){
                /*if the user is speaking, get the start time of the speaking*/
                        time(&startTime);
                }
                else{
                /*if the user is not speaking, obtain the current time and
                *check if it has been 3 seconds since the user stopped speaking
                */
                        time(&endTime);
                        if(endTime-startTime >= 3)
                                speaking = TRUE;


                }


                if(speech && !utterStarted){
                /*if speech was picked up and an instance of utterance wasnt
                *already started, then start the utterance and begin listening
                *for trigger
                */
                        utterStarted = TRUE;
                        PRINT("Listening...\n");
                }

                if(/*!speech*/speaking && utterStarted){
                /*input just went from audible speech to silence, if audible
                *sound is no longer being picked up and the flag for an act of
                *utterance is active, then it means the user is no longer
                *speaking
                */
                        if(ps_end_utt(ps) < 0)
                                return ERR_END_UTTER;
                        /*stop scanning the utterance*/
                        char* tmp = ps_get_hyp(ps, NULL);
                        /*Obtain the hypothesis of what was spoken*/

                        if(tmp != NULL){
                        /*if there was a valid vocal words spoken, copy the
                        * command
                        */
                                *command = malloc(strlen(tmp)+1);
                                strcpy(*command, tmp);

                        }
                        else{
                                /*if there was no understandable text, then
                                *just make a NULL string
                                */
                                command = NULL;
                        }
                        utterStarted = FALSE;
                        /*set utterance flag to false*/


                        if(ps_start_utt(ps) < 0)
                                return ERR_START_UTTER;
                        /*start utterance again*/

                }


        }
        while(0 == speaking);
        /*continue looping unless triggered*/
        if(ad_stop_rec(a) < 0)
                return ERR_END_AUD_REC;

        if(ps_end_utt(ps) < 0)
                return ERR_END_UTTER;

        return ERR_SUCCESS;

}


/***************************************************************************
* int wasTriggered(char* str)
* Author: SkibbleBip
* Date: 06/24/2021
* Description: Checks if the inputted string matches the trigger sentence.
*               Returns 1 if matches and 0 if not
*
* Parameters:
*        str             I/P    char*    inputted string
*        wasTriggered    O/P    int      boolean return value of if string
*                                               matches or not
**************************************************************************/
int wasTriggered(char* str)
{
        const char* trigger = "EXCUSE ME XENIA";
        if(0 == strcmp(trigger, str))
                return 1;
        return 0;

}
/***************************************************************************
* void handleRecordingError(int8 err)
* Author: SkibbleBip
* Date: 06/26/2021
* Description: function that takes a recording error code and prints out
*               english equivelant before handling the failure
*
* Parameters:
*        err    I/P    int8    the error code
**************************************************************************/
void handleRecordingError(int8 err)
{

        char* toOut;
        switch (err){
        /*handle the response of the trigger handling. if negative, then was an
        *error. if 0, then no error occured
        */


                case ERR_START_AUD_REC:
                        toOut = "Failed to start recording audio";
                        break;
                case ERR_START_UTTER:
                        toOut = "Failed to start scanning utterances";
                        break;
                case ERR_READ_AUD:
                        toOut = "Failed to read to audio buffer";
                        break;
                case ERR_PROCCESS:
                        toOut = "Failed to process audio buffer";
                        break;
                case ERR_END_UTTER:
                        toOut = "Failed to end scanning for utterances";
                        break;
                case ERR_END_AUD_REC:
                        toOut = "Failed to end recording audio";
                        break;
                default: toOut = "Unspecified error";


        }

        failedShutdown(toOut);
        /*handle the error */
}



