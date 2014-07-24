SerialCommander
===============

Serial command parsing library for Arduino


This library is born out of the need to improve the serial command parsing in the PixelCommander library.

PixelCommander is a single node in an intended larger internet of things, 
and as such it seemed silly to use a poorly implemented serial command parsing method
while duplicating the code into every node of the network.

With SerialCommander, all nodes of the project can now have the same parsing capabilities as well as the same flaws, 
and hopefully fixes and enhancements as the library develops.

As with all things you will find here, this library is intended for my own use,
but is distributed under the GPL in the hope that it may be of use to somebody.



Usage
=====

Easy Way
--------
Check out the example sketch in this repository.

Hard Way
--------
- First, include the library header in your arduino project.
- Create an instance of SerialCommander by calling the constructor; It takes a single word (two bytes) to use as the start word.
      twobytes_SerialCommander(word startWord);
      
- in setup()
  - Add commands:

      ```
      addCommand("COMMAND_NAME", handlerName);
      ```
      
- in loop()
  - Call the Serial reader to read a byte:

      ```
      readSerialLooper()
      ```
      
Optional
--------
- Add default handler:

      ```
      setDefaultHandler(someFunctionHandlerForUnknownCommands);
      ```
- Override command terminator:

      ```
      overrideTerminator(word newTerminator);
      ```

      NOTE: The default terminator is a newline (hex chars 0x0D, 0x0A), 
      usually achieved by pressing the enter key on your keyboard.


Bugs
----
If you find bugs, please do let me know.
