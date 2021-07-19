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
* File:  Recording.h
* Author:  SkibbleBip
* Procedures:
***************************************************************************/

#ifndef RECORDING_H
#define RECORDING_H

#include <pocketsphinx.h>
#include <ad.h>

#define         ERR_SUCCESS              0
#define         ERR_START_AUD_REC       -1
#define         ERR_START_UTTER         -2
#define         ERR_READ_AUD            -3
#define         ERR_PROCCESS            -4
#define         ERR_END_UTTER           -5
#define         ERR_END_AUD_REC         -6

/*Initializes the recording stream*/
ad_rec_t* setupRecordingStream(cmd_ln_t* c);

/*Blocks until trigger word is detected, returns 0 if not found, returns
* 1 if found, returns negative if error
*/
int triggerRecord(ps_decoder_t *ps, ad_rec_t* a);

/*Obtain the char string of what command the user spoke. Char pointer
* will need to be freed when done processing
*/
int obtainCommand(char** command, ps_decoder_t *ps, ad_rec_t* a);


int wasTriggered(char* str);

void handleRecordingError(int8 err);

#endif // RECORDING_H
