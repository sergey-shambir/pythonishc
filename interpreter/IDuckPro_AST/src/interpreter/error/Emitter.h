#pragma once

#include "IListener.h"
#include <list>

namespace Error {

class Emitter
{
public:
    Emitter();
    ~Emitter();

    void notify(const ErrorInfo &einfo);
    void addListener(const IListener *listener);
    void removeListener(const IListener *listener);

private:
    std::list<const IListener*> m_listeners;
};

} // namespace Error
