Extending the Oac-Tree with plugins
====================================

.. contents::
   :local:

Plugins allow users to extend the functionality of the Oac-Tree by providing new instructions or variables. This documentation provides a step-by-step guide on how to create a plugin and integrate it into the Oac-Tree.
Creating a plugin to access another library in the oac-tree is simple when following a structured approach: first, implement the instruction/variable interface; then register the instruction/variable; and finally deploy the plugin in the plugin folder.

Implementation
--------------

Start by creating a new project (E.g. ``oac-tree-plugin-examplelib``). This page uses the following directory structure for the source files (but this is not mandatory): ``src > lib > oac-tree``.
Inside the ``oac-tree`` folder, you may create additional directories for different parts of the library you wish to create the plugin for.

To implement instructions and variables you can take a look at their respective sections:
:ref:`Instruction`, :ref:`Variable`.

The absolute mininum that you have to implement are the following virtual functions:

For instructions:

* ``ExecutionStatus Instruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)``

For variables:

* ``bool Variable::GetValueImpl(sup::dto::AnyValue& value) const``
* ``bool Variable::SetValueImpl(const sup::dto::AnyValue& value)``

Registration
------------

To register an instruction, include the instruction registry header file, and declare a static initialisation flag in the instruction's source file.

.. code-block:: c++

   #include <sup/oac-tree/instruction_registry.h>

.. code-block:: c++

   static bool _example_initialised_flag = RegisterGlobalInstruction<ExampleInstruction>();

In the same way, a variable is registered by including the variable registry header file, and similarly declaring a flag in the variable's source file.

.. code-block:: c++

   #include <sup/oac-tree/variable_registry.h>

.. code-block:: c++

   static bool _example_initialised_flag = RegisterGlobalVariable<ExampleVariable>();

Plugin deployment
-----------------

To deploy the plugin, install the shared library in an appropriate folder. If you want to avoid having to specify the full pathname in the XML procedure later, use a folder that is used by the operating system to look for shared libraries (or add the path to ``LD_LIBRARY_PATH``).

Usage
-----

To use variables and instructions of any plugin within your procedure, declare the plugin right after declaring the procedure, in said procedure's ``xml`` file.

.. code-block:: xml

   <Plugin>liboac-tree-example.so</Plugin>
