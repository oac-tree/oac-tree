.. _Variable:

Variable
========

.. contents::
   :local:


The ``Variable`` class is an interface for workspace variables, designed to manage and manipulate various types of variables within a software system. It provides an abstract representation of variables with different types and allows users to interact with them uniformly while ensuring thread safety for specific methods.

The Variable class uses ``sup::dto::AnyValue`` as a common interface to represent any structured data.

While all variables are globally accessible by the instructions in a procedure, the backend, i.e. where the values of these variables are stored, could be very diverse: file, memory, network, database, etc.

Architecture
------------

The ``Variable`` class is based on the non-virtual interface (NVI) idiom to support the distinction between thread-safe and non-thread-safe methods and to allow for a simpler API to override by implementers of concrete variables. It employs locks to provide thread safety for certain public methods that can be called during execution, ensuring that multiple threads can safely access these methods concurrently. Other methods should be called only from a single thread to prevent data corruption or race conditions.

The ``Variable`` class allows users to define variables of different types. Since it is an abstract base class, concrete variables are implemented in derived classes.

Usage
-----

Two specialisations of the ``Variable`` class are included in **Sequencer**. They are the `local variable` and the `file variable`. The implementation of `Local Variable` will be used to provide examples.

Note that more specialized variables are provided by plugins. For example, an EPICS ChannelAccess client variable is defined by ``sequencer-plugin-epics``.

Creating a Variable
^^^^^^^^^^^^^^^^^^^^

To obtain an instance of an existing variable type, provided by the core library or one of the loaded plugins, one typically asks a ``VariableRegistry`` to instantiate it:

.. code-block:: c++

   auto local_var = GlobalVariableRegistry().Create("Local");

Variable Initialization
^^^^^^^^^^^^^^^^^^^^^^^

Before instructions can access, i.e. read or write, the value of a variable, it needs to be properly initialized first. This is done by calling the ``Setup`` method on the variable. Initialization may include connecting to a database, obtaining a file handle, etc. One can pass a pointer to an ``AnyTypeRegistry`` object during setup, which can be used by the variable to obtain predefined types from it.

.. code-block:: c++

   local_var->Setup(nullptr);

Setting and Getting Variable Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Users can access and modify the value of a variable using the ``GetValue`` and ``SetValue`` methods, respectively. These methods provide a way to read and update variable values, or their subfields, in a controlled manner.

.. code-block:: c++

   // Set the current value of a variable
   sup::dto::AnyValue value = {{"index", {SignedInteger8Type, 1}}};
   local_var->SetValue(value);

   // Get the index field from the variable
   sup::dto::AnyValue index;
   local_var->GetValue(index, "index");

The access to subfields of a variable's value is provided by the base class ``Variable`` itself, requiring implementers of custom variables to only override the virtual private methods for non field-based access.

Checking Variable Availability
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The availability of a variable depends on the specific derived class implementation. For example, for a network variable, availability could mean that a stable network connection was made to be able to obtain its value. Users can check if a variable is available using the ``IsAvailable`` method.

.. code-block:: c++

   if (local_var->IsAvailable()) {
       // Variable is available
   } else {
       // Variable is not available
   }

Registering Callback for Value Updates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Users can register a callback function to be notified of value updates using the ``SetNotifyCallback`` method. This allows for real-time responsiveness to changes in variable values. Note however that it is typically the responsibility of the ``Workspace`` to subscribe to such changes and propagate these to other interested software components.

.. code-block:: c++

   // Define a callback function
   void OnValueUpdate(const sup::dto::AnyValue& value, bool connected) {
       // Handle the value update
       // ...
   }

   local_var->SetNotifyCallback(OnValueUpdate); // Register the callback function

Managing Attributes
^^^^^^^^^^^^^^^^^^^

The `Variable` class supports an attribute system (see :ref:`Attribute System`). Users can set, retrieve, and manipulate attributes using various attribute-related methods:

.. code-block:: c++

   // Add attributes to the numeric variable
   local_var->AddAttribute("units", "kg");
   local_var->AddAttribute("precision", "2");

   // Retrieve attribute values
   std::string units = local_var->GetAttributeString("units");
   int precision = local_var->GetAttributeValue<int>("precision");

Resetting a Variable
^^^^^^^^^^^^^^^^^^^^

The `Reset` method resets the variable to the state it had prior to initialization. This means that attributes are still present, but other internal state data is reset. For example, it can disconnect from external resources or clear values.

.. code-block:: c++

   local_var->Reset(); // Reset the numeric variable

.. _Attribute System:

Attribute System
----------------

The attribute system, together with a fixed typename for each concrete variable implementation, allows for handling variables in an opaque way: together they fully define the behavior of a variable and no implementation specific methods are required to initialize them. This system makes it possible to fully instantiate and initialize variables in a data-driven way and is used when parsing procedure XML files.

As an example, consider a procedure XML file containing the following variable definition element:

.. code-block:: xml

   <Local name="my_variable" type='{"type":"uint8"}' value='1'>

During parsing, this will result in the following method calls:

.. code-block:: c++

   auto var = GlobalVariableRegistry().Create("Local");
   var->AddAttribute("name", "my_variable");
   var->AddAttribute("type", R"RAW({"type":"uint8"})RAW");
   var->AddAttribute("value", "1");
   var->Setup(registry);  // The registry is passed from the workspace.

The attribute system also supports constraints that may result in throwing an exception during the `setup` phase. This provides feedback to the client about missed mandatory attributes, wrongly formatted ones, etc. Since all variables, and instructions, are initiaized before execution of a procedure, this provides `fail fast` behavior.

Implementers of concrete variable types can use protected member functions to signal which attributes are defined by the variable, which types they have, if they are mandatory and other more complex constraints.

As an example, consider creating a variable `MyVariable`, that has four predefined attributes:

* "country": a mandatory string;
* "max_retry": an optional unsigned integer;
* "left": an optional string;
* "right": an optional string.

Furthermore, assume that exactly one of the attributes `left` or `right` needs to be present. All this information can then be encoded in the constructor of the concrete variable:

.. code-block:: c++

   MyVariable::MyVariable()
     : Variable("MyVariable")
   {
     AddAttributeDefinition("country", sup::dto::StringType).SetMandatory();
     AddAttributeDefinition("max_retry", sup::dto::UnsignedInteger16Type);
     AddAttributeDefinition("left", sup::dto::StringType);
     AddAttributeDefinition("right", sup::dto::StringType);
     AddConstraint(MakeConstraint<Xor>(MakeConstraint<Exists>("left"),
                                       MakeConstraint<Exists>("right")));
   }

The generic implementation of the ``Setup`` method will ensure that if no exceptions were thrown, all these conditions are satisfied after setup.

Class definition
----------------

Next is presented the definition of the ``Variable`` class and its main methods.

.. doxygenclass:: sup::sequencer::Variable
   :members:
