#ifndef SEQUENCEPARSERIMPL_H
#define SEQUENCEPARSERIMPL_H

#include <memory>

#include "ProcedureData.h"

std::unique_ptr<ProcedureData> ParseProcedureXML(const char * const filename);

#endif // SEQUENCEPARSERIMPL_H