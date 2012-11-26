//=============================================================================
//File Name: Box2DBase.cpp
//Description: Base class for Box2D shapes with SFML
//Author: Tyler Veness
//=============================================================================

#include "Box2DBase.hpp"

float BoxToSFML_x( float x ) {
    return x * 30.f;
}

float BoxToSFML_y( float y , float boxHeight ) {
    return boxHeight - y * 30.f;
}

sf::Vector2f BoxToSFML( float x , float y , float boxHeight ) {
    return sf::Vector2f( x * 30.f , boxHeight - y * 30.f );
}

float SFMLToBox_x( float x ) {
    return x / 30.f;
}

float SFMLToBox_y( float y , float boxHeight ) {
    return ( boxHeight - y ) / 30.f;
}

b2Vec2 SFMLToBox( float x , float y , float boxHeight ) {
    return b2Vec2( x / 30.f , ( boxHeight - y ) / 30.f );
}

b2World Box2DBase::world( b2Vec2( 0.f , 0.f ) );

Box2DBase::Box2DBase( sf::Shape* sfShape , const sf::Vector2f& position , b2BodyType bodyType ) {
    b2BodyDef bodyDef;

    bodyDef.type = bodyType;
    bodyDef.position = b2Vec2( position.x / 30.f , ( 600 - position.y ) / 30.f );

    body = world.CreateBody( &bodyDef );

    drawShape = sfShape;
}

Box2DBase::~Box2DBase() {
    world.DestroyBody( body );
    clearPath();
}

void Box2DBase::syncObject( const sf::Window& referTo ) {
    drawShape->setPosition( BoxToSFML( body->GetPosition().x , body->GetPosition().y , referTo.getSize().y ) );
    drawShape->setRotation( 360.f - body->GetAngle() * 180.f / b2_pi );

    m_objectPath.push_back( new sf::CircleShape( 1.f , 4 ) );
    m_objectPath.at( m_objectPath.size() - 1 )->setPosition( drawShape->getPosition() );
    m_objectPath.at( m_objectPath.size() - 1 )->setFillColor( sf::Color( 255 , 0 , 0 ) );
}

void Box2DBase::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
    // Draw object at current position
    target.draw( *drawShape , states );

    // Draw previous path
    for ( unsigned int index = 0 ; index < m_objectPath.size() ; index++ ) {
        target.draw( *m_objectPath[index] );
    }
}

void Box2DBase::clearPath() {
    while ( m_objectPath.size() > 0 ) {
        delete m_objectPath[0]; // Free object's memory
        m_objectPath.erase( m_objectPath.begin() ); // Remove object from array
    }
}
