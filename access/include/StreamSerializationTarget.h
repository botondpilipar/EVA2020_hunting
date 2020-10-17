#ifndef ABSTRACTSTREAMSERIALIZATIONTARGET_H
#define ABSTRACTSTREAMSERIALIZATIONTARGET_H

#endif // ABSTRACTSTREAMSERIALIZATIONTARGET_H

#include <common_pch.h>

#include "ISerializationTarget.h"

namespace kd417d
{
namespace eva
{
namespace access
{
class StreamSerializationTarget : public ISerializationTarget
{
public:
    StreamSerializationTarget(std::ostream& stream) : mStream(stream) {}
    virtual void serializeInt(int) override;
    virtual void serializeFloat(float) override;
    virtual void serializeBool(bool) override;
    virtual void serializeDouble(double) override;
    virtual void serializeChar(char) override;
    virtual void serializeString(std::string) override;
    virtual void serializeUnsigned(unsigned int) override;
    virtual inline std::ostream& getUnderlyingStream() { return mStream; }
protected:
    template<class T>
    void writeAny(const T& p)
    {
        if(!mStream.fail())
        {
            mStream << p;
        }
    }
private:
    std::ostream& mStream;
};

}
}
}
