#ifndef IDATAINITIALIZABLE_HPP
#define IDATAINITIALIZABLE_HPP


namespace kd417d
{
namespace eva
{
namespace access
{
template<class DataReprType>
class IDataInitializable
{
public:
    virtual ~IDataInitializable() = default;
    IDataInitializable() = default;

    virtual void initialize(const DataReprType& representation) = 0;
    virtual DataReprType* save() const = 0;
};

}
}
}
#endif // IDATAINITIALIZABLE_HPP
