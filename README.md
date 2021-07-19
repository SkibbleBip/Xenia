# About
##### WARNING: THIS SOFTWARE IS IN IT'S EARLY STATE

Xenia ("Zee-Nia") is a Home Assistant that is designed with the idea of privacy, simplicity, and decentralization. No "headquarters" or main cloud/service is needed to process voice commands so no data is sent across the internet, other than to the online request information is being received. For example, should a user ask for a joke, a request is performed across the internet to an online API to pull a joke, and return it. There is no intermediary cloud that stores user data like there is in other Home Assistants such as the Google Home or Amazon Alexa. No data about the user is stored on the device through the Xenia software. No user data is farmed through the Xenia software. Xenia is simply a tool, designed to do one specific set of tasks and that is all.

## How to use
To activate Xenia so it can listen for a command, simply speak "Excuse me Xenia". Xenia will then listen until 3 seconds of silense is heard. The program will then process the command and return with the requested information by generating speech.

# Things It Does

Currently Xenia can perform these tasks:
* Look up the weather in your location
* Tell you a joke
* Search Wikipedia for a subject
* Search DuckDuckGo for a subject

Plans for the future:
* Play music from online or from a file/network location
* Improve the parsing of commands
* Read emails to the user
* Set alarms and timers
* Configuration through a phone app


# Current State

The software is still in it's early state. I have plans to add it as a main service on an embedded Linux system on hardware such as a Raspberry Pi. Curretly the software is only a program that needs to be run on a Linux System that will need a user to interface through the shell to configure. This means to hook up a system to run Xenia, the embedded Linux system will need to be manually configured for network access. I have plans to allow devices to communicate with a phone app through bluetooth to configure the system for network connectivity.
I also plan on building my own Linux system for embedded systems like the Raspberry Pi through Linux From Scratch or another "Embedded System-friendly" Linux OS. This will automatically include Xenia built in as a background process so that after the OS and file system is built, all the user needs to do is to burn the file system onto an SD card or USB drive and mount it on a Raspberry Pi.

# Comparisons With Other Home Assistant Systems

Reasons why other Assistants are better
* Other Assistants usually are better with speech recognition and speech generation
    * This is because all audio data is sent to a central cloud server to be processed which generally has better processing abilities than if processed on-site
* Other Assistants have more features
    * Other Assistants objectively have more resources spent on them and include more features. However, many users never use all the features nor ever really need to, which leads the system being bloated.

Reasons why Xenia is better
* Xenia's speech engine is solely processed on-site
    * No audio data is ever sent across the internet. That means users cannot be spied on or have their personal data sold
* Xenia has easy to use and simple features
    * Xenia is designed to be as simple as possible for the end-user. It also is stripped of any unneeded features. Xenia is a tool, not a toy, so there is no reason for games or to have a psuedo-conversation with Xenia.
    * Xenia is for specific users who want only a small set of features they will ever use. No need for the other unneeded features.

# Current Commands
* "Tell me a joke"
* "What is the weather outside / Tell me the weather"
* "What is [Search Term]"
* "Look up / Search up [Search Term]"

# APIs used
* DuckDuckGo <https://api.duckduckgo.com/>
* Wikipedia <https://en.wikipedia.org/api/rest_v1/>
* WTTR IN <http://wttr.in/>
* Jokeapi.dev <https://v2.jokeapi.dev/joke/>

# Compiling

### Dependencies
To compile, Xenia requires **Flite, Sphinxbase/Pocketsphinx, libCurl-dev, Pulseaudio.**
These can be either installed through a package manager, or directly compiled and installed from source.
It is recommended to compile a recent version from github for Flite and Sphinxbase/Pocketsphinx, as the "stable" versions on their University webpages of these have not been updated for more than a decade.
    https://github.com/festvox/flite
    https://github.com/cmusphinx/pocketsphinx
Follow the instructions of installation of these dependancies given by their manpages and github links


To compile, simply cd to the root directory of the project (.Xenia/) and type 
`make`
The generated binary will be `Xenia`

# Forking and editing
Users are completely welcome to fork and add features and patches to Xenia, as well as request for them to be merged. If a feature is added that I feel is a good addition to Xenia, I will merge it.


# License 
### Xenia
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

### Flite 
\
                   Language Technologies Institute                      
                     Carnegie Mellon University                        
                        Copyright (c) 1999                             
                        All Rights Reserved.                           
                                                                       
  Permission is hereby granted, free of charge, to use and distribute  
  this software and its documentation without restriction, including   
  without limitation the rights to use, copy, modify, merge, publish,  
  distribute, sublicense, and/or sell copies of this work, and to      
  permit persons to whom this work is furnished to do so, subject to   
  the following conditions:                                            
   1. The code must retain the above copyright notice, this list of    
      conditions and the following disclaimer.                         
   2. Any modifications must be clearly marked as such.                
   3. Original authors' names are not deleted.                         
   4. The authors' names are not used to endorse or promote products   
      derived from this software without specific prior written        
      permission.                                                      
                                                                       
  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         
  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      
  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   
  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   
  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       
  THIS SOFTWARE.                                                       

### PocketSphinx/SphinxBase

 Copyright (c) 1999-2001 Carnegie Mellon University.  All rights
 reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

 This work was supported in part by funding from the Defense Advanced
 Research Projects Agency and the National Science Foundation of the
 United States of America, and the CMU Sphinx Speech Consortium.

 THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND
 ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
