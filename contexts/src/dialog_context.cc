#include "contexts/dialog_context.h"
#include "contexts/game_context.h"
#include "helpers.h"
#include <iostream>
#include "osdialog.h"


DialogContext::DialogContext(Context* previous, sf::TcpSocket& socket, std::string str)
    : Context(previous),
      m_default_text(str + " " /*gets deleted*/)
{
      IPTextInput.backspace();
      quitButton.setPosition(window_width / 2 - 15, window_height/2 - 50);
}

Context* DialogContext::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
        if (quitButton.contains(m_mousepos)) setReturnContext(m_previous);
	}
	if (event.key.code == sf::Keyboard::BackSpace)
		IPTextInput.backspace();
	if (event.key.code == sf::Keyboard::Enter) {
		setReturnContext(nullptr);
		IPTextInput.backspace(); // last space
//		return m_function(IPTextInput.getText());
	}
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128) {

			if (event.text.unicode == '\b') IPTextInput.append(' ');
			else IPTextInput.append(static_cast<char>(event.text.unicode));
		}
	}
	return nullptr;
}




sf::Texture DialogContext::render() {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(IPTextInput);
	rentex.draw(quitButton);

	rentex.display();

	return rentex.getTexture();
}
