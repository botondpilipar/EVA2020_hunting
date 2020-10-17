#ifndef STREAMSERIALIZATIONSOURCE_H
#define STREAMSERIALIZATIONSOURCE_H

#include <common_pch.h>
#include "ISerializationSource.h"

namespace kd417d
{
namespace eva
{
namespace access
{

class StreamSerializationSource : public ISerializationSource
{
public:
    virtual ~StreamSerializationSource() override = default;

    StreamSerializationSource(std::istream& stream) : mStream(stream) {}
    virtual std::optional<int> deserializeInt() override;
    virtual std::optional<float> deserializeFloat() override;
    virtual std::optional<bool> deserializeBool() override;
    virtual std::optional<double> deserializeDouble() override;
    virtual std::optional<std::string> deserializeString() override;
    virtual std::optional<char> deserializeChar() override;
    virtual std::optional<unsigned int> deserializeUnsigned() override;

    virtual inline std::istream& getUnderlyingStream() { return mStream; }
protected:
protected:
    template<class T>
    std::optional<T> readAny()
    {
        if(mStream.fail()) { return std::nullopt; }
        T toReturn;
        mStream >> toReturn;
        return std::optional(toReturn);
    }
private:
    std::istream& mStream;
};

}
}
}
#endif // STREAMSERIALIZATIONSOURCE_H
