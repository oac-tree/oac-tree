Basic Instructions
==================

Instruction is an abstract interface for all executable instructions. It serves as the base class for various types of instructions.
Instructions can be of the following categories:

* :ref:`compound` (multiple children)
* :ref:`decorator` (one child)
* :ref:`action` (no child instructions)

There are no restrictions beside the number of children on where an instruction can be used. e.g. a compound instruction can be inside another compound instruction.

Next we present the different types of available instructions and some basic examples of how to use them. The examples will contain mainly the body of a procedure in XML and an explanation of how it behaves.

.. _compound:

Compound Instructions
---------------------
Compound instructions are instructions that have multiple child instructions, and they control the flow of execution among their children based on certain conditions or criteria.

Sequence
^^^^^^^^

The sequence instruction executes its child instructions one by one in the order they are defined until:

* A child instruction returns `FAILURE`: `Sequencer` also returns `FAILURE`;
* All child instructions returned `SUCCESS`: `Sequencer` returns `SUCCESS`.

A sequence represents a number of steps that all have to succeed for a specific goal to be reached.

`Sequence` has no specific attributes.

.. _seq_exp:

**Example**

In the example, the first child will succeed, so the sequence will try the second child, which also succeeds. When the third child returns failure (inverted success), the sequence will stop and return failure.

.. code-block:: xml

    <Sequence>
        <Wait name="First" />
        <Wait name="Second" />
        <Inverter>
            <Wait name="Third" />
        </Inverter>
    </Sequence>

Fallback
^^^^^^^^

The fallback instruction executes its child instructions one by one in the order they are defined until:

* A child instruction returns `SUCCESS`: `Fallback` also returns `SUCCESS`;
* All child instructions returned `FAILURE`: `Fallback` returns `FAILURE`.

A fallback can be interpreted as aggregating a number of alternatives to reach a specific goal: as soon as one of these alternatives (child instruction) succeeds, the fallback succeeds.

`Fallback` has no specific attributes.

.. _fall_exp:

**Example**

In the example, the first child will fail (inverted success), so the fallback will try the second child, which succeeds. This will make the fallback succeed and the third child is never executed.

.. code-block:: xml

    <Fallback>
        <Inverter>
            <Wait name="One" timeout="0.2" />
        </Inverter>
        <Wait name="Two" timeout="0.2" />
        <Wait name="Three" timeout="3.0" />
    </Fallback>

ParallelSequence
^^^^^^^^^^^^^^^^

The parallel sequence instruction executes all its child instructions concurrently. The execution status of the parallel sequence is determined by its attributes (attribute types are denoted by their corresponding `AnyType` from `sup-dto`):

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - successThreshold
     - UnsignedInteger32Type
     - no
     - number of children to succeed for the parallel sequence to succeed
   * - failureThreshold
     - UnsignedInteger32Type
     - no
     - number of children to fail for the parallel sequence to fail

For a parallel sequence with `N` children, the defaults of the above thresholds are `N` and `1` respectively. The sum of both thresholds will always be made to not exceed `N + 1` as that would lead to an undetermined status of the parallel sequence.

.. note::

   As soon as an exit condition is reached, i.e. one of the thresholds was reached, the parallel sequence will halt all remaining child instructions.

.. _par_exp:

**Example**

The parallel sequence will execute all its children concurrently. The first two child instructions will return `SUCCESS` after one and two seconds respectively. At this point, the threshold for success is reached and the parallel sequence halts the third child instruction before returning `SUCCESS` itself.

.. code-block:: xml

    <ParallelSequence name="Parallel Wait" successThreshold="2">
        <Wait name="One" timeout="1.0" />
        <Wait name="Two" timeout="2.0" />
        <Wait name="Three" timeout="3.0" />
    </ParallelSequence>

.. _choice_exp:

Choice
^^^^^^

The choice instruction will execute instructions based on their index. That index is passed from a workspace variable that can be a scalar (single index) or an array (list of indices). An index can be repeated inside the array, making the correspondent instruction execution be repeated as many times as it appears in the array.

The execution status returned by the choice instruction follows the same logic as for `Sequence`: a single failure immediately leads to failure and success means all selected child instructions returned success.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - varName
     - StringType
     - yes
     - name of the workspace variable to fetch the index (or array) from

**Example**

The example below will execute first the second instruction, with index one, then the first and finally, the third child instruction. These all succeed, so the `Choice` will succeed. The last child instruction (inverted wait) will never execute.

.. code-block:: xml

    <Choice varName="choice">
        <Wait/>
        <Wait/>
        <Wait/>
        <Inverter>
            <Wait/>
        </Inverter>
    </Choice>
    <Workspace>
        <Local name="choice" type='{"type":"uint32_arr","multiplicity":3,"element":{"type":"uint32"}}' value="[1,0,2]"/>
    </Workspace>

UserChoice
^^^^^^^^^^

This instruction delegates to the UserInterface a selection to be made from one of its child instructions. It will then execute that child and directly return its status.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - description
     - StringType
     - no
     - text description that will be passed to the UserInterface to guide selection

.. note::

   If the UserInterface returns an invalid index, i.e. out of bounds for the number of children, the instruction immediately returns `FAILURE`.

.. _uchoice_exp:

**Example**

In this example, the user is asked to select one of the `Wait` instructions with different timeouts. On valid selection, that instruction will be executed and its status (success) will be returned from the `Choice` instruction.

.. code-block:: xml

    <Choice description="select wait">
        <Wait timeout="1.0"/>
        <Wait timeout="2.0"/>
        <Wait timeout="3.0"/>
    </Choice>
    <Workspace/>

.. _decorator:

Decorator Instructions
----------------------

Decorator instructions modify or add functionality to a single child instruction, allowing for the addition of behavior to an existing instruction without modifying its core implementation.

For
^^^

This instruction applies a child instruction to the elements of an array.

Executes the child instruction on the elements of an array, until either: the child fails (FAILURE) or the child instruction is applied to all elements of the provided array (SUCCESS).

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - elementVar
     - StringType
     - yes
     - Element variable to contain the elements of the cycled array
   * - arrayVar
     - StringType
     - yes
     - Array to cycle

.. _for_exp:

**Example**
This example will apply the Increment instruction to all elements of the array "arr".

.. code-block:: xml

    <Sequence>
        <For elementVar="i" arrayVar="arr">
            <Increment varName="i"/>
        </For>
    </Sequence>
    <Workspace>
    <Local name="arr" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[2,4,6]"/>
    <Local name="i" type='{"type":"uint32"}' value='0' />
    </Workspace>

Force Success
^^^^^^^^^^^^^

This instructions wrap a child instruction and always return SUCCESS when the child has finished execution. 

`Force Success` has no specific attributes.

**Example**

.. code-block:: xml

    <ForceSuccess name="success">
        <Inverter name="failure">
            <Wait name="wait" />
        </Inverter>
    </ForceSuccess>


Include
^^^^^^^

Decorator that includes an instruction tree by reference.
The reference can point to an instruction tree in the same definition file or to one defined in a separate file (‘file’ attribute).

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - path
     - StringType
     - yes
     - Instruction name where to include the new instruction
   * - file
     - StringType
     - no
     - File name where to include the new instruction


**Example**

This example will include an instruction named "Counts" in sequence named "DontWait".

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

Decorator instruction that includes an external procedure (workspace and instruction tree).

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - file
     - StringType
     - yes
     - File name where to get the instruction to include
   * - path
     - StringType
     - no
     - Instruction name where to include the new instruction

**Example**

This example will include the procedure in file "test_procedure_1.xml" into the procedure where IncludeProcedure iinstruction is called.

test_procedure_1.xml file:

.. code-block:: xml

   <Sequence name="CopyAndCheck" isRoot="True">
       <Copy input="a" output="b"/>
       <Equals name="Check" lhs="a" rhs="b"/>
   </Sequence>
   <Wait name="ParameterizedWait" timeout="$timeout"/>
   <Inverter name="AlwaysFails">
       <Wait/>
   </Inverter>
   <Workspace>
       <Local name="a" type='{"type":"uint16"}' value='1' />
       <Local name="b" type='{"type":"uint16"}' value='0' />
   </Workspace>

Main procedure:

.. code-block:: xml

   <IncludeProcedure name="IncludeRoot" file="test_procedure_1.xml"/>
   <IncludeProcedure name="IncludeWait" file="test_procedure_1.xml" path="ParameterizedWait"/>
   <Workspace>
       <Local name="a" type='{"type":"string"}' value='"does_not_matter"' />
   </Workspace>

Inverter
^^^^^^^^

Instruction that inverts the execution status of its child, interchanging SUCCESS and FAILURE.

`Inverter` has no specific attributes.

* An example for this instruction is already present in :ref:`Fallback example <fall_exp>`.

Listen
^^^^^^

Instruction that executes its child instruction each time specific variables are updated.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - varNames
     - StringType
     - yes
     - Name of the variable to listen to
   * - forceSuccess
     - StringType
     - no
     - Execute children instruction until successful if active


.. _listen_exp:

Listen Example
~~~~~~~~~~~~~~~

This example will "Listen" on the variable "monitor" and check if it is equal to variable "update" everytime "monitor" is updated.

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

Instruction that repeats its child a fixed number of times while successful.

Repeatedly executes the child instruction, until either: the child fails (FAILURE) or maximum number of repetitions is reached (SUCCESS).

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - maxCount
     - Signedinteger32type
     - no
     - Maximum number of repetitions


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

* An example for this instruction is already present in :ref:`ParallelSequence example <par_exp>`.

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


* The Greater Than usage is equal to that of the Less Than that can be seen in :ref:`Sequence example <seq_exp>`.

Greater than or Equal
^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::GreaterThanOrEqual
   :members:

* The Greater Than or Equal usage is equal to that of the Less Than that can be seen in :ref:`Sequence example <seq_exp>`.


Increment
^^^^^^^^^

.. doxygenclass:: sup::sequencer::Increment
   :members:

* The Increment usage is equal to that of the Decrement that can be seen in :ref:`Sequence example <seq_exp>`.

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

* An example for this instruction is already present in :ref:`Sequence example <seq_exp>`.

Less than or Equal
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::LessThanOrEqual
   :members:

* The Less Than or Equal usage is equal to that of the Less Than that can be seen in :ref:`Sequence example <seq_exp>`.

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

* An example for this instruction is already present in :ref:`ParallelSequence example <par_exp>`.

Wait for Variable
^^^^^^^^^^^^^^^^^

.. doxygenclass:: sup::sequencer::WaitForVariable
   :members:

* An example for this instruction is already present in :ref:`repeat_exp`.
