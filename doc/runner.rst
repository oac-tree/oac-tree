Runner
======

The ``Runner`` class represents a component that aggregates a ``Procedure`` and a ``UserInterface``, allowing the execution of the procedure with user interaction and the ability to set breakpoints for debugging purposes. The class provides methods to control the execution of the procedure, handle breakpoints, and execute the procedure step by step.

Usage
-----

Creating a Runner
^^^^^^^^^^^^^^^^^
   To use the ``Runner``, you need to create an instance of this class, passing a reference to a ``UserInterface`` object in its constructor.

.. code-block:: c++

   UserInterface myUserInterface;
   Runner myRunner(myUserInterface);

Setting the Procedure
^^^^^^^^^^^^^^^^^^^^^
   Use the ``SetProcedure`` method to set the procedure that will be executed by the runner. Pass a pointer to the ``Procedure`` object that you want to execute.

.. code-block:: c++

   Procedure myProcedure;
   myRunner.SetProcedure(&myProcedure);

Setting a Tick Callback
^^^^^^^^^^^^^^^^^^^^^^^
   The ``SetTickCallback`` method allows you to set a callback function that will be called after each execution step. The callback function should have the signature ``void(const Procedure&)``.

.. code-block:: c++

   void MyTickCallback(const Procedure& procedure) {
       // Implement logic for actions to perform after each execution step
   }

   myRunner.SetTickCallback(MyTickCallback);

Managing Breakpoints
^^^^^^^^^^^^^^^^^^^^
   Use the methods ``SetBreakpoint``, ``RemoveBreakpoint`` and ``GetBreakpoints`` to manage breakpoints within the procedure. Breakpoints are points in the code where the execution will pause to allow new procedures to be verified and tested at the set breakpoints. It is important to note that these methods should not be called during execution. They are intended to be used when the procedure is paused, before/after a single execution step, or after a breakpoint has been triggered.

.. code-block:: c++

   const Instruction* instructionToBreak = myProcedure.RootInstruction();
   myRunner.SetBreakpoint(instructionToBreak);

   std::vector<breakpoint> breaks = myRunner.GetBreakpoints();

   myRunner.RemoveBreakpoint(instructionToBreak);

Executing the Procedure
^^^^^^^^^^^^^^^^^^^^^^^
   To execute the entire procedure, use the ``ExecuteProcedure`` method. The procedure will run until it finishes or encounters a breakpoint.

.. code-block:: c++

   myRunner.ExecuteProcedure();

Executing Single Instruction
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   Use the ``ExecuteSingle`` method to execute a single instruction within the procedure. This method allows you to step through the procedure execution, particularly useful for debugging.

.. code-block:: c++

   myRunner.ExecuteSingle();

Pausing and Halting Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   You can pause the procedure execution using the ``Pause`` method, and you can halt the procedure using the ``Halt`` method.

.. code-block:: c++

   myRunner.Pause();
   // To resume, call ExecuteProcedure again.
   myRunner.Halt();

Checking Execution Status
^^^^^^^^^^^^^^^^^^^^^^^^^
   The ``IsFinished`` and ``IsRunning`` methods allow you to check the current execution status of the procedure.

.. code-block:: c++

   bool finished = myRunner.IsFinished();
   bool running = myRunner.IsRunning();

Additional Notes
----------------

- The ``Runner`` class includes the ``TickCallback`` type, which represents a function that will be called after each execution step. You can set this callback using the ``SetTickCallback`` method. The callback function takes a `const Procedure&` parameter, allowing you to access information about the procedure's state after each step.

- The ``TimeoutWhenRunning`` class is provided as a callback for in-between ticks. It performs a fixed timeout when the procedure reports a running status during asynchronous operations.

Class definition
----------------

   Next is presented the definition of the ``Runner`` class and its main methods.

.. doxygenclass:: sup::sequencer::Runner
   :members:   Runner, SetProcedure, SetTickCallback, SetBreakpoint,
	       RemoveBreakpoint, GetBreakpoints, ExecuteProcedure,
	       ExecuteSingle, Halt, Pause, IsFinished, IsRunning

.. doxygenclass:: sup::sequencer::TimeoutWhenRunning
   :members:
