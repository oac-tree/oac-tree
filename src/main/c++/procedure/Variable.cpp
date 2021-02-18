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

#include "Variable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

Variable::Variable() :
        _setup_successful { false } {
}

Variable::~Variable() = default;

bool Variable::GetValue(::ccs::types::AnyValue &value) const {
    std::lock_guard < std::mutex > lock(_access_mutex);
    if (!_setup_successful) {
        log_warning("Variable::GetValue() - Variable was not successfully set up..");
        return false;
    }
    return GetValueImpl(value);
}

bool Variable::SetValue(const ::ccs::types::AnyValue &value) {
    std::lock_guard < std::mutex > lock(_access_mutex);
    if (!_setup_successful) {
        log_warning("Variable::SetValue() - Variable was not successfully set up..");
        return false;
    }
    return SetValueImpl(value);
}

bool Variable::HasAttribute(const std::string &name) const {
    return _attributes.HasAttribute(name);
}

std::string Variable::GetAttribute(const std::string &name) const {
    return _attributes.GetAttribute(name);
}

bool Variable::AddAttribute(const std::string &name,
                            const std::string &value) {
    std::lock_guard < std::mutex > lock(_access_mutex);
    return _attributes.AddAttribute(name, value);
}

bool Variable::AddAttributes(const std::vector<std::pair<const std::string, std::string>> &attributes) {
    std::lock_guard < std::mutex > lock(_access_mutex);
    bool status = true;
    for (const auto &attr : attributes) {
        // Order in AND matters: add all attributes, even if previous adding failed.
        status = _attributes.AddAttribute(attr.first, attr.second) && status;
    }
    return status;
}

bool Variable::Setup() {
    _setup_successful = SetupImpl();
    if (_setup_successful) {
        log_warning("Variable::Setup() - Variable successfully set up..");
    }
    return _setup_successful;
}

bool Variable::SetupImpl() {
    return true;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
