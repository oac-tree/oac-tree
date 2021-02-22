/**
 * @file NoMatter.h
 * @brief Header file for class NoMatter
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

 * @details This header file contains the declaration of the class NoMatter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SRC_MAIN_C___INSTRUCTIONS_NOMATTER_H_
#define SRC_MAIN_C___INSTRUCTIONS_NOMATTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DecoratorInstruction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup{

namespace sequencer{

class NoMatter: public DecoratorInstruction {

private:
    /**
     * @brief Execute the child a fixed number of times while successful.
     *
     * @return SUCCESS if the child returns FAILURE
     */
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

public:
    NoMatter();

    ~NoMatter() override;

    static const std::string Type;
};

}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SRC_MAIN_C___INSTRUCTIONS_NOMATTER_H_ */

