#include "Emitter.h"

namespace Error {

Emitter::Emitter()
{
}

Emitter::~Emitter()
{
}

void Emitter::notify(const ErrorInfo &einfo)
{
    for (auto i = m_listeners.begin(); i != m_listeners.end(); ++i)
    {
        (*i)->onError(einfo);
    }
}

void Emitter::addListener(const IListener *listener)
{
    for (auto i = m_listeners.begin(); i != m_listeners.end(); ++i)
    {
        if (*i == listener)
            return;
    }
    m_listeners.push_back(listener);
}

void Emitter::removeListener(const IListener *listener)
{
    for (auto i = m_listeners.begin(); i != m_listeners.end(); ++i)
    {
        if (*i == listener)
        {
            m_listeners.erase(i);
            break;
        }
    }
}

} // namespace Error
