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
* Ownership of Instructions is passed to Procedure, as for Variables to Workspace. By passing in a pointer, this creates some non-ideal code (see e.g. Workspace::AddVariable, which could potentially segfault when adding an already owned Variable under a duplicate name). While the client has to be rather evil-minded to trigger such behavior, it can easily be cured by not passing ownership and cloning Instructions and Variables inside.

## UserInterface

* Provide input for complex type (from json?).
* Improve asynchronous handling of user input: if two user input instructions are executed concurrently, one will block until the other one finishes (mutex on UserInterface). This might lead to ParallelSequence to have reached final conditions (success or failure threshold reached), but still blocking for the second user input instruction. The wait for the mutex on the UserInterface could be smarter and also check if Halt() was not called in the meantime.
