#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <sstream>
#include <iomanip>

struct CannonBall;
struct Cannon;
struct Level;

struct Level
{
    int timer;
    sf::Clock *gameTime;
    sf::Sprite *background;
    float xBoundaries[2], yBoundaries[2];

    char velBarState;
    sf::RectangleShape *velBorder, *velProgress;
    enum Stats
    {
        ANGLE, VELOCITY, POSX, POSY, TIME, RESULTX, RESULTY
    };
    sf::Text *statText[RESULTY];

    Level(float x1 = 0.0f, float y1 = 0.0f,
        float x2 = 0.0f, float y2 = 0.0f,
        const sf::Texture *texture = nullptr);
    ~Level();

    void Wait(int ms);
    bool Ready();
    void InitVelocityBar();
    void DestroyVelocityBar();
    void InitPositionStats(const sf::Font &font);
    void DestroyPositionStats();
    void UpdatePositionStats(const CannonBall *projectile);
    bool GetVelocity(float *velocity);
    void Render(sf::RenderWindow *window);
};
