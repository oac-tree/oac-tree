/**
 * @file NoMatter.cpp
 * @brief Source file for class NoMatter
 * @date Feb 22, 2021 TODO Verify the value and format of the date
 * @author ferrog TODO Verify the name and format of the author
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class NoMatter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <common/log-api.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "NoMatter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

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
