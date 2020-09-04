#ifndef TESTUI_H
#define TESTUI_H

#include "runner/UserInterface.h"

class TestUI : public UserInterface
{
public:
    TestUI();

    ~TestUI() override;

    void UpdateInstructionStatus(const Instruction * instruction) override;

    void StartSingleStep() override;

    void EndSingleStep() override;
};

#endif // TESTUI_H
