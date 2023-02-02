
#ifndef SUP_SEQUENCER_TEST_PROCEDURE_FACTORY_H_
#define SUP_SEQUENCER_TEST_PROCEDURE_FACTORY_H_

#include <sup/sequencer/procedure.h>

#include <memory>

class TestProcedureFactory
{
public:
  TestProcedureFactory();

  ~TestProcedureFactory();

  std::unique_ptr<sup::sequencer::Procedure> CreateProcedure() const;
};

#endif  // SUP_SEQUENCER_TEST_PROCEDURE_FACTORY_H_
