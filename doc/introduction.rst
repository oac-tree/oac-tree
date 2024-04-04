Introduction
============

The sequencer framework leverages on a goal-oriented approach to optimize procedure execution by allowing complex, and typically manual, tasks to be automated. The framework decision-making flow is controlled by a hierarchical tree of nodes, ensuring an efficient control and execution.

Behavior trees for automation
-----------------------------

The sequencer framework aims at allowing for routine and repetitive operational procedures to be executed with reduced human intervention, and therefore improving the overall operational efficiency, and ensuring a deterministic execution over a wide range of scenarios. By allowing laborious, and typically manual, tasks to be performed automatically or semi-automatically, human involvement can be shifted to tasks with a high added value.

Behavioural trees are a formal, graphical modelling language used to represent the behaviour of systems and software engineering projects. They provide flexible mathematical models used in various fields for executing plans. BTs organize tasks into a finite set, enabling the creation of complex behaviours by modularly combining simpler tasks in a structured and hierarchical manner. 
They're particularly useful for expressing complex requirements in a clear and unambiguous way, by providing a well-defined notation that captures the behaviour described in requirements, natural language.BTs semantics are grounded in process algebra and operational semantics, providing a basis for simulation, model checking, and failure modes and effects analysis.
BTs modular approach enhances versatility and reduces errors, making them a perfect fit for a wide range of applications. 

The hierarchical structure of behaviour trees are always composed by a root node, one or more control nodes and one or more task nodes. During execution, the root node periodically sends ticks to its child nodes, which in turn will provide feedback about their status to their parents.
In a BT, different behaviours and control mechanisms are provided via dedicated and specialized nodes. The main types of nodes are:

- Action Nodes: These nodes represent atomic actions that can be performed during execution. These types of nodes are also known as the tree leaf's, or childless nodes.
- Control Nodes: These nodes are used to control the execution flow of the tree. In this group of nodes, one can find, at least, the following ones:

  - Sequence: Executes child nodes in order from left to right until one fails or all succeed;
  - Fallback: Executes child nodes in order from left to right until one succeeds or all fail;
  - Parallel: Executes all child nodes simultaneously.
  - Decorator: Modifies the default behaviour of its child node.

As ticks are propagated through the tree until an action node is reached, status information will be provided in the opposite direction from child's to their parents. This feedback mechanism provides useful information regarding the tree execution status, and depending on the application, at least, the following status values are available:

  - Success: The action or condition succeeded.
  - Failure: The action or condition failed.
  - Running: The action or condition is still in progress and needs more time to complete.


Structure of Sequencer procedures
---------------------------------

Sequencer procedures encapsulate all the instruction trees that the sequencer can execute. They rely on simple and self-contained blocks of logic that define the execution flow.
The sequencer framework requires procedures to be defined in a XML format. The XMl structure of the procedure depends, on the behaviour complexity and on the type of instructions being used.

The following table presents the main sections of a procedure, their multiplicity and their respective description:

.. list-table::
  :widths: 25 25 15 50
  :header-rows: 1

  * - XML Section
    - Multiplicity
    - Description
    - Example
  * - Plugins
    - 0..* (non-mandatory section). One entry per plugin.
    - | Contains the name of the dynamic libraries to be loaded at runtime.
      | Plugins allow to extend the existing sequencer functionalities by providing new instructions.
      | In the XML procedure file, plugins are defined using the XML element *<Plugin>*. The plugin element doesn't contemplate configuration attributes.
      | For a complete list of supported plugins, please refer to the plugins documentation ``TBD``.
    - <Plugin>libsequencer-ca.so</Plugin>
      <Plugin>libsequencer-pvxs.so</Plugin>
  * - Type declarations
    - 0..* (non-mandatory section). One entry per type declaration.
    - | Contains the type definitions used by the instructions. Type declarations can be used to simplify the procedure definition by defining complex types or just an alias that can be reused across the procedure. 
      | In the XML procedure file, type declarations are defined using the XML element *<RegisterType>*. 
      | The type declaration element has the following attributes:
      |  - jsonfile: The name of the JSON file containing the type definition.
      |  - jsontype: The JSON type definition.
    - | <RegisterType jsonfile="range_uint32.json"/>
      | <RegisterType jsontype='{"type":"ranges_uint32","multiplicity":3,"element":{"type":"range_uint32"}}'/>
  * - Instruction trees
    - 1..* (mandatory section). One entry per instruction tree.
    - | Instruction trees, provide the structured representation of the execution flow within a procedure.
      | Graphically, they depict the sequence and hierarchy of instructions or behaviours that guide the execution.
      | In the XML procedure file, instructions are defined via XML elements using the Core Instructions names or the ones defined in the plugins. 
      | For a complete list of supported instructions, please refer to the instructions documentation ``core-instructions/variables``.
    - | <Sequence name="TestInRunningState">
      |   <Equals leftVar="test_is_running.value" rightVar="one"/>
      | </Sequence>
  * - Workspace with variables
    - 1..* (mandatory section). One entry per variable.
    - | Workspace section contains the variables used by the instructions (to read from or write to).
      | Variables can be of different types, core types or custom types as the ones defined using Type Declarations.
      | In the XML procedure file, the workspace is defined using the XML element *<WorkSpace>* and the variables using the XML elements *<Local>*, *<ChannelAccessClient>*, *<PvAccessServer>*, *<File>*.
      | For a complete list of supported types, please refer to the variables documentation ``core-instructions/variables``.
    - | <Local name="zero" type='{"type":"uint32"}' value="0"/>
      | <PvAccessServer name="test_is_running" channel="FTEST02:RUNNING" 
      |   type='{"type":"seq::test::Type/v1.0","attributes":[{"value":{"type":"uint32"}}]}' value='{"value":1}'
      | />
      | <File name="file" file="/tmp/variable.bck"/>


The following is an example of a procedure used in the sequencer test campaign. The procedure is defined in XML format, and contains some of sections presented before:

.. code-block:: xml

  <?xml version="1.0" encoding="UTF-8"?>
  <Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
            name="Sequencer functional test"
            xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
            xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Plugin>libsequencer-ca.so</Plugin>
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
