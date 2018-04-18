# 6502 Emulator

An emulator of the 6502 CPU

![image](http://jonathan-chandler.github.io/6502-CPU-Emulator/img/demo.gif)

## Compilation/Execution

- Included makefile uses g++ with C++14 standard
- Requires SDL 2.0 library (https://www.libsdl.org/download-2.0.php)
- Compilation was tested with GCC 7.3.1 with SDL2 2.0.8

## Gameplay

- Controls with W-A-S-D keys

## Credits

All keyboard input and graphics are handled using SDL 2.0 (https://www.libsdl.org/)

Snake program bytecode was created by Nick Morgan (http://skilldrick.github.io/easy6502/)

Instruction implementation is based on reference written by John Pickens, Bruce Clark, and Ed Spittles (http://www.obelisk.me.uk/6502/reference.html)

Instruction table used to create the addressing mode and instruction tables were adapted from the instruction chart written by Neil Parker (http://www.llx.com/~nparker/a2/opcodes.html)

