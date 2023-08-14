#ifndef json_cmdline_json_hpp_included_
#define json_cmdline_json_hpp_included_

#include <optional>
#include <string>

#include "json.hpp"

/** Interface for commandline tools with common JSON interface.
 */

namespace Json::cmdline {

struct Reply {
    Reply(int status = 200, const std::string &message = "OK")
        : status(status), message(message)
    {}

    Reply(const std::string &message)
        : status(200), message(message)
    {}

    Reply& setResultCode(const std::optional<int> &value) {
        resultCode = value;
        return *this;
    }

    Reply& makeSilenceable(bool value = true) {
        silenceable = value;
        return *this;
    }

    int status;
    std::string message;

    Value extra;

    std::optional<int> resultCode;

    bool silenceable = false;
};

bool failed(const Reply &reply);

int resultCode(const Reply &reply);

bool silent(const Reply &reply, bool enabled);

void serialize(std::ostream &os, const Reply &reply, bool enabled = false);

// inlines

inline bool failed(const Reply &reply) {
    return (reply.status / 100) != 2;
}

inline int resultCode(const Reply &reply)
{
    if (reply.resultCode) { return *reply.resultCode; }
    return failed(reply);
}

inline bool silent(const Reply &reply, bool enabled)
{
    return enabled && (reply.silenceable || !resultCode(reply));
}

} // namespace Json::cmdline

#endif // json_cmdline_json_hpp_included_
