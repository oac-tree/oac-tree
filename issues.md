# Issues

* Handle termination in `ParallelSequence`.
* Properly handle reset: check also setup method to verify correct configuration of instructions (attributes).
* Exception safety is not everywhere managed as it should.
* Add unit tests.
* Investigate daemon procedures for data aggregation and summarizing (could be alternative to MARTe2)
* Non-interactive mode for monitoring procedures. This would require introspecting into the instruction tree to ensure none of the instructions requires user input (simple NVI and new InputInstruction interface that overrides the virtual implementation).
* Provide enhanced interface to Workspace: allow get/set specific fields/substructures of the variable.
* Provide user input instruction and way to distinguish procedures that need user input.
* Add doxygen to many methods.
* Automatically load plugins in plugin folder.
* Make child instructions private in decorator and compound.
* Clearly distinguish between InitHook() and SetupImpl() and document.
