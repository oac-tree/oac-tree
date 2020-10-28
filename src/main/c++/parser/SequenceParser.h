#ifndef SEQUENCEPARSER_H
#define SEQUENCEPARSER_H

#include <memory>

#include "ProcedureData.h"

namespace SequenceParser
{
std::unique_ptr<ProcedureData> ParseProcedureXML(const char * const filename);
}

#endif // SEQUENCEPARSER_H