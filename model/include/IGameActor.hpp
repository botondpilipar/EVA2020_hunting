#ifndef IGAMEACTOR_HPP
#define IGAMEACTOR_HPP

#include "IMovableObject.hpp"

namespace kd417d
{
namespace eva
{
namespace logic
{

template<class DataReprType,
         class ActionSet,
         class = typename std::enable_if<std::is_enum<ActionSet>::value>>
class IGameActor : public IMovableObject<DataReprType>
{
public:
    virtual ~IGameActor() override = default;
    IGameActor() = default;

    virtual void performAction(ActionSet action) = 0;
    virtual int getUniqueId() const = 0;

};

}
}
}
#endif // IGAMEACTOR_HPP
