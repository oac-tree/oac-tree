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
#include <common/log-api.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "NoMatter.h"


namespace sup {

namespace sequencer {

const std::string NoMatter::Type = "NoMatter";

NoMatter::NoMatter() :
        DecoratorInstruction(Type) {
//Auto-generated constructor stub for NoMatter

//TODO Verify if manual additions are needed here
}

NoMatter::~NoMatter() {
//Auto-generated destructor stub for NoMatter

}

ExecutionStatus NoMatter::ExecuteSingleImpl(UserInterface *ui,
                                            Workspace *ws) {
    _child->ExecuteSingle(ui, ws);
    auto status = _child->GetStatus();

    if (status == ExecutionStatus::FAILURE) {
        status = ExecutionStatus::SUCCESS;
    }
    return status;
}

}

}
