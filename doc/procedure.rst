Procedure
=========

The ``Procedure`` class represents a sequence of instructions to be executed within the Sequencer. It provides methods to manage the execution flow, handle variables, and control the execution status. A procedure can include a series of top-level instructions, each with its own set of operations and logic.

Architecture
------------

The ``Procedure`` class is responsible for managing a collection of instructions. It contains a vector of top-level instructions, each represented by a derived class of the ``Instruction`` class. The class also includes an internal ``Workspace`` that holds variables and their values during the execution of instructions.

Usage
-----

Creating a Procedure
^^^^^^^^^^^^^^^^^^^^
   To create a new procedure, instantiate an object of the ``Procedure`` class. Optionally, you can specify the filename if the procedure is loaded from a file:

.. code-block:: c++

   Procedure myProcedure("example.procedure");


Managing Instructions
^^^^^^^^^^^^^^^^^^^^^
   You can add, retrieve, and remove instructions in the procedure. The procedure allows managing instructions at the top level.

.. code-block:: c++

   // Add a new instruction to the top level
   std::unique_ptr<Instruction> myInstruction = std::make_unique<MyInstruction>();
   myProcedure.PushInstruction(std::move(myInstruction));

   // Retrieve the root instruction
   Instruction* root = myProcedure.RootInstruction();

   // Remove an instruction from a specific position
   std::unique_ptr<Instruction> removedInstruction = myProcedure.TakeInstruction(0);

Managing Variables
^^^^^^^^^^^^^^^^^^
   The procedure can hold and manage variables using the internal workspace. Users can add, retrieve, and list variables associated with the procedure.

.. code-block:: c++

   // Add a new variable to the procedure's workspace
   std::unique_ptr<Variable> myVariable = std::make_unique<MyVariable>();
   myProcedure.AddVariable("var1", std::move(myVariable));

   // Retrieve the value of a variable
   sup::dto::AnyValue value;
   if (myProcedure.GetVariableValue("var1", value)) {
       // Successfully retrieved the value
   }

   // List all variable names in the procedure
   std::vector<std::string> variableNames = myProcedure.VariableNames();

Execution Control
^^^^^^^^^^^^^^^^^
   The procedure can be executed step-by-step using the ``ExecuteSingle`` method. This allows controlling the execution flow and interacting with user interfaces if necessary. The ``Setup`` method prepares the procedure for execution. The procedure can also be halted and reset.

.. code-block:: c++

   // Setup the procedure before execution
   myProcedure.Setup();

   // Execute a single step of the procedure
   UserInterface ui; // Assume UserInterface is properly implemented
   myProcedure.ExecuteSingle(ui);

   // Halt the procedure's execution
   myProcedure.Halt();

   // Reset the procedure and wait for all asynchronous instructions to finish
   myProcedure.Reset();

Procedure Status and Control
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   Users can retrieve the execution status of the root instruction to determine whether the procedure is still running, has completed, or encountered an error.

.. code-block:: c++

   // Get the status of the root instruction
   ExecutionStatus status = myProcedure.GetStatus();

Attributes
^^^^^^^^^^
   The procedure can have attributes associated with it. Attributes are key-value pairs that store additional information about the procedure.

.. code-block:: c++

   // Add an attribute to the procedure
   myProcedure.AddAttribute("version", "1.0");

   // Retrieve the value of an attribute
   std::string version = myProcedure.GetAttributeString("version");

Remember to refer to the specific derived classes for instructions and variables to understand their specific implementations and functionality.

Additional Notes
----------------

The ``Procedure`` class supports various additional features, such as setting up preamble information, registering types and plugins, and handling callbacks for variable updates. Users can refer to the specific class methods, presented in the following section, for more details on these advanced features.


Class definition
----------------

   Next is presented the definition of the ``Procedure`` class and its main methods.

.. doxygenclass:: sup::sequencer::Procedure
   :members:
