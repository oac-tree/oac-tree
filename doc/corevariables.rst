Core Variables
==============

The :ref:`variable` class is an interface for workspace variables, designed to manage and manipulate various types of variables within a software system. The core sequencer library supports two types of variable: local and file.

Local Variable
--------------

This Variable type defines a variable in memory. These variables can be used (read and written to) from the workspace by instructions.
If a variable has a defined type, without a defined value, a zero-initialized AnyValue will be allocated.

Unless the attribute `dynamicType` is equal to 'true', the underlying AnyType of the variable is fixed and all assignments will try to convert to the destination type. The only exception to this rule is when the definition does not include a type. In that case, the variable will be initialized as an `Empty` type and will have a fixed `AnyType` from the moment of its first assignment.

When `dynamicType` is set, all assignments to it will have the semantics of normal `AnyValue` assignments, i.e. types can change.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - type
     - StringType
     - no
     - json style definition of the variable type
   * - value
     - StringType
     - no
     - json style definition of the variable value
   * - dynamicType
     - BooleanType
     - no
     - defines if the variable's type is dynamic

.. _local_exp:

**Example**

In the example, two local variables are defined, and variable "a" is copied to variable "b".

.. code-block:: xml

    <Sequence>
        <Copy name="copy" inputVar="a" outputVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' />
    </Workspace>



File Variable
-------------

This Variable type defines a variable that will read and write values from a file.

A file variable does not impose extra type constraints during assignment. It does however inherit the constraints from the underlying `AnyValue`, such as immutable element types for arrays.

Attributes:

.. list-table::
   :widths: 25 25 15 50
   :header-rows: 1

   * - Attribute name
     - Attribute type
     - Mandatory
     - Description
   * - fileName
     - StringType
     - yes
     - name of the file where variables are defined
   * - pretty
     - BooleanType
     - no
     - defines if the JSON representation is written with pretty printing

.. _file_exp:

**Example**

In the example, the variable "input" is copied to the variable "file", that writes it in the file "/tmp/variable.bck".

.. code-block:: xml

    <Sequence>
        <Copy name="copy" inputVar="input" outputVar="file"/>
    </Sequence>
    <Workspace>
        <Local name="input"
               type='{"type":"MyStruct","attributes":[{"value":{"type":"float32"}}]}'
               value='{"value":0.0}'/>
        <File name="file"
              fileName="/tmp/variable.bck"/>
    </Workspace>
