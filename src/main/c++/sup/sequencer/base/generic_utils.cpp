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

#include <chrono>
#include <cstdlib>
#include <dlfcn.h>
#include <fstream>
#include <stdexcept>

#include <sys/types.h>
#include <sys/stat.h>

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

// Adpted from https://stackoverflow.com/a/11366985
bool CreateDir(const std::string& pathname, unsigned mode)
{
  if (pathname.empty())
  {
    return false;
  }
  if (::mkdir(pathname.c_str(), mode) == -1)
  {
    switch (errno)
    {
      case ENOENT:
        // parent folder doesn't exist, so try to create it
        if (CreateDir(pathname.substr(0, pathname.find_last_of('/'))))
        {
          return ::mkdir(pathname.c_str(), mode) == 0;
        }
        break;
      case EEXIST:
        // the folder already exists, so success
        return true;
      default:
        return false;
    }
  }
  else
  {
    return true;
  }
  return false;
}

std::string GetEnvironmentVariable(const std::string& varname)
{
  auto result_c = getenv(varname.c_str());
  return result_c ? std::string(result_c) : std::string{};
}

bool SafeStringToInt(int& result, const std::string& str)
{
  try
  {
    result = std::stoi(str);
  }
  catch(const std::invalid_argument&)
  {
    return false;
  }
  catch(const std::out_of_range&)
  {
    return false;
  }
  return true;
}

bool SafeStringToUnsigned(unsigned long& result, const std::string& str)
{
  try
  {
    result = std::stoul(str);
  }
  catch(const std::invalid_argument&)
  {
    return false;
  }
  catch(const std::out_of_range&)
  {
    return false;
  }
  return true;
}

bool SafeStringToDouble(double& result, const std::string& str)
{
  try
  {
    result = std::stod(str);
  }
  catch(const std::invalid_argument&)
  {
    return false;
  }
  catch(const std::out_of_range&)
  {
    return false;
  }
  return true;
}

unsigned long long GetNanosecsSinceEpoch()
{
  auto now = std::chrono::system_clock::now();
  auto ns = std::chrono::duration_cast<std::chrono::duration<unsigned long long, std::nano>>(
    now.time_since_epoch());
  return ns.count();
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