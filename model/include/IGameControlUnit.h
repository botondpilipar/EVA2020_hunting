#ifndef IGAMELOGICUNIT_H
#define IGAMELOGICUNIT_H

#include <model_pch.h>
#include <IDataInitializable.hpp>
#include <ISerializable.h>

#include "ISettingsChangedObserver.h"

namespace kd417d
{
namespace eva
{
namespace logic
{

using namespace kd417d::eva::access;

class IGameControlUnit : public ISettingsChangedObserver
{
public:
    virtual ~IGameControlUnit() = default;
    IGameControlUnit() = default;

    virtual void startNewGame() = 0;
    virtual void pauseGame() = 0;
    virtual void continueGame() = 0;
};

}
}
}
#endif // IGAMELOGICUNIT_H
