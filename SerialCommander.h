/**
 * SerialCommander, an Arduino Serial command parsing library
 * 
 * Copyright (C) 2014 2bytes, http://2byt.es
 * 
 * This file is part of SerialCommander.
 * 
 * SerialCommander is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * SerialCommander is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with SerialCommander. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: Hamid Elaosta hamid@2byt.es
 */

#ifndef TWOBYTES_SERIAL_COMMANDER_H
#define TWOBYTES_SERIAL_COMMANDER_H

#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif

#define INPUT_BUFFER_SIZE		16
#define CMD_BUFFER_SIZE			16
#define MAX_COMMAND_COUNT	  	16

class twobytes_SerialCommander 
{
	public:
		twobytes_SerialCommander(word);
		~twobytes_SerialCommander();
		bool addCommand(char*,  void (*function)(char*));
		void setDefaultHandler(void (*function)(char*));
		void handleCommand(char*);
		void readSerialLooper();
		void overrideTerminator(word);
		
	private:
		
		typedef struct functionHandler
		{
			char command[CMD_BUFFER_SIZE];
			void (*function)( char*);
		} CommandHandler;

		CommandHandler CommandList[MAX_COMMAND_COUNT];
		
		/* How many commands we've added to the list so far */
		int cmdCount;
		
		/* Holds the default function handler for when no command is found to match serial input*/
		void (*defaultHandler)(char*);
		
		/* Byte buffer to read the input from the Serial into while waiting for a command */
		byte ib[INPUT_BUFFER_SIZE];
		
		/* Hols the status of command start. If we have received the start word, 
		then we have started, everything that proceeds is payload */
		boolean cmdStarted;
		
		/* Input buffer count. Essentially, how many bytes we've read so far for this command*/
		int ic;
		
		/* Command start word indicates the start of an incoming command */
		word cs;
		/* Command end word indicates the end of an incoming command */
		word ce;
};

#endif // TWOBYTES_SERIAL_COMMANDER