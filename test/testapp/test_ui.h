#ifndef SUP_SEQUENCER_TEST_UI_H_
#define SUP_SEQUENCER_TEST_UI_H_

#include <sup/sequencer/user_interface.h>

class TestUI : public sup::sequencer::UserInterface
{
public:
  TestUI();

  ~TestUI() override;

  void UpdateInstructionStatus(const sup::sequencer::Instruction* instruction) override;
  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value, bool connected) override;
  bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override;
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override;
  int GetUserChoice(const std::vector<std::string>& options,
                        const sup::dto::AnyValue& metadata) override;
  void Message(const std::string& message) override;
  void Log(int severity, const std::string& message) override;
};

#endif  // SUP_SEQUENCER_TEST_UI_H_
