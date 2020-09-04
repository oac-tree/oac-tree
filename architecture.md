# Functional analysis

## Drivers for the need of a sequencer

* Changeability:
  * Lessons learnt during integration, commissioning and early operation will require changes to the detailed implementation of the ITER control system. A data-driven approach can accomodate such flexibility, without compromising stability or security issues (version controlled workflows).
  * As experience is gathered during these early stages, many of the manual steps of an operational task can be gradually automated.
* Automation: routine and repetitive operations should become fully automated to ensure deterministic execution and limit human involvement to tasks with high added value.
* Re-usability and traceability: tasks should be repeatable and traceable.
* Scalability: tasks should be composable from subtasks.
* Central versus local tasks: it should be possible to maximally reuse components of the sequencer in different environments and use cases: execution by SUP (central control), local commissioning, etc. This also maximizes the reuse of subtasks (e.g. configuration of a Plant System) into more complex tasks.
* Robustness: especially during the early stages of defining the different steps of a task, there should be a possibility of checking correctness and normal operation at each consecutive step. The sequencer therefor needs to support such user interactions as: ask for confirmation to proceed, ask for user input of parameters, pauze or stop a sequence, etc.

## High-level use cases

* Create, edit and run basic operational tasks.
* Verify and debug task sequences.
* Provide a means for gradually evolving those tasks into more automated ones.
* Define a schema for executing those tasks in a data-driven way.
* Making this execution component useable for integration into CODAC Supervision.

## Design

### SequenceRunner

The SequenceRunner component contains the basic building blocks for constructing a sequence (workspace, instructions, ...). Furthermore, it is the main component that will execute the sequence.

An operational task is represented by a `Task` object that contains:
* A `Workspace` object, containing the shared variables that can be manipulated by the instructions.
* A root `Instruction` object (polymorphic). The `Instruction` interface will be subclassed to implement the basic building blocks for constructing behavior trees (sequence, selector, etc.).

Leaf `Instruction` objects will consist of:
* Basic RPC instructions,
* Specialized RPC instructions (configuration, CVVF, etc.)
* Basic user interactions (input value, wait for confirmation, etc.)

The interactions with the user depend strongly on the framework used for such interactions. To maximize the useability of the `SequenceRunner` in different environments (stand-alone application, integration in SUP, etc.), this interaction is completely decoupled and has to be injected into the `SequenceRunner`. Different types of user interaction can be injected: HMI screens, GUI application, CLI, ...

### SequenceGUI
