Runner
======

The ``Runner`` class represents a component that aggregates a ``Procedure`` and a ``UserInterface``, allowing the execution of the procedure with input/output directed from/to the concrete user interface class and the ability to set breakpoints for debugging purposes. The class provides methods to control the execution of the procedure, handle breakpoints, and execute the procedure step by step.

Usage
-----

Creating a Runner
^^^^^^^^^^^^^^^^^

To use the ``Runner``, you need to create an instance of this class, passing a reference to a ``UserInterface`` object in its constructor.

.. code-block:: c++

   UserInterface my_user_interface;
   Runner my_runner(my_user_interface);

Setting the Procedure
^^^^^^^^^^^^^^^^^^^^^

Use the ``SetProcedure`` method to set the procedure that will be executed by the runner. Pass a pointer to the ``Procedure`` object that you want to execute.

.. code-block:: c++

   Procedure my_procedure;
   my_runner.SetProcedure(&my_procedure);

Setting a Tick Callback
^^^^^^^^^^^^^^^^^^^^^^^

The ``SetTickCallback`` method allows you to set a callback function that will be called after each execution step. The callback function should have the signature ``void(const Procedure&)``. The callback can be used to query procedure status, the list of next instructions t execute, etc.

.. code-block:: c++

   void MyTickCallback(const Procedure& procedure) {
       // Implement logic for actions to perform after each execution step
   }

   my_runner.SetTickCallback(MyTickCallback);

Managing Breakpoints
^^^^^^^^^^^^^^^^^^^^

Use the methods ``SetBreakpoint``, ``RemoveBreakpoint`` and ``GetBreakpoints`` to manage breakpoints within the procedure. Breakpoints are points in the instruction tree where the execution will pause to allow procedures to be verified and tested at the set breakpoints. It is important to note that these methods should not be called during execution. They are intended to be used when the procedure is paused, before/after a single execution step, or after a breakpoint has been triggered.

.. code-block:: c++

   const Instruction* instruction_to_break = my_procedure.RootInstruction();
   my_runner.SetBreakpoint(instruction_to_break);

   std::vector<breakpoint> breaks = my_runner.GetBreakpoints();

   my_runner.RemoveBreakpoint(instruction_to_break);

Executing the Procedure
^^^^^^^^^^^^^^^^^^^^^^^

To execute the entire procedure, use the ``ExecuteProcedure`` method. The procedure will run until it finishes or encounters a breakpoint.

.. code-block:: c++

   my_runner.ExecuteProcedure();

Executing Single Instruction
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Use the ``ExecuteSingle`` method to execute a single instruction within the procedure. This method allows you to step through the procedure execution, particularly useful for debugging.

.. code-block:: c++

   my_runner.ExecuteSingle();

Pausing and Halting Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can pause the procedure execution using the ``Pause`` method, and you can halt the procedure using the ``Halt`` method.

.. code-block:: c++

   my_runner.Pause();
   // To resume, call ExecuteProcedure again.
   my_runner.Halt();

Checking Execution Status
^^^^^^^^^^^^^^^^^^^^^^^^^

The ``IsFinished`` and ``IsRunning`` methods allow you to check the current execution status of the procedure.

.. code-block:: c++

   bool finished = my_runner.IsFinished();
   bool running = my_runner.IsRunning();

Note that ``IsRunning`` has a very specific meaning in the context of the Sequencer: this member function returns ``true`` only when parts of the executing instruction tree are being run in a separate thread. This function is used to distinguish between instructions waiting to be ticked again and instructions that may require some time to finish before they can proceed. In this way, busy loops can be avoided, i.e. continuously ticking an instruction tree that is being run in a separate thread.

Additional Notes
----------------

- The ``Runner`` class includes the ``TickCallback`` type, which represents a function that will be called after each execution step. You can set this callback using the ``SetTickCallback`` method. The callback function takes a `const Procedure&` parameter, allowing you to access information about the procedure's state after each step.

- The ``TimeoutWhenRunning`` class is provided as a standard callback for in-between ticks. It performs a fixed timeout when the procedure reports a running status during asynchronous operations.

Class definition
----------------

   Next is presented the definition of the ``Runner`` class and its main methods.

.. doxygenclass:: sup::sequencer::Runner
   :members:   Runner, SetProcedure, SetTickCallback, SetBreakpoint,
               RemoveBreakpoint, GetBreakpoints, ExecuteProcedure,
               ExecuteSingle, Halt, Pause, IsFinished, IsRunning

.. doxygenclass:: sup::sequencer::TimeoutWhenRunning
   :members:
