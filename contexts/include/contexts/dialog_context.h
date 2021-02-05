#ifndef DIALOG_CONTEXT_LESSCPP
#define DIALOG_CONTEXT_LESSCPP

#include "context.h"

#include "const_globals.h"
#include "less.h"

#include "SFMLWidgets/button.h"
#include "SFMLWidgets/text_input.h"
#include "helpers.h"
#include "contexts/main_menu_context.h"
#include <functional>

class DialogContext : public Context {
private:
	// side text
	std::string m_default_text;
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(window_width / 3, window_height / 3, Large, sf::Color::Green);
	widget::TextInput IPTextInput = widget::TextInput("Enter Value", 15, Medium, m_default_text, [](std::string t) { return true; });
	widget::Button quitButton = widget::Button("Quit", Medium);
	std::function <Context* (std::string str)> m_function;
	Context* p_return_context;

public:

	DialogContext() = default;
	DialogContext(Context*, std::function <Context* (std::string str)> m_function, std::string str);

	void update(const float dt, const sf::Vector2f& mousepos) override { m_dt = dt; m_mousepos = mousepos; };
	Context* processEvent(const sf::Event&) override;
	Context* processBackgroundTask(void) override;
	sf::Texture render() override;
};


#endif // DIALOG_CONTEXT_LESSCPP
