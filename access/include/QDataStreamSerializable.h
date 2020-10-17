#ifndef QDATASTREAMSERIALIZABLE_H
#define QDATASTREAMSERIALIZABLE_H

#include <QDataStream>

struct QDataStreamSerializable
{
public:
    virtual ~QDataStreamSerializable() = default;
    QDataStreamSerializable() = default;

    virtual QDataStream& serialize(QDataStream&) const = 0;
    virtual QDataStream& deserialize(QDataStream&) = 0;

    friend QDataStream& operator>>(QDataStream& stream, QDataStreamSerializable& s)
    {
        return s.deserialize(stream);
    }

    friend QDataStream& operator<<(QDataStream& stream, const QDataStreamSerializable& s)
    {
        return s.serialize(stream);
    }

};

#endif // QDATASTREAMSERIALIZABLE_H
