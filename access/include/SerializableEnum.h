#ifndef SERIALIZABLEENUM_HH
#define SERIALIZABLEENUM_HH

#define DEFINE_ENUM_SERIALIZATION(SerializationT, EnumT) \
SerializationT& operator<<(SerializationT& s, EnumT e) \
{ \
    s << static_cast<std::underlying_type_t<EnumT>>(e); \
   return s; \
} \
SerializationT& operator>>(SerializationT& s, EnumT e) \
{ \
    std::underlying_type_t<EnumT> underlyingValue; \
    s  >> underlyingValue; \
    e = static_cast<EnumT>(underlyingValue); \
    return s; \
}
#endif // SERIALIZABLEENUM_HH
