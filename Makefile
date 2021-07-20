
#**************************************************************************
#Copyright (C) 2021 SkibbleBip
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#**************************************************************************/



OBJS	= main.o Commands.o Speech.o Recording.o Parse.o
SOURCE	= main.c Commands.c Speech.c Recording.c Parse.c
HEADER	= main.h Commands.h Speech.h Recording.h Parse.h
OUT	= Xenia
CC	= gcc
FLAGS	= -s -c -Wall -Wextra -O3
LFLAGS	= `pkg-config --cflags --libs pocketsphinx sphinxbase` -lpocketsphinx -lsphinxbase -lsphinxad -lpulse -lpulse-simple -lpthread -lm -lflite_cmu_us_kal16 -lflite_usenglish -lflite_cmulex -lflite -lm -lcurl -ljson-c
LIBS	= /usr/local/lib
DEF	= -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\"

INC1	= /usr/local/include/sphinxbase/
INC2	= /usr/local/include/pocketsphinx/
INC3	= /usr/local/include/flite

INCLUDE = -I$(INC1) -I$(INC2) -I$(INC3)

all: $(OBJS)
	$(CC) -s -O3 $(OBJS) $(INCLUDE) -o $(OUT) -L$(LIBS) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) $(DEF) $(INCLUDE) main.c

Commands.o: Commands.c
	$(CC) $(FLAGS) $(INCLUDE) Commands.c

Speech.o: Speech.c
	$(CC) $(FLAGS) $(INCLUDE) Speech.c

Recording.o: Recording.c
	$(CC) $(FLAGS) $(INCLUDE) Recording.c

Parse.o: Parse.c
	$(CC) $(FLAGS) $(INCLUDE) Parse.c


clean:
	rm -f $(OBJS) $(OUT)




