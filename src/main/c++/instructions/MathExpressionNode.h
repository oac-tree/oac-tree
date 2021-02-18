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
 * @file MathExpressionNode.h
 * @brief Header file for MathExpressionNode class.
 * @date 15/01/2021
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the MathExpressionNode class.
 */


#ifndef _SEQ_MathExpressionInstruction_h_
#define _SEQ_MathExpressionInstruction_h_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MathExpressionEngineI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Computes a Mathematical/Logical expression on workspace variables
 */
class MathExpressionNode: public Instruction {

private:

    /**
     * @brief Compile the expression (only at the first execution and then) executes it
     */
    virtual ExecutionStatus ExecuteSingleImpl(UserInterface * ui, Workspace * ws);

    /**
     * external engine
     */
    MathExpressionEngineI *engine;

    /**
     * to compile the expression the first time
     */
    bool firstTime;
public:

    /**
     * @brief Constructor
     */
    MathExpressionNode();

    /**
     * @brief Destructor
     */
    virtual ~MathExpressionNode();

    /**
     * @brief Compiles the math expression
     * @details
     * @param
     * @return
     */
    bool Setup(Workspace * ws) override;

    /**
     * @brief The class name
     */
    static const std::string Type;
};
// Global variables

// Function declarations

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* _SEQ_MathExpressionInstruction_h_ */

