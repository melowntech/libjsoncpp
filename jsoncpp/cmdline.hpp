/**
 * Copyright (c) 2023 Melown Technologies SE
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
 * @file jsoncpp/cmdline.hpp
 * @author Vaclav Blazek <vaclav.blazek@melowntech.net>
 *
 * Interface for commandline tools with common JSON interface.
 */

#ifndef json_cmdline_json_hpp_included_
#define json_cmdline_json_hpp_included_

#include <optional>
#include <string>

#include "json.hpp"

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
