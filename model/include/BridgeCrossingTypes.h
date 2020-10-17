#ifndef GAMELOGICTYPES_H
#define GAMELOGICTYPES_H

#include <model_pch.h>

using Position = std::pair<unsigned int, unsigned int>;
using Dimension2D = std::pair<unsigned int, unsigned int>;
using Identifier = int;
using CrossSpeed = unsigned int;
using ScoredPoint = unsigned int;

namespace kd417d
{
namespace eva
{
namespace logic
{

class BridgeCrossingTypes : public QObject
{
    Q_OBJECT

public:
    virtual ~BridgeCrossingTypes() override = default;
    BridgeCrossingTypes() = default;

    enum class BoardActionSet : int
    {

        SELECT_TO_CROSS,
        DESELECT_TO_CROSS,
        CROSS,
        SELECT_TO_RETURN,
        DESELECT_TO_RETURN,
        RETURN
    };

    enum class PlayerState : int
    {
        SELECTED_TO_CROSS,
        SELECTED_TO_RETURN,
        ON_CROSSING_SIDE,
        ON_RETURNING_SIDE
    };

    enum class PlayerType : int
    {
        SLOW,
        MEDIUM,
        FAST
    };

    enum class GameState : int
    {
        CROSS_SELECTION,
        RETURN_SELECTION
    };

    enum class PlayerActionSet : int
    {
        MOVE_TO_BRIDGE,
        CROSS,
        RETURN
    };

    Q_ENUM(BoardActionSet)
    Q_ENUM(PlayerState)
    Q_ENUM(PlayerType)
    Q_ENUM(GameState)
    Q_ENUM(PlayerActionSet)

};


}
}
}

#endif // GAMELOGICTYPES_H
