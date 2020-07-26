#include <iostream>
#include <array>
#include <thread>
#include <SFML/Graphics.hpp>
#include "boost/lockfree/queue.hpp"

//Function provided by object file output by hack_lifter
extern "C" void f(int16_t *);

/*
 * Keyboard input buffer for the Hack code.
 * Has to be a global variable (ack!) because the keyboard/screen update code needs access,
 * and the Hack code needs a raw C-style function callback.  Can't make it local and bind/lambda
 * my way to a function callback the LLVM IR stage will accept.
 */
boost::lockfree::queue<int16_t> key_buffer{1024};

bool volatile hack_code_finished = false;

void handle_keyboard(int16_t *mem) {
    int16_t val = 0;
    key_buffer.pop(val);
    mem[0x6000] = val;
}

void keyboard_and_screen_update(std::array<int16_t, 32768> *mem) {
    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 256), "Hack Lifter Screen");
    window.setFramerateLimit(10);

    //Framebuffer that will be drawn to the screen
    //RGBA byte order
    auto *pixels = new sf::Uint8[512 * 256 * 4];
    sf::Image img;
    img.create(512, 256, sf::Color(255, 255, 255));
    sf::Texture texture;
    texture.loadFromImage(img);
    sf::Sprite sprite(texture);

    // run the program as long as the window is open
    while (window.isOpen() && !hack_code_finished) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event{};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else {
                    key_buffer.push(event.key.code);
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        //TODO: Parse hack screen memory into an image

        // draw everything here...
        // window.draw(...);
        window.draw(sprite);

        // end the current frame
        window.display();
    }
}

int main() {
    std::cout << "Starting code..." << std::endl;

    std::array<int16_t, 32768> mem{};
    mem[0] = 4;
    mem[1] = 0;

    std::thread updater(keyboard_and_screen_update, &mem);

    //launch Hack code
    f(mem.data());
    std::cout << "Code complete" << std::endl;
    std::cout << mem[0] << " + " << mem[1] << " = " << mem[2] << std::endl;

    updater.join();

    //std::cout << "Key buffer:\n";
    //key_buffer.consume_all([](const int16_t v){ std::cout << v << ' ';});


    return 0;
}
