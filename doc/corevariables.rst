Core Variables
==============

The :ref:`variable` class is an interface for workspace variables, designed to manage and manipulate various types of variables within a software system. The core sequencer library supports two types of variable: local and file.

Local Variable
--------------

This Variable type defines variable directly in the workspace. This variables can be used (read and written to) from the workspace by instructions.
If a variable has a defined type, without a defined value, a zero-initialized AnyValue will be allocated.

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
     - defines if the variable is dynamic

.. _local_exp:

**Example**

In the example, two local variables are defined, and variable "a" is copied to variable "b".

.. code-block:: xml

    <Sequence>
        <Copy name="copy" input="a" output="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' />
    </Workspace>



File Variable
-------------

This Variable type defines variable that will read and write values from a file.

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
     - defines if variable is written with pretty print

.. _file_exp:

**Example**

In the example, the variable "input" is copied to the variable "file", that writes it in the file "/tmp/variable.bck".

.. code-block:: xml

    <Sequence>
        <Copy name="copy" input="input" output="file"/>
    </Sequence>
    <Workspace>
        <Local name="input"
            type='{"type":"MyStruct","attributes":[{"value":{"type":"float32"}}]}'
            value='{"value":0.0}'/>
        <File name="file"
            fileName="/tmp/variable.bck"/>
    </Workspace>
