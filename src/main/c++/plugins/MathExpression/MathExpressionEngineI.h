/**
 * @file MathExpressionEngineI.h
 * @brief Header file for class MathExpressionEngineI
 * @date Jan 15, 2021 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class MathExpressionEngineI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONENGINEI_H_
#define MATHEXPRESSIONENGINEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <common/BasicTypes.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Instruction.h"
#include <string>
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

/**
 * @brief MathExpressionEngineI interface
 */
class MathExpressionEngineI {
public:

    /**
     * @brief Constructor
     */
    MathExpressionEngineI() = default;

    /**
     * @brief Destructor
     */
    virtual ~MathExpressionEngineI() = default;

    /**
     * @brief Compile the expression
     */
    virtual bool Compile(const ccs::types::char8 *expressionIn,
                         Workspace *workspace)=0;

    /**
     * @brief Execute the computation.
     */
    virtual bool Execute()=0;
};

}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONENGINEI_H_ */

