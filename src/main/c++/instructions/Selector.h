/**
 * @file Selector.h
 * @brief Header file for class Selector
 * @date Feb 25, 2021 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class Selector
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SRC_MAIN_C___INSTRUCTIONS_SELECTOR_H_
#define SRC_MAIN_C___INSTRUCTIONS_SELECTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <common/BasicTypes.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompoundInstruction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup{

namespace sequencer{

class Selector: public CompoundInstruction
{
private:

	ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws) override;

	ExecutionStatus ExecuteChild(::ccs::types::uint32 idx, UserInterface *ui, Workspace *ws);

	std::string varName;

	::ccs::types::uint32 numberOfElements;

	::ccs::types::uint32 elementSize;

	bool isMask;

public:

	Selector();

	virtual ~Selector();

  bool Setup(Workspace * ws) override;

  static const std::string Type;

};

}

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SRC_MAIN_C___INSTRUCTIONS_SELECTOR_H_ */

