#ifndef SEQUENCEPARSER_H
#define SEQUENCEPARSER_H

#include <memory>

#include "procedure/Procedure.h"

namespace SequenceParser
{
std::unique_ptr<Procedure> ParseProcedureFile(const char * const filename);
}

#endif // SEQUENCEPARSER_H