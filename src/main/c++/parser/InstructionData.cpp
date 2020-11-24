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

// Global header files

#include <common/log-api.h>

// Local header files

#include "InstructionData.h"
#include "instructions/InstructionRegistry.h"
#include "instructions/CompoundInstruction.h"
#include "instructions/DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

namespace
{
bool AddChildInstructions(Instruction *instruction,
                          const std::vector<InstructionData> & child_data);
} // Unnamed namespace

// Function definition

InstructionData::InstructionData(std::string instr_type)
  : _type{std::move(instr_type)}
{}

std::string InstructionData::GetType() const
{
  return _type;
}

bool InstructionData::HasAttribute(const std::string & name)
{
  return _attributes.find(name) != _attributes.end();
}

bool InstructionData::AddAttribute(const std::string & name,
                                   const std::string & value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  _attributes[name] = value;
}

bool InstructionData::AddChild(const InstructionData & child)
{
  _children.push_back(child);
}

const std::map<std::string, std::string> & InstructionData::Attributes() const
{
  return _attributes;
}

const std::vector<InstructionData> & InstructionData::Children() const
{
  return _children;
}

std::unique_ptr<Instruction> InstructionData::GenerateInstruction() const
{
  auto instr = GlobalInstructionRegistry().Create(_type);
  if (!instr)
  {
    return {};
  }
  for (const auto & attr : _attributes)
  {
    instr->AddAttribute(attr.first, attr.second);
  }
  bool status = AddChildInstructions(instr.get(), _children);
  if (status)
  {
    return instr;
  }
  return {};
}

namespace
{
bool AddChildInstructions(Instruction *instruction,
                          const std::vector<InstructionData> & child_data)
{
  auto decorator = dynamic_cast<DecoratorInstruction *>(instruction);
  if (decorator && child_data.size() == 1u)
  {
    decorator->SetInstruction(child_data[0].GenerateInstruction().release());
    return true;
  }

  auto compound = dynamic_cast<CompoundInstruction *>(instruction);
  if (compound && child_data.size() > 0)
  {
    for (const auto & child : child_data)
    {
      compound->PushBack(child.GenerateInstruction().release());
    }
    return true;
  }
  if (!decorator && !compound && child_data.size()==0)
  {
    // Leaf instructions don't have children, so ok.
    return true;
  }
  return false;
}

} // Unnamed namespace

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
