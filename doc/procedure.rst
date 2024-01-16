Procedure
=========

The ``Procedure`` class represents instruction trees to be executed within the Sequencer and a workspace of globally accessible variables (see :ref:`Variable`). It provides methods to handle variables and instructions, and to manage the execution flow. A procedure can include multiple top-level instructions, where at most one is defined to be the root instruction that will be executed on procedure execution.

Architecture
------------

The ``Procedure`` class is responsible for managing a collection of instruction trees. It contains a vector of top-level instructions, each represented by a derived class of the ``Instruction`` class. The class also includes an internal ``Workspace`` that holds variables and their values during the execution of instructions.

Usage
-----

Creating a Procedure
^^^^^^^^^^^^^^^^^^^^

To create a new procedure, instantiate an object of the ``Procedure`` class. Optionally, you can specify the filename if the procedure is loaded from a file:

.. code-block:: c++

   Procedure my_procedure("example.procedure");

The optional filename argument shall be provided when the procedure is constructed from a file and it contains special include instructions pointing to other files via relative path names.

Managing Instructions
^^^^^^^^^^^^^^^^^^^^^

You can add, retrieve, and remove instructions in the procedure. The procedure allows managing instructions at the top level.

.. code-block:: c++

   // Add a new instruction to the top level
   auto my_instruction = GlobalInstructionRegistry().Create("MyInstruction");
   my_procedure.PushInstruction(std::move(my_instruction));

   // Retrieve the root instruction
   Instruction* root = my_procedure.RootInstruction();

   // Remove an instruction from a specific position
   std::unique_ptr<Instruction> removed_instruction = my_procedure.TakeInstruction(0);

Managing Variables
^^^^^^^^^^^^^^^^^^

The procedure can hold and manage variables using the internal workspace. Users can add, retrieve, and list variables associated with the procedure's workspace.

Variables are registered in the global variable registry using their type names. In the code snippet below, it is assumed that `class MyVariable : public Variable` was provided and registered in the global variable registry under the name "MyVariable".

.. code-block:: c++

   // Add a new variable to the procedure's workspace
   auto my_variable = GlobalVariableRegistry().Create("MyVariable");
   my_procedure.AddVariable("var1", std::move(my_variable));

   // Retrieve the value of a variable
   sup::dto::AnyValue value;
   if (my_procedure.GetVariableValue("var1", value)) {
       // Successfully retrieved the value
   }

   // List all variable names in the procedure
   std::vector<std::string> variable_names = my_procedure.VariableNames();

Execution Control
^^^^^^^^^^^^^^^^^

The procedure can be executed step-by-step using the ``ExecuteSingle`` method. This allows controlling the execution flow and validating the procedure at each step. The ``Setup`` method prepares the procedure for execution. The procedure can also be halted and reset.

.. code-block:: c++

   // Setup the procedure before execution
   my_procedure.Setup();

   // Execute a single step of the procedure
   UserInterface ui; // Assume UserInterface is properly implemented
   my_procedure.ExecuteSingle(ui);

   // Halt the procedure's execution
   my_procedure.Halt();

   // Reset the procedure and wait for all asynchronous instructions to finish
   my_procedure.Reset(ui);

Procedure Status and Control
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Users can retrieve the execution status of the root instruction to determine whether the procedure is still running, has completed, or encountered an error.

.. code-block:: c++

   // Get the status of the root instruction
   ExecutionStatus status = my_procedure.GetStatus();

Attributes
^^^^^^^^^^

The procedure can have attributes associated with it. Attributes are key-value pairs that store additional information about the procedure.

.. code-block:: c++

   // Add an attribute to the procedure
   my_procedure.AddAttribute("version", "1.0");

   // Retrieve the value of an attribute
   std::string version = my_procedure.GetAttributeString("version");

Additional Notes
----------------

The ``Procedure`` class supports various additional features, such as setting up preamble information, registering types and plugins, and handling callbacks for variable updates. Users can refer to the specific class methods, presented in the following section, for more details on these advanced features.

Procedures can be serialized to XML files and restored back using helper functions from `sequence_parser.h`.

Class definition
----------------

Next is presented the definition of the ``Procedure`` class and its main methods.

.. doxygenclass:: sup::sequencer::Procedure
   :members:
