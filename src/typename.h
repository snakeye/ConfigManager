#pragma once

namespace internal
{
template <typename T>
struct GetTypeNameHelper
{
    static const char *getTypeName()
    {
        return "";
    }
};

template <>
struct GetTypeNameHelper<bool>
{
    static const char *getTypeName()
    {
        return "bool";
    }
};

template <>
struct GetTypeNameHelper<int>
{
    static const char *getTypeName()
    {
        return "int";
    }
};
} // namespace internal

template <typename T>
const char *GetTypeName(void)
{
    return internal::GetTypeNameHelper<T>::getTypeName();
}
