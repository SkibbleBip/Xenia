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
* File:  Parse.c
* Author:  SkibbleBip
* Procedures:
* What_Is       -Tokenizer for "what is" and "what's" for parsing the command
*                       string
* Tell_Me       -Tokenizer for "tell me" when parsing
* Look_Up       -Tokenizer for "look up"
* Search_Up     -Tokenizer for "search up"
* A_Joke        -Tokenizer for "a joke"
* The_Weather   -Tokenizer for "The weather"
* Tail          -The tokenizer for the optional tail strings for searching the
*                       weather
* a_an          -Tokenizer for a/an in the command parser
* toUpper       -Converts a string to uppercase
***************************************************************************/
#include "Parse.h"
#include <string.h>
#include <stdlib.h>


/***************************************************************************
* Command_t What_Is(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for "what is" and "what's" for parsing the command string
*
* Parameters:
*        in     I/O     char*   The input string to parse
*        What_Is        O/P     Command_t       Return command enum for the parsed command
**************************************************************************/
Command_t What_Is(char* in)
{
        const char* whatIs = "WHAT IS";
        const char* whats  = "WHAT'S";
        int failed = 0;
        for(size_t i = 0; i < strlen(whatIs); i++)
                if(in[i] != whatIs[i]){
                        failed = 1;
                        break;
                }
        /*check if the string starts with "WHAT IS"*/

        if(failed == 1)
        /*if the previous check didnt match, try again with "WHAT'S"*/
                for(size_t i = 0; i < strlen(whats); i++)
                        if(in[i] != whats[i])
                                return Unknown;
                        /*if it still didnt match, then return 0, it was not
                        *found
                        */


        /*if it didnt fail one of those tries, then determin by the fail flag
        *which string was found, and then jump forward until the string is
        *overrun
        */
        const char* toProcess;
        if(failed)
                toProcess = whats;
        else
                toProcess = whatIs;
        /*determine which string was the calling string*/

        size_t len = strlen(in)-strlen(toProcess);
        for(size_t i = 0; i <len; i++)
                in[i] = in[i+strlen(toProcess)+1];
        /*back up the string and overwrite the trigger string*/

        Command_t reply = The_Weather(in);
        /*check if it's asking for the weather*/

        if(reply == Unknown)
                reply = a_an(in);
        /*if not, then it's asking for wiki lookup. check if a/an is present*/
        if(reply == Unknown)
                reply = Wiki;
        /*if no a/an, then just set the command to wiki*/


        return reply;
        /*return the outcome*/

}
/***************************************************************************
* Command_t Tell_Me(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for "tell me" when parsing the command string
*
* Parameters:
*        in             I/O     char*           The input string to parse
*        Tell_Me        O/P     Command_t       The output command enum detected
*                                                       from the parsed string
**************************************************************************/
Command_t Tell_Me(char* in)
{

        const char* tellMe = "TELL ME ";
        if(strncmp(in, tellMe, strlen(tellMe)))
                return Unknown;
        /*check if the string starts with "tell me"*/

        size_t len = strlen(in) - strlen(tellMe);
        for(size_t i = 0; i <= len; i++)
                in[i] = in[i+strlen(tellMe)];
        /*if it does, then push back the string to remove the "tell me" part*/
        Command_t reply = A_Joke(in);

        if(reply == Unknown)
        /*if the joke parse didnt pass, then assume its asking about the weather*/
                reply = The_Weather(in);

        return reply;
}
/***************************************************************************
* Command_t Look_Up(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for "look up" for parsing the command string
*
* Parameters:
*        in     I/O     char*   The input string to be parsed
*        Look_Up        O/P     Command_t       The command enum to be returned
*                                                       of the parsed string
**************************************************************************/
Command_t Look_Up(char* in)
{

        const char* lookUp = "LOOK UP ";
        if(strncmp(in, lookUp, strlen(lookUp)))
                return Unknown;
        /*check if the string starts with "look up"*/
        size_t len = strlen(in) - strlen(lookUp);
        for(size_t i=0; i <=len; i++)
                in[i] = in[strlen(lookUp)+i];
        /*if it was found, remove "look up and return the search string*/

        return Search;
        /*Return the type of command requested*/
}
/***************************************************************************
* Command_t Search_Up(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for "search up" for parsing the command
*
* Parameters:
*        in     I/O             char*           The input string to parse
*        Search_Up      O/P     Command_t       The returned Command enum for
*                                                       which command the
*                                                       parsing partains to
**************************************************************************/
Command_t Search_Up(char* in)
{
        const char* searchUp = "SEARCH UP ";
        if(strncmp(in, searchUp, strlen(searchUp)))
                return Unknown;
        /*check if the string starts with "look up"*/
        size_t len = strlen(in) - strlen(searchUp);
        for(size_t i=0; i <=len; i++)
                in[i] = in[strlen(searchUp)+i];
        /*if it was found, remove "look up and return the search string*/

        return Search;
        /*Return the type of command requested*/
}

/***************************************************************************
* Command_t A_Joke(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for "a joke" in the parser for commands
*
* Parameters:
*        in     I/O     char*           The input string to parse
*        A_Joke O/P     Command_t       The return command enum that was found
*                                               for the parsed command
**************************************************************************/
Command_t A_Joke(char* in)
{
        if(!strcmp(in, "A JOKE")){
                //free(in);
                //in = NULL;
                return Joke;
        }
        /*if the request was for a joke, then free the string and nullptr it*/
        return Unknown;

}
/***************************************************************************
* Command_t The_Weather(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for "The weather" in command parser
*
* Parameters:
*        in     I/O     char*   The input string to parse
*        The_Weather    O/P     Command_t       The return enum of the command
*                                                       type
**************************************************************************/
Command_t The_Weather(char* in)
{

        if(!strcmp(in, "THE WEATHER")){
                free(in);
                in = NULL;
                return Weather;
        }
        /*if the string ends with "the weather", then return the weather
        *command
        */
        const char* str = "THE WEATHER ";
        if(strncmp(in, str, strlen(str)))
                return Unknown;
        /*if the string doesnt contain "the weather" at the front, return
        *failure
        */
        size_t len = strlen(in) - strlen(str);
        for(size_t i = 0; i <= len; i++)
                in[i] = in[strlen(str)+i];
        /*remove "the weather", and check if it has the optional tail. if not,
        *the program misundertood the user, return failure.
        */
        Command_t reply = Tail(in);
        /*The "tail" is optional text from the user asking for the weather.
        *the user may ask "whats the weather today" or "what is the weather
        *outside", but should not be able to answer "whats the weather
        *_in germany_", this is an invalid command request. The tail checks if
        *the command request is of a valid possible request for the weather
        *in the current location.
        */

        return reply;

}
/***************************************************************************
* Command_t Tail(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: The tokenizer for the optional tail strings for searching the
*                       weather
*
* Parameters:
*        in     I/O     char*           The input string to parse
*        Tail   O/P     Command_t       The return command enum that was
*                                               detected
**************************************************************************/
Command_t Tail(char* in)
{
        if(!strcmp(in, "TODAY") ||
         !strcmp(in, "OUTSIDE") ||
         !strcmp(in, "OUTSIDE TODAY")){
         /*all valid implementations of the tail*/
                free(in);
                in = NULL;
                return Weather;
        }
        return Unknown;
}
/***************************************************************************
* Command_t a_an(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Tokenizer for a/an in the command parser
*
* Parameters:
*        in     I/O     char*   The string to parse
*        a_an   O/P     Command_t       The command enum that was returned
**************************************************************************/
Command_t a_an(char* in)
{
        /*if the string starts with "a "*/
        if(in[0] == 'A' && in[1] == ' '){
                for(size_t i = 0; i <= strlen(in)-2; i++)
                        in[i] = in[i+2];
                return Wiki;
        }
        /*if string starts with "an "*/
        if(in[0] == 'A' && in[1] == 'N' && in[2] == ' '){
                for(size_t i = 0; i <= strlen(in)-3; i++)
                        in[i] = in[i+3];

                return Wiki;
        }

        return Unknown;
}

/***************************************************************************
* void toUpper(char* in)
* Author: SkibbleBip
* Date: 07/09/2021
* Description: Converts a string to uppercase
*
* Parameters:
*        in     I/O     char*   The input string
**************************************************************************/
void toUpper(char* in)
{
        for(size_t i = 0; i<strlen(in); i++)
                if(in[i] >= 'a' && in[i]<='z')
                        in[i]-=32;
        /*if a character is lowercase, then make it uppercase*/
}

