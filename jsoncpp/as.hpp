/**
 * @file jsoncpp/as.hpp
 * @author Vaclav Blazek <vaclav.blazek@citationtech.net>
 *
 * Convenience as<Type>(json-value) "cast" operator.
 */

#ifndef jsoncpp_as_hpp_included_
#define jsoncpp_as_hpp_included_

#include <type_traits>

#include "dbglog/dbglog.hpp"
#include "utility/raise.hpp"

#include "json.hpp"

namespace Json {

namespace detail {

template <typename T>
inline typename std::enable_if<std::is_signed<T>::value, T>::type
asArithmetic(const Value &v)
{
    return v.asLargestInt();
}

template <typename T>
inline typename std::enable_if<std::is_unsigned<T>::value, T>::type
asArithmetic(const Value &v) {
    return v.asLargestUInt();
}

inline void throwError(const char *type, const char *name)
{
    if (name) {
        utility::raise<RuntimeError>("Value stored in %s is not %s value"
                                     , name, type);
    }

    utility::raise<RuntimeError>("Stored value is not %s value", type);
}

} // namespace detail

template <typename T, typename Enable = void>
T as(const Value&, const char *name = nullptr);

template
<typename T
 , typename = typename std::enable_if<std::is_integral<T>::value>::type>
inline T as(const Value &v, const char *name)
{
    if (!v.isIntegral()) {
        detail::throwError("an integral", name);
    }
    return detail::asArithmetic<T>(v);
}

template <>
inline float as<float, void>(const Value &v, const char *name)
{
    if (!v.isDouble()) {
        detail::throwError("a real", name);
    }
    return v.asFloat();
}

template <>
inline double as<double, void>(const Value &v, const char *name)
{
    if (!v.isDouble()) {
        detail::throwError("a real", name);
    }
    return v.asDouble();
}

template <>
inline std::string as<std::string, void>(const Value &v
                                         , const char *name)
{
    if (!v.isString()) {
        detail::throwError("a string", name);
    }
    return v.asString();
}

template <>
inline bool as<bool, void>(const Value &v, const char *name)
{
    if (!v.isBool()) {
        detail::throwError("a boolean", name);
    }
    return v.asBool();
}

template <typename T>
inline T& get(T &dest, const Json::Value &value)
{
    return (dest = as<T>(value));
}

template <typename T>
inline T& get(T &dest, const Json::Value &object, const char *member)
{
    if (!object.isMember(member)) {
        LOGTHROW(err1, RuntimeError)
            << "Passed object doesn't have member <" << member << ">.";
    }

    return (dest = as<T>
            (object[member]
             , utility::formatError("object[%s]", member).c_str()));
}

template <typename T>
inline T& get(T &dest, const Json::Value &list
              , Json::ArrayIndex index, const char *name = "unknown")
{
    if (!list.isArray()) {
        LOGTHROW(err1, RuntimeError) << name << "is not an array.";
    }

    if (index >= list.size()) {
        LOGTHROW(err1, RuntimeError)
            << name << "[ " << index
            << " doesn't exist (length is " << list.size() << ").";
    }

    return (dest = as<T>
            (list[index]
             , utility::formatError("%s[%i]", name, index)
             .c_str()));
}

template <typename T>
inline T& get(T &dest, const Json::Value &object, const char *member
              , Json::ArrayIndex index)
{
    if (!object.isMember(member)) {
        LOGTHROW(err1, RuntimeError)
            << "Passed object doesn't have member <" << member << ">.";
    }

    const auto &list(object[member]);
    if (!list.isArray()) {
        LOGTHROW(err1, RuntimeError)
            << "Object[" << member << "] is not an array.";
    }

    if (index >= list.size()) {
        LOGTHROW(err1, RuntimeError)
            << "Object[" << member << "][ " << index
            << " doesn't exist (lenght is " << list.size() << ").";
    }

    return (dest = as<T>
            (list[index]
             , utility::formatError("object[%s][%i]", member, index)
             .c_str()));
}

inline Value& check(Value &value, ValueType type)
{
    if (value.type() != type) {
        throw RuntimeError("Invalid type.");
    }
    return value;
}

inline Value& check(Value &value, ValueType type, const char *what)
{
    if (value.type() != type) {
        throw RuntimeError
            (utility::formatError("Invalid type of \"%s\".", what));
    }
    return value;
}

inline const Value& check(const Value &value, ValueType type)
{
    if (value.type() != type) {
        throw RuntimeError("Invalid type.");
    }
    return value;
}

inline const Value& check(const Value &value, ValueType type
                          , const char *what)
{
    if (value.type() != type) {
        throw RuntimeError
            (utility::formatError("Invalid type of \"%s\".", what));
    }
    return value;
}

} // namespace Json

#endif // jsoncpp_as_hpp_included_
