#include "cannon.hpp"

int main()
{
    // Load all resources 
    sf::Font font;
    sf::Texture bg;
    if (!font.loadFromFile("assets/Erewhon-Regular.otf")
        || !bg.loadFromFile("assets/background.png"))
        return 1;

    //Setup window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Cannon Game");
    window.setFramerateLimit(60);

    //Setup background
    Level level(bg, 0.0f, -500.0f, 1280.0f, 615.0f);

    //Setup cannon
    Cannon cannon(50.0f, 575.0f, 50.0f, 25.0f);

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
                    return 0;
            }   
        }

        //Monitor Keys
        if (window.hasFocus())
            HandleControls(level, cannon);

        //Update World
        float velocity;
        if (level.GetVelocity(&velocity))
        {
            cannon.InitProjectile(velocity);
            level.InitPositionStats(font);
        }
        if (CannonBall *projectile = cannon.projectile.get())
        {
            if (projectile->UpdateTrajectory(level, cannon))
            {
                cannon.DestroyProjectile();
                level.DestroyPositionStats();
                level.DestroyVelocityBar();
            }
            else
            {
                level.UpdatePositionStats(projectile);
            }
        }

        //Draw Screen
        window.clear();
        level.Render(window);
        cannon.Render(window);
        window.display();
    }

    return 0; 
}