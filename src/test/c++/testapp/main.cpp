#include "TestProcedureFactory.h"
#include "TestUI.h"

#include <sup/sequencer/runner.h>

#include <iostream>

using namespace sup::sequencer;

int main(int argc, char *argv[])
{
  std::cout << "Starting test application..." << std::endl;

  TestProcedureFactory factory;
  TestUI test_ui;
  auto runner = std::unique_ptr<Runner>(new Runner(&test_ui));
  auto procedure = factory.CreateProcedure();
  runner->SetProcedure(procedure.get());
  runner->ExecuteProcedure();
}
