#ifndef CONTEXT_LESSCPP
#define CONTEXT_LESSCPP

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "const_globals.h"

class Context {
protected:
    // texture that will be passed out to render
    sf::RenderTexture rentex;

    float m_dt;
    sf::Vector2f m_mousepos;


private:
    Context* m_return_context = nullptr;
    bool m_return = false;

public:
    Context* const m_previous; // being nullptr means there is no previous context

    virtual ~Context() = default;
    Context(Context* previous) : m_previous(previous) {
        // force the derived class to always initialize the pointer to previous context
        rentex.create(window_width, window_height);
    }
    Context() = delete;

    virtual void update(const float /* delta time */, const sf::Vector2f& /* Mouse position */) = 0;
    virtual Context* processBackgroundTask() = 0;
    virtual Context* processEvent(const sf::Event&) = 0;
    virtual sf::Texture render() = 0;

    Context* getReturnContext() const { return m_return_context; }
    void setReturnContext(Context* return_context) {
        m_return = true;
        m_return_context = return_context;
    }
    void setReturn() { m_return = true; }

    bool isQuitting() const { return m_return; }
};


#endif // CONTEXT_LESSCPP
