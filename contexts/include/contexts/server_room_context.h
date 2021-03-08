#ifndef SERVER_ROOM_CONTEXT_LESSCPP
#define SERVER_ROOM_CONTEXT_LESSCPP

#include "const_globals.h"

#include "context.h"
#include "SFMLWidgets/button.h"
#include "SFMLWidgets/text_input.h"
#include "game.h"
#include "less.h"

#include <regex>
#include <memory>

class ServerRoomContext : public Context {
private:

	// side text
	//sf::Font font = getFont("resources/Roboto_Medium.ttf");
    sf::Text title = initializeText(100, window_height / 10, *Large, sf::Color::Green);

    widget::TextInput RoomNameTextInput = widget::TextInput("Room name", 10, *Medium, "");
    widget::TextInput IPTextInput = widget::TextInput("IP", 15, *Medium, "127.0.0.1",
        [](std::string t) { return std::regex_match(t, std::regex("(\\d\\d?\\d?\\.){3}\\d\\d?\\d?")); } );
    widget::TextInput* focusedTextInput = &RoomNameTextInput;

    widget::Button ConnectButton = widget::Button("Connect", *Medium);

	widget::Button joinRoomButton = widget::Button("Join Room", *Medium); 
	widget::Button listRoomsButton = widget::Button("List Game Rooms", *Medium);
	widget::Button postGameButton = widget::Button("Host game", *Medium);
	widget::Button returnButton = widget::Button("Return to Menu", *Medium);

    std::unique_ptr<sf::TcpSocket> m_tcp_socket;

    float m_timer = 0;
    bool m_connected = false;
    bool m_pinging = false;


    sf::Packet m_packet;

public:

    ServerRoomContext(Context*);

    Context* processEvent(const sf::Event&) override;
	sf::Texture render() override;
    void processBackgroundTask() override;

    Context* join_game_server(std::unique_ptr<sf::TcpSocket> tcp_socket, std::string room_name);
};


#endif // SERVER_ROOM_CONTEXT_LESSCPP

