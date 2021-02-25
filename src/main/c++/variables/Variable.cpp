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
#include <common/AnyValueHelper.h>

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

Variable::Variable(std::string type) : _type(type), _setup_successful { false }
{
}

Variable::~Variable() = default;

std::string Variable::GetType() const
{
	return _type;
}

std::string Variable::GetName() const
{
	return GetAttribute(attributes::NAME_ATTRIBUTE);
}

void Variable::SetName(std::string name)
{
	AddAttribute(attributes::NAME_ATTRIBUTE, name);
}

bool Variable::GetValue(::ccs::types::AnyValue &value, const std::string &fieldname) const
{
	std::lock_guard < std::mutex > lock(_access_mutex);
	if (!_setup_successful)
	{
		log_warning("Variable::GetValue() - Variable was not successfully set up..");
		return false;
	}
	if (fieldname.empty())
	{
		return GetValueImpl(value);
	}

	::ccs::types::AnyValue var_copy;
	bool status = GetValueImpl(var_copy);
	if (status)
	{
		status = ::ccs::HelperTools::GetAttributeValue(&var_copy, fieldname.c_str(), value);
	}
	if (!status)
	{
		log_error("Variable::GetValue() - Failed with field name '%s'", fieldname.c_str());
	}
	return status;
}

bool Variable::SetValue(const ::ccs::types::AnyValue &value, const std::string &fieldname)
{
	std::lock_guard < std::mutex > lock(_access_mutex);
	if (!_setup_successful)
	{
		log_warning("Variable::SetValue() - Variable was not successfully set up..");
		return false;
	}

	if (fieldname.empty())
	{
		return SetValueImpl(value);
	}

	::ccs::types::AnyValue var_copy;
	bool status = GetValueImpl(var_copy);
	if (status)
	{
		status = ::ccs::HelperTools::SetAttributeValue(&var_copy, fieldname.c_str(), value);
		if (status)
		{
			//need to update it in the Variable
			status = SetValueImpl(var_copy);
		}
	}
	if (!status)
	{
		log_error("Variable::SetValue() - Failed with field name '%s'", fieldname.c_str());
	}
	return status;
}

bool Variable::HasAttribute(const std::string &name) const
{
  return _attributes.HasAttribute(name);
}

std::string Variable::GetAttribute(const std::string &name) const
{
  return _attributes.GetAttribute(name);
}

bool Variable::AddAttribute(const std::string &name, const std::string &value)
{
	std::lock_guard < std::mutex > lock(_access_mutex);
	bool status = _attributes.AddAttribute(name, value);
	_setup_successful = SetupImpl();
	return status;
}

bool Variable::AddAttributes(const std::vector<std::pair<const std::string, std::string>> &attributes)
{
	std::lock_guard < std::mutex > lock(_access_mutex);
	bool status = true;
	for (const auto &attr : attributes)
	{
		// Order in AND matters: add all attributes, even if previous adding failed.
		status = _attributes.AddAttribute(attr.first, attr.second) && status;
	}
	_setup_successful = SetupImpl();
	return status;
}

bool Variable::SetupImpl()
{
	return true;
}

} // namespace sequencer

} // namespace sup

extern "C"
{

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
