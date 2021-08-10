#ifndef TESTPROCEDUREFACTORY_H
#define TESTPROCEDUREFACTORY_H

#include "procedure/Procedure.h"

#include <memory>

class TestProcedureFactory
{
public:
  TestProcedureFactory();

  ~TestProcedureFactory();

  std::unique_ptr<Procedure> CreateProcedure() const;
};

#endif  // TESTPROCEDUREFACTORY_H
