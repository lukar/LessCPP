#ifndef SUB_MENU_CONTEXT_LESSCPP
#define SUB_MENU_CONTEXT_LESSCPP

#include "const_globals.h"

#include "context.h"

#include "SFMLWidgets/button.h"
#include "game.h"

class SubMenuContext : public Context {
private:
    Context* p_main_menu_context;      // aka back to main menu

    sf::Text text = sf::Text("Pause", Large.font, Large.chSize);

    widget::Button returnButton = widget::Button("Return to Menu", Medium);
    widget::Button continueButton = widget::Button("Continue", Medium);
    widget::Button saveGameButton = widget::Button("Save Game", Medium);

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

