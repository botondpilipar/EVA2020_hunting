#include "Serializer.h"
#include <QDateTime>
#include <FileException.h>

static const char * const saveDirectoryPath = "data";
static const char* const saveFileExtension = ".bin";

Serializer::Serializer() : mWorkingDirectory(QDir::currentPath())
{
    if( !mWorkingDirectory.exists(saveDirectoryPath) )
    {
        bool successfull = mWorkingDirectory.mkdir(saveDirectoryPath);
        if(!successfull) { throw DirectoryException(saveDirectoryPath); }

    }
    mWorkingDirectory.setCurrent(mWorkingDirectory.relativeFilePath(saveDirectoryPath));
}
void Serializer::serialize(const QDataStreamSerializable& input, const QString& fileName)
{

    QFile outputFile(fileName);
    if( !outputFile.open(QIODevice::WriteOnly) ) { throw FileException(fileName.toStdString()); }

    QDataStream dataStream(&outputFile);
    dataStream << input;

    outputFile.close();
}

bool Serializer::deserialize(QDataStreamSerializable& input, const QString& fileName)
{
    QFile inputFile(fileName);
    if ( !inputFile.open(QIODevice::ReadOnly) ) { return false; }

    QByteArray byteArray;
    QDataStream dataStream(&inputFile);
    dataStream >> input;

    inputFile.close();

    return true;
}

QStringList Serializer::availableFiles(const QRegExp& regex)
{
    assert(regex.isValid());
    QStringList files = mWorkingDirectory.entryList();
    std::remove_if(files.begin(), files.end(),
                   [regex](const QString& fileName) { return !regex.exactMatch(fileName); });
    return files;
}

QString Serializer::generateFileName()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate).remove(":") + saveFileExtension;
}
QString Serializer::getWorkingDirectory() const
{
    return mWorkingDirectory.path();
}

