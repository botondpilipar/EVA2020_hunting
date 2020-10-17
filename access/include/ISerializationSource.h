#ifndef ISERIALIZATIONSOURCE_H
#define ISERIALIZATIONSOURCE_H

#include <common_pch.h>

namespace kd417d
{
namespace eva
{
namespace access
{

class ISerializationSource
{
public:
    virtual ~ISerializationSource() = default;
    ISerializationSource() = default;
    ISerializationSource(const ISerializationSource&) = delete;
    ISerializationSource(ISerializationSource&&) = delete;
    virtual ISerializationSource& operator=(ISerializationSource&&) = delete;
    virtual ISerializationSource& operator=(ISerializationSource&) = delete;

    virtual std::optional<int> deserializeInt() = 0;
    virtual std::optional<float> deserializeFloat() = 0;
    virtual std::optional<bool> deserializeBool() = 0;
    virtual std::optional<double> deserializeDouble() = 0;
    virtual std::optional<std::string> deserializeString() = 0;
    virtual std::optional<char> deserializeChar() = 0;
    virtual std::optional<unsigned int> deserializeUnsigned() = 0;

};

}
}
}

#endif // ISERIALIZATIONSOURCE_H
