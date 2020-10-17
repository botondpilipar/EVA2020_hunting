#ifndef DESERIALIZATIONFAILEDEXCEPTION_H
#define DESERIALIZATIONFAILEDEXCEPTION_H

#include <common_pch.h>

namespace kd417d
{
namespace eva
{
namespace access
{

class DeserializationFailedException : public std::exception
{
public:
    DeserializationFailedException(std::string objectName) :
        mObjectFailed(objectName),
        mMessage(new char[150]){}

    virtual ~DeserializationFailedException() override = default;

    virtual const char* what() const noexcept override
    {
        snprintf(mMessage.get(), sizeof(mMessage.get()),
                 "Failed to initialize %s, fallback to default value",
                 mObjectFailed.c_str());
        return mMessage.get();
    }

private:
    std::string mObjectFailed;
    std::unique_ptr<char> mMessage;
};

}
}
}
#endif // DESERIALIZATIONFAILEDEXCEPTION_H
