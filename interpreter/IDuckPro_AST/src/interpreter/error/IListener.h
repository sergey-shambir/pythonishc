#pragma once

#include <string>

#include "../Token.hpp"

namespace Error {

struct ErrorInfo
{
    const std::string &description;
    TokenPos start;
    TokenPos end;

    ErrorInfo(const std::string &vDescription, const Token &token)
        :description(vDescription)
        ,start(token.getStartPos())
        ,end(token.getEndPos())
    {
    }
};

class IListener
{
public:
    virtual ~IListener() {}
    virtual void onError(const ErrorInfo &info) const = 0;
};

}
