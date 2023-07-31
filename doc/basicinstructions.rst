Basic Instructions
==================

Instruction is an abstract interface for all executable instructions. It serves as the base class for various types of instructions.
Instructions can be of the following categories:
* :ref:`compound` (multiple children)
* :ref:`decorator` (one child)
* :ref:`action` (no child instructions)

There are no restrictions beside the number of children on where an instruction can be used. e.g. a compound instruction can be inside an action.

Next we present the different types of available instructions and some basic examples of how to use them. The examples will contain mainly the necessary xml configuration and some c++ directives to test the instruction.

.. _compound:

Compound Instructions
---------------------
Compound instructions are instructions that have multiple child instructions, and they control the flow of execution among their children based on certain conditions or criteria.

Sequence
^^^^^^^^

The sequence instruction executes its child instructions in the order they are set.

.. doxygenclass:: sup::sequencer::Sequence
   :members:

.. _seq_exp:

Sequence Example
~~~~~~~~~~~~~~~~

.. code-block:: c++

   const std::string body{
    R"(
        <Sequence>
            <For elementVar="i" arrayVar="arr">
                <Sequence>
                    <Increment varName="i"/>
                    <LessThan lhs="i" rhs="a"/>
            </For>
        </Sequence>
        <Workspace>
            <Local name="arr" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[2,4,6]"/>
            <Local name="i" type='{"type":"uint32"}' value='0' />
            <Local name="a" type='{"type":"uint32"}' value='9' />
        </Workspace>
    )"};


Parallel Sequence
^^^^^^^^^^^^^^^^^

The parallel sequence instruction executes its child instructions in a parallel environment.

.. doxygenclass:: sup::sequencer::ParallelSequence
   :members:

.. _par_exp:

Parallel Sequence Example
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   const std::string body{
    R"(
    <ParallelSequence name="parallel" successThreshold="1" failureThreshold="3">
        <Wait name="wait" timeout="2.0"/>
        <Copy name="copy" input="input" output="output"/>
        <Wait name="again" timeout="2.0"/>
    </ParallelSequence>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
    )"};

.. _choice_exp:

Choice
^^^^^^

The choice instruction will execute instructions based on their index. That index is passed within a variable that can be a scalar or an array. An index can be repeated inside the array, making the correspondent instruction execution be repeated as many times as it appears in the array. An instruction that has no correspondent call inside the variable, will be ignored.

.. doxygenclass:: sup::sequencer::Choice
   :members:

Choice Example
~~~~~~~~~~~~~~

.. code-block:: c++

    // this example will execute the two counter and the wait instructions, the inverter will be ignored
    const std::string body{R"(
        <Choice varName="choice">
            <Counter/>
            <Counter incr="2"/>
            <Wait/>
            <Inverter>
                <Counter/>
            </Inverter>
        </Choice>
        <Workspace>
            <Local name="choice" type='{"type":"uint32_arr","multiplicity":3,"element":{"type":"uint32"}}' value="[0,1,2]"/>
        </Workspace>
    )"};


User Choice
^^^^^^^^^^^

This instruction is similar to ``Choice``, but the selection of the instruction to execute is delegated to the UserInterface.

.. doxygenclass:: sup::sequencer::UserChoice
   :members:

.. _uchoice_exp:

User Choice Example
~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

    // this example will execute the two counter and the wait instructions, the inverter will be ignored
    const std::string body{R"(
        <UserChoice varName="choice">
            <Counter/>
            <Counter incr="2"/>
            <Wait/>
            <Inverter>
                <Counter/>
            </Inverter>
        </UserChoice>
    )"};

    // After the setup, the UserInterface can provide an index to execute an instruction
      ui.SetChoice(1); // when executed, this option should return the value "2"


Fallback
^^^^^^^^

.. doxygenclass:: sup::sequencer::Fallback
   :members:


.. _fall_exp:

Fallback Example
~~~~~~~~~~~~~~~~

.. code-block:: c++

    const std::string body{R"(
        <Fallback>
            <Inverter name="failure">
                <Counter/>
            </Inverter>
            <ForceSuccess name="success">
                <Counter/>
            </ForceSuccess>
        </Fallback>
        <Workspace>
        </Workspace>
    )"};

    // This example will execute both "Counter", has the Fallback only
    // stops executing when one instruction is successful


.. _decorator:

Decorator Instructions
----------------------

Decorator instructions modify or add functionality to a single child instruction, allowing for the addition of behavior to an existing instruction without modifying its core implementation.

For
^^^

.. doxygenclass:: sup::sequencer::ForInstruction
   :members:


* An example for this instruction is already present in :ref:`seq_exp`.

Force Success
^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::ForceSuccess
   :members:

* An example for this instruction is already present in :ref:`uchoice_exp`.

Include
^^^^^^^

.. doxygenclass:: sup::sequencer::Include
   :members:

Include Example
~~~~~~~~~~~~~~~

.. code-block:: c++

    const std::string body{R"(
        <Sequence name="DontWait">
            <Wait timeout="$to" />
        </Sequence>
        <Include isRoot="true" name="Counts" path="DontWait" to="0.2"/>
        <Workspace>
        </Workspace>
    )"};


Include Procedure
^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::IncludeProcedure
   :members:

Inverter
^^^^^^^^

.. doxygenclass:: sup::sequencer::Inverter
   :members:

* An example for this instruction is already present in :ref:`fall_exp`.

Listen
^^^^^^

.. doxygenclass:: sup::sequencer::Listen
   :members:

.. _listen_exp:

Listen Example
~~~~~~~~~~~~~~~

.. code-block:: c++

    static const std::string procedure_body{
    R"RAW(
        <Fallback>
             <ParallelSequence>
                 <Listen varNames="monitor">
                    <Inverter>
                        <Equals lhs="monitor" rhs="update"/>
                    </Inverter>
                 </Listen>
                 <Sequence>
                    <Copy input="update" output="monitor"/>
                 </Sequence>
                 <Inverter>
                    <Wait timeout="2.0"/>
                 </Inverter>
             </ParallelSequence>
             <Equals lhs="monitor" rhs="update"/>
        </Fallback>
        <Workspace>
            <Local name="monitor"
                   type='{"type":"uint64"}'
                   value='0'/>
            <Local name="update"
                   type='{"type":"uint64"}'
                   value='1729'/>
        </Workspace>
    )RAW"};


Repeat
^^^^^^

.. doxygenclass:: sup::sequencer::Repeat
   :members:

.. _repeat_exp:

Repeat example
~~~~~~~~~~~~~~

.. code-block:: c++

    const std::string body{
    R"(
       <ParallelSequence name="parallel">
           <WaitForVariable timeout="4.0" varName="a" equalsVar="b"/>
           <Repeat maxCount="8">
               <Increment varName="a"/>
           </Repeat>
           <Repeat maxCount="2">
                <Decrement varName="b"/>
           </Repeat>
       </ParallelSequence>
       <Workspace>
           <Local name="a" type='{"type":"uint8"}' value='3' />
           <Local name="b" type='{"type":"uint8"}' value='13' />
       </Workspace>
    )"};

.. _action:

Action Instructions
-------------------

An action instruction represents a discrete operation or step within a larger sequence of instructions. Actions are fundamental building blocks that perform specific tasks or operations to achieve a particular goal. Action instructions are typically used within compound or decorator instructions.

Condition
^^^^^^^^^

.. doxygenclass:: sup::sequencer::Condition
   :members:

Condition example
~~~~~~~~~~~~~~~~~

.. code-block:: c++

    const std::string body{R"(
        <Sequence>
            <Condition name="Condition" varName="a" />
        </Sequence>
        <Workspace>
            <Local name="a"
                   type='{"type":"int8"}'
                   value='1' />
            <Local name="b"
                   type='{"type":"uint8"}'
                   value='0' />
            <Local name="c"
                   type='{"type":"uint16"}'
                   value='3' />
            <Local name="d"
                   type='{"type":"uint32"}'
                   value='0' />
        </Workspace>
    )"};

Copy
^^^^

.. doxygenclass:: sup::sequencer::Copy
   :members:

* An example for this instruction is already present in :ref:`par_exp`.

Decrement
^^^^^^^^^

.. doxygenclass:: sup::sequencer::Decrement
   :members:

* An example for this instruction is already present in :ref:`repeat_exp`.

Equals
^^^^^^

.. doxygenclass:: sup::sequencer::Equals
   :members:

* An example for this instruction is already present in :ref:`listen_exp`.

Greater than
^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::GreaterThan
   :members:


* The Greater Than usage is equal to that of the Less Than that can be seen in :ref:`seq_exp`.

Greater than or Equal
^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::GreaterThanOrEqual
   :members:

* The Greater Than or Equal usage is equal to that of the Less Than that can be seen in :ref:`seq_exp`.


Increment
^^^^^^^^^

.. doxygenclass:: sup::sequencer::Increment
   :members:

* The Increment usage is equal to that of the Decrement that can be seen in :ref:`seq_exp`.

Input
^^^^^

.. doxygenclass:: sup::sequencer::Input
   :members:

Input Example
~~~~~~~~~~~~~

.. code-block:: c++

    const std::string body{R"(
        <Sequence>
            <Input description="Put some uint32 here" output="uint32"/>
        </Sequence>
        <Workspace>
            <Local name="uint32" type='{"type":"uint32"}'/>
        </Workspace>
    )"};
    // After the setup, the UserInterface can provide the value to populate the local variable
    sup::dto::AnyValue value(1234u);
    ui.SetValue(value);


Less than
^^^^^^^^^

.. doxygenclass:: sup::sequencer::LessThan
   :members:

* An example for this instruction is already present in :ref:`seq_exp`.

Less than or Equal
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::LessThanOrEqual
   :members:

* The Less Than or Equal usage is equal to that of the Less Than that can be seen in :ref:`seq_exp`.

Message
^^^^^^^

.. doxygenclass:: sup::sequencer::Message
   :members:

Output
^^^^^^

.. doxygenclass:: sup::sequencer::Output
   :members:

Reset Variable
^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::ResetVariable
   :members:

Reset Variable Example
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

    const std::string body{R"(
        <Sequence>
            <Copy input="a" output="target"/>
            <ResetVariable varName="target"/>
            <Copy input="b" output="target"/>
        </Sequence>
        <Workspace>
            <Local name="target"/>
            <Local name="a" type='{"type":"uint8"}' value='1' />
            <Local name="b" type='{"type":"string"}' value='"some name"' />
        </Workspace>
    )"};


User Confirmation
^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::UserConfirmation
   :members:

Wait
^^^^

.. doxygenclass:: sup::sequencer::Wait
   :members:

* An example for this instruction is already present in :ref:`par_exp`.

Wait for Variable
^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::WaitForVariable
   :members:

* An example for this instruction is already present in :ref:`repeat_exp`.
