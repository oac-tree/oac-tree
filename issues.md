# Issues

* Handle termination in `ParallelSequence`.
* Replace hardcoded 100ms in Runner.cpp by configuration parameter from xml.
* Properly handle reset: check also setup method to verify correct configuration of instructions (attributes).
* Exception safety is not everywhere managed as it should.
* Having a simple `Sequence` at the root of a procedure might not be ideal in all use cases. `ParallelSequence` makes a lot of sense there too in many cases. Options:
  * Remove the `Root` element altogether. Instruction names will appear directly under the `Procedure` element. This makes parsing a bit harder if we intend to add more than just the `Workspace`.
  * Add attributes to the `Root` sequence that determine its behaviour (e.g. `mode="parallel"` to indicate a ParallelSequence). This requires custom code for each type of instruction that needs to be supported at the root. It also looks quite asymmetric (attributes determining the type of instruction in the root, while in the rest of the procedure, this is determined by the element name). I believe this option is only viable if a very limited set of instructions should be supported at the root (e.g. only `Sequence` and `ParallelSequence`).
* Implement `ResetHook` in `ParallelSequence` (be careful with `RUNNING` children).
