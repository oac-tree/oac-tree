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

#include "ParallelSequence.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string ParallelSequence::Type = "ParallelSequence";

// Function declaration

// Function definition

ExecutionStatus ParallelSequence::ExecuteSingleImpl(UserInterface *ui,
                                                    Workspace *ws) {
    if (_children.empty()) {
        return ExecutionStatus::SUCCESS;
    }

    for (auto &wrapper : _wrappers) {
        auto wrapper_status = wrapper.GetStatus();

        if (NeedsExecute(wrapper_status)) {
            wrapper.Tick(ui, ws);
        }
    }
    return CalculateCompoundStatus();
}

ExecutionStatus ParallelSequence::CalculateCompoundStatus() const {
    int n_success = 0;
    int n_failure = 0;

    for (const auto &wrapper : _wrappers) {
        auto wrapper_status = wrapper.GetStatus();

        if (wrapper_status == ExecutionStatus::SUCCESS) {
            n_success++;
        }
        if (wrapper_status == ExecutionStatus::FAILURE) {
            n_failure++;
        }
    }

    if (n_success >= _success_th) {
        return ExecutionStatus::SUCCESS;
    }
    if (n_failure >= _failure_th) {
        return ExecutionStatus::FAILURE;
    }
    return ExecutionStatus::RUNNING;
}

bool ParallelSequence::Setup(Workspace * ws) {
    bool status = CompoundInstruction::Setup(ws);
    if (status) {
        int N = static_cast<int>(_children.size());

        // default values for thresholds:
        _success_th = N;
        _failure_th = 1;

        bool success_th_from_attributes = false;
        if (HasAttribute("successThreshold")) {
            auto success_th = GetAttribute("successThreshold");
            try {
                _success_th = std::stoi(success_th);
                success_th_from_attributes = true;
            }
            catch (const std::exception&) {
                log_warning("ParallelSequence::InitThresholds() - could not parse successThreshold attribute!");
                status = false;
            }
        }

        if (HasAttribute("failureThreshold")) {
            auto failure_th = GetAttribute("failureThreshold");
            try {
                int th = std::stoi(failure_th);
                if (success_th_from_attributes) {
                    _failure_th = std::min(th, N - _success_th + 1);
                }
                else {
                    _failure_th = th;
                    _success_th = N - th + 1;
                }
            }
            catch (const std::exception&) {
                log_warning("ParallelSequence::InitThresholds() - could not parse failureThreshold attribute!");
                status = false;
            }
        }
    }
    //init wrappers
    {
      _wrappers.clear();
      for (auto child : _children) {
          _wrappers.emplace_back(child);
      }
    }
    return status;
}


ParallelSequence::ParallelSequence() :
        CompoundInstruction(Type) {
}

ParallelSequence::~ParallelSequence() {
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
