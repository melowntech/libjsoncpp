#include "io.hpp"

#include "cmdline.hpp"

namespace Json::cmdline {

namespace {

Value asJson(const Reply &reply) {
    auto value(reply.extra);
    value["status"] = reply.status;
    value["message"] = reply.message;
    return value;
}

void serialize(std::ostream &os, const Value &v, bool enabled)
{
    if (enabled) {
        write(os, v, true);
    }
}

} // namespace

void serialize(std::ostream &os, const Reply &reply, bool enabled)
{
    if (!enabled) { return; }
    serialize(os, asJson(reply), true);
}

} // namespace Json::cmdline

