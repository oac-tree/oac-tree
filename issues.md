# Issues

## General

* Review exception safety.
* Add unit tests.
* Non-interactive mode for monitoring procedures. This would require introspecting into the instruction tree to ensure none of the instructions requires user input (simple NVI and new InputInstruction interface that overrides the virtual implementation).
* Provide user input instruction and way to distinguish procedures that need user input.
* Add doxygen to many methods.
* Automatically load plugins in plugin folder.
* Add field get/set tests to LocalVariable-tests.
* Start/EndSingleStep may require const Instruction pointer too: this is connected to the procedure having an interface to request the next instruction to be executed, which will also be something useful in the GUI.

## UserInterface

* Provide input for complex type (from json?).
