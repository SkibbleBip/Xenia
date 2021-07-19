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
* File:  Speech.h
* Author:  SkibbleBip
* Procedures:
***************************************************************************/

#ifndef SPEECH_H
#define SPEECH_H

#include <flite.h>


cst_voice* speak_init(void);

void speak_destroy(cst_voice* v);

float speak(cst_voice* v, char* text);




#endif //SPEECH_H
