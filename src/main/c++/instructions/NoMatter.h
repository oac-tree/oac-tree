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
 * @file Repeat.h
 * @brief Header file for Repeat instruction class.
 * @date 16/12/2020
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Repeat class.
 */

#ifndef _SEQ_NoMatter_h_
#define _SEQ_NoMatter_h_

#include "DecoratorInstruction.h"

namespace sup{

namespace sequencer{

class NoMatter: public DecoratorInstruction {

private:
    /**
     * @brief Executes the child and never fails
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

