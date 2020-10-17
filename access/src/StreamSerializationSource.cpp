#include <StreamSerializationSource.h>

using namespace kd417d::eva::access;

std::optional<int>
StreamSerializationSource::deserializeInt()
{
    return readAny<int>();
}
std::optional<float>
StreamSerializationSource::deserializeFloat()
{
    return readAny<float>();
}
std::optional<bool>
StreamSerializationSource::deserializeBool()
{
    return readAny<bool>();
}
std::optional<double>
StreamSerializationSource::deserializeDouble()
{
    return readAny<double>();
}
std::optional<std::string>
StreamSerializationSource::deserializeString()
{
    return readAny<std::string>();
}
std::optional<char>
StreamSerializationSource::deserializeChar()
{
    return readAny<char>();
}
std::optional<unsigned int>
StreamSerializationSource::deserializeUnsigned()
{
    return readAny<unsigned>();
}
