#include "level.h"

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
    Cannon(float x, float y, float width, float height);
    ~Cannon();

    void InitProjectile(float velocity);
    void DestroyProjectile();
    void Render(sf::RenderWindow *window);
};

struct CannonBall
{
    sf::CircleShape *shape;
    sf::Vector2f velocity;
    float shotVelocity;
    sf::Vector2f shotPosition;
    sf::Clock *airTime;
    
    float shotAngle;

    CannonBall() = delete;
    CannonBall(const Cannon *cannon, float velocity);
    ~CannonBall();

    void UpdateTrajectory(Level *level, Cannon *cannon);
};

void HandleControls(Level *level, Cannon *cannon);