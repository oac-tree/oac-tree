# Issues

* Handle termination in `ParallelSequence`.
* Properly handle reset: check also setup method to verify correct configuration of instructions (attributes).
* Exception safety is not everywhere managed as it should.
* Add unit tests.
* Investigate daemon procedures for data aggregation and summarizing (could be alternative to MARTe2)
* Non-interactive mode for monitoring procedures. This would require introspecting into the instruction tree to ensure none of the instructions requires user input (simple NVI and new InputInstruction interface that overrides the virtual implementation).
