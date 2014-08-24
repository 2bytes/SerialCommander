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
 
#include "SerialCommander.h"
#include <iostream>
using namespace std;

#define DEBUG false

#define ACK		"ACK"
#define NACK	"NACK"

twobytes_SerialCommander::twobytes_SerialCommander(word commandStart, boolean doAck) : cmdCount(0), defaultHandler(0), cmdStarted(false), ic(0), ce(word(0x0D, 0x0A))/*newline*/
{
	cs = commandStart;
	ack = doAck;
}

twobytes_SerialCommander::~twobytes_SerialCommander()
{
	
}

void twobytes_SerialCommander::overrideTerminator(word endWord)
{
	ce = endWord;
}

bool twobytes_SerialCommander::addCommand(char* cmdBytes,  void (*function)(char*))
{
	if(DEBUG)
	{
		Serial.print("Add Command: ");Serial.println(cmdBytes);
		Serial.print("Length: ");Serial.println(strlen(cmdBytes));
	}

	if(strlen(cmdBytes) > CMD_BUFFER_SIZE)
	{
		if(DEBUG)
		{
			Serial.print("The command you entered is longer than the max command length set by CMD_BUFFER_SIZE of ");Serial.println(CMD_BUFFER_SIZE);
			Serial.println("Please shorten it and try again"); 
		}
	}
	else
	if(cmdCount < MAX_COMMAND_COUNT)
	{
		strncpy(CommandList[cmdCount].command, cmdBytes, strlen(cmdBytes));
		CommandList[cmdCount].function = function;
		cmdCount++;
		
		if(DEBUG)
		{
			Serial.println("Command Added");
		}
		
		return true;
	}
	else
	{
		if(DEBUG)
		{
			Serial.print("Max command count reached; %d. Command not added.\n");Serial.println(cmdCount);
		}
	}
	
	return false;
}

void twobytes_SerialCommander::setDefaultHandler(void (*function)(char*))
{
	defaultHandler = function;
}

void twobytes_SerialCommander::handleCommand(char* cmdToken)
{
	if(DEBUG)
	{
		Serial.print("Handle command called, testing for match to: ");Serial.println(cmdToken);
	}

	for(int i = 0; i < cmdCount; i++)
	{
		if(strncmp(cmdToken, CommandList[i].command, strlen(CommandList[i].command)) == 0)
		{
			if(DEBUG)
			{
				Serial.print("\n\nMatched Command: ");Serial.println(CommandList[i].command);
			}
			
			(*CommandList[i].function)(cmdToken+strlen(CommandList[i].command)); // Cut command string off the start first.
			if(ack)
			{
				Serial.println(ACK);
			}
			return;
		}
	}
	
	if(ack)
	{
		Serial.println(NACK);
	}
	
	if(defaultHandler != NULL)
	{
		(*defaultHandler)(cmdToken);
	}
}

int ic = 0;

void twobytes_SerialCommander::readSerialLooper()
{
	if(Serial.available() > 0)
	{
		ib[ic] = Serial.read();
	
		word iw = word(ib[ic-1], ib[ic]);
    
		if(ce == iw && cmdStarted)
		{
			if(DEBUG)
			{
				// Command Received
				Serial.print("Received Command: ");
				
				int i = 0;
      
				while(!(word(ib[i],ib[i+1]) == ce))
				{
					Serial.write(ib[i]);
					i++;
				}
      
				Serial.println("");
			}
      
			ic = 0;
			cmdStarted = false;
			
			handleCommand((char*)ib);
			
			for(int i = 0; i < INPUT_BUFFER_SIZE; i++)
			{
				ib[i] = 0x00;
			}
		}
		else
		{
		  if(!cmdStarted && (ic == 0 && !(ib[0] == highByte(cs))))
		  {
			if(DEBUG)
			{
				Serial.print("First byte doesn't match command start. Resetting: [");Serial.print(highByte(iw), HEX);Serial.println("]");
			}
			
			ic = 0; // Unnecessary as we're already at ic == 0, but it removes some ambiguity
		  }
		  else
		  if(!cmdStarted && (ic == 1 && !(iw == cs)))
		  {
			if(DEBUG)
			{
				Serial.print("First word doesn't match command start. Resetting: [");Serial.print(iw, HEX);Serial.println("]");
			}
				
			ic = 0; // Reset buffer
		  }
		  else
		  if(ic == 1 && (iw == cs))
		  {
			ic = 0; // Reset buffer
			cmdStarted = true; // Command started
			
			if(DEBUG)
			{
				Serial.println("First word matches, reading command payload.");
			}
			
		  }
		  else
		  {
			ic++;
		  }
		}
	}
}