#include "SequenceParser.h"
#include "SequenceParserImpl.h"

namespace SequenceParser
{
std::unique_ptr<Procedure> ParseProcedureFile(const char * const filename)
{
  auto proc_data = ParseProcedureXML(filename);

  if (!proc_data)
  {
    // Parsing failed
    return {};
  }

  auto proc = proc_data->CreateProcedure();

  return proc;
}

} // namespace SequenceParser
