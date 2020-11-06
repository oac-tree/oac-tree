#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>

namespace SequenceParser
{

class Attribute
{
private:
  const std::string _name;
  const std::string _value;

public:
  Attribute(std::string name, std::string value);

  ~Attribute();

  const std::string Name() const;
  const std::string Value() const;
};

} // namespace SequenceParser

#endif // ATTRIBUTE_H