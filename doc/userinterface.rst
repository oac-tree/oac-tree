UserInterface
=============

The ``UserInterface`` class represents an interface for interacting with the user during the execution of instructions within a software system. It provides methods to request user input, display messages, and log information. Implementations of this interface can be created to suit different user interaction scenarios.

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

Updating Instruction Status
^^^^^^^^^^^^^^^^^^^^^^^^^^^

   The `UpdateInstructionStatus` method is called when the execution status of an instruction changes. Implement this method to handle any updates related to instruction execution.

.. code-block:: c++

   virtual void UpdateInstructionStatus(const Instruction* instruction) override {
       // Implement the logic for handling instruction status updates here
   }

Handling Variable Updates
^^^^^^^^^^^^^^^^^^^^^^^^^
   When a workspace variable receives a value update, the `VariableUpdated` method is called. Implement this method to handle updates related to workspace variables.

.. code-block:: c++

   virtual void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                                bool connected) override {
       // Implement the logic for handling variable updates here
   }

Requesting and Displaying Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   Use the `PutValue` and `GetUserValue` methods to interact with the user for value input and output. Implement these methods to handle user value interactions.

.. code-block:: c++

   virtual bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override {
       // Implement logic to display the value to the user
       return true; // Return true on successful retrieval of the value
   }

   virtual bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override {
       // Implement logic to request the user to input a value
       return true; // Return true on successful retrieval of the value
   }

Requesting User Choice
^^^^^^^^^^^^^^^^^^^^^^
   The `GetUserChoice` method is used to request the user to choose one of the provided options. Implement this method to handle user choice interactions.

.. code-block:: c++

   virtual int GetUserChoice(const std::vector<std::string>& options,
                             const sup::dto::AnyValue& metadata = {}) override {
       // Implement logic to present options to the user and retrieve their choice
       return -1; // Return the index of the chosen option or -1 if not chosen
   }

Displaying Messages and Logging
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   Use the `Message` and `Log` methods to display messages and log information. Implement these methods to handle message display and logging.

.. code-block:: c++

   virtual void Message(const std::string& message) override {
       // Implement logic to display the message to the user
   }

   virtual void Log(int severity, const std::string& message) override {
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
