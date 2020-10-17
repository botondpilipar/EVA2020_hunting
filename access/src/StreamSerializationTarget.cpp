#include <StreamSerializationTarget.h>

using namespace kd417d::eva::access;

void
StreamSerializationTarget::serializeInt(int p)
{
    writeAny<int>(p);
}

void
StreamSerializationTarget::serializeBool(bool p)
{
    writeAny<bool>(p);

}

void
StreamSerializationTarget::serializeFloat(float p)
{
    writeAny<float>(p);
}

void
StreamSerializationTarget::serializeDouble(double p)
{
    writeAny<double>(p);
}

void
StreamSerializationTarget::serializeChar(char p)
{
    writeAny<char>(p);
}

void
StreamSerializationTarget::serializeString(std::string p)
{
    writeAny<std::string>(p);
}
void
StreamSerializationTarget::serializeUnsigned(unsigned int p)
{
    writeAny<unsigned int>(p);
}
