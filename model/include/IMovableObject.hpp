#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <model_pch.h>
#include <IDataInitializable.hpp>
#include <ISerializable.h>

#include "ISettingsChangedObserver.h"
#include "BridgeCrossingTypes.h"

namespace kd417d
{
namespace eva
{
namespace logic
{

using namespace kd417d::eva::access;

template<class DataReprType,
         class = typename std::enable_if<
             std::is_base_of<ISerializable, DataReprType>::value>>
class IMovableObject : public IDataInitializable<DataReprType>,
                       public ISettingsChangedObserver
{
public:
    virtual ~IMovableObject() = default;
    IMovableObject() = default;

    virtual void move(QPair<int, int> newPosition) = 0;
    virtual QPair<int, int> getPositionOnBoard() const = 0;

};

}
}
}
#endif // IGAMEOBJECT_H
