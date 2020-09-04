#include "TestProcedureFactory.h"
#include "runner/Runner.h"

#include <iostream>

int main(int argc, char *argv[])
{
    TestProcedureFactory factory;
    auto procedure = factory.CreateProcedure();
    auto runner = std::unique_ptr<Runner>(new Runner());
    runner->SetProcedure(procedure.get());
    std::cout << "Starting test application..." << std::endl;
}
