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

#include <sup/sequencer/generic_utils.h>

#include <dlfcn.h>
#include <fstream>
#include <stdio.h>

namespace
{
class ReadFileHandle
{
public:
  ReadFileHandle(const std::string& filename);
  ~ReadFileHandle();

  bool IsValid() const;
private:
  std::ifstream m_file_stream;
};
}

namespace sup
{
namespace sequencer
{
namespace utils
{
bool FileExists(const std::string& filename)
{
  ReadFileHandle handle(filename);
  return handle.IsValid();
}

bool LoadLibrary(const std::string& libname)
{
  if (libname.empty())
  {
    return false;
  }
  auto lib_handle = dlopen(libname.c_str(), RTLD_NOW);
  return lib_handle != nullptr;
}


}  // namespace utils

}  // namespace sequencer

}  // namespace sup

namespace
{
ReadFileHandle::ReadFileHandle(const std::string& filename)
  : m_file_stream{filename}
{}

ReadFileHandle::~ReadFileHandle() = default;

bool ReadFileHandle::IsValid() const
{
  return m_file_stream.is_open();
}
}  // unnamed namespace