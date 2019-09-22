#include "cannon.h"

int main()
{
    //Setup window + clock
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Cannon Game");
    window.setFramerateLimit(60);
    //Setup background
    Level *level = new Level(0.0f, -500.0f, 1280.0f, 615.0f);
    sf::Texture bg;
    if (!bg.loadFromFile("assets/background.png"))
        return 0;
    level->sprite = new sf::Sprite;
    level->sprite->setTexture(bg);

    //Setup cannons
    Cannon *cannon = new Cannon(50.0f, 575.0f);
    cannon->wheel = new sf::CircleShape(25.0f);
    cannon->wheel->setPosition(cannon->x, cannon->y);
    cannon->wheel->setFillColor(sf::Color::Black);
    cannon->launcher = new sf::RectangleShape();
    cannon->launcher->setSize(sf::Vector2f(50.0f, 25.0f));
    cannon->launcher->setPosition(cannon->x + 25.0f, cannon->y + 25.0f);
    cannon->launcher->setOrigin(0, 12.5f);

    //Begin SFML rendering procedure
    while(window.isOpen())
    {
        //Poll window
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    delete cannon;
                    cannon = nullptr;
                    delete level;
                    level = nullptr;
                    return 0;
            }   
        }
        
        //Monitor Keys
        HandleControls(level, cannon);

        //Update World
        if (cannon->projectile)
            cannon->projectile->UpdateTrajectory(level, cannon);
        
        //Draw Screen
        window.clear();
        window.draw(*level->sprite);
        if (cannon->projectile)
            window.draw(*cannon->projectile->shape);
        window.draw(*cannon->launcher);
        window.draw(*cannon->wheel);
        window.display();
    }

    return 0; 
}