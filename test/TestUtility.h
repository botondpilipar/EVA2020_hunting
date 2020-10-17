#ifndef TESTUTILITY_H
#define TESTUTILITY_H

#include <QtTest>
#define QINRANGE(value, rangeMin, rangeMax) \
    QVERIFY(value >= rangeMin); \
    QVERIFY(value <= rangeMax)

template<class PointerT>
struct DereferenceFunctor
{
    auto operator()(const PointerT& ptr) -> decltype (*ptr)
    {
        return *ptr;
    }
};

#endif // TESTUTILITY_H
