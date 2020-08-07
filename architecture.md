# Functional analysis

## High-level use cases

* Create, edit and run basic operational tasks.
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
