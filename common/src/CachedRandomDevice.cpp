#include <CachedRandomDevice.h>

using namespace kd417d::eva;

int CachedRandomDevice::random()
{
    int randomAttemp = dist(generator);
    while(used.find(randomAttemp) != used.end())
    {
        randomAttemp = dist(generator);
    }
    used.insert(randomAttemp);
    return randomAttemp;
}
