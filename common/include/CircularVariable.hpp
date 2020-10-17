#ifndef CIRCULARVARIABLE_HPP
#define CIRCULARVARIABLE_HPP

#include <cstdint>
#include <type_traits>
#include <QDataStreamSerializable.h>

template<class T,
         typename std::enable_if<
             std::is_destructible_v<T> &&
             std::is_constructible_v<T> &&
             std::is_copy_constructible_v<T> &&
             std::is_move_constructible_v<T> &&
             std::is_copy_assignable_v<T> &&
             std::is_move_assignable_v<T> >
         >
class CircularVariable
{

public:
    virtual ~CircularVariable() = default;

    CircularVariable(const CircularVariable& other) = default;
    CircularVariable(CircularVariable&& other) = default;
    CircularVariable& operator=(const CircularVariable&) = default;
    CircularVariable& operator=(CircularVariable&&) = default;

    CircularVariable(T min, T max, T initial)
        : mMin(min), mMax(max), mCurrent(initial)
    {}

    CircularVariable& advanceForward()
    {
        if(mCurrent == mMax)
        {
            mCurrent = mMin;
            onRewind();
        }
        ++mCurrent;
        return this;
    }
    CircularVariable& advanceBackward()
    {
        if(mCurrent == mMin)
        {
            mCurrent = mMax;
            onRewind();
        }
        --mCurrent;
        return this;
    }
    inline CircularVariable& operator++() { return advanceForward(); }
    inline CircularVariable& operator--() { return advanceBackward(); }

    void setMin(const T& min) { mMin = min; }
    void setMax(const T& max) { mMax = max; }
    void setCurrent(const T& current) { mCurrent = current; }

    inline const T& current() const { return mCurrent; }

    void onRewind() = 0;
private:
    T mCurrent;
    T mMin;
    T mMax;
};
#endif // CIRCULARVARIABLE_HPP
