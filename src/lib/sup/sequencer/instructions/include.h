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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_INCLUDE_H_
#define SUP_SEQUENCER_INCLUDE_H_

#include "decorator_instruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator that includes an instruction tree by reference.
 *
 * @details The reference can point to an instruction tree in the same definition
 * file or to one defined in a separate file ('file' attribute).
 */
class Include : public DecoratorInstruction
{
public:
  Include();

  ~Include() override;

  /**
   * @brief Set the filename of this include instruction (if loaded from file).
   *
   * @param filename Filename of this include instruction.
   * @details This filename is used for external includes with relative pathnames.
   */
  void SetFilename(const std::string& filename);

  /**
   * @brief Get the filename of this include instruction (if loaded from file).
   *
   * @return Filename of this include instruction.
   * @details This filename is used for external includes with relative pathnames.
   */
  std::string GetFilename() const;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  bool PostInitialiseVariables(const AttributeMap& source) override;

  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus CalculateStatus() const;

  /**
   * @brief Name of file from which this include instruction was loaded (if loaded form file).
   */
  std::string _filename;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INCLUDE_H_
