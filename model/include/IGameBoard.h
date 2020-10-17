#ifndef IGAMEBOARD_H
#define IGAMEBOARD_H

#include "IGameControlUnit.h"
#include "BridgeCrossingTypes.h"

namespace kd417d
{
namespace eva
{
namespace logic
{

template<class DataReprType,
         class = typename std::enable_if<
             std::is_base_of<ISerializable, DataReprType>::value>>
class IGameBoard : public IGameControlUnit,
                   public IDataInitializable<DataReprType>
{
public:
    virtual Dimension2D getDimensions() const = 0;
    virtual void setDimensions(Dimension2D&) = 0;
};
}
}
}
#endif // IGAMEBOARD_H
