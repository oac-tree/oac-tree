UserInterface
=============

The ``UserInterface`` class represents the interface for all input/output during the execution of instructions within a software system. It provides methods to request user input, display messages, and log information. Implementations of this interface can be created to suit different user interaction scenarios, e.g. command line interfaces or GUIs.

Architecture
------------

The ``UserInterface`` class is based on the non-virtual interface (NVI) idiom to support the distinction between thread-safe and non-thread-safe methods. It employs locks to provide thread safety for certain public methods that can be called during execution, ensuring that multiple threads can safely access these methods concurrently. Other methods should be called only from a single thread to prevent data corruption or race conditions.

Usage
-----

Creating a UserInterface
^^^^^^^^^^^^^^^^^^^^^^^^

To use the `UserInterface`, you need to create an implementation of this class that provides concrete implementations for its virtual methods. For example, you can create a class `MyUserInterface` that inherits from `UserInterface` and overrides its virtual methods.

.. code-block:: c++

   class MyUserInterface : public UserInterface {
       // Implement the virtual methods of UserInterface here
   };

Alternatively, one can derive from `DefaultUserInterface`, which provides default empty implementations of all pure virtual methods in `UserInterface`. This allows one to implement only the methods that are relevant for the use case at hand.

Updating Instruction Status
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The `UpdateInstructionStatus` method is called when the execution status of an instruction changes. Implement this method to handle any updates related to instruction execution.

.. code-block:: c++

   void UpdateInstructionStatus(const Instruction* instruction) override {
       // Implement the logic for handling instruction status updates here
   }

Handling Variable Updates
^^^^^^^^^^^^^^^^^^^^^^^^^

When a workspace variable receives a value update, the `VariableUpdated` method is called. Implement this method to handle updates related to workspace variables.

.. code-block:: c++

   void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                                bool connected) override {
       // Implement the logic for handling variable updates here
   }

Displaying Values
^^^^^^^^^^^^^^^^^

Use the `PutValue` method to interact with the user for value output. Implement this method to handle such interactions.

.. code-block:: c++

   bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override {
       // Implement logic to display the value to the user
       return true; // Return true on successful retrieval of the value
   }

Requesting User Input
^^^^^^^^^^^^^^^^^^^^^

The `RequestUserInput` method is used to request the user to provide input. It returns a future of type `IUserInputFuture` that needs to be checked for readiness before retrieving its value. This allows instructions to properly exit when they are halted, instead of blocking on user input methods.

Implementations will typically use a member `AsyncInputWrapper` object to implement this method.

Displaying Messages and Logging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Use the `Message` and `Log` methods to display messages and log information. Implement these methods to handle message display and logging.

.. code-block:: c++

   void Message(const std::string& message) override {
       // Implement logic to display the message to the user
   }

   void Log(int severity, const std::string& message) override {
       // Implement logic to log the message with the specified severity level
   }

Remember to properly implement each of the virtual methods in your derived `MyUserInterface` class according to your specific user interaction requirements.

Additional Notes
----------------

The `UserInterface` class provides a non-virtual interface for most of its methods, meaning that it wraps the virtual methods with non-virtual ones. The virtual methods must be implemented in your derived class, while the non-virtual methods can be called directly in your application code to interact with the user interface.

The `UserInterface` class also includes helper methods related to user choice metadata, which can be used to provide additional information to the user interface for displaying user choice options.

Class definition
----------------

Next is presented the definition of the ``UserInterface`` class and its main methods.

.. doxygenclass:: sup::sequencer::UserInterface
   :members:
