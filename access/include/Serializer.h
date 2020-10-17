#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QString>
#include <QVariant>
#include <QDataStream>
#include <QFileDevice>
#include <QBuffer>
#include <QFile>
#include <QRegExp>
#include <QDir>

#include <memory>
#include <QDataStreamSerializable.h>
#include <FileException.h>

namespace
{
const QRegExp saveFileRegex{"*.bat"};
}

class Serializer
{
public:
    Serializer();

    void serialize(const QDataStreamSerializable& input, const QString& fileName);
    bool deserialize(QDataStreamSerializable& input, const QString& fileName);

    QStringList availableFiles(const QRegExp& regex = saveFileRegex);
    static QString generateFileName();

private:
    QDir mWorkingDirectory;
};

#endif // SERIALIZER_H
