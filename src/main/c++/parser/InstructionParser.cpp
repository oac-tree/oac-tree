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

#include "InstructionParser.h"
#include "Include.h"
#include "InstructionRegistry.h"
#include "CompoundInstruction.h"
#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

#define TAG_INIT_CHAR '$'

// Global variables

// Function declaration

namespace {

bool AddChildInstructions(Instruction *instruction, const std::vector<TreeData> &children,
                          const TreeData *declarationData, AttributeMap &attributes,
                          const TreeData *appended);

std::unique_ptr<Instruction> GetDeclInstruction(const TreeData &data, const TreeData *declarationData,
                                                AttributeMap &attributes, std::string instr_type,
                                                const TreeData *appended);

std::string
SolveMapAttribute(AttributeMap &attributes, std::string attrTag);

} // Unnamed namespace

// Function definition

std::unique_ptr<Instruction> ParseInstruction(const TreeData &data, const TreeData *declarationData,
                                              AttributeMap &attributes, const TreeData *appended)
{
  auto instr_type = data.GetType();
  auto instr = GlobalInstructionRegistry().Create(instr_type);
  if (!instr)
  {
    if (declarationData != NULL)
    {
      log_info("sup::sequencer::ParseInstruction() - Checking declarations for '%s'", instr_type.c_str());
      return GetDeclInstruction(data, declarationData, attributes, instr_type, appended);
    }
    //if not returned
    log_warning("sup::sequencer::ParseInstruction() - couldn't parse instruction with type: '%s'", instr_type.c_str());
    return {};
  }
  for (const auto &attr : data.Attributes())
  {
    auto attrTag = attr.second;
    auto attrVal = SolveMapAttribute(attributes, attrTag);
    instr->AddAttribute(attr.first, attrVal);
  }
  log_info("sup::sequencer::ParseInstruction() - parsing child instructions for instruction of type: '%s'", instr_type.c_str());
  bool status = AddChildInstructions(instr.get(), data.Children(), declarationData, attributes, appended);
  if (!status)
  {
    log_info("sup::sequencer::ParseInstruction() - parsing child appended instr for instruction of type: '%s'", instr_type.c_str());
    status = AddChildInstructions(instr.get(), appended->Children(), declarationData, attributes, NULL);
  }
  if (!status)
  {
    log_warning("sup::sequencer::ParseInstruction() - instruction with type: '%s' has wrong number of "
                "child instructions: '%d'",
                instr->GetType().c_str(), data.Children().size());
  }
  return instr;
}

namespace {

std::string SolveMapAttribute(AttributeMap &attributes, std::string attrTag)
{
  //be careful with cyclic remappings
  if (attrTag[0] == TAG_INIT_CHAR)
  {
    attrTag = attrTag.c_str() + 1;
    if (attributes.HasAttribute(attrTag))
    {
      auto attrValue = attributes.GetAttribute(attrTag);
      return SolveMapAttribute(attributes, attrValue);
    }
  }
  return attrTag;
}

std::unique_ptr<Instruction> GetDeclInstruction(const TreeData &data, const TreeData *declarationData,
                                                AttributeMap &attributes, std::string instr_type,
                                                const TreeData *appended)
{
  std::unique_ptr<Instruction> decl_instr;
  for (const auto &decl_data : declarationData->Children())
  {
    auto decl_type = decl_data.GetType();
    if (instr_type == decl_type)
    {
      for (const auto &attr : data.Attributes())
      {
        if (!attributes.HasAttribute(attr.first))
        {
          attributes.AddAttribute(attr.first, attr.second);
        }
      }
      log_info("sup::sequencer::GetDeclInstruction() - Parsing '%s' from declarations", instr_type.c_str());
      const std::vector<TreeData> decl_children = decl_data.Children();
      if (decl_children.size() > 0u)
      {
        //eventually go back to data and add other children
        bool setAppended = (appended == NULL);
        if (setAppended)
        {
          appended = &data;
        }
        decl_instr = ParseInstruction(decl_children[0], declarationData, attributes, appended);
        if (setAppended)
        {
          appended = NULL;
        }
      }
      //remove from the stack, other nodes are not interested
      for (const auto &attr : data.Attributes())
      {
        if (attributes.HasAttribute(attr.first))
        {
          attributes.Remove(attr.first);
        }
      }
      break;
    }
  }
  return decl_instr;
}

bool AddChildInstructions(Instruction *instruction, const std::vector<TreeData> &children,
                          const TreeData *declarationData, AttributeMap &attributes,
                          const TreeData *appended)
{
  auto decorator = dynamic_cast<DecoratorInstruction *>(instruction);
  auto instr_name = instruction->GetName();
  auto instr_type = instruction->GetType();

  // Ignore Include instruction:
  if (instr_type == Include::Type)
  {
    return true;
  }
  log_info("AddChildInstructions() - (%s:%s)", instr_name.c_str(), instr_type.c_str());

  if (decorator && children.size() == 1u)
  {
    log_info("AddChildInstructions() - to (%s:%s)", instr_type.c_str(), instr_name.c_str());
    auto child_instr = ParseInstruction(children[0], declarationData, attributes, appended);
    auto child_name = child_instr->GetName();
    auto child_type = child_instr->GetType();

    log_info("AddChildInstructions() - Decorator->SetInstruction(%s:%s)", child_name.c_str(), child_type.c_str());
    decorator->SetInstruction(child_instr.release());
    return true;
  }
  auto compound = dynamic_cast<CompoundInstruction *>(instruction);
  if (compound && children.size() > 0)
  {
    for (const auto &child : children)
    {
      log_info("AddChildInstructions() - to (%s:%s)", instr_type.c_str(), instr_name.c_str());
      auto child_instr = ParseInstruction(child, declarationData, attributes, appended);
      auto child_name = child_instr->GetName();
      auto child_type = child_instr->GetType();
      log_info("AddChildInstructions() - Compound->PushBack(%s:%s)", child_name.c_str(), child_type.c_str());

      compound->PushBack(child_instr.release());
    }
    return true;
  }
  if (!decorator && !compound && children.size() == 0)
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

}// extern C

#undef LOG_ALTERN_SRC
