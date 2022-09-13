#ifndef TESTUI_H
#define TESTUI_H

#include <sup/sequencer/UserInterface.h>

class TestUI : public sup::sequencer::UserInterface
{
public:
  TestUI();

  ~TestUI() override;

private:
  void UpdateInstructionStatusImpl(const sup::sequencer::Instruction* instruction) override;

  void StartSingleStepImpl() override;

  void EndSingleStepImpl() override;
};

#endif  // TESTUI_H
