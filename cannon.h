#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <iostream>

struct CannonBall;
struct Cannon;
struct Level;

struct Cannon
{ 
    float x, y;
    int health;

    CannonBall *projectile;
    sf::CircleShape *wheel;
    sf::RectangleShape *launcher;

    Cannon(float x = 0.0f, float y = 0.0f, 
        sf::CircleShape *wheel = nullptr, 
        sf::RectangleShape *launcher = nullptr) 
        : x(x), y(y), health(100), projectile(0), 
        wheel(wheel), launcher(launcher) {}
    ~Cannon();

    void DestroyProjectile();
};

struct CannonBall
{
    sf::CircleShape *shape;
    sf::Vector2f velocity;
    sf::Vector2f shotPosition;
    sf::Clock *airTime;
    float shotAngle;
    CannonBall() = delete;
    CannonBall(const Cannon *cannon, sf::Vector2f velocity);
    ~CannonBall();

    void UpdateTrajectory(const Level *level, Cannon *cannon);
};

struct Level
{
    sf::Clock *gameTime;
    int timer;
    float xBoundaries[2], yBoundaries[2];
    sf::Sprite *sprite;

    Level(float x1 = 0.0f, float y1 = 0.0f,
        float x2 = 0.0f, float y2 = 0.0f,
        sf::Sprite *sprite = nullptr);
    ~Level();

    void Wait(int ms);
    bool Ready();
};

void HandleControls(Level *level, Cannon *cannon);