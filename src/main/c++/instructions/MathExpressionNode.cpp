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

// Global header files
#include <common/log-api.h>

// Local header files

#include "MathExpressionNode.h"
#include "MathExpressionEngineProvider.h"
// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string MathExpressionNode::Type = "MathExpressionNode";

// Function declaration

MathExpressionNode::MathExpressionNode() :
        Instruction(Type) {
    //todo when I read an expression
    firstTime = true;
    engine = NULL;
}

MathExpressionNode::~MathExpressionNode() {
//Auto-generated destructor stub for MathExpressionNode

//TODO Verify if manual additions are needed here
    if (engine != NULL) {
        delete engine;
    }
}

bool MathExpressionNode::Setup(Workspace *ws) {

    engine = MathExpressionEngineProvider::Instance()->CreateNewEngine();
    bool ret = (engine != NULL);
    if (ret) {
        std::string expression = GetAttribute("expression");

        ret = engine->Compile(expression.c_str(), ws);
        if (!ret) {
            log_info("MathExpressionNode::ExecuteSingleImpl Failed Compilation of %s", expression.c_str());
        }
    }
    else {
        log_error("MathExpressionEngine is NULL");
    }

    return ret;

}

ExecutionStatus MathExpressionNode::ExecuteSingleImpl(UserInterface *ui,
                                                      Workspace *ws) {

    ExecutionStatus status = ExecutionStatus::SUCCESS;

    bool ret = engine->Execute();
    if (!ret) {
        log_info("MathExpressionNode::ExecuteSingleImpl Failed Execution");
    }
    if (!ret) {
        status = ExecutionStatus::FAILURE;
    }
    return status;

}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
