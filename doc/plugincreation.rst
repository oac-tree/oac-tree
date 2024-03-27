Extending the Sequencer with plugins
====================================

Implementing new instructions is simple when following a structured aproach: first, we implement instruction/variable interface; then we need to register the instruction/variable; and finally we can deploy the plugin in the plugin folder.

Implementing new instructions
-----------------------------

Start by creating a class that inherits from class ``Instruction`` akin to this example:

.. code-block:: c++

   class ExampleInstruction : public Instruction
   {
   public:
     ExampleInstruction();

     ~ExampleInstruction() override;

     static const std::string Type;

   private:
     ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
   };

Then, in the source file, define the type as the instruction's name.

.. code-block:: c++

   const std::string ExampleInstruction::Type = "ExampleInstruction";

In the constructor, add the attributes that the instruction needs.

.. code-block:: c++

   ExampleInstruction::ExampleInstruction() : Instruction(ExampleInstruction::Type)
   {
     AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
       .SetCategory(AttributeCategory::kVariableName)
       .SetMandatory();
     AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
       .SetCategory(AttributeCategory::kVariableName)
       .SetMandatory();
   }

.. note:: AttributeCategory types
  
   Attributes already in the workspace should be categorized as kVariableName, and their naming should end in 'var'.

Finally, implement the instruction functionality in the ``ExecuteSingleImpl`` function.

.. code-block:: c++

   ExecutionStatus ExampleInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
   {
     sup::dto::AnyValue input_var;
     if (!GetAttributeValue(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, input_var))
     {
       return ExecutionStatus::FAILURE;
     }
     sup::dto::AnyValue output_var;
     if (!GetAttributeValue(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, output_var))
     {
       return ExecutionStatus::FAILURE;
     }

     // Type check
     if (input_var.GetTypeCode() != sup::dto::TypeCode::Bool)
     {
       const std::string warning =
           InstructionErrorProlog(*this) + " Not a boolean.";
       LogWarning(ui, warning);
       return ExecutionStatus::FAILURE;
     }
     output_var = input_var;

     // Add variable to workstation
     if (!SetValueFromAttributeName(*this, ws, ui, Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME,
                                    Woutput_var))
     {
       return ExecutionStatus::FAILURE;
     }
     return ExecutionStatus::SUCCESS;
   }

In this example, we just verify if the input variable is a bool type variable, and set it as the output variable.

.. note:: Logging warnings and errors

   When verifying conditions to decide the outcome of the implementation, you should log impeditive results. Though the difference between a warning and error log is very tenuous, a good rule of thumb is to look at how the condition was achieved. For instance,  

.. note:: Exception handling

   The ``ExecuteSingleImpl`` function is not prepared to handle exceptions. So if you happen to use functions that throw exceptions do insert them within a try-block. 

Register the instruction
------------------------

Registering the instruction, is just heading over to ``instruction_registry.cpp``, include the new header file, and declare ``RegisterInstruction`` with the new instruction as the template.

.. code-block:: c++

   #include "example_instruction.cpp"

.. code-block:: c++

   (void)RegisterInstruction<ExampleInstruction>(registry);

.. note:: Exception handling

   Remember to keep instruction registry in alphabetical order.

Deploying the plugin
--------------------

To deploy the plugin simple use the command ``make install`` on your build folder.

.. note:: Build Documentation

   To build the documentation, add the flag ``-DCOA_BUILD_DOCUMENTATION=ON`` when running ``cmake``.
