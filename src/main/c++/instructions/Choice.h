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

/**
 * @file Choice.h
 * @brief Header file for ForceSuccess decorator class.
 * @date 16/12/2020
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the ForceSuccess decorator class.
 */

#ifndef _SEQ_Choice_h_
#define _SEQ_Choice_h_

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

namespace sup {

namespace sequencer {

class Choice: public CompoundInstruction {
private:

    /**
     * @brief Executes a subset of children depending on a variable value
     *
     * @return SUCCESS if all the selected children return SUCCESS.
     */
    ExecutionStatus ExecuteSingleImpl(UserInterface * ui,
                                      Workspace * ws) override;

    /**
     * @brief child execution
     */
    ExecutionStatus ExecuteChild(::ccs::types::uint32 idx,
                                 UserInterface *ui,
                                 Workspace *ws);

    bool SetupImpl(const Procedure &proc) override;

    std::string varName;

    ::ccs::types::uint32 numberOfElements;

    ::ccs::types::uint32 elementSize;

    bool isMask;

public:

    Choice();

    virtual ~Choice();

    static const std::string Type;

};

}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SRC_MAIN_C___INSTRUCTIONS_SELECTOR_H_ */

