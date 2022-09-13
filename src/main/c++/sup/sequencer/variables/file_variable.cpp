/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : File-based variable implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#include "file_variable.h"

#include <common/AnyValueHelper.h>

#include <fstream>
#include <sstream>

static std::string StripWhitespaceFromFile(const std::string& filename);

namespace sup
{
namespace sequencer
{
const std::string FileVariable::Type = "FileVariable";

bool FileVariable::SetupImpl(void)
{
  bool status = Variable::HasAttribute("file");
  return status;
}

bool FileVariable::GetValueImpl(::ccs::types::AnyValue& value) const
{
  auto str = StripWhitespaceFromFile(GetAttribute("file"));
  if (str.empty())
  {
    return false;
  }
  bool status = ccs::HelperTools::ParseFromJSONStream(&value, str.c_str());
  return status;
}

bool FileVariable::SetValueImpl(const ::ccs::types::AnyValue& value)
{
  bool status = static_cast<bool>(value.GetType());  // Valid variable
  if (status)
  {
    status = ::ccs::HelperTools::DumpToFile(&value, Variable::GetAttribute("file").c_str());
  }
  return status;
}

FileVariable::FileVariable() : Variable(FileVariable::Type) {}
FileVariable::~FileVariable() = default;

}  // namespace sequencer

}  // namespace sup

static std::string StripWhitespaceFromFile(const std::string& filename)
{
  if (!ccs::HelperTools::Exist(filename.c_str()))
  {
    return {};
  }
  std::ostringstream result;
  std::ifstream fin(filename);
  std::string s;
  while (fin >> s)
  {
    result << s;
  }
  return result.str();
}
