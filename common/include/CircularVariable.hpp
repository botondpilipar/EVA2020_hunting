#ifndef CIRCULARVARIABLE_HPP
#define CIRCULARVARIABLE_HPP

#include <cstdint>
#include <type_traits>
#include <QDataStreamSerializable.h>

template<class T,
         typename = typename std::enable_if<
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
        return *this;
    }
    CircularVariable& advanceBackward()
    {
        if(mCurrent == mMin)
        {
            mCurrent = mMax;
            onRewind();
        }
        --mCurrent;
        return *this;
    }
    inline CircularVariable& operator++(int) { return advanceForward(); }
    inline CircularVariable& operator--(int) { return advanceBackward(); }

    void setMin(const T& min) { mMin = min; }
    void setMax(const T& max) { mMax = max; }
    void setCurrent(const T& current) { mCurrent = current; }

    inline const T& current() const { return mCurrent; }

    virtual void onRewind() {}
private:
    T mCurrent;
    T mMin;
    T mMax;
};
#endif // CIRCULARVARIABLE_HPP
