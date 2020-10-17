#ifndef ISERIALIZATIONTARGET_H
#define ISERIALIZATIONTARGET_H

namespace kd417d
{
namespace eva
{
namespace access
{

class ISerializationTarget
{
public:
    virtual ~ISerializationTarget() = default;
    ISerializationTarget() = default;
    ISerializationTarget(const ISerializationTarget&) = delete;
    ISerializationTarget(ISerializationTarget&&) = delete;
    virtual ISerializationTarget& operator=(ISerializationTarget&&) = delete;
    virtual ISerializationTarget& operator=(ISerializationTarget&) = delete;

    virtual void serializeInt(int) = 0;
    virtual void serializeFloat(float) = 0;
    virtual void serializeBool(bool) = 0;
    virtual void serializeDouble(double) = 0;
    virtual void serializeChar(char) = 0;
    virtual void serializeString(std::string) = 0;
    virtual void serializeUnsigned(unsigned int) = 0;
};

}
}
}
#endif // ISERIALIZATIONTARGET_H
