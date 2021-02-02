#ifndef SUB_MENU_CONTEXT_LESSCPP
#define SUB_MENU_CONTEXT_LESSCPP

#include "const_globals.h"

#include "context.h"

#include "menu_button.h"
#include "game.h"

class SubMenuContext : public Context {
private:
    Context* p_main_menu_context;      // aka back to main menu

    // side text
    sf::Text text = initializeText(font, large_font, window_width/3, window_height/3, sf::Color::Green);

    menu::Button returnButton = menu::Button("Return to Menu", Medium);
    menu::Button continueButton = menu::Button("Continue", Medium);
    menu::Button saveGameButton = menu::Button("Save Game", Medium);

    sf::Texture m_pretext;
    const Game & m_game;

public:

    SubMenuContext(Context* previous, sf::Texture pretext, const Game& game);

    void update(const float dt, const sf::Vector2f& mousepos) override { m_dt = dt; m_mousepos = mousepos; }
    Context* processEvent(const sf::Event&) override;
    Context* processBackgroundTask() override;
    sf::Texture render() override;

};


#endif // SUB_MENU_CONTEXT_LESSCPP

