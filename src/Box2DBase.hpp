//=============================================================================
//File Name: Box2DBase.hpp
//Description: Base class for Box2D shapes with SFML
//Author: Tyler Veness
//=============================================================================

#ifndef BOX2D_BASE_HPP
#define BOX2D_BASE_HPP

#include <Box2D/Box2D.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>

namespace Constant {
    static double G = 6.6738480f;
}

float BoxToSFML_x( float x );
float BoxToSFML_y( float y , float boxHeight );

sf::Vector2f BoxToSFML( float x , float y , float boxHeight );

float SFMLToBox_x( float x );
float SFMLToBox_y( float y , float boxHeight );

b2Vec2 SFMLToBox( float x , float y , float boxHeight );


class Box2DBase : public sf::Drawable {
public:
    Box2DBase( sf::Shape* sfShape , // SFML shape to sync with Box2D
            const sf::Vector2f& position , // starting position of object
            b2BodyType bodyType = b2_staticBody // Box2D body type
            );
    virtual ~Box2DBase();

    void syncObject( const sf::Window& referTo ); // syncs Box2D body attributes with SFML shape
    void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

    static b2World world;

    b2Body* body;
    sf::Shape* drawShape;

private:
    std::vector<sf::CircleShape*> m_objectPath;

    // Removes all dots from path drawing
    void clearPath();
};

#endif // BOX2D_BASE_HPP
