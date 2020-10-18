#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include <exception>
#include <string>

namespace
{
    const std::string generalFileMessage = "Unable to handle file operation for ";
    const std::string generalDirectoryMessage = "Unable to handle directory operation for ";

}
using namespace std::literals::string_literals;

class FileException : public std::exception
{
public:
    FileException(std::string fileName) :
        mFileName(fileName),
        mMessage(generalFileMessage)
    {
        mMessage += mFileName;
    }
    const char* what() const noexcept override
    {
        return mMessage.c_str();
    }

protected:
    std::string mFileName;
    std::string mMessage;
};

class DirectoryException : std::exception
{
public:
    DirectoryException(std::string dirName) :
        mDirName(dirName),
        mMessage(generalDirectoryMessage)
    {
        mMessage += mDirName;
    }
    const char* what() const noexcept override
    {
        return mMessage.c_str();
    }

protected:
    std::string mDirName;
    std::string mMessage;
};

#endif // FILEEXCEPTION_H
