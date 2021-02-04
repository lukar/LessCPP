#include "contexts/sub_menu_context.h"
#include <fstream>
#include <array>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "helpers.h"
#include "osdialog.h"

static std::string getTimeStr(){
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::array<char, 30> s = {'\0'};
    std::strftime(s.data(), s.size(), "%Y-%m-%d-%H-%M-%S", std::localtime(&now));
    return std::string(s.data());
}

SubMenuContext::SubMenuContext(Context* previous, sf::Texture pretext, const Game& game)
    : Context(previous), m_pretext(pretext), m_game(game)
{
    text.setPosition(window_width/3, window_height/3);
    text.setFillColor(sf::Color::Green);

    returnButton.setPosition(window_width/2 - 250, window_height - 50);
    continueButton.setPosition(window_width/2 + 60, window_height - 50);
    saveGameButton.setPosition(window_width/3 + 50, window_height/3 + 100);
}

Context* SubMenuContext::processBackgroundTask(){ return nullptr; }
Context* SubMenuContext::processEvent(const sf::Event & event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (returnButton.contains(m_mousepos)) { setReturnContext(m_previous->m_previous); }
        else if (continueButton.contains(m_mousepos)) { setReturnContext(m_previous); }
        else if (saveGameButton.contains(m_mousepos)) {
            osdialog_filters* filters = osdialog_filters_parse("json:json");
            char* filename = osdialog_file(OSDIALOG_SAVE, ".json", std::string(getTimeStr() + ".json").c_str(), filters);

            if (filename != nullptr) {
                std::ofstream of(filename);
                of << std::setw(4) << m_game.getJsonRepresentation();
                free(filename);
            }
        }
    }
    if (event.type == sf::Event::KeyPressed) {
    }
    return nullptr;
}

sf::Texture SubMenuContext::render() {
    rentex.clear();

    rentex.draw(text);
    rentex.draw(returnButton);
    rentex.draw(continueButton);
    rentex.draw(saveGameButton);
    rentex.display();

    return rentex.getTexture();
}

