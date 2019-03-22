#pragma once

namespace internal
{
template <typename T>
struct GetTypeNameHelper
{
    /**
     * @brief Return empty name for generic type
     *
     * @return const char*
     */
    static const char *getTypeName()
    {
        return "";
    }
};

template <>
struct GetTypeNameHelper<bool>
{
    /**
     * @brief Return type name for bool
     *
     * @return const char*
     */
    static const char *getTypeName()
    {
        return "bool";
    }
};

template <>
struct GetTypeNameHelper<int>
{
    /**
     * @brief Return type name for integer
     *
     * @return const char*
     */
    static const char *getTypeName()
    {
        return "number";
    }
};

template <>
struct GetTypeNameHelper<long>
{
    /**
     * @brief Return type name for integer
     *
     * @return const char*
     */
    static const char *getTypeName()
    {
        return "number";
    }
};
} // namespace internal

/**
 * @brief Return type name
 *
 * @tparam T
 * @return const char*
 */
template <typename T>
const char *GetTypeName(void)
{
    return internal::GetTypeNameHelper<T>::getTypeName();
}
