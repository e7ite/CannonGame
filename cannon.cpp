#include "cannon.h" 

Level::Level(float x1, float y1, float x2, float y2, sf::Sprite *sprite)
{
    this->timer = 0;
    this->gameTime = new sf::Clock;
    this->xBoundaries[0] = x1;     
    this->xBoundaries[1] = x2;     
    this->yBoundaries[0] = y1;
    this->yBoundaries[1] = y2;
    this->sprite = sprite;
}

Level::~Level()
{
    delete this->sprite;
    this->sprite = nullptr;
    delete this->gameTime;
    this->gameTime = nullptr;
}

void Level::Wait(int ms)
{
    this->timer = this->gameTime->getElapsedTime().asMilliseconds() + ms;
}

bool Level::Ready()
{
    return this->gameTime->getElapsedTime().asMilliseconds() > this->timer;
}

Cannon::~Cannon()
{
    delete this->launcher;
    this->launcher = nullptr;
    delete this->projectile;
    this->projectile = nullptr;
    delete this->wheel;
    this->wheel = nullptr;
}

void Cannon::DestroyProjectile()
{
    this->projectile->~CannonBall();
    delete this->projectile;
    this->projectile = nullptr;
}

CannonBall::CannonBall(const Cannon *cannon, sf::Vector2f velocity)
{
    this->shape = new sf::CircleShape(10.0f);
    this->airTime = new sf::Clock;
    this->velocity = velocity;
    sf::Vector2f cannonPos = cannon->launcher->getPosition();
    float hyp = cannon->launcher->getSize().x;
    this->shotAngle = cannon->launcher->getRotation() * M_PI / 180.0f;
    this->velocity.x *= cos(this->shotAngle);
    this->velocity.y *= sin(this->shotAngle);
    this->shape->setPosition(
        cannonPos.x + hyp * cos(this->shotAngle),
        cannonPos.y + hyp * sin(this->shotAngle)
    );
    this->shape->setOrigin(10.0f, 10.0f);
    this->shape->setFillColor(sf::Color::Black);
    this->shotPosition = this->shape->getPosition();
}

CannonBall::~CannonBall()
{
    delete this->shape;
    this->shape = nullptr;
    delete this->airTime;
    this->airTime = nullptr;
}

void CannonBall::UpdateTrajectory(const Level *level, Cannon *cannon)
{
    //Position equation = X(t) = Xi + Vit + 0.5at^2
    float seconds = this->airTime->getElapsedTime().asSeconds() * 4.0f;
    //std::cout << this->velocity.x << " " << this->velocity.y << std::endl;
    this->shape->setPosition(
        this->shotPosition.x + (this->velocity.x * seconds) 
            /* + 0.5f * 0 * pow(seconds, 2)**/,
        this->shotPosition.y + this->velocity.y * seconds 
            + 0.5f * 9.8f * pow(seconds, 2)
    );

    sf::Vector2f position = this->shape->getPosition();
    if (position.x < level->xBoundaries[0] || position.x > level->xBoundaries[1]
        || position.y < level->yBoundaries[0] || position.y > level->yBoundaries[1])
        cannon->DestroyProjectile();
}

void HandleControls(Level *level, Cannon *cannon)
{
    if (level->Ready())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            cannon->launcher->rotate(-0.5f);
            level->Wait(8);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            cannon->launcher->rotate(0.5f);
            level->Wait(8);
        }

        if (!cannon->projectile && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            cannon->projectile = new CannonBall(cannon, 
                sf::Vector2f(86.51f, 86.51f));
        }   
    }
}