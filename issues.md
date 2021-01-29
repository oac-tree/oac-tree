# Issues

* Handle termination in ParallelSequence.
* Replace hardcoded 100ms in Runner.cpp by configuration parameter from xml.
* Properly handle reset: check also setup method to verify correct configuration of instructions (attributes).
* The `RUNNING` state was not handled correctly in Sequence. This should be checked for other compound and decorator instructions.
* Exception safety is not everywhere managed as it should.
