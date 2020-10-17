#ifndef IDESERIALIZABLE_H
#define IDESERIALIZABLE_H


namespace kd417d
{
namespace eva
{
namespace access
{
class ISerializationSource;

class IDeserializable
{
public:
    virtual ~IDeserializable() = default;
    IDeserializable() = default;

    virtual void deserialize(ISerializationSource&) = 0;
};

}
}
}
#endif // IDESERIALIZABLE_H
