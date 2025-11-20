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

#ifndef SUP_OAC_TREE_INSTRUCTION_DOC_OBJECTS_H_
#define SUP_OAC_TREE_INSTRUCTION_DOC_OBJECTS_H_

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/documentation_entry.h>

#include "all_instructions.h"

namespace sup
{
namespace oac_tree
{

/***************************************/
/* Compound instructions               */
/***************************************/

template <>
DocomentationEntry InstructionDocFor<Choice>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Executes a subset of children depending on a variable value" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Fallback>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Compound instruction representing a list of alternative actions" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<ParallelSequence>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Compound instruction representing a parallel sequence" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<ReactiveFallback>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Compound instruction representing a reactive fallback" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<ReactiveSequence>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Compound instruction representing a reactive sequence" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Sequence>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Compound instruction representing a sequence" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<UserChoice>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Compound instruction representing a choice of instructions" }
  }};
  return doc;
}

/***************************************/
/* Decorator instructions              */
/***************************************/

template <>
DocomentationEntry InstructionDocFor<Async>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that makes the execution of its child asynchronous" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<ForInstruction>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that applies a child instruction to the elements of an array" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<ForceSuccess>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Wraps a child instruction and always returns SUCCESS when the child has finished" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Include>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that includes an instruction tree by reference" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<IncludeProcedure>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator instruction that includes an external procedure (workspace and instruction tree)" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Inverter>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that inverts the execution status of its child, interchanging SUCCESS and FAILURE" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Listen>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that executes its child instruction each time specific variables are updated" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Repeat>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that repeats its child a fixed number of times while successful" }
  }};
  return doc;
}

/***************************************/
/* Leaf instructions                   */
/***************************************/

template <>
DocomentationEntry InstructionDocFor<AddElement>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction for adding a new element to a workspace array variable" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<AddMember>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction for adding a new member to a workspace variable" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Condition>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Checks a workspace variable for being true" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<CopyFromProcedureInstruction>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that copies a variable from the workspace of an external procedure into the current workspace" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<CopyToProcedureInstruction>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that copies a variable from the current workspace into a workspace of an external procedure" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Copy>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction copying between workspace variables" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Decrement>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction to decrement a variable" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Equals>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking the equality of two variables" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Fail>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that returns FAILURE" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<GreaterThan>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking the if a variable is greater than the other" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<GreaterThanOrEqual>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking the if a variable is greater than or equal to another" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Increment>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction to increment a variable" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Input>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that writes a user defined value into a workspace variable" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<LessThan>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking if a variable is smaller than an other one" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<LessThanOrEqual>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking the if a variable is smaller than or equal to another one" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<LogInstruction>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Outputs a message to the log" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Message>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that outputs a text message to the user interface" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Output>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction node that outputs a workspace value to the user interface" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<ResetVariable>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that resets a variable to its initial state" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Succeed>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that returns SUCCESS" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<UserConfirmation>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction representing a user defined confirmation (SUCCESS) or rejection (FAILURE)" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<VarExistsInstruction>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking the existence of a variable (or field thereof)" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<Wait>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that returns SUCCESS after a given timeout" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<WaitForVariable>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that waits 'timeout' seconds for a variable to be readable and non-empty" }
  }};
  return doc;
}

template <>
DocomentationEntry InstructionDocFor<WaitForVariables>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction node that waits 'timeout' seconds for all variables of a certain type to be available" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_DOC_OBJECTS_H_
