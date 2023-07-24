Variable
========

.. contents::
   :local:


The ``Variable`` class is an interface for workspace variables, designed to manage and manipulate various types of variables within a software system. It provides an abstract representation of variables with different types and allows users to interact with them while ensuring thread safety for specific methods.

Architecture
------------

The ``Variable`` class is based on the non-virtual interface (NVI) idiom to support the distinction between thread-safe and non-thread-safe methods. It employs locks to provide thread safety for certain public methods that can be called during execution, ensuring that multiple threads can safely access these methods concurrently. Other methods should be called only from a single thread to prevent data corruption or race conditions.

Construction
------------

The ``Variable`` class allows users to define variables of different types. Since it is an abstract base class, the specific types are implemented in derived classes. To create a specific type of variable, users can instantiate an instance of the corresponding derived class.

Usage
-----

Two specialisations of the ``Variable`` class are included in **Sequencer**. They are the `local variable` and the `file variable`. The implementation of `Local Variable` will be used to provide examples. 

Creating a Variable
^^^^^^^^^^^^^^^^^^^^
Variables are typically created by instantiating the derived classes representing specific variable types. For example, to create a NumericVariable, use the constructor and provide the type:

.. code-block:: c++

		LocalVariable locVar("Integer");

Setting and Getting Variable Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   Users can access and modify the value of a variable using the ``GetValue`` and ``SetValue`` methods, respectively. These methods provide a way to read and update variable values in a controlled manner.

.. code-block:: c++

   // Get the current value of the numeric variable
    sup::dto::AnyValue value;
    LocVar.GetValue(value);

   // Modify the value
   value = {{"int", {SignedInteger8Type, 1}}};
   LocVar.SetValue(value);

Checking Variable Availability
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   The availability of a variable depends on the specific derived class implementation. Users can check if the variable is available using the ``IsAvailable`` method.

.. code-block:: c++

   if (LocVar.IsAvailable()) {
       // Variable is available
   } else {
       // Variable is not available
   }

Registering Callback for Value Updates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
   Users can register a callback function to be notified of value updates using the ``SetNotifyCallback`` method. This allows for real-time responsiveness to changes in variable values.

.. code-block:: c++

   // Define a callback function
   void OnValueUpdate(const sup::dto::AnyValue& value, bool connected) {
       // Handle the value update
       // ...
   }

   LocVar.SetNotifyCallback(OnValueUpdate); // Register the callback function

Managing Attributes
^^^^^^^^^^^^^^^^^^^
   The `Variable` class supports managing attributes associated with variables. Users can set, retrieve, and manipulate attributes using various attribute-related methods.

 .. code-block:: c++

   // Add attributes to the numeric variable
   LocVar.AddAttribute("units", "kg");
   LocVar.AddAttribute("precision", "2");

   // Retrieve attribute values
   std::string units = LocVar.GetAttributeString("units");
   int precision = LocVar.GetAttributeValue<int>("precision");

   
Resetting a Variable
^^^^^^^^^^^^^^^^^^^^
   The `Reset` method resets the variable to its initial state, similar to when it was first created. For example, it can disconnect from external resources, clear values, and reset any stateful attributes.

.. code-block:: c++

   LocVar.Reset(); // Reset the numeric variable



 
Class definition
----------------

   Next is presented the definition of the ``Variable`` class and its main methods.

.. doxygenclass:: sup::sequencer::Variable
   :members:





