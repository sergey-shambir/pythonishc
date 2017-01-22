#pragma once

struct Token
{
    unsigned line;
    unsigned column;
    union {
        // Id in CStringPool object. Always 0 for most tokens.
        unsigned stringId;
        double value;
    };
};
