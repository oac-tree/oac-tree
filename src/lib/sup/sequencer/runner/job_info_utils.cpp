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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/job_info_utils.h>

#include <sup/sequencer/anyvalue_utils.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction_info_utils.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable_utils.h>

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
using namespace Constants;

namespace utils
{
JobInfo CreateJobInfo(const Procedure& proc, const InstructionMap& instr_map)
{
  auto fullname = GetProcedureName(proc);
  auto ws_info = CreateWorkspaceInfo(proc.GetWorkspace());
  auto root = proc.RootInstruction();
  if (root == nullptr)
  {
    const std::string error = "CreateJobInfo(): procedure does not contain a root instruction";
    throw InvalidOperationException(error);
  }
  auto instr_tree_info = CreateInstructionInfoTree(*root, instr_map);
  return JobInfo{ fullname, ws_info, std::move(instr_tree_info) };
}

JobInfo ToJobInfo(const sup::dto::AnyValue& job_info_anyvalue)
{
  if (!ValidateJobInfoAnyValue(job_info_anyvalue))
  {
    const std::string error = "ToJobInfo(): wrong format of job info AnyValue";
    throw InvalidOperationException(error);
  }
  auto full_name = job_info_anyvalue[kFullNameFieldName].As<std::string>();
  auto ws_info = ToWorkspaceInfo(job_info_anyvalue[kWorkspaceInfoFieldName]);
  auto instr_info_tree = ToInstructionInfoTree(job_info_anyvalue[kInstructionTreeInfoFieldName]);
  return JobInfo{ full_name, ws_info, std::move(instr_info_tree) };
}

sup::dto::AnyValue ToAnyValue(const JobInfo& job_info)
{
  auto result = kJobInfoAnyValue;
  result[kFullNameFieldName] = job_info.GetProcedureName();
  result[kWorkspaceInfoFieldName] = ToAnyValue(job_info.GetWorkspaceInfo());
  result[kInstructionTreeInfoFieldName] = ToAnyValueTree(*job_info.GetRootInstructionInfo());
  return result;
}

bool ValidateJobInfoAnyValue(const sup::dto::AnyValue& job_info)
{
  if (!ValidateMemberType(job_info, kFullNameFieldName, sup::dto::StringType))
  {
    return false;
  }
  if (!job_info.HasField(kWorkspaceInfoFieldName))
  {
    return false;
  }
  if (!job_info.HasField(kInstructionTreeInfoFieldName))
  {
    return false;
  }
  return true;
}

}  // namespace utils

}  // namespace sequencer

}  // namespace sup
