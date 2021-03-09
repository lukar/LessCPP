#ifndef GAME_MENU_CONTEXT_LESSCPP
#define GAME_MENU_CONTEXT_LESSCPP

#include "const_globals.h"

#include "contexts/context.h"
#include "contexts/game_context.h"

#include "SFMLWidgets/button.h"

#include "game.h"

class GameMenuContext : public Context {
private:
    Context* p_main_menu_context;      // aka back to main menu

    sf::Text text = sf::Text("Pause", Large->font, Large->chSize);

    widget::Button returnButton = widget::Button("Return to Menu", *Medium);
    widget::Button continueButton = widget::Button("Continue", *Medium);
    widget::Button saveGameButton = widget::Button("Save Game", *Medium);

    sf::Texture m_pretext;
    GameContext* m_game_context;
    const Game & m_game;

public:

    GameMenuContext(GameContext* previous, sf::Texture pretext, const Game& game);

    Context* processEvent(const sf::Event&) override;
    sf::Texture render() override;

};


#endif // GAME_MENU_CONTEXT_LESSCPP

