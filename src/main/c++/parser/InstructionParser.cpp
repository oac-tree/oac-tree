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
#include "InstructionRegistry.h"
#include "CompoundInstruction.h"
#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

namespace {
bool AddChildInstructions(Instruction *instruction,
                          const std::vector<TreeData> &children,
                          const TreeData *declarationData,
                          AttributeMap &attributes);
} // Unnamed namespace

// Function definition

std::unique_ptr<Instruction> ParseInstruction(const TreeData &data,
                                              const TreeData *declarationData,
                                              AttributeMap &attributes) {
    auto instr_type = data.GetType();
    auto instr = GlobalInstructionRegistry().Create(instr_type);
    if (!instr) {
        if (declarationData != NULL) {
            log_info("sup::sequencer::ParseInstruction() - Checking declarations for '%s'", instr_type.c_str());

            for (const auto &decl_data : declarationData->Children()) {
                auto decl_type = decl_data.GetType();
                if (instr_type == decl_type) {
                    for (const auto &attr : data.Attributes()) {
                        if (!attributes.HasAttribute(attr.first)) {
                            attributes.AddAttribute(attr.first, attr.second);
                        }
                    }
                    log_info("sup::sequencer::ParseInstruction() - Parsing '%s' from declarations", instr_type.c_str());
                    const std::vector<TreeData> decl_children = decl_data.Children();
                    std::unique_ptr<Instruction> decl_instr;
                    if (decl_children.size() > 0u) {
                        decl_instr = ParseInstruction(decl_children[0], declarationData, attributes);
                        //eventually go back to data and add other children
                        bool status = AddChildInstructions(decl_instr.get(), data.Children(), declarationData, attributes);
                        if (!status) {
                            log_error("ParseInstruction - Failed AddChildInstructions for %s", decl_type.c_str());
                        }
                    }
                    //remove from the stack, other nodes are not interested
                    for (const auto &attr : data.Attributes()) {
                        if (attributes.HasAttribute(attr.first)) {
                            attributes.Remove(attr.first);
                        }
                    }
                    return decl_instr;
                }
            }
        }
        //if not returned
        log_warning("sup::sequencer::ParseInstruction() - couldn't parse instruction with type: '%s'", instr_type.c_str());
        return {};
    }
    for (const auto &attr : data.Attributes()) {
        instr->AddAttribute(attr.first, attr.second);
    }
    for (const auto &attr : attributes.AttributeList()) {
        if (!instr->HasAttribute(attr.first)) {
            instr->AddAttribute(attr.first, attr.second);
        }
    }
    log_info("sup::sequencer::ParseInstruction() - parsing child instructions for instruction of type: '%s'", instr_type.c_str());
    bool status = AddChildInstructions(instr.get(), data.Children(), declarationData, attributes);
    if (status) {
        return instr;
    }
    log_warning("sup::sequencer::ParseInstruction() - instruction with type: '%s' has wrong number of "
                "child instructions: '%d'",
                instr->GetType().c_str(), data.Children().size());
    return {};
}

namespace {
bool AddChildInstructions(Instruction *instruction,
                          const std::vector<TreeData> &children,
                          const TreeData *declarationData,
                          AttributeMap &attributes) {
    auto decorator = dynamic_cast<DecoratorInstruction*>(instruction);
    if (decorator && children.size() == 1u) {
        auto child_instr = ParseInstruction(children[0], declarationData, attributes);
        decorator->SetInstruction(child_instr.release());
        return true;
    }
    auto compound = dynamic_cast<CompoundInstruction*>(instruction);
    if (compound && children.size() > 0) {
        for (const auto &child : children) {
            auto child_instr = ParseInstruction(child, declarationData, attributes);
            compound->PushBack(child_instr.release());
        }
        return true;
    }
    if (!decorator && !compound && children.size() == 0) {
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
