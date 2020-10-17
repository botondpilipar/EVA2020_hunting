#ifndef SINGLETONFACTORY_HPP
#define SINGLETONFACTORY_HPP

#include <assert.h>
namespace kd417d
{
namespace eva
{

template<typename T>
class SingletonFactory
{
public:
    static void setFactory(T* instance)
    {
        mSingletonInstance = instance;
    }
    static void deleteFactory()
    {
        if(isFactorySet())
        {
            delete mSingletonInstance;
        }
    }
    static bool isFactorySet() { return mSingletonInstance != nullptr; }
    static T& instance()
    {
        assert(mSingletonInstance != nullptr);
        return *mSingletonInstance;
    }
    static T* ptrInstance()
    {
        assert(mSingletonInstance != nullptr);
        return mSingletonInstance;
    }
protected:
    static T* mSingletonInstance;
};

template<class T>
T* SingletonFactory<T>::mSingletonInstance = nullptr;
}
}
#endif // SINGLETONFACTORY_HPP
