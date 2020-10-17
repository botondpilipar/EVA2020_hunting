#ifndef MODELTYPES_HH
#define MODELTYPES_HH

#include <model_pch.h>

typedef quint64 DimensionType;
typedef std::pair<DimensionType, DimensionType> Dimension;
typedef qint64 Identifier;
typedef QPair<DimensionType, DimensionType> DimensionQ ;
typedef DimensionQ PlayerPosition;

using RgbTuple = std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>;
using ColorMap = QMap<DimensionQ, QColor>;
using ColorMapPtr = std::shared_ptr<ColorMap>;


#endif // MODELTYPES_HH
