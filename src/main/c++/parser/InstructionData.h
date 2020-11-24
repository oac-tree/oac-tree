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
 * @file InstructionData.h
 * @brief Header file for InstructionData class.
 * @date 23/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the InstructionData class.
 */

#ifndef _SEQ_InstructionData_h_
#define _SEQ_InstructionData_h_

// Global header files

#include <map>
#include <memory>
#include <string>
#include <vector>

// Local header files

#include "instructions/Instruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Data representation of an Instruction.
 */

class InstructionData
{
  private:
    std::string _type;
    std::map<std::string, std::string> _attributes;
    std::vector<InstructionData> _children;

  protected:

  public:
    /**
     * @brief Constructor.
     * @param instr_type Instruction typename.
     */
    InstructionData(std::string instr_type);

    /**
     * @brief Destructor.
     */
    ~InstructionData() = default;

    /**
     * @brief Retrieve the instruction's typename.
     *
     * @return Typename of instruction.
     */
    std::string GetType() const;

    /**
     * @brief Indicate presence of attribute with given name.
     *
     * @param name Attribute name.
     * @return true when present.
     */
    bool HasAttribute(const std::string & name);

    /**
     * @brief Add attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return true on successful adding.
     */
    bool AddAttribute(const std::string & name, const std::string & value);

    /**
     * @brief Add child instruction.
     *
     * @param child Data representation of child instruction.
     * @return true on success.
     */
    bool AddChild(const InstructionData & child);

    /**
     * @brief Retrieve map of all attributes.
     *
     * @return Map of all attributes.
     */
    const std::map<std::string, std::string> & Attributes() const;

    /**
     * @brief Retrieve all child instructions.
     *
     * @return List of child instructions.
     */
    const std::vector<InstructionData> & Children() const;

    /**
     * @brief Generate the Instruction object corresponding to the current data.
     *
     * @return Unique pointer to Instruction object.
     * @note The generated instruction will contain all child instructions encoded
     * in the current InstructionData.
     */
    std::unique_ptr<Instruction> GenerateInstruction() const;
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

#endif // _SEQ_InstructionData_h_
