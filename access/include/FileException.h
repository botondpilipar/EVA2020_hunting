#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include <exception>
#include <string>

using namespace std::literals::string_literals;

class FileException : public std::exception
{
public:
    FileException(std::string fileName) : mFileName(fileName) {}
    const char* what() const noexcept override
    {
        std::string generalMessage = "Unable to handle file operation for ";
        generalMessage += mFileName;
        return generalMessage.c_str();
    }

private:
    std::string mFileName;
};

#endif // FILEEXCEPTION_H
