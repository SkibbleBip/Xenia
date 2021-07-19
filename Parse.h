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
* File:  Parse.h
* Author:  SkibbleBip
* Procedures:
***************************************************************************/

#ifndef PARSE_H
#define PARSE_H

typedef enum {Unknown, Search, Wiki, Weather, Joke, Unpermittable } Command_t;
/*definitions of the different commands*/


/*general "trunks and branches" of the BNFT of the parser*/
Command_t expr(char* in);
Command_t search(char* in);
Command_t joke(char* in);
Command_t a_an(char* in);

/*Tokens of the parser*/
Command_t What_Is(char* in);
Command_t Tell_Me(char* in);
Command_t Look_Up(char* in);
Command_t Search_Up(char* in);
Command_t A_Joke(char* in);
Command_t The_Weather(char* in);

/*the optional tail for searching for weather*/
Command_t Tail(char* in);

void toUpper(char* in);
/*converts string to uppercase*/

#endif // PARSE_H
