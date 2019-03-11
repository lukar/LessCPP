#include "block_shape.hh"
#include <SFML/Graphics.hpp>

int main()
{
    using namespace sf;

    RenderWindow window(VideoMode(400.f, 400.f), "Less game", Style::Titlebar);
    window.setPosition(Vector2i(0.f, 0.f));

    BlockShape block1{200.f, 2.f};
    BlockShape block2{200.f, 2.f};
    BlockShape block3{200.f, 2.f};

    block2.setPosition(200.f, 0);
    block3.setPosition(0, 200.f);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
			if (event.type == Event::KeyPressed){
				if (event.key.code == Keyboard::Q){
					window.close();
				}
			}
        }

        window.clear();
        window.draw(block1);
        window.draw(block3);
        window.draw(block2);
        window.display();
    }

    return 0;
}
