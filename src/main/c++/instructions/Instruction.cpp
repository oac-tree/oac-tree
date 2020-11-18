#include "Instruction.h"

#include "runner/UserInterface.h"

static const std::string NAME_ATTRIBUTE = "name";

void Instruction::Preamble(UserInterface * ui)
{
  if (_status == ExecutionStatus::UNDEFINED)
  {
    _status = ExecutionStatus::STARTED;
    ui->UpdateInstructionStatus(this);
  }
}

void Instruction::Postamble(UserInterface * ui)
{
  if (_status != _status_before)
  {
    ui->UpdateInstructionStatus(this);
  }
}

Instruction::Instruction(std::string type)
  : _type{std::move(type)}
  , _status{ExecutionStatus::UNDEFINED}
  , _status_before{ExecutionStatus::UNDEFINED}
{}

Instruction::~Instruction() = default;

std::string Instruction::GetType() const
{
  return _type;
}

std::string Instruction::GetName() const
{
  return GetAttribute(NAME_ATTRIBUTE);
}

void Instruction::SetName(std::string name)
{
  SetAttribute(NAME_ATTRIBUTE, name);
}

void Instruction::ExecuteSingle(UserInterface * ui, Workspace * ws)
{
  Preamble(ui);

  _status_before = _status;

  _status = ExecuteSingleImpl(ui, ws);

  Postamble(ui);
}

ExecutionStatus Instruction::GetStatus() const
{
  return _status;
}

void Instruction::ResetStatus()
{
  _status = ExecutionStatus::UNDEFINED;
}

std::string Instruction::GetAttribute(const std::string & name) const
{
  if (_attributes.find(name) == _attributes.end())
  {
      return {};
  }
  return _attributes.at(name);
}

std::string Instruction::SetAttribute(const std::string & name, const std::string & value)
{
  _attributes[name] = value;
}