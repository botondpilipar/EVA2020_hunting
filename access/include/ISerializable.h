#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H


namespace kd417d
{
namespace eva
{
namespace access
{
class ISerializationTarget;

class ISerializable
{
public:
    virtual ~ISerializable() = default;
    ISerializable() = default;

    virtual void serialize(ISerializationTarget&) = 0;
};

}
}
}
#endif // ISERIALIZABLE_H
