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
* File:  Commands.h
* Author:  SkibbleBip
* Procedures:
***************************************************************************/
#ifndef COMMANDS_H
#define COMMANDS_H

#include <curl/curl.h>
#include "Parse.h"

#define         ERR_SUCCESS             0
#define         ERR_NOTFOUND           -1
#define         ERR_CURL_INIT          -2
#define         ERR_CURL               -3
#define         ERR_CURL_REALLOC       -4


struct Curl_Reply{
        size_t size;    /*length of the response*/
        char* response; /*string of the response (DYNAMIC)*/
};

struct Json_Reply{
        size_t count;   /*number of queries that have been loaded*/
        size_t size;    /*number of queries added to the reply*/
        char* response; /*the reply output*/
};

typedef struct {
        char* reply;    /*DYNAMIC answer from the command parser*/
        Command_t cmd;  /*command ID*/
} Cmd_response_t;

int getCommandInfo(Cmd_response_t* response);

void beginParse(Cmd_response_t* in);

int cmd_search(Cmd_response_t* cmd);

int cmd_wiki(Cmd_response_t* cmd);

int cmd_weather(Cmd_response_t* cmd);

int cmd_joke(Cmd_response_t* cmd);

int permittable(char* input);
#endif // COMMANDS_H
