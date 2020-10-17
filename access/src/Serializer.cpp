#include "Serializer.h"
#include <QDateTime>

static const char * const saveDirectoryPath = "./data";
static const char* const saveFileExtension = ".bin";

Serializer::Serializer() : mWorkingDirectory(saveDirectoryPath)
{
    if( !mWorkingDirectory.exists() )
    {
        mWorkingDirectory.mkdir(saveDirectoryPath);
        mWorkingDirectory.setPath(saveDirectoryPath);
    }
}
void Serializer::serialize(const QDataStreamSerializable& input, const QString& fileName)
{

    QFile outputFile(fileName);
    if( !outputFile.open(QIODevice::WriteOnly) ) { throw FileException(fileName.toStdString()); }

    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::ReadWrite);
    dataStream << input;
    outputFile.write(byteArray);

    outputFile.close();
}

bool Serializer::deserialize(QDataStreamSerializable& input, const QString& fileName)
{
    QFile inputFile(fileName);
    if ( !inputFile.open(QIODevice::ReadOnly) ) { return false; }

    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::ReadWrite);
    dataStream << inputFile.readAll();
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
