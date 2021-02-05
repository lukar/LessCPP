#ifndef SERVER_ROOM_CONTEXT_LESSCPP
#define SERVER_ROOM_CONTEXT_LESSCPP

#include "const_globals.h"

#include "context.h"
#include "SFMLWidgets/button.h"
#include "game.h"
#include "less.h"

class ServerRoomContext : public Context {
private:
	// side text
	//sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(100, window_height / 4, Large, sf::Color::Green);

	widget::Button joinRoomButton = widget::Button("Join Room", Medium); 
	widget::Button listRoomsButton = widget::Button("List Game Rooms", Medium);
	widget::Button postGameButton = widget::Button("Host game", Medium);
	widget::Button returnButton = widget::Button("Return to Menu", Medium);

	sf::TcpSocket* m_tcp_socket;
	sf::Texture m_pretext;

public:

	ServerRoomContext(Context*, sf::TcpSocket* tcp_socket);

	void update(const float dt, const sf::Vector2f& mousepos) override { m_dt = dt; m_mousepos = mousepos; };
	Context* processEvent(const sf::Event&) override;
	Context* processBackgroundTask(void) override;
	sf::Texture render() override;
	Context* join_game_server(sf::TcpSocket* tcp_socket, std::string room_name);

};


#endif // SERVER_ROOM_CONTEXT_LESSCPP

