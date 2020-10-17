#ifndef IGAMEBOARD_H
#define IGAMEBOARD_H

#include <IGameControlUnit.h>
#include <ModelTypes.hh>
#include <QDataStreamSerializable.h>

using namespace kd417d::eva::access;

namespace kd417d
{
namespace eva
{
namespace logic
{

template<class DataReprType,
         class = typename std::enable_if<
             std::is_base_of<QDataStreamSerializable, DataReprType>::value>>
class IGameBoard : public IGameControlUnit,
                   public IDataInitializable<DataReprType>
{
public:
    virtual DimensionQ getDimensions() const = 0;
    virtual void setDimensions(DimensionQ&) = 0;
};
}
}
}
#endif // IGAMEBOARD_H
