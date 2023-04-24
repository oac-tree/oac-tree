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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_GENERIC_UTILS_H_
#define SUP_SEQUENCER_GENERIC_UTILS_H_

#include <string>

namespace sup
{
namespace sequencer
{
namespace utils
{
using LibraryHandle = void*;

bool FileExists(const std::string& filename);

LibraryHandle LoadLibrary(const std::string& libname);

bool UnloadLibrary(LibraryHandle handle);

bool CreateDir(const std::string& pathname, unsigned mode = 0755);

std::string GetEnvironmentVariable(const std::string& varname);

unsigned long long GetNanosecsSinceEpoch();

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_GENERIC_UTILS_H_
