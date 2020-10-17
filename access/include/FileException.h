#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include <exception>
#include <string>

namespace
{
    const std::string generalMessage = "Unable to handle file operation for ";
}
using namespace std::literals::string_literals;

class FileException : public std::exception
{
public:
    FileException(std::string fileName) :
        mFileName(fileName),
        mMessage(generalMessage)
    {
        mMessage += mFileName;
    }
    const char* what() const noexcept override
    {
        return mMessage.c_str();
    }

private:
    std::string mFileName;
    std::string mMessage;
};

#endif // FILEEXCEPTION_H
