#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <access_pch.h>

#include <memory>
#include <QDataStreamSerializable.h>
#include <FileException.h>

namespace
{
const QRegExp saveFileRegex{".*\.bin"};
}

class Serializer
{
public:
    Serializer();

    void serialize(const QDataStreamSerializable& input, const QString& fileName);
    bool deserialize(QDataStreamSerializable& input, const QString& fileName);

    QStringList availableFiles(const QRegExp& regex = saveFileRegex);
    QString getWorkingDirectory() const;
    static QString generateFileName();
    static inline QDateTime lastFileModification(QString& fileName) { return QFileInfo(fileName).lastModified(); }

private:
    QDir mWorkingDirectory;
};

#endif // SERIALIZER_H
