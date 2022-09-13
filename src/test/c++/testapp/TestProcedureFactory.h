
#ifndef TESTPROCEDUREFACTORY_H
#define TESTPROCEDUREFACTORY_H

#include <sup/sequencer/Procedure.h>

#include <memory>

class TestProcedureFactory
{
public:
  TestProcedureFactory();

  ~TestProcedureFactory();

  std::unique_ptr<sup::sequencer::Procedure> CreateProcedure() const;
};

#endif  // TESTPROCEDUREFACTORY_H
