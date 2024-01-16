Instruction
===========

.. contents::
   :local:


The ``Instruction`` class is an interface for all executable instructions in a procedure. It provides an abstract representation of actions that can be executed through a uniform API.

There are three main categories of instructions:

Compound instruction
  Instruction that can have multiple child instructions.

Decorator instruction
  Instruction that has exactly one child instruction.

Action
  These are the leafs of instruction trees and represent a concrete action to be taken. They have no child instructions.

Compound instructions are intended for executing their children with a certain execution logic depending on the implementation (simple sequence, parallel execution, execute until at least one child succeeds, etc). The decorator instruction can be considered as a modifier of the behaviour of the enclosed child (e.g. turn failure into success). Actions, depending on implementation, can perform standard simple operations (wait, print log message, etc) or take care of complex activity (compare variables in sequencer workspace).

Architecture
------------

The ``Instruction`` class is based on the non-virtual interface (NVI) idiom to allow for a simpler API to override by implementers of concrete instructions.

The ``Instruction`` class allows users to define instructions that are specific to the environment where the **Sequencer** will be used. Since it is an abstract base class, concrete instructions are implemented in derived classes.

Execution Status
----------------

Each instruction has an ``ExecutionStatus`` enumeration that indicates in which phase of execution the instruction currently is:

NOT_STARTED
  The instruction is ready for its first execution. This is the state of an instruction that was never executed or just reset.

NOT_FINISHED
  The instruction already received at least one tick, but has not yet finished and is waiting to be ticked again. This state often indicates compound instructions. For example, a `Sequence` instruction that was ticked once, but has multiple child instructions, will report this status, since only its first child was ticked. It expects to be ticked again, so it can propagate those ticks to the other child instructions. In compound instructions, this status takes precedence over the `RUNNING` status to allow for immediate ticking of child instructions that are waiting.

RUNNING
  The instruction, or one of its descendants (child or descendants of child) is executing in a separate thread. The instruction is not truly waiting to be ticked again, but must be ticked at some point to check if those threads have finished executing. The difference with `NOT_FINISHED` is that it would not be beneficial to continuously tick an instruction with the `RUNNING` status, as this would consume unnecessary resources. Typically, a small timeout is used between ticks of an instruction that return this status.

SUCCESS
  The instruction's execution finished successfully.

FAILURE
  The instruction's execution finished with a failure. Note that `FAILURE` is not meant to carry a negative connotation and simply means that the expected result has not been achieved. For example, an instruction waiting for a certain condition with a timeout will typically report `FAILURE` if the timeout expires before reaching the condition.

Execution Logic
---------------

The non-virtual implementation of ``Instruction::ExecuteSingle`` provides a uniform way of managing the instruction's execution status, calling specific (virtual) hooks on the instruction and notifying the UserInterface of any status updates.

During a single tick, the ``ExecuteSingle`` function will perform the following actions:

* If the status is `NOT_STARTED`, i.e. this is the first time the instruction will be ticked (or just after a reset), the virtual method ``InitHook`` is called, which can be overriden in case some extra code needs to run the first time. Afterwards, if the initialization was successful, the status is put to `NOT_FINISHED` and the UserInterface is notified of this status change. If the initialization failed, the status is set to `FAILURE` and the UserInterface is notified.
* If the initialzation was successful, the status is updated with the result of the virtual ``ExecuteSingleImpl`` method. This function needs to be overriden by all concrete instructions and should contain the main execution logic.
* If the previous step resulted in a change of status, the UserInterface is notified of this change.

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

The execution of instruction trees follows a model where the root of the tree is *ticked* until that root reports a status that signifies the termination of the tree's execution. Compound and decorator instructions will propagate these *ticks* to their appropriate child instructions. A single tick of an instruction results in a single call to the ``ExecuteSingle`` member function of the instruction, leading to a call to the private member function ``ExecuteSingleImpl`` (see also `Execution Logic`_).

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
   double timeout = SOME_DEFAULT_VALUE;
   if (!GetAttributeValueAs("timeout", ws, ui, timeout))
   {
     // some error occured, act accordingly...
   }

Halting an Instruction
^^^^^^^^^^^^^^^^^^^^^^

The `Halt` method tries to stop the execution of an instruction. It is typically used in cases where multiple instructions are being executed concurrently (e.g. by using a `ParallelSequence` compound instruction) and a terminal status (success or failure) is reached before all threads have finished executing. The framework will then try to halt the remaining ones to avoid unnecessary delays.

Implementers of custom instructions should try to regularly check the protected function ``IsHaltRequested`` to prevent blocking the execution needlessly.

.. code-block:: c++

   // Halt the wait instruction. Note that this has no effect here, since we're in the same thread.
   wait->Halt();

Resetting an Instruction
^^^^^^^^^^^^^^^^^^^^^^^^

The `Reset` method puts the instruction in a state to be executed anew. This state corresponds to its state just after the last ``Setup`` was called. Note that this is different from how ``Variable::Reset`` is defined.

Resetting an instruction is mainly used when the same instruction needs to be executed multiple times: after each full execution, i.e. status of instruction indicates it is finished, the instruction is reset before the next execution can start.

.. code-block:: c++

   MyUserInterface ui;
   wait->Reset(ui); // Reset the wait instruction

.. _Attribute System:

Attribute System
----------------

The attribute system allows to parameterize instructions in a custom way. Each instruction can declare which attributes it supports, their types, if they are mandatory and what those attributes refer to.

As an example, consider a procedure XML file containing the following instruction element:

.. code-block:: xml

   <Wait timeout="3.0"/>

During parsing, this will result in the following method calls:

.. code-block:: c++

   auto instr = GlobalInstructionRegistry().Create("Wait");
   instr->AddAttribute("timeout", "3.0");
   instr->Setup();

The attribute system also supports constraints that may result in throwing an exception during the `setup` phase. This provides feedback to the client about missed mandatory attributes, wrongly formatted ones, etc. Since all variables and instructions are initialized before execution of a procedure, this provides `fail fast` behavior.

Implementers of concrete instruction types can use protected member functions to signal which attributes are defined by the variable, which types they have, their category, if they are mandatory and other more complex constraints.

Attribute categories define how the attribute's value needs to be retrieved:

* `AttributeCategory::kLiteral`: the string value of the attribute will be parsed into the correct type,
* `AttributeCategory::kVariableName`: the string value of the attribute refers to a workspace variable field and that field's value will be fetched from the workspace,
* `AttributeCategory::kBoth`: in this case, both options are possible: if the attribute's string value starts with an `@` character, the rest will be interpreted as a variable field; otherwise, it is treated as a literal attribute.

As an example, consider creating a variable `MyInstruction`, that has three predefined attributes:

* "country": a mandatory string that can be literal or refer to a variable field;
* "max_retry": an optional unsigned integer (literal);
* "phoneVar": an optional string that refers to a variable field.

Furthermore, assume that exactly one of the attributes `max_retry` or `phoneVar` needs to be present. All this information can then be encoded in the constructor of the concrete variable:

.. code-block:: c++

   MyInstruction::MyInstruction()
     : Instruction(MyInstruction::Type)
   {
     AddAttributeDefinition("country").SetCategory(AttributeCategory::kBoth).SetMandatory();
     AddAttributeDefinition("max_retry", sup::dto::UnsignedInteger16Type);
     AddAttributeDefinition("phoneVar").SetCategory(AttributeCategory::kVariableName);
     AddConstraint(MakeConstraint<Xor>(MakeConstraint<Exists>("max_retry"),
                                       MakeConstraint<Exists>("phoneVar")));
   }

As you can see, the `country` attribute did not need to define a type, as `sup::dto::StringType` is the default. Likewise, `kLiteral` is the default category for attributes and does not need to be specified in case of the `max_retry` attribute.
The generic implementation of the ``Setup`` method will ensure that if no exceptions were thrown, all these conditions are satisfied after setup.

Furthermore, `Instruction` provides a public API to retrieve an attribute's value, taking into account its category. This means that the handling of the `@` character and the choice between literal interpretation and fetching from a variable field is performed automatically. The API consists of two methods: one that retrieves an `AnyValue` and a templated one that tries to cast to a custom type. The following example shows how this works for the above defined `MyInstruction`:

.. code-block:: c++

   ExecutionStatus MyInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
   {
     std::string country;
     if (!GetAttributeValueAs("country", ws, ui, country))
     {
       return ExecutionStatus::FAILURE;
     }
     // provide a default value for when the attribute is not present
     sup::dto::uint16 max_retry = 2;
     // note that GetAttributeValueAs does not return false when the attribute is not present
     if (!GetAttributeValueAs("max_retry", ws, ui, max_retry))
     {
       return ExecutionStatus::FAILURE;
     }
     sup::dto::AnyValue phone_nr; // defaul empty
     // now we retrieve the AnyValue from the variable field
     if (!GetAttributeValue("phoneVar", ws, ui, phone_nr))
     {
       return ExecutionStatus::FAILURE;
     }
     ...
     return ExecutionStatus::SUCCESS;
   }

.. note::

   In the case of attributes that always refer to variable fields, the indicated type of the attribute is not really important and can best be left to the default (`sup::dto::StringType`). This is because the retrieval to an `AnyValue` of the attribute will not take into account the indicated type. If `GetAttributeValueAs` is used however, that function will signal issues with conversion to the `UserInterface`.

Class definition
----------------

Next is presented the definition of the ``Instruction`` class and its main methods.

.. doxygenclass:: sup::sequencer::Instruction
   :members:
