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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <common/AnyValue.h>
#include <common/AnyType.h>
#include <common/log-api.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Choice.h"
#include "Workspace.h"
#include "Procedure.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

const std::string Choice::Type = "Choice";

Choice::Choice() :
        CompoundInstruction(Type) {
    numberOfElements = 1u;
    elementSize = 0u;
    isMask = false;
}

Choice::~Choice() {
//Auto-generated destructor stub for Choice

//TODO Verify if manual additions are needed here
}

bool Choice::SetupImpl(const Procedure &proc) {
    bool ret = SetupChildren(proc);
    if (ret) {
        ret = HasAttribute("var_name");
        if (ret) {
            varName = GetAttribute("var_name");
            ::ccs::types::AnyValue _val;
            ret = proc.GetVariableValue(varName, _val);
            if (ret) {
                elementSize = _val.GetSize();
                isMask = HasAttribute("is_mask");
                if (isMask) {
                    isMask = (GetAttribute("is_mask") == "true");
                }
                //check if array
                if (!isMask) {
                    ::ccs::base::SharedReference<const ::ccs::types::ArrayType> myArrayType = _val.GetType();
                    if (myArrayType.IsValid()) {
                        numberOfElements = myArrayType->GetElementNumber();
                        elementSize = myArrayType->GetElementType()->GetSize();
                    }
                    //they must be arrays of uint32 or positive int32
                    ret = (elementSize <= 4u);
                    if (!ret) {
                        log_error("Choice::Setup - element size must be <= 4");
                    }
                }
            }
        }
        else {
            log_error("Choice::Setup - No attribute var_name found");
        }

        log_info("Choice::Setup - With var_name=%s numberOfElements=%u elementSize=%u isMask=%u", varName.c_str(), numberOfElements, elementSize, isMask);
    }
    return ret;
}

ExecutionStatus Choice::ExecuteSingleImpl(UserInterface *ui,
                                          Workspace *ws) {
    ExecutionStatus child_status = ExecutionStatus::SUCCESS;
    ::ccs::types::AnyValue _val;
    ws->GetValue(varName, _val);
    ::ccs::types::uint8 *valPtr = reinterpret_cast<::ccs::types::uint8*>(_val.GetInstance());

    if (isMask) {
        ::ccs::types::uint32 nElems = (elementSize * sizeof(::ccs::types::uint64));
        log_info("Choice::ExecuteSingleImpl - isMask nElems=%d", nElems);
        bool exit = false;
        //generic...can consider very big elements
        while ((nElems > 0u) && (!exit)) {
            ::ccs::types::uint32 remained = (nElems > 64u) ? (64u) : (nElems);
            ::ccs::types::uint64 value = 0u;
            memcpy(&value, valPtr, (remained / 8u));
            log_info("Choice::ExecuteSingleImpl - isMask value=%d", value);

            for (::ccs::types::uint32 i = 0u; (i < remained) && (!exit); i++) {
                log_info("Choice::ExecuteSingleImpl - Considering bit %d of %d", i, remained);
                if (((value >> i) & (0x1u))) {
                    child_status = ExecuteChild(i, ui, ws);
                    //continue only if success
                    if (child_status != ExecutionStatus::SUCCESS) {
                        //child_status = _children[i]->GetStatus();
                        exit = true;
                    }
                }
            }
            nElems -= remained;
            valPtr += sizeof(::ccs::types::uint64);
        }
    }
    else {
        for (::ccs::types::uint32 i = 0u; i < numberOfElements; i++) {
            ::ccs::types::uint32 value = 0u;

            memcpy(&value, valPtr, elementSize);

            child_status = ExecuteChild(value, ui, ws);
            //continue only if success
            if (child_status != ExecutionStatus::SUCCESS) {
                //child_status = _children[value]->GetStatus();
                break;
            }
            valPtr += elementSize;
        }
    }
    return child_status;
}

ExecutionStatus Choice::ExecuteChild(::ccs::types::uint32 idx,
                                     UserInterface *ui,
                                     Workspace *ws) {
    ExecutionStatus child_status = ExecutionStatus::SUCCESS;
    if (idx < ChildInstructions().size()) {
        child_status = ChildInstructions()[idx]->GetStatus();

        if (NeedsExecute(child_status)) {
            auto childName = ChildInstructions()[idx]->GetName();
            log_info("Choice::ExecuteChild - Executing child[%u]=%s", idx, childName.c_str());

            ChildInstructions()[idx]->ExecuteSingle(ui, ws);
            child_status = ExecutionStatus::NOT_FINISHED;
        }
    }
    else {
        log_warning("Status Choice::ExecuteSingleImpl - child[%u] not executed because exceeding children size (%u)", idx, ChildInstructions().size());
    }
    return child_status;
}

}

}
