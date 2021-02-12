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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Condition.h"
#include "Workspace.h"
#include <common/BasicTypes.h>
#include <common/AnyValue.h>
#include <common/AnyType.h>
#include <common/log-api.h>
#include <common/AnyValueHelper.h>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

const std::string Condition::Type = "Condition";

Condition::Condition() :
        Instruction(Type) {
}

Condition::~Condition() {
}

ExecutionStatus Condition::ExecuteSingleImpl(UserInterface *ui,
                                             Workspace *ws) {
    std::string varName = GetAttribute("var_name");
    ::ccs::types::AnyValue var;
    ::ccs::types::uint32 pos1 = varName.find('.');
    ::ccs::types::uint32 pos2 = varName.find('[');

    ::ccs::types::uint32 pos = (pos1 < pos2) ? (pos1) : (pos2);

    ::ccs::types::uint32 varLen = varName.length();
    std::string nodeName = varName;
    std::string path;
    if (pos < varLen) {
        nodeName = varName.substr(0u, pos);
        //should work
        path = varName.substr(pos + 1u, varLen);
    }
    bool ret = ws->GetValue(nodeName, var);

    if (ret) {

        //the type is supposed to be an unsigned integer (or boolean)
        //helper already does it!
        bool success;
        if (path.empty()) {
            ::ccs::base::SharedReference <const ::ccs::types::ScalarType > varType = var.GetType();
            ret = varType.IsValid();
            if (ret) {
                ::ccs::types::uint64 check = 0;
                //var size must be less than 64 bit
                success = (memcmp(var.GetInstance(), &check, var.GetSize()) != 0);
            }
            else{
                log_error("Condition::ExecuteSingleImpl - The variable %s is not scalar", nodeName.c_str(), path.c_str());
            }
        }
        else {
            ret = ::ccs::HelperTools::GetAttributeValue(&var, path.c_str(), success);
        }
        if (ret) {
            ret = success;
        }
        else {
            log_error("Condition::ExecuteSingleImpl - Failed GetAttributeValue(%s-%s)", nodeName.c_str(), path.c_str());
        }
    }
    else {
        log_error("Condition::ExecuteSingleImpl - Failed WorkSpace::GetValue(%s-%s)", nodeName.c_str());
    }

    return ret ? (ExecutionStatus::SUCCESS) : (ExecutionStatus::FAILURE);
}

}
}
