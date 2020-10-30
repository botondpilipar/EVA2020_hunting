#ifndef MODELTYPES_HH
#define MODELTYPES_HH

#include <model_pch.h>
#include <QMetaType>

typedef quint64 DimensionType;
typedef std::pair<DimensionType, DimensionType> Dimension;
typedef qint64 Identifier;
typedef QPair<DimensionType, DimensionType> DimensionQ ;
typedef DimensionQ PlayerPosition;

using RgbTuple = std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>;
using ColorMap = QMap<DimensionQ, QColor>;
using ColorMapPtr = std::shared_ptr<ColorMap>;

inline DimensionQ operator+(const DimensionQ& l, const DimensionQ& r)
{
    return { l.first + r.first, l.second + r.second };
}


Q_DECLARE_METATYPE(DimensionQ);
#endif // MODELTYPES_HH
