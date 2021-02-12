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
 * @file Condition.h
 * @brief Header file for Condition class.
 * @date 15/01/2021
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Condition class.
 */

#ifndef _SEQ_Condition_h_
#define _SEQ_Condition_h_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Instruction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup{

namespace sequencer{

/**
 * @brief Checks a boolean workspace variable
 */
class Condition: public Instruction {

private:

    /**
     * @brief Checks a boolean workspace variable
     * @details If the variable is not boolean, it will check if the value is different than 0
     * @return SUCCESS if the variable is true, FAILURE if the variable is false
     */
    virtual ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws);


public:

    /**
     * @brief Constructor
     */
    Condition();

    /**
     * @brief Destructor
     */
    virtual ~Condition();

    /**
     * @brief The class name
     */
    static const std::string Type;

};

}
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SRC_MAIN_C___INSTRUCTIONS_CONDITION_H_ */

