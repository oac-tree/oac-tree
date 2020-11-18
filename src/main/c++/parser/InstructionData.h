#ifndef INSTRUCTIONDATA_H
#define INSTRUCTIONDATA_H

#include <string>
#include <vector>

#include "Attribute.h"

class InstructionData
{
private:
  const std::string _type;
  std::vector<Attribute> _attributes;
  std::vector<InstructionData> _children;

public:
  InstructionData();

  ~InstructionData() = default;

  bool AddAttribute(const std::string & att_name, const std::string & value);

  bool AddChild(const InstructionData & child);

  const std::vector<Attribute> Attributes() const;

  const std::vector<InstructionData> Children() const;
};

#endif // INSTRUCTIONDATA_H