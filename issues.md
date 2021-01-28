# Issues

* Rename ExecutionStatus enumerators:
  * `UNDEFINED`: means not started yet,
  * `STARTED`: should mean waiting for next tick.
* Handle termination in ParallelSequence
* Replace hardcoded 100ms in Runner.cpp by configuration parameter from xml.