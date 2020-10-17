#include <BridgeCrossingSettings.h>

using namespace kd417d::eva::logic;

BridgeCrossingSettings::BridgeCrossingSettings()
    : mSlowPlayerNumber(BridgeCrossingSettingDefaults::slowPlayerNumber),
      mMediumPlayerNumber(BridgeCrossingSettingDefaults::mediumPlayerNumber),
      mFastPlayerNumber(BridgeCrossingSettingDefaults::fastPlayerNumber),
      mSlowPlayerCrossTime(BridgeCrossingSettingDefaults::slowPlayerCrossSpeed),
      mMediumPlayerCrossTime(BridgeCrossingSettingDefaults::mediumPlayerCrossSpeed),
      mFastPlayerCrossTime(BridgeCrossingSettingDefaults::fastPlayerCrossSpeed)
{
}
