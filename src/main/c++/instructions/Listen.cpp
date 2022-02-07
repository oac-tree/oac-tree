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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "Listen.h"

#include <cstring>

static const std::string VARNAMES_ATTRIBUTE_NAME = "varNames";
static const std::string FORCESUCCESS_ATTRIBUTE_NAME = "forceSuccess";

namespace sup
{
namespace sequencer
{

namespace
{
bool Equals(const ccs::types::AnyValue& lhs, const ccs::types::AnyValue& rhs);
}

const std::string Listen::Type = "Listen";

Listen::Listen()
  : DecoratorInstruction(Type)
  , force_success{false}
{}

Listen::~Listen() = default;

ExecutionStatus Listen::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  if (!HasChild())
  {
    return ExecutionStatus::SUCCESS;
  }
  std::vector<std::string> var_names;
  std::transform(var_cache.begin(), var_cache.end(), std::back_inserter(var_names),
                 [](decltype(var_cache)::const_reference item) { return item.first; });

  // setup callbacks
  auto cb_guard = RegisterCallbacks(ws, var_names);

  // TODO: listen loop:
  // exit if child returns failure and forceSuccess was not set
  // exit on halt too

  return ExecutionStatus::FAILURE;
}

bool Listen::SetupImpl(const Procedure& proc)
{
  force_success = false;
  if (HasAttribute(FORCESUCCESS_ATTRIBUTE_NAME))
  {
    auto force_success_attr = GetAttribute(FORCESUCCESS_ATTRIBUTE_NAME);
    force_success = attributes::AttributeAsBool(force_success_attr);
  }
  if (!HasAttribute(VARNAMES_ATTRIBUTE_NAME))
  {
    return false;
  }
  var_cache.clear();
  for (const auto& var_name : VariableNames())
  {
    var_cache[var_name] = ccs::types::AnyValue{};
  }
  return true;
}

std::vector<std::string> Listen::VariableNames() const
{
  auto var_names_attr = GetAttribute(VARNAMES_ATTRIBUTE_NAME);
  std::vector<std::string> result;
  size_t pos = var_names_attr.find_first_not_of(',');
  while (pos != std::string::npos)
  {
    auto next = var_names_attr.find_first_of(',', pos);
    result.push_back(var_names_attr.substr(pos, next - pos));
    pos = var_names_attr.find_first_not_of(',', next);
  }
  return result;
}

void Listen::UpdateCallback(const std::string& name, const ccs::types::AnyValue& val)
{
  std::lock_guard<std::mutex> lk(mx);
  // TODO: if val != cached value: update cache, set updated and notify
}

CallbackGuard<NamedCallbackManager<const ccs::types::AnyValue&>> Listen::RegisterCallbacks(
    Workspace* ws, std::vector<std::string> var_names)
{
  auto cb_guard = ws->GetCallbackGuard(this);
  for (const auto& var_name : var_names)
  {
    ws->RegisterCallback(
        var_name,
        [this, &var_name](const ccs::types::AnyValue& val) { UpdateCallback(var_name, val); },
        this);
  }
  return cb_guard;
}

namespace
{
bool Equals(const ccs::types::AnyValue& lhs, const ccs::types::AnyValue& rhs)
{
  // Very rudimentary, but false negatives are not an issue here.
  if (lhs.GetSize() != rhs.GetSize())
  {
    return false;
  }
  return std::memcmp(lhs.GetInstance(), rhs.GetInstance(), lhs.GetSize()) == 0;
}
}

}  // namespace sequencer

}  // namespace sup
