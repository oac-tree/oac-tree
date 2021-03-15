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
 * @file DecoratorInstruction.h
 * @brief Header file for DecoratorInstruction class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the DecoratorInstruction class.
 */

#ifndef _SEQ_DecoratorInstruction_h_
#define _SEQ_DecoratorInstruction_h_

// Global header files

#include <memory>

// Local header files

#include "Instruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Interface class representing a decorated instruction.
 */

class DecoratorInstruction : public Instruction
{
  private:
    void ResetHook() override;

    void HaltImpl() override;

    /**
     * @brief Get list of child instructions implementation (const version).
     *
     * @return List of child instructions.
     */
    std::vector<const Instruction *> ChildInstructionsImpl() const override;

  protected:
    /**
     * @brief Decorated instruction
     */
    std::unique_ptr<Instruction> _child;

    /**
     * @brief Setup implementation.
     *
     * @param proc Procedure containing Workspace and instruction declarations.
     * @return true on successful instruction setup.
     */
    bool SetupImpl(const Procedure & proc) override;

  public:
    /**
     * @brief Constructor.
     *
     * @param type The type of instruction.
     */
    DecoratorInstruction(const std::string & type);

    /**
     * @brief Destructor.
     */
    ~DecoratorInstruction() override;

    /**
     * @brief Set the decorated child instruction.
     *
     * @param instruction Pointer to decorated instruction.
     * @note DecoratorInstruction takes ownership of the decorated instruction.
     */
    void SetInstruction(Instruction * instruction);
};

// Global variables

// Function declarations

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_DecoratorInstruction_h_
