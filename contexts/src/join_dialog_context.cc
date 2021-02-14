#include "contexts/join_dialog_context.h"
#include "contexts/main_menu_context.h"
#include "contexts/game_context.h"
#include "contexts/server_room_context.h"

#include "helpers.h"
#include <iostream>
#include "osdialog.h"
#include <fstream>
#include <sstream>

JoinDialogContext::JoinDialogContext(Context* previous) : Context(previous)
{
    IPTextInput.setPosition(window_width / 3 - 100, window_height / 3 + 100);

    quitButton.setPosition(window_width / 2 - 15, window_height - 50);
}

Context* JoinDialogContext::processEvent(const sf::Event & event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (quitButton.contains(m_mousepos)) setReturnContext(m_previous);
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace)
            IPTextInput.backspace();
        else if (event.key.code == sf::Keyboard::Delete)
            IPTextInput.delete_front();
        else if (event.key.code == sf::Keyboard::Enter) {
            ip_player2 = IPTextInput.getText();
//            return new ServerRoomContext(this, connect_to_server(ip_player2, tcp_port));
        }
        else if (event.key.code == sf::Keyboard::Left) {
            IPTextInput.cursorLeft();
        }
        else if (event.key.code == sf::Keyboard::Right) {
            IPTextInput.cursorRight();
        }
    }
    else if (event.type == sf::Event::TextEntered) {
        if (31 < event.text.unicode and event.text.unicode < 126) {
            IPTextInput.append(static_cast<char>(event.text.unicode));
        }
    }
    return nullptr;
}

sf::Texture JoinDialogContext::render() {
    rentex.clear();

    rentex.draw(IPTextInput);
    rentex.draw(quitButton);

    rentex.display();

    return rentex.getTexture();
}
