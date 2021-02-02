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
 * @file MathExpressionEngineI.h
 * @brief Header file for MathExpressionEngineProvider class.
 * @date 15/01/2021
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the MathExpressionEngineProvider class.
 */


#ifndef _SEQ_MathExpressionEngineProvider_h_
#define _SEQ_MathExpressionEngineProvider_h_

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

namespace sup{

namespace sequencer{

typedef  MathExpressionEngineI* (*MathExpressionEngineIBuilderFcn) ();

class MathExpressionEngineProvider {
private:

    MathExpressionEngineProvider();

    static MathExpressionEngineProvider *mySelf;

    MathExpressionEngineIBuilderFcn regEngineCreator;
public:

    virtual ~MathExpressionEngineProvider();

    static MathExpressionEngineProvider *Instance();

    void Register(MathExpressionEngineIBuilderFcn engineCreator);

    MathExpressionEngineI *CreateNewEngine();
};

}
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* _SEQ_MathExpressionEngineProvider_h_ */

