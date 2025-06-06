/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_PROCEDURE_H_
#define SUP_OAC_TREE_PROCEDURE_H_

#include <sup/oac-tree/attribute_handler.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/procedure_context.h>
#include <sup/oac-tree/procedure_preamble.h>
#include <sup/oac-tree/scope_guard.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sup
{
namespace dto
{
class AnyType;
class AnyTypeRegistry;
class AnyValue;
}  // namespace dto

namespace oac_tree
{
class Instruction;
class InstructionTree;
class ProcedureStore;
class UserInterface;
class Variable;
class Workspace;

const std::string kTickTimeoutAttributeName = "tickTimeout";
const std::string kTimingAccuracyAttributeName = "timingAccuracy";

/**
 * @brief Procedure contains a tree of instructions
 *
 * @details A Procedure object contains a full instruction tree and a workspace
 * @note The client of the Procedure, i.e. the object responsible for its creation and
 * destruction, needs to ensure that the Procedure is correctly set up before executing it (by
 * calling the Setup method on it). Likewise, the client needs to call Reset on the Procedure
 * before destroying the UserInterface class. Reset will block until all threads have terminated.
 */
class Procedure
{
public:
  using GenericCallback = std::function<void(const std::string&, const sup::dto::AnyValue&, bool)>;

  /**
   * @brief Constructor.
   *
   * @param filename Filename of this procedure or empty if not created from file.
   */
  Procedure(const std::string& filename = "");
  ~Procedure();

  /**
   * @brief Get the filename of this procedure (if loaded from file).
   *
   * @return Filename of this procedure.
   * @details This filename is used for external includes with relative pathnames.
   */
  std::string GetFilename() const;

  /**
   * @brief Get root instruction.
   *
   * @return Root instruction.
   */
  Instruction* RootInstruction();

  /**
   * @brief Get root instruction (const version).
   *
   * @return Root instruction.
   */
  const Instruction* RootInstruction() const;

  /**
   * @brief Get top-level instructions.
   *
   * @return List of top-level instructions.
   */
  std::vector<const Instruction*> GetTopInstructions() const;

  /**
   * @brief Get the procedure's context, like its filename and the main ProcedureStore.
   *
   * @return ProcedureContext structure.
   */
  ProcedureContext GetContext() const;

  /**
   * @brief Get tree instructions that will be executed next.
   *
   * @return Tree of instructions.
   * @details This method should only be called in between execution steps.
   */
  InstructionTree GetNextInstructionTree() const;

  /**
   * @brief Get number of top-level instructions.
   *
   * @return Number of instructions
   */
  int GetInstructionCount() const;

  /**
   * @brief Push Instruction at top level.
   *
   * @param instruction Instruction to push.
   *
   * @throw InvalidOperationException when trying to pass an empty unique_ptr.
   */
  void PushInstruction(std::unique_ptr<Instruction>&& instruction);

  /**
   * @brief Inserts Instruction at the specified position.
   *
   * @param instruction Instruction to be inserted
   * @param index Position in the vector where instruction is inserted.
   * @return true on successful insertion.
   */
  bool InsertInstruction(std::unique_ptr<Instruction>&& instruction, int index);

  /**
   * @brief Removes Instruction from the specified position.
   *
   * @param index Position in the vector from where instruction is removed.
   * @return Removed instruction
   */
  std::unique_ptr<Instruction> TakeInstruction(int index);

  /**
   * @brief Add variable.
   *
   * @param name Variable name.
   * @param var Variable to add.
   * @return true on successful addition.
   */
  bool AddVariable(std::string name, std::unique_ptr<Variable>&& var);

  /**
   * @brief List all variable names.
   *
   * @return Variable name list.
   */
  std::vector<std::string> VariableNames() const;

  /**
   * @brief Get variable value.
   *
   * @param name Variable name
   *
   * @param value AnyValue to where the variable will be retrieved.
   *
   * @return true on successful retrieval.
   */
  bool GetVariableValue(std::string name, sup::dto::AnyValue& value) const;

  /**
   * @brief Setup the procedure.
   */
  void Setup();

  /**
   * @brief Execute single step of procedure.
   *
   * @param ui UserInterface to use for instruction input/output.
   */
  void ExecuteSingle(UserInterface& ui);

  /**
   * @brief Halt the procedure's execution.
   *
   * @note This is mainly used for interrupting all instructions that are running in a separate
   * thread.
   */
  void Halt();

  /**
   * @brief Reset the procedure to its initial state after setup.
   *
   * @param ui UserInterface to use for instruction status updates.
   *
   * @details All workspaces are torn down and setup again; instructions receive a reset command.
   * The procedure can be started afterwards in a fresh state.
   */
  void Reset(UserInterface& ui);

  /**
   * @brief Retrieve status of root sequence.
   *
   * @return Current execution status of the root instruction.
   */
  ExecutionStatus GetStatus() const;

  /**
   * @brief Indicate presence of attribute with given name.
   *
   * @param name Attribute name.
   * @return true when present.
   */
  bool HasAttribute(const std::string& name) const;

  /**
   * @brief Get attribute with given name.
   *
   * @param name Attribute name.
   * @return Attribute value.
   */
  std::string GetAttributeString(const std::string& name) const;

  /**
   * @brief Set attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true when successful.
   */
  bool AddAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Add all attributes from a given map.
   *
   * @param str_attributes List of attributes.
   * @return true when successful.
   */
  bool AddAttributes(const StringAttributeList& str_attributes);

  /**
   * @brief Get attribute value with given name and type.
   *
   * @param attr_name Attribute name.
   * @return Attribute value of requested type.
   *
   * @throws RuntimeException when attribute with given name was not found or its value could not
   * be converted to the requested type.
   */
  template <typename T>
  T GetAttributeValue(const std::string& attr_name) const
  {
    T val;
    if (!m_attribute_handler.GetValueAs(attr_name, val))
    {
      const std::string message = "Procedure::GetAttributeValue(): no attribute with name [" +
                                  attr_name + "]";
      throw RuntimeException(message);
    }
    return val;
  }

  /**
   * @brief Returns pointer to internal workspace.
   *
   * @return workspace.
   */
  const Workspace& GetWorkspace() const;

  /**
   * @brief Returns pointer to internal workspace.
   *
   * @return workspace.
   */
  Workspace& GetWorkspace();

  /**
   * @brief Returns procedure's preamble.
   *
   * @return ProcedurePreamble object reference.
   */
  const ProcedurePreamble& GetPreamble() const;

  /**
   * @brief Returns procedure's preamble.
   *
   * @return ProcedurePreamble object reference.
   */
  ProcedurePreamble& GetPreamble();

  /**
   * @brief Register an AnyType instance under its own name.
   *
   * @param anytype AnyType instance to register.
   *
   * @return true on successful registration.
   */
  bool RegisterType(const sup::dto::AnyType& anytype);

  /**
   * @brief Get the current type registry.
   *
   * @return Reference to type registry.
   */
  const sup::dto::AnyTypeRegistry& GetTypeRegistry() const;

  /**
   * @brief Add a generic callback for variable updates.
   *
   * @param cb Callback function object.
   * @param listener Pointer to object that listens to updates.
   *
   * @return true if adding the callback was successful.
   *
   * @note Users are responsible for ensuring the callback outlives the underlying workspace or to
   * unregister it using ScopeGuard object.
   */
  bool RegisterGenericCallback(const GenericCallback& cb, void* listener);

  void SetParentProcedure(Procedure* parent);

  /**
   * @brief Register types and load plugins defined in the preamble.
   */
  void SetupPreamble();

private:
  void Teardown(UserInterface& ui);
  const ProcedureStore& GetProcedureStore() const;

  std::vector<std::unique_ptr<Instruction>> m_instructions;
  std::unique_ptr<Workspace> m_workspace;

  /**
   * @brief Attribute handler.
  */
  AttributeHandler m_attribute_handler;

  /**
   * @brief Preamble information (loaded plugins and type registrations).
   */
  ProcedurePreamble m_preamble;

  // Parent procedure pointer if this procedure was loaded from another
  Procedure* m_parent;

  // Cache for other procedures loaded from files and to be used by include nodes.
  std::unique_ptr<ProcedureStore> m_procedure_store;
};

/**
 * @brief Query the desired timeout in ns of the procedure.
 *
 * @returns Timeout in ns of the procedure (attribute or default value)
 *
 * @details Applications are free to ignore this setting. It is only provided as a guideline.
 */
sup::dto::int64 TickTimeoutNs(const Procedure& procedure);

/**
 * @brief Query the desired timing accuracy in ns of the procedure.
 *
 * @returns Timing accuracy in ns of the procedure (attribute or default value)
 *
 * @details Applications are free to ignore this setting. It is only provided as a guideline.
 */
sup::dto::int64 TimingAccuracyNs(const Procedure& procedure);

/**
 * @brief Get the name of the procedure.
 *
 * @param procedure Procedure to query.
 * @return Name attribute of the procedure if present or its possibly empty filename.
 */
std::string GetProcedureName(const Procedure& procedure);

  /**
   * @brief Get list of instructions that will be executed next.
   *
   * @param proc Procedure to query.
   *
   * @return List of instructions.
   * @details This function should only be called in between execution steps.
   */
std::vector<const Instruction*> GetNextInstructions(const Procedure& proc);

  /**
   * @brief Get list of instruction leaves that will be executed next.
   *
   * @param proc Procedure to query.
   *
   * @return List of instruction leaves.
   * @details This function should only be called in between execution steps.
   */
std::vector<const Instruction*> GetNextLeaves(const Procedure& proc);

  /**
   * @brief Create an AnyType from a TypeRegistrationInfo object.
   *
   * @param info TypeRegistrationInfo object.
   * @param filename filename of the procedure to which the TypeRegistrationInfo object belongs.
   * @param type_registry Type registry to use while parsing.
   *
   * @return Created AnyType.
   *
   * @throw ProcedureSetupException when the parsing did not succeed.
   *
   * @details This function does not add the created AnyType to the registry.
   */
sup::dto::AnyType ParseTypeRegistrationInfo(const TypeRegistrationInfo& info,
                                            const std::string& filename,
                                            const sup::dto::AnyTypeRegistry& type_registry);

/**
 * @brief Create a full pathname from a given directory and filename.
 *
 * @param directory Directory path.
 * @param filename Filename.
 *
 * @return Full pathname, assembling the directory and the filename if the filename does not yet
 * denote an absolute path (starts with '/'), otherwise returns the filename unchanged.
 */
std::string GetFullPathName(const std::string& directory, const std::string& filename);

/**
 * @brief Get the dirctory part of a filename.
 *
 * @param filename Filename.
 *
 * @return Directory path of a filename or empty string if the filename does not contain any path
 * delimiters ('/').
 */
std::string GetFileDirectory(const std::string& filename);

  /**
   * @brief Get the path of the given Procedure to allow to resolve relative pathnames.
   *
   * @param proc Procedure to query.
   *
   * @return Path of the provided Procedure.
   */
std::string GetProcedurePath(const Procedure& proc);

  /**
   * @brief Resolve a filename that may contain a relative pathname with respect to a Procedure's
   * location.
   *
   * @param proc Procedure to query.
   * @param filename Filename that may contain a relative path.
   *
   * @return Resolved filename.
   */
std::string ResolveRelativePath(const Procedure& proc, const std::string& filename);

  /**
   * @brief Get a ScopeGuard object that will unregister callbacks upon destruction.
   *
   * @param proc Procedure that will hold the callback object.
   * @param listener Pointer to object that listens to updates.
   *
   * @return ScopeGuard object.
   */
ScopeGuard GetCallbackGuard(Procedure& proc, void *listener);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_PROCEDURE_H_
