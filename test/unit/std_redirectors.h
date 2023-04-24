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

#ifndef SUP_SEQUENCER_STD_REDIRECTORS_H_
#define SUP_SEQUENCER_STD_REDIRECTORS_H_

#include <iostream>

namespace sup
{
namespace sequencer
{
/**
 * @brief Class redirecting std::cin.
 * @details The destructor resets the default buffer of std::cin.
 */
class CinRedirector
{
private:
  std::streambuf* cin_buf;

public:
  CinRedirector(std::istream& input_stream);
  ~CinRedirector();
};

/**
 * @brief Class redirecting std::cout.
 * @details The destructor resets the default buffer of std::cout.
 */
class CoutRedirector
{
private:
  std::streambuf* cout_buf;

public:
  CoutRedirector(std::ostream& output_stream);
  ~CoutRedirector();
};

inline CinRedirector::CinRedirector(std::istream& input_stream) : cin_buf{std::cin.rdbuf()}
{
  std::cin.rdbuf(input_stream.rdbuf());
}

inline CinRedirector::~CinRedirector()
{
  std::cin.rdbuf(cin_buf);
}

inline CoutRedirector::CoutRedirector(std::ostream& output_stream) : cout_buf{std::cout.rdbuf()}
{
  std::cout.rdbuf(output_stream.rdbuf());
}

inline CoutRedirector::~CoutRedirector()
{
  std::cout.rdbuf(cout_buf);
}

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_STD_REDIRECTORS_H_
