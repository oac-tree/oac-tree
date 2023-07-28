Instruction
===========

.. contents::
   :local:


The ``Instruction`` class is an interface for all executable instructions in a procedure. It provides an abstract representation of actions that can be executed through a uniform API.

There are three main categories of instructions:

* Compound instructions: instructions that can have multiple child instructions;
* Decorator instructions: instructions that have exactly one child instruction;
* Actions: these are the leafs of instruction trees and represent an concrete action to be taken, while compound and decorator instructions represent the overall execution logic.

Architecture
------------

The ``Instruction`` class is based on the non-virtual interface (NVI) idiom to allow for a simpler API to override by implementers of concrete instructions.

The ``Instruction`` class allows users to define instructions that are specific to the environment where the **Sequencer** will be used. Since it is an abstract base class, concrete instructions are implemented in derived classes.

Execution Status
----------------

Each instruction has an ``ExecutionStatus`` enumeration that indicates in which phase of execution the instruction currently is:

* `NOT_STARTED`: The instruction is ready for its first execution. This is the state of an instruction that was never executed or just reset.
* `NOT_FINISHED`: The instruction already received at least one tick, but has not yet finished and is waiting to be ticked again. This state often indicates compound instructions. For example, a `Sequence` instruction that was ticked once, but has multiple child instructions, will report this status, since only its first child was ticked. It expects to be ticked again, so it can propagate those ticks to the other child instructions.
* `RUNNING`: The instruction, or one of its descendants (child or descendants of child) is executing in a separate thread. The instruction is not truly waiting to be ticked again, but must be ticked at some point to check if those threads have finished executing. The difference with `NOT_FINISHED` is that it would not be beneficial to continuously tick an instruction with the `RUNNING` status, as this would consume unnecessary resources. Typically, a small timeout is used between ticks of an instruction that return this status.
* `SUCCESS`: the instruction's execution finished successfully.
* `FAILURE`: the instruction's execution finished with a failure.

Usage
-----

For compound and decorator instructions, specialisations of the ``Instruction`` class are provided: ``CompoundInstruction`` and ``DecoratorInstruction``. Since these specialisations only provide a common implementation for handling child instructions (and forwarding certain calls to their children), the remainder of this page will focus on action instructions and will use a simple concrete example: the ``Wait`` instruction.

Note that many more specialized instructions are provided by plugins.

Creating an Instruction
^^^^^^^^^^^^^^^^^^^^^^^

To obtain an instance of an existing instruction type, provided by the core library or one of the loaded plugins, one typically asks an ``InstructionRegistry`` to instantiate it:

.. code-block:: c++

   auto wait = GlobalInstructionRegistry().Create("Wait");

Instruction Initialization
^^^^^^^^^^^^^^^^^^^^^^^^^^

Before instructions can be executed, they need to be properly initialized first to ensure that they contain the proper attributes to be able to execute. This is done by calling the ``Setup`` method on the instruction. Initialization typically consists of checking attribute presence, attribute constraints and specialized parsing of some attribute values.

The ``Setup`` member function takes a reference to a procedure as an argument. This procedure reference can be used for complex initialization that requires the context of the instruction that is being set up: for example, ``Include`` instructions need to know from where the current procedure was loaded to be able to load instructions/procedures from disk using a relative path.

.. code-block:: c++

   wait->Setup(proc);

Executing Instructions
^^^^^^^^^^^^^^^^^^^^^^

The execution of instruction trees follows a model where the root of the tree is *ticked* until that root reports a status that signifies the termination of the tree's execution. Compound and decorator instructions will propagate these *ticks* to their appropriate child instructions. A single tick of an instruction results in a single call to the ``ExecuteSingle`` member function of the instruction, which is a non-virtual implementation that ends up calling the private member function ``ExecuteSingleImpl`` (see also `Execution Logic`_).

For example, to execute the wait instruction:

.. code-block:: c++

   // Assume the existence of a UserInterface implementation, called MyUserInterface
   MyUserInterface ui;
   Workspace ws;
   // Send a single tick to the wait instruction
   wait->ExecuteSingle(ui, ws);

The ``ExecuteSingle`` function takes two reference parameters:

* UserInterface reference: to allow input/output and error logging;
* Workspace reference: to be able to access workspace variables.

Managing Attributes
^^^^^^^^^^^^^^^^^^^

The `Instruction` class supports the same attribute system as ``Variable``: see :ref:`Attribute System`. Users can set, retrieve, and manipulate attributes using various attribute-related methods:

.. code-block:: c++

   // Add attribute to the wait instruction
   wait->AddAttribute("timeout", "1.0");

   // Retrieve attribute value
   double timeout = wait->GetAttributeValue<double>("timeout");

Halting an Instruction
^^^^^^^^^^^^^^^^^^^^^^

The `Halt` method tries to stop the execution of an instruction. This only makes sense for instructions with status equal to `RUNNING`, as otherwise the caller is in the same thread as the instruction's execution implying that the instruction is not currently executing anything. However, a compound instruction may have descendants running in seperate threads, i.e. with status `RUNNING`, and still report a status of `NOT_FINISHED`, to indicate that there are descendants waiting to be ticked.

Implementers of custom instructions should try to regularly check the protected function ``IsHaltRequested`` to prevent blocking the execution needlessly.

.. code-block:: c++

   // Halt the wait instruction. Note that this has no effect here, since we're in the same thread.
   wait->Halt();

Resetting an Instruction
^^^^^^^^^^^^^^^^^^^^^^^^

The `Reset` method puts the instruction in a state to be executed anew. This state corresponds to its state just after the last ``Setup`` was called. Note that this is different from how ``Variable::Reset`` is defined.

Resetting an instruction is mainly used when the same instruction needs to be executed multiple times: after each full execution, i.e. status of instruction indicates it is finished, the instruction is reset before the next execution can start.

.. code-block:: c++

   wait->Reset(); // Reset the wait instruction

Execution Logic
---------------

The non-virtual implementation of ``Instruction::ExecuteSingle`` provides a uniform way of managing the instruction's execution status, calling specific (virtual) hooks on the instruction and notifying the UserInterface of any status updates.

During a single tick, the ``ExecuteSingle`` function will perform the following actions:

* If the status is `NOT_STARTED`, i.e. this is the first time the instruction will be ticked (or just after a reset), the virtual method ``InitHook`` is called, which can be overriden in case some extra code needs to run the first time. Afterwards, the status is put to `NOT_FINISHED` and the UserInterface is notified of this status change.
* The status is updated with the result of the virtual ``ExecuteSingleImpl`` method. This function needs to be overriden by all concrete instructions and should contain the main execution logic.
* If the previous step resulted in a change of status, the UserInterface is notified of this change.

Class definition
----------------

Next is presented the definition of the ``Instruction`` class and its main methods.

.. doxygenclass:: sup::sequencer::Instruction
   :members:
