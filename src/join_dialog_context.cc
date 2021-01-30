#include "join_dialog_context.h"
#include "main_menu_context.h"
#include "game_context.h"
#include "helpers.h"
#include <iostream>
#include "osdialog.h"
#include <fstream>
#include <sstream>

JoinDialogContext::JoinDialogContext(Context* previous) : Context(previous) {
    IPTextInput.setPosition(window_width / 3 - 100, window_height / 3 + 100);
}

Context* JoinDialogContext::processBackgroundTask() { return nullptr; }
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
            return join_game();
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

Context* JoinDialogContext::join_game() {
    std::string json_string = get_game_tcp_packets(ip_player2, tcp_port);
    std::stringstream ss;
    ss << json_string;
    nlohmann::json game_json{};
    ss >> game_json;
    return new GameContext(this, game_json, ip_player2, tcp_port);
}

sf::Texture JoinDialogContext::render() {
    text.setString("Less Game");

    rentex.clear();

    rentex.draw(IPTextInput);
    rentex.draw(quitButton);

    rentex.display();

    return rentex.getTexture();
}
