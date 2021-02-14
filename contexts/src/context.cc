#include "contexts/context.h"


void Context::update(const float dt, const sf::Vector2f& mousepos )
{
    m_dt = dt;
    m_mousepos = mousepos;
}

void Context::processBackgroundTask()
{
}
