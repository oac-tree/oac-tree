/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_PROCEDURE_H_
#define SUP_SEQUENCER_PROCEDURE_H_

#include <sup/sequencer/attribute_map.h>
#include <sup/sequencer/execution_status.h>

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

namespace sequencer
{
class Instruction;
class UserInterface;
class Variable;
class Workspace;

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
private:
  std::vector<std::unique_ptr<Instruction>> m_instructions;
  std::unique_ptr<Workspace> m_workspace;

  AttributeMap m_attributes;

  /**
   * @brief Name of file from which this procedure was loaded (if loaded form file).
   *
   * @note This filename may include the relative or absolute paths.
   */
  std::string m_filename;

  // Cache for other procedures loaded from files and to be used by include nodes.
  mutable std::map<std::string, std::unique_ptr<Procedure>> m_procedure_cache;

  /**
   * @brief Load a procedure from file or cache.
   *
   * @param filename Filename of the procedure file.
   * @return Pointer to the procedure or nullptr in case of failure to load the procedure.
   *
   * @details This method returns a pointer to a const Procedure. The Procedure itself
   * is owned by the cache.
   */
  const Procedure* LoadProcedure(const std::string& filename) const;

  /**
   * @brief Clear the cached procedures.
   *
   * @details This method needs to be called anytime there is a possibility of changes
   * to the loaded files on disk.
   */
  void ClearProcedureCache() const;

protected:
public:
  /**
   * @brief Constructor.
   */
  Procedure();

  /**
   * @brief Destructor.
   */
  ~Procedure();

  /**
   * @brief Set the filename of this procedure (if loaded from file).
   *
   * @param filename Filename of this procedure.
   * @details This filename is used for external includes with relative pathnames.
   */
  void SetFilename(const std::string& filename);

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
   * @brief Get top-level instructions (from other procedure file if requested).
   *
   * @param filename Optional filename for external loading of instructions.
   * @return List of top-level instructions.
   * @details If the filename argument is not empty, this method will first look into the
   * procedure cache to see if this file was already loaded. If not, it will load it into the
   * cache and then return its top-level instructions.
   */
  std::vector<const Instruction*> GetInstructions(const std::string& filename = {}) const;

  /**
   * @brief Get number of top-level instructions.
   */
  int GetInstructionCount() const;

  /**
   * @brief Push Instruction at top level.
   *
   * @note Procedure takes ownership of the instruction and will take care of its timely
   * destruction. This implies that the client code should NEVER push the same
   * instruction more than once.
   *
   * @throw InvalidOperationException when trying to push a nullptr.
   */
  void PushInstruction(Instruction* instruction);

  /**
   * @brief Inserts Instruction at the specified position.
   *
   * @param Position in the vector where instruction is inserted.
   * @note Procedure takes ownership of the instruction.
   */
  bool InsertInstruction(Instruction* instruction, int index);

  /**
   * @brief Removes Instruction from the specified position.
   *
   * @param Position in the vector from where instruction is removed.
   * @note Ownership is handed to the user.
   */
  Instruction* TakeInstruction(int index);

  /**
   * @brief Add variable.
   *
   * @param name Variable name.
   * @param var Variable to add.
   * @return true on successful addition.
   * @note Procedure, or rather its underlying Workspace, takes ownership of the
   * variable and will take care of its timely destruction.
   */
  bool AddVariable(std::string name, Variable* var);

  /**
   * @brief List all variable names.
   *
   * @return Variable name list.
   */
  std::vector<std::string> VariableNames() const;

  /**
   * @brief Get variable value.
   *
   * @return true on successful retrieval.
   */
  bool GetVariableValue(std::string name, sup::dto::AnyValue& value) const;

  /**
   * @brief Setup the procedure.
   *
   * @return true on successful setup.
   */
  bool Setup();

  /**
   * @brief Execute single step of procedure.
   *
   * @param ui UserInterface to use for instruction input/output.
   */
  void ExecuteSingle(UserInterface* ui);

  /**
   * @brief Halt the procedure's execution.
   *
   * @note This is mainly used for interrupting all instructions that are running in a separate
   * thread.
   */
  void Halt();

  /**
   * @brief Reset procedure.
   *
   * @details This method request a halt and blocks until all asynchronous instructions are
   * finished.
   */
  void Reset();

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
  std::string GetAttribute(const std::string& name) const;

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
   * @param attributes Attribute map.
   * @return true when successful.
   */
  bool AddAttributes(const AttributeMap& attributes);

  /**
   * @brief Returns pointer to internal workspace.
   *
   * @return workspace.
   */
  const Workspace* GetWorkspace() const;

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
   * @return Pointer to type registry.
   */
  const sup::dto::AnyTypeRegistry* GetTypeRegistry() const;

  /**
   * @brief Add a generic callback for variable updates.
   *
   * @param cb Callback function object.
   * @return true if adding the callback was successful.
   *
   * @note Users are responsible for ensuring the callback outlives the underlying workspace.
   */
  bool RegisterGenericCallback(
    const std::function<void(const std::string&, const sup::dto::AnyValue&)>& cb);

  /**
   * @brief Name of attribute that defines the timeout between ticks.
   */
  static const std::string TICK_TIMEOUT_ATTRIBUTE_NAME;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_H_
