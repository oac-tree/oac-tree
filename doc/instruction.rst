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

Usage
-----

For compound and decorator instructions, specialisations of the ``Instruction`` class are provided: ``CompoundInstruction`` and ``DecoratorInstruction``. Since these specialisations only provide common implementation for handling child instructions (and forwarding certain calls to their children), the remainder of this page will focus on action instructions and will use a simple concrete example: the ``Wait`` instruction.

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

The execution of instruction trees follows a model where the root of the tree is *ticked* until that root reports a status that signifies the termination of the tree's execution. Compound and decorator instructions will propagate these *ticks* to their appropriate child instructions. A single tick of an instruction result in calling the ``ExecuteSingle`` member function on the instruction, which is a non-virtual implementation that ends up calling the private member function ``ExecuteSingleImpl``.

For example, to execute the wait instruction:

.. code-block:: c++

   // Assume the existence of a UserInterface implementation, called MyUserInterface
   MyUserInterface ui;
   Workspace ws;
   // Send a single tick to the wait instruction
   wait->ExecuteSingle(ui, ws);

The ``ExecuteSingle`` function takes two reference parameters:

* UserInterface reference: to allow input/output and error logging;
* Worspace reference: to be able to access workspace variables.

Managing Attributes
^^^^^^^^^^^^^^^^^^^

The `Instruction` class supports the same attribute system as ``Variable``: see :ref:`Attribute System`. Users can set, retrieve, and manipulate attributes using various attribute-related methods:

.. code-block:: c++

   // Add attribute to the wait instruction
   wait->AddAttribute("timeout", "1.0");

   // Retrieve attribute value
   double timeout = wait->GetAttributeValue<double>("timeout");

Resetting a Instruction
^^^^^^^^^^^^^^^^^^^^^^^

The `Reset` method puts the instruction in a state to be executed anew. This state corresponds to its state jsut after the last ``Setup`` was called. Note that this is different from how ``Variable::Reset`` is defined.

Resetting an instruction is mainly used when the same instruction needs to be executed multiple times: after each full execution, i.e. status of instruction indicates it is finished, the instruction is reset before the next execution can start.

.. code-block:: c++

   wait->Reset(); // Reset the wait instruction

Class definition
----------------

Next is presented the definition of the ``Instruction`` class and its main methods.

.. doxygenclass:: sup::sequencer::Instruction
   :members:
