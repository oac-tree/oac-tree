Extending the Sequencer with plugins
====================================

.. contents::
   :local:

Plugins allow users to extend the functionality of the Sequencer by providing new instructions or variables. This documentation provides a step-by-step guide on how to create a plugin and integrate it into the Sequencer.
Creating a plugin to access another library in sequencer is simple when following a structured aproach: first, implement instruction/variable interface; then register the instruction/variable; and finally deploy the plugin in the plugin folder.

Implementation
--------------

Start by creating a new project (E.g. ``sequencer-plugin-examplelib``) with the following the directory structure: ``src > lib > sequencer``.
Inside the ``sequencer`` folder, you may create additional directories for different parts of the library you wish to create the plugin for.

To implement instructions and variables you can take a look at their respective sections: 
:ref:`Instruction`, :ref:`Variable`.


Registration
------------

To register an instruction, include the instruction registry header file, and declare a static initialisation flag in the instruction's source file.

.. code-block:: c++

   #include <sup/sequencer/instruction_registry.h>

.. code-block:: c++

   static bool _example_initialised_flag = RegisterGlobalInstruction<ExampleInstruction>();

In the same way, a variable is registered by including the variable registry header file, and similarly declaring a flag in the variable's source file.

.. code-block:: c++

   #include <sup/sequencer/variable_registry.h>

.. code-block:: c++

   static bool _example_initialised_flag = RegisterGlobalVariable<ExampleVariable>();

Plugin deployment
-----------------

To deploy the plugin, execute ``make install`` on your build directory. 

Usage
-----

To use variables and instructions of any plugin within ``sequencer-cli``, declare the plugin right after declaring the procedure, in said procedure's ``xml`` file.

.. code-block:: xml

   <Plugin>libsequencer-example.so</Plugin>

At the time of writing, if you want to use plugin variables and instructions in the GUI, you need to hardcode the plugin into line 153 at ``domain_utils.cpp`` in ``sequencer-gui`` project. And export the path using the following command:

.. code-block:: bash

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/sequencer/plugins/