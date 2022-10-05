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

#ifndef SUP_SEQUENCER_GENERIC_UTILS_H_
#define SUP_SEQUENCER_GENERIC_UTILS_H_

#include <string>

namespace sup
{
namespace sequencer
{
namespace utils
{
bool FileExists(const std::string& filename);

bool LoadLibrary(const std::string& libname);

bool CreateDir(const std::string& pathname, unsigned mode = 0755);

std::string GetEnvironmentVariable(const std::string& varname);

/**
 * @brief Try to convert string to an int or return false if failed.
 */
bool SafeStringToInt(int& result, const std::string& str);

/**
 * @brief Try to convert string to a double or return false if failed.
 */
bool SafeStringToDouble(double& result, const std::string& str);

unsigned long long GetNanosecsSinceEpoch();

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_GENERIC_UTILS_H_
