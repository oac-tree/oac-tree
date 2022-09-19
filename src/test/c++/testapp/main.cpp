#include "test_procedure_factory.h"
#include "test_ui.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/runner.h>

#include <iostream>

using namespace sup::sequencer;

int main(int argc, char *argv[])
{
  std::cout << "Starting test application..." << std::endl;
  sup::sequencer::log::Info("Launching testapp...");

  TestProcedureFactory factory;
  TestUI test_ui;
  auto runner = std::unique_ptr<Runner>(new Runner(&test_ui));
  auto procedure = factory.CreateProcedure();
  runner->SetProcedure(procedure.get());
  runner->ExecuteProcedure();
}
