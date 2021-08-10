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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/**
 * @file Include.h
 * @brief Header file for Include class.
 * @date 03/03/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the Include class.
 */

#ifndef _SEQ_Include_h_
#define _SEQ_Include_h_

// Global header files

// Local header files

#include "DecoratorInstruction.h"

// Constants

// Type definition

#ifdef __cplusplus

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
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;
  bool PostInitialiseVariables(const AttributeMap& source) override;

  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Tries to load and copy an instruction tree as its child.
   */
  bool SetupImpl(const Procedure& proc) override;

  std::string GetPath() const;

  ExecutionStatus CalculateStatus() const;

  /**
   * @brief Name of file from which this include instruction was loaded (if loaded form file).
   *
   * @note This filename may include the relative or absolute paths.
   */
  std::string _filename;

protected:
public:
  /**
   * @brief Constructor.
   */
  Include();

  /**
   * @brief Destructor.
   */
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

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

// Global variables

// Function declarations

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_Include_h_
