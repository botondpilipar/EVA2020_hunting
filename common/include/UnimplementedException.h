#ifndef UNIMPLEMENTEDEXCEPTION_H
#define UNIMPLEMENTEDEXCEPTION_H

#include <exception>

namespace kd417d
{
namespace eva
{

class UnimplementedException : public std::exception
{
public:
    UnimplementedException()
        : mClassName(__FILE__),
          mMethodName(__func__)
    {}
    UnimplementedException(std::string className, std::string methodName)
        : mClassName(className),
          mMethodName(methodName),
          mMessage(new char[150]){}

    virtual ~UnimplementedException() override = default;

    virtual const char* what() const noexcept override
    {
        snprintf(mMessage.get(), sizeof(mMessage.get()),
                 "Unimplemented function or method call to %s %s",
                 mClassName.c_str(),
                 mMethodName.c_str());
        return mMessage.get();
    }

private:
    std::string mClassName;
    std::string mMethodName;
    std::unique_ptr<char> mMessage;
};
}
}
#endif // UNIMPLEMENTEDEXCEPTION_H
