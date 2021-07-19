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
* File:  main.h
* Author:  SkibbleBip
* Procedures:
* failedShutdown        -Prints error message occured and exits -1
***************************************************************************/

#ifndef MAIN_H
#define MAIN_H


#include <sphinxbase/err.h>
#include <stdint.h>

#define SILENT
/*comment out if you want debugging output*/

#ifdef SILENT
/*if we define SILENT, then the output will be muted albiet specified instances
*specified by the user
*/
        #define         PRINT(X)        printf(X)
        #define         ERRPRINT(X)     perror(X); exit(-1);

#else
        #define         PRINT(X)        E_INFO(X)
        #define         ERRPRINT(X)     E_FATAL_SYSTEM(X)
#endif // SILENT


/*typedef struct{
        char city[1024];
        uint8_t IP_addr[4];
        double longitude, latitude;

} Location_t;*/
/*Location struct, as of the current version we don't use it, we may in the
*future though
*/

/***************************************************************************
* static void failedShutdown(char* in)
* Author: SkibbleBip
* Date: 06/19/2021
* Description: Prints error message occured and exits -1
*
* Parameters:
*        in     I/P     char*   The string to inform the user of what error
*                                       occured
**************************************************************************/
static void failedShutdown(char* in)
{
        ERRPRINT(in);

}

//int getLocation(Location_t* location);



#endif //MAIN_H
