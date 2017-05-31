/**
 * Copyright (c) 2017 Melown Technologies SE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * *  Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file jsoncpp/as.hpp
 * @author Vaclav Blazek <vaclav.blazek@citationtech.net>
 *
 * Convenience as<Type>(json-value) "cast" operator.
 */

#ifndef jsoncpp_as_hpp_included_
#define jsoncpp_as_hpp_included_

#include <type_traits>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

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
typename std::enable_if<std::is_unsigned<T>::value, T>::type
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

template <typename T>
T asIntegral(const Value &v, const char *name)
{
    if (!v.isIntegral()) {
        detail::throwError("an integral", name);
    }
    return detail::asArithmetic<T>(v);
}

template <>
inline bool asIntegral<bool>(const Value &v, const char *name)
{
    if (!v.isBool()) {
        detail::throwError("a boolean", name);
    }
    return v.asBool();
}

} // namespace detail

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
as(const Value &v, const char *name = nullptr)
{
    return detail::asIntegral<T>(v, name);
}

template <typename T>
typename std::enable_if<std::is_same<float, T>::value, T>::type
as(const Value &v, const char *name = nullptr)
{
    if (!v.isDouble()) {
        detail::throwError("a real", name);
    }
    return v.asFloat();
}

template <typename T>
typename std::enable_if<std::is_same<double, T>::value, T>::type
as(const Value &v, const char *name = nullptr)
{
    if (!v.isDouble()) {
        detail::throwError("a real", name);
    }
    return v.asDouble();
}

template <typename T>
typename std::enable_if<std::is_same<std::string, T>::value, T>::type
as(const Value &v, const char *name = nullptr)
{
    if (!v.isString()) {
        detail::throwError("a string", name);
    }
    return v.asString();
}

template <typename T>
typename std::enable_if<std::is_enum<T>::value, T>::type
as(const Value &v, const char *name = nullptr)
{
    if (!v.isString()) {
        detail::throwError("an string", name);
    }
    return boost::lexical_cast<T>(v.asString());
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
inline bool getOpt(T &dest, const Json::Value &object, const char *member)
{
    if (!object.isMember(member)) { return false; }

    dest = as<T>
        (object[member]
         , utility::formatError("object[%s]", member).c_str());
    return true;
}

template <typename T>
inline boost::optional<T>&
get(boost::optional<T> &dest, const Json::Value &object, const char *member)
{
    if (!object.isMember(member)) {
        return dest;
    }

    return (dest = as<T>
            (object[member]
             , utility::formatError("object[%s]", member).c_str()));
}

template <typename T>
inline std::vector<T>&
get(std::vector<T> &dest, const Json::Value &object, const char *member)
{
    if (!object.isMember(member)) {
        LOGTHROW(err1, RuntimeError)
            << "Passed object doesn't have member <" << member << ">.";
    }

    const auto &list(object[member]);
    if (!list.isArray()) {
        LOGTHROW(err1, RuntimeError)
            << "Member <" << member << "> is not an array.";
    }

    dest.clear();
    for (const auto &item : list) {
        dest.push_back
            (as<T>
             (item, utility::formatError("object[%s][i]", member).c_str()));
    }

    return dest;
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
