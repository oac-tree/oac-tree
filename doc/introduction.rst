============
Introduction
============


The oac-tree framework leverages a goal-oriented approach to optimize procedure execution by allowing complex, and typically manual, tasks to be automated. The framework decision-making flow is controlled by a hierarchical tree of nodes, allowing the construction of maintainable and extensible procedures.

Behavior trees for automation
====================================

The oac-tree framework aims at allowing for routine and repetitive operational procedures to be executed with reduced human intervention, and therefore improving the overall operational efficiency, and ensuring a deterministic execution over a wide range of scenarios. By allowing laborious, and typically manual, tasks to be performed automatically or semi-automatically, human involvement can be shifted to tasks with a high added value.

Behavioural trees are a formal modelling language used to represent the behaviour of systems and software engineering projects. They provide flexible mathematical models used in various fields for executing tasks. Behavioural trees organize tasks into a finite set, enabling the creation of complex behaviours by modularly combining simpler tasks in a structured and hierarchical manner.
They are particularly useful for expressing complex requirements in a clear and unambiguous way, by providing a well-defined notation that captures the behaviour described in requirements and natural language. Behavioural tree semantics are grounded in process algebra and operational semantics, providing a basis for simulation, model checking, and failure modes and effects analysis.
The behavioural tree's modular approach enhances versatility and reduces errors, making them a perfect fit for a wide range of applications.

The hierarchical structure of behaviour trees is always composed from a root node, containing more control or action nodes. During execution, the root node periodically sends ticks to its child nodes, which in turn will provide feedback about their status to their parents.
In a behavioural tree, different behaviours and control mechanisms are provided via dedicated and specialized nodes. The main types of nodes are:

- Action Nodes: These nodes represent atomic actions that can be performed during execution. These types of nodes are also known as the tree's leafs and they cannot have child nodes.
- Control Nodes: These nodes are used to control the execution flow of the tree. In this group of nodes, one can find, at least, the following ones:

  - Sequence: Executes child nodes in order from left to right until one fails or all succeed;
  - Fallback: Executes child nodes in order from left to right until one succeeds or all fail;
  - Parallel: Executes all child nodes simultaneously.
  - Decorator: Modifies the default behaviour of a single child node.

As ticks are propagated through the tree until an action node is reached, status information will be provided in the opposite direction from child nodes to their parents. This feedback mechanism provides useful information regarding the tree's execution status. Depending on the application, at least, the following status values are available:

  - Not started: the node has not yet received any tick and is in its initial state.
  - Not finished: the node has already received at least one tick, but is not yet finished. It is waiting to receive further ticks to be able to reach a finished status.
  - Running: The node or one of its descendent nodes is executing in an asynchronous manner, i.e. on a different thread, and the node requires at least one more tick to be able to provide the exact status. Typically, this status indicates to the top level client that it may be good to wait a short while before inquiring again about the node's status, since continuous ticking would be counterproductive and waste computing resources.
  - Success: The node finished with success status.
  - Failure: The node finished with failure status.


Structure of oac-tree procedures
====================================

oac-tree procedures encapsulate all the instruction trees that the oac-tree can execute. They rely on simple and self-contained blocks of logic that define the execution flow.
The oac-tree framework requires procedures to be defined in a XML format. The XML structure of the procedure depends on the behaviour complexity and on the type of instructions being used.

The main sections of a procedure, their multiplicity, description and attributes, can be seen below.

Plugins Section
----------------

The plugins section Contains the name of the dynamic libraries to be loaded at runtime. Plugins allow to extend the existing oac-tree functionalities by providing new instructions or variables. In the XML procedure file, plugins are defined using the XML element *<Plugin>*. The plugin element has no attributes.
For a complete list of supported plugins, please refer to the plugins documentation ``TBD``.

.. list-table::
  :widths: auto

  * - Multiplicity
    - | 0..* (non-mandatory section).
      | One entry per plugin.
  * - Attributes
    - | N/A
  * - Example
    - | <Plugin>liboac-tree-ca.so</Plugin>
      | <Plugin>liboac-tree-pvxs.so</Plugin>


Type Declarations Section
--------------------------

The type declarations section contains the type definitions used by the variables. Type declarations can be used to simplify the variable definition by defining complex types or just an alias that can be reused across the workspace. In the XML procedure file, type declarations are defined using the XML element *<RegisterType>*.


.. list-table::
  :widths: auto

  * - Multiplicity
    - | 0..* (non-mandatory section).
      | One entry per type declaration.
  * - Attributes
    - | The type declaration element has the following attributes:
      |  - jsonfile: The name of the JSON file containing the type definition.
      |  - jsontype: The JSON type definition.
  * - Example
    - | <RegisterType jsonfile="range_uint32.json"/>
      | <RegisterType jsontype='{"type":"ranges_uint32","multiplicity":3,"element":{"type":"range_uint32"}}'/>


Instruction Trees Section
--------------------------

The instruction trees section provides the structured representation of the execution flow within a procedure. They depict the tree-like hierarchy of instructions or behaviours that guide the execution. In the XML procedure file, instructions are defined via XML elements using the instruction's names (from the core library or defined in the plugins).
For a complete list of supported instructions, please refer to the instructions' documentation ``core-instructions/variables``.

.. list-table::
  :widths: auto

  * - Multiplicity
    - | 1..* (mandatory section).
      | One entry per instruction tree.
  * - Attributes
    - | N/A
  * - Example
    - | <Sequence name="TestInRunningState">
      |   <Equals leftVar="test_is_running.value" rightVar="one"/>
      | </Sequence>


Workspace With Variables Section
---------------------------------

 The workspace section contains the global variables used by the instructions (to read, modify or write). Variables can be of different types and may interface to memory, the filesystem, the network, etc. In the XML procedure file, the workspace is defined using the XML element *<Workspace>* and the variables using the XML elements *<Local>*, *<ChannelAccessClient>*, *<PvAccessServer>*, *<File>*.
 For a complete list of supported types, please refer to the variables documentation ``core-instructions/variables``.

.. list-table::
  :widths: 25 50

  * - Multiplicity
    - | 1 (mandatory section).
      | One child entry per variable.
  * - Attributes
    - | N/A
  * - Example
    - | <Workspace>
      |   <Local name="zero" type='{"type":"uint32"}' value="0"/>
      |   <PvAccessServer name="test_is_running" channel="FTEST02:RUNNING"
      |     type='{"type":"seq::test::Type/v1.0","attributes":[{"value":{"type":"uint32"}}]}' value='{"value":1}'
      |   />
      |   <File name="file" file="/tmp/variable.bck"/>
      | </Workspace>

Procedure Example
------------------

The following procedure was extracted from the oac-tree test campaign, where it is possible to see that the procedure is defined in a typical XML format, and contains some of sections presented before.

.. code-block:: xml

  <?xml version="1.0" encoding="UTF-8"?>
  <Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
            name="oac-tree functional test"
            xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
            xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd">
    <Plugin>liboac-tree-ca.so</Plugin>
    <Repeat isRoot="True" maxCount="-1">
      <Sequence>
        <Wait timeout="0.2"/>
        <Include name="Check if test is running" path="TestInRunningState"/>
        <ForceSuccess>
          <Include name="Evaluate device status" path="ProcessDeviceStatus"/>
        </ForceSuccess>
        <Output fromVar="devices_ready" description="devices_ready" />
      </Sequence>
    </Repeat>
    <Sequence name="TestInRunningState">
      <!-- <Output fromVar="test_is_active" description="test_is_active:" /> -->
      <Equals leftVar="test_is_active" rightVar="one"/>
    </Sequence>
    <Sequence name="ProcessDeviceStatus">
      <Inverter>
        <Include name="Conditionally set ready status" path="ConditionallySetSystemInReadyState"/>
      </Inverter>
      <Copy name="Set status to Not Ready" inputVar="zero" outputVar="devices_ready"/>
    </Sequence>
    <Sequence name="ConditionallySetSystemInReadyState">
        <Sequence name="AllReady">
          <Equals leftVar="dev1_status" rightVar="one"/>
          <Equals leftVar="dev2_status" rightVar="one"/>
        </Sequence>
        <Copy name="Set status Ready" inputVar="one" outputVar="devices_ready"/>
        <!-- <Output fromVar="devices_ready" description="devices_ready" /> -->
    </Sequence>
    <Workspace>
      <Local name="zero" type='{"type":"uint32"}' value="0"/>
      <Local name="one" type='{"type":"uint32"}' value="1"/>
      <ChannelAccessClient name="test_is_active" channel="FTEST01:RUNNING" type='{"type":"uint32"}'/>
      <ChannelAccessClient name="dev1_status" channel="FTEST01:DEV1-STATUS" type='{"type":"uint32"}'/>
      <ChannelAccessClient name="dev2_status" channel="FTEST01:DEV2-STATUS" type='{"type":"uint32"}'/>
      <ChannelAccessClient name="devices_ready" channel="FTEST01:DEVICES-READY" type='{"type":"uint32"}'/>
    </Workspace>
  </Procedure>
