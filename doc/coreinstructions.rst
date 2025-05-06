Core Instructions
=================

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

For a parallel sequence with `N` children, the defaults of the above thresholds are `N` and `1` respectively. The sum of both thresholds will always be made to not exceed `N + 1` as that would lead to an undetermined status of the parallel sequence. If needed, that means that the real failure threshold could be made smaller than is indicated in the attribute if the success threshold was overriden. Equivalently, the success threshold can become smaller if only the failure threshold was overriden.

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

ReactiveSequence
^^^^^^^^^^^^^^^^

The reactive sequence instruction executes its child instructions one by one in the order they are defined until:

* A child instruction returns `FAILURE`: `reactive sequence` also returns `FAILURE`;
* A child instruction returns `RUNNING`: all other child instructions are reset and the reactive sequence returns `RUNNING`; on the next execution cycle, this will lead to the re-execution of the child instructions before the `RUNNING` one.
* All child instructions returned `SUCCESS`: `reactive sequence` returns `SUCCESS`.

A reactive sequence most often consists of synchronous child nodes that represent preconditions and a single asynchronous child node in the end that only should be executed if all preconditions are met. The reactive sequence interrupts the asynchronous child as soon as one of the preconditions is no longer met.

`ReactiveSequence` has no specific attributes.

.. note::

   When all child instructions of the `reactive sequence` are synchronous, i.e. they never return `RUNNING`, it will behave as a normal `sequence`.

.. _react_seq_exp:

**Example**

In the example, two branches are executed in parallel:

* The first branch contains the reactive sequence which will execute the (asynchronous) wait instruction as long as the variable `break` is zero. From the moment this variable is no longer zero, the wait instruction will be interrupted and the reactive sequence will return `FAILURE`.
* The second branch will wait for one second and then set the variable `break` to one. This will interrupt the reactive sequence in the other branch.

.. code-block:: xml

    <ParallelSequence>
        <ReactiveSequence>
            <Equals leftVar="break" rightVar="zero"/>
            <AsyncWait name="AsyncTask" timeout="10.0"/>
        </ReactiveSequence>
        <Sequence>
            <Wait timeout="1.0" />
            <Copy inputVar="one" outputVar="break"/>
        </Sequence>
    </ParallelSequence>

Sequence
^^^^^^^^

The sequence instruction executes its child instructions one by one in the order they are defined until:

* A child instruction returns `FAILURE`: `sequence` also returns `FAILURE`;
* All child instructions returned `SUCCESS`: `sequence` returns `SUCCESS`.

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

This example will apply the Increment instruction to all elements of the array "arr". Note that the array elements will NOT be changed, as only a copy of those elements is inremented.

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

ForceSuccess
^^^^^^^^^^^^

This instruction wraps a child instruction and always return SUCCESS when the child has finished execution.

`ForceSuccess` has no specific attributes.

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
The reference can point to an instruction tree in the same definition file or to one defined in a separate file (`file` attribute).

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
     - Name of instruction to include
   * - file
     - StringType
     - no
     - File name from where to include the new instruction


**Example**

This example will include an instruction named "Counts" in sequence named "DontWait".

.. code-block:: xml

   <Sequence name="DontWait">
       <Wait timeout="$to" />
   </Sequence>
   <Include isRoot="true" name="Counts" path="DontWait" to="0.2"/>
   <Workspace>
   </Workspace>


IncludeProcedure
^^^^^^^^^^^^^^^^

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

This example will include the procedure in file "test_procedure_1.xml" into the procedure where IncludeProcedure instruction is called.

test_procedure_1.xml file:

.. code-block:: xml

   <Sequence name="CopyAndCheck" isRoot="True">
       <Copy inputVar="a" outputVar="b"/>
       <Equals name="Check" leftVar="a" rightVar="b"/>
   </Sequence>
   <Wait name="ShortWait" timeout="1.0"/>
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
   <IncludeProcedure name="IncludeWait" file="test_procedure_1.xml" path="ShortWait"/>
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

Instruction that executes its child instruction each time specific variables are updated. By default, it will only report a finished status (success or failure) when the child instruction fails.

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
     - BooleanType
     - no
     - Always handle child as successful if active (only halt will exit the loop)

.. _listen_exp:

**Example**

This example will "Listen" on the variable "monitor" and check if it is equal to variable "update" everytime "monitor" is updated.

.. code-block:: xml

   <Fallback>
       <ParallelSequence>
           <Listen varNames="monitor">
               <Inverter>
                   <Equals leftVar="monitor" rightVar="update"/>
               </Inverter>
           </Listen>
           <Sequence>
               <Copy inputVar="update" outputVar="monitor"/>
           </Sequence>
           <Inverter>
               <Wait timeout="2.0"/>
           </Inverter>
       </ParallelSequence>
       <Equals leftVar="monitor" rightVar="update"/>
   </Fallback>
   <Workspace>
       <Local name="monitor"
                    type='{"type":"uint64"}'
                    value='0'/>
       <Local name="update"
                    type='{"type":"uint64"}'
                    value='1729'/>
   </Workspace>


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

**Example**


.. code-block:: xml

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


.. _action:

Action Instructions
-------------------

An action instruction represents a discrete operation or step within a larger sequence of instructions. Actions are fundamental building blocks that perform specific tasks or operations to achieve a particular goal. Action instructions are typically used within compound or decorator instructions.

AddElement
^^^^^^^^^^

Instruction to add a new array element to a workspace variable using the information provided by a different variable.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - inputVar
     - StringType
     - yes
     - Name of the input variable or field thereof containing the information to be added as a new element
   * - outputVar
     - StringType
     - yes
     - Name of the output variable or field thereof that will receive a new element

**Example**

This example showcases how the AddElement instruction can be used to add variable var1 "a" to the workspace array variable "var2".

.. code-block:: xml

    <Sequence>
        <AddElement inputVar="var1" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.[1]"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"bool"}'
               value='true'/>
        <Local name="var2"
               dynamicType="true"
               type='{"type":"array","multiplicity":1,"element":{"type":"bool"}}'
               value='[false]'/>
    </Workspace>

AddMember
^^^^^^^^^

Instruction to add a new member to a workspace variable using the information provided by a different variable.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - inputVar
     - StringType
     - yes
     - Name of the input variable or field thereof containing the information to be added to the new member
   * - varName
     - StringType
     - yes
     - Name of the new member
   * - outputVar
     - StringType
     - yes
     - Name of the output variable or field thereof that will hold the new member

**Example**

This example showcases how the AddMember instruction can be used to add member "a" to workspace variable "var2" with the information provided by "var1". The `Equals` instruction can can be used to validate the `AddMember` instruction, since it will check for variable existence and equality.

.. code-block:: xml

    <Sequence>
        <AddMember inputVar="var1" varName="a" outputVar="var2"/>
        <Equals leftVar="var1" rightVar="var2.a"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
            type='{"type":"uint8"}'
            value='125'/>
        <Local name="var2"
            dynamicType="true"
            type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
            value='{"value":1729}'/>
    </Workspace>

Condition
^^^^^^^^^

Instruction that checks a boolean workspace variable.
Returns SUCCESS if the variable is true and FAILURE otherwise. If the variable is not a boolean, it will try to convert it to a boolean first, using the usual arithmetic conversions (e.g. 0 is false). If it cannot be converted to a boolean, the instruction reports FAILURE.

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
     - Variable name to check

.. _condition_exp:

**Example**

.. code-block:: xml

   <Sequence>
       <Condition name="Condition" varName="a" />
   </Sequence>
   <Workspace>
       <Local name="a"
                    type='{"type":"int8"}'
                    value='1' />
   </Workspace>

CopyFromProcedure
^^^^^^^^^^^^^^^^^

Instruction that copies a variable from the workspace of an external procedure into the current workspace.

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
     - Filename for the included procedure
   * - inputVar
     - StringType
     - yes
     - Name of the input variable in the included procedure
   * - outputVar
     - StringType
     - yes
     - Name of the output variable in the current procedure

CopyToProcedure
^^^^^^^^^^^^^^^

Instruction that copies a variable from the current workspace into a workspace of an external procedure. When including (parts of) that external procedure later, it will have access to the copied value.

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
     - Filename for the included procedure
   * - inputVar
     - StringType
     - yes
     - Name of the input variable in the current procedure
   * - outputVar
     - StringType
     - yes
     - Name of the output variable in the included procedure

Copy
^^^^

Instruction that copies the value of an input variable to an output variable

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - inputVar
     - StringType
     - yes
     - Name of the input variable
   * - outputVar
     - StringType
     - yes
     - Name of the output variable

* An example for this instruction is already present in :ref:`Reset example <reset_exp>`.

Decrement
^^^^^^^^^

Instruction to decrement a numeric variable by 1.

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
     - Name of the variable to decrement

* An example for this instruction is already present in :ref:`Repeat example <repeat_exp>`.

Equals
^^^^^^

Instruction to check the equality of two variables.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - leftVar
     - StringType
     - yes
     - Name of the left hand side variable to compare
   * - rightVar
     - StringType
     - yes
     - Name of the right hand side variable to compare

* An example for this instruction is already present in :ref:`Listen example <listen_exp>`.

GreaterThan
^^^^^^^^^^^

Instruction to check if a variable is greater than other.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - leftVar
     - StringType
     - yes
     - Name of the left hand side variable to compare
   * - rightVar
     - StringType
     - yes
     - Name of the right hand side variable to compare


* The Greater Than usage is similar to that of the `Equals` that can be seen in :ref:`Listen example <listen_exp>`.

GreaterThanOrEqual
^^^^^^^^^^^^^^^^^^

Instruction to check if a variable is greater or equal to other.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - leftVar
     - StringType
     - yes
     - Name of the left hand side variable to compare
   * - rightVar
     - StringType
     - yes
     - Name of the right hand side variable to compare


* The Greater Than usage is similar to that of the `Equals` that can be seen in :ref:`Listen example <listen_exp>`.


Increment
^^^^^^^^^

Instruction to increment a numeric variable by 1.

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
     - Name of the variable to increment

* The Increment usage is equal to that of the Decrement that can be seen in :ref:`Sequence example <seq_exp>`.

Input
^^^^^

Instruction node that writes a user defined value (from UserInterface) into a workspace variable.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - outputVar
     - StringType
     - yes
     - Name of the variable where to write the user input value
   * - description
     - StringType
     - no
     - Description of the requested variable

.. _input_exp:

**Example**

.. code-block:: xml

   <Sequence>
       <Input description="Put some uint32 here" outputVar="uint32"/>
   </Sequence>
   <Workspace>
       <Local name="uint32" type='{"type":"uint32"}'/>
   </Workspace>

After the setup, the UserInterface can provide the value to populate the local variable

.. code-block:: c++

   sup::dto::AnyValue value(1234u);
   ui.SetValue(value);


LessThan
^^^^^^^^

Instruction to check if a variable is smaller than other.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - leftVar
     - StringType
     - yes
     - Name of the left hand side variable to compare
   * - rightVar
     - StringType
     - yes
     - Name of the right hand side variable to compare


* The Greater Than usage is similar to that of the `Equals` that can be seen in :ref:`Listen example <listen_exp>`.

LessThanOrEqual
^^^^^^^^^^^^^^^

Instruction to check if a variable is smaller or equal to other.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - leftVar
     - StringType
     - yes
     - Name of the left hand side variable to compare
   * - rightVar
     - StringType
     - yes
     - Name of the right hand side variable to compare


* The Greater Than usage is similar to that of the `Equals` that can be seen in :ref:`Listen example <listen_exp>`.

Log
^^^

Instruction that sends a message and/or variable value to the oac-tree log, which is handled by the specific UserInterface used.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - message
     - StringType
     - no
     - Text message to log
   * - inputVar
     - StringType
     - no
     - Name of variable to log
   * - severity
     - StringType
     - no
     - Severity of the log message

Note that either the `message` or `inputVar` attribute (or both) need to be defined.

The `severity` attribute needs to be one of the following (in decreasing order of severity): emergency, alert, critical, error, warning, notice, info, debug, trace. In the absence of this attribute, the default severity is `info`.

Message
^^^^^^^

Instruction forwarding a text message to the UserInterface.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - text
     - StringType
     - yes
     - Message to be passed to the UserInterface


Output
^^^^^^

Instruction node that outputs a workspace value to the user interface.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - fromVar
     - StringType
     - yes
     - Name of the variable to be displayed in the UserInterface
   * - description
     - StringType
     - no
     - Description of the displayed variable

**Example**

.. code-block:: xml

   <Sequence>
       <Output fromVar="var1"/>
   </Sequence>
   <Workspace>
       <Local name="var1" type='{"type":"uint32"}' value='42' />
   </Workspace>

ResetVariable
^^^^^^^^^^^^^

Instruction to reset a variable to its initial state.

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
     - Name of the variable to reset

.. _reset_exp:

**Example**

.. code-block:: xml

   <Sequence>
       <Copy inputVar="a" outputVar="target"/>
       <ResetVariable varName="target"/>
       <Copy inputVar="b" outputVar="target"/>
   </Sequence>
   <Workspace>
       <Local name="target"/>
       <Local name="a" type='{"type":"uint8"}' value='1' />
       <Local name="b" type='{"type":"string"}' value='"some name"' />
   </Workspace>


UserConfirmation
^^^^^^^^^^^^^^^^

Simple instruction representing a user defined confirmation (success) or rejection (failure).

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
     - yes
     - Description of the needed user confirmation
   * - okText
     - StringType
     - no
     - text to be displayed in case of positive user confirmation
   * - cancelText
     - StringType
     - no
     - text to be displayed in case of negative user confirmation

VarExists
^^^^^^^^^

Instruction that checks the existence of a variable in the current workspace.


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
     - Name of variable to check

Wait
^^^^

Instruction node that returns SUCCESS after a given timeout.
The `timeout` attribute is optional. When this attribute is not present, the instruction returns SUCCESS immediately.


Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - timeout
     - Float64Type
     - no
     - Maximum time to wait

* An example for this instruction is already present in :ref:`ParallelSequence example <par_exp>`.

WaitForVariable
^^^^^^^^^^^^^^^

Instruction node that waits `timeout` seconds for a variable to be readable and non-empty.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - timeout
     - Float64Type
     - yes
     - Maximum time to wait
   * - varName
     - StringType
     - yes
     - name of the variable to be read
   * - equalsVar
     - StringType
     - no
     - variable to compare. The instruction will wait until the variables are equal or the timout s reached

* An example for this instruction is already present in :ref:`Repeat example <repeat_exp>`.

WaitForVariables
^^^^^^^^^^^^^^^^

Instruction node that waits `timeout` seconds for all workspace variables of a given type to be available.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - timeout
     - Float64Type
     - yes
     - Maximum time to wait
   * - varType
     - StringType
     - yes
     - typename of the variables to check
