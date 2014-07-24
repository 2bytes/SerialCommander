#include <SerialCommander.h>

twobytes_SerialCommander cmdr = twobytes_SerialCommander(word('S','C'));

void unknownCommand(char data[])
{
  Serial.print("Unknown command, no handler defined: ");Serial.println(data);
}

void firstCommand(char data[])
{
  Serial.println("First command called");
}

void secondCommand(char data[])
{
  Serial.print("Second command called ");Serial.println(data);
}

void thirdCommand(char data[])
{
  Serial.print("Third command called ");Serial.println(data);
}

void fourthCommand(char data[])
{
  Serial.print("Fourth command called ");Serial.println(data);
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("Welcome to SerialCommander");
  
  cmdr.addCommand("HELLO", firstCommand);
  cmdr.addCommand("WORLD", secondCommand);
  cmdr.addCommand("2BYTES", thirdCommand);
  cmdr.addCommand("SERIALCOMMANDER", fourthCommand);
  
  cmdr.setDefaultHandler(unknownCommand);

}

void loop() 
{
  cmdr.readSerialLooper();
}
