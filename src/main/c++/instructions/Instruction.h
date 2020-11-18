#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <map>

#include "Attribute.h"

#include "procedure/ExecutionStatus.h"

class UserInterface;
class Workspace;

/**
 * @brief Abstract interface for all executable instructions
 * @details Instruction is the base class for all executable
 * instructions. These include: sequences, selectors, decorator
 * nodes and leaf instruction nodes
 *
 * @note The design of the execution of these instructions
 * is based on private virtual implementation.
 */
class Instruction
{
private:
  std::string _type;

  ExecutionStatus _status;

  ExecutionStatus _status_before;

  std::map<std::string, std::string> _attributes;

  virtual void Preamble(UserInterface * ui);

  virtual ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) = 0;

  virtual void Postamble(UserInterface * ui);

public:
  /**
   * @brief Constructor
   */
  Instruction(std::string type);

  /**
   * @brief Virtual destructor
   */
  virtual ~Instruction();

  /**
   * @brief Get instruction type
   * @return instruction type
   */
  std::string GetType() const;

  /**
   * @brief Get instruction name
   * @return instruction name
   */
  std::string GetName() const;

  /**
   * @brief Set instruction name
   * @param name Name to set
   * @return void
   */
  void SetName(std::string name);

  /**
   * @brief Execution method
   * @details
   * @param
   * @return
   */
  void ExecuteSingle(UserInterface * ui, Workspace * ws);

  /**
   * @brief Get execution status
   */
  ExecutionStatus GetStatus() const;

  /**
   * @brief Reset execution status
   */
  void ResetStatus();

  /**
   * @brief Get attribute with given name
   *
   * @param name Attribute name
   */
  std::string GetAttribute(const std::string & name) const;

  /**
   * @brief Set attribute with given name and value
   *
   * @param name Attribute name
   * @param value Attribute value
   */
  std::string SetAttribute(const std::string & name, const std::string & value);
};

#endif // INSTRUCTION_H
