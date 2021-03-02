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

#include <new> // std::nothrow, etc.

#include <common/BasicTypes.h> // Misc. type definition
#include <common/StringTools.h> // Misc. helper functions

#include <common/AnyValueHelper.h> // Misc. helper functions

// Local header files

#include "Variable.h"
#include "VariableRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

/**
 * @brief FileVariable class.
 * @detail Variable with file-based backend.
 */

class FileVariable : public Variable
{

  private:

  protected:

  public:

    /**
     * @brief Constructor.
     */

    FileVariable (void);

    /**
     * @brief Destructor.
     */

    ~FileVariable (void) override;

    /**
     * @brief See sup::sequencer::Variable.
     */

    virtual bool SetupImpl (void);
    virtual bool GetValueImpl (ccs::types::AnyValue& value) const;
    virtual bool SetValueImpl (const ccs::types::AnyValue& value);

    /**
     * @brief Class name for VariableRegistry.
     */

    static const std::string Type;

};

// Function declaration

// Global variables

const std::string FileVariable::Type = "FileVariable";
static bool _filevariable_initialised_flag = RegisterGlobalVariable<FileVariable>();

// Function definition

bool FileVariable::SetupImpl (void)
{

  bool status = Variable::HasAttribute("file");

  return status;

}

bool FileVariable::GetValueImpl (ccs::types::AnyValue& value) const
{

  bool status = ccs::HelperTools::ReadFromFile(&value, Variable::GetAttribute("file").c_str());

  return status;

}

bool FileVariable::SetValueImpl (const ccs::types::AnyValue& value)
{

  bool status = static_cast<bool>(value.GetType()); // Valid variable

  if (status)
    {
      status = ccs::HelperTools::DumpToFile(&value, Variable::GetAttribute("file").c_str());
    }

  return status;

}

FileVariable::FileVariable (void) : Variable(FileVariable::Type) {}
FileVariable::~FileVariable (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
