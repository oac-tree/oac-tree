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
#include "Workspace.h"

static const std::string VARNAMES_ATTRIBUTE_NAME = "varNames";
static const std::string FORCESUCCESS_ATTRIBUTE_NAME = "forceSuccess";

namespace sup
{
namespace sequencer
{

namespace
{
CallbackGuard<NamedCallbackManager<const ccs::types::AnyValue&>> RegisterCallbacks(
  Workspace* ws, Listen* self, std::vector<std::string> var_names);
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
  std::vector<std::string> var_names = VariableNames();

  // setup callbacks
  auto cb_guard = RegisterCallbacks(ws, this, var_names);

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
  return true;
}

std::vector<std::string> Listen::VariableNames() const
{
  // TODO: get variable names to monitor
  return {};
}

namespace
{
CallbackGuard<NamedCallbackManager<const ccs::types::AnyValue&>> RegisterCallbacks(
  Workspace* ws, Listen* self, std::vector<std::string> var_names)
{
  return ws->GetCallbackGuard(self);
  // TODO: add callback registration
}
}

}  // namespace sequencer

}  // namespace sup
