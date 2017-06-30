#pragma once

// Enable runtime safety checks for things like stack over/underflow and malloc
// failures.
// If enabled, these errors will result in a message being printed and a longjmp
// back to the last known safe state. If disabled, these errors are completely
// unchecked and, if they occur, will (at best) crash the program/reset the AVR.
// Adds about 2k of flash, mostly for error message strings.
#define SAFETY_CHECKS

// Size of data stack, in machine words (== 2 bytes each on AVR, 8 bytes on amd64).
// This is used not just for values explicitly put on the stack, but also as a
// scratch buffer for string/number input and for compilation, so this imposes a
// limit on maximum string literal length and maximum (non-precompiled) function
// length as well.
// We give it lots of extra stack on Linux for the bootstrapping process.
#define LINUX_STACKSIZE 1024
#define AVR_STACKSIZE 32

// Size of the longjmp() stack. Notforth itself never uses more than two of these,
// and those only when processing source code or user input.
#define NROF_JMP_BUFS 2

// Whether to enable the serial REPL for the Arduino build.
// Turning this off disables the ability to run code by typing it into the
// serial port, but also saves about 3k of flash.
#define SERIAL_REPL
