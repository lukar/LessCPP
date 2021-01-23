#include "join_dialog_context.h"
#include "game_context.h"
#include "helpers.h"

Context* JoinDialogContext::processBackgroundTask() { return nullptr; }
Context* JoinDialogContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(m_mousepos)) quit = true;
		else if (IPGameButton.contains(m_mousepos)); // do nothing
	}
	return nullptr;
}

sf::Texture JoinDialogContext::render() {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(IPGameButton);
	rentex.draw(quitButton);
	
	rentex.display();

	return rentex.getTexture();
}

