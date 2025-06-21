// Copyright (c) Tyler Veness

#pragma once

#include <box2d/box2d.h>

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window/Window.hpp>

float BoxToSFML_x(float x);
float BoxToSFML_y(float y, float boxHeight);

sf::Vector2f BoxToSFML(float x, float y, float boxHeight);

float SFMLToBox_x(float x);
float SFMLToBox_y(float y, float boxHeight);

b2Vec2 SFMLToBox(float x, float y, float boxHeight);

class Box2DBase : public sf::Drawable {
 public:
  Box2DBase(sf::Shape* sfShape,            // SFML shape to sync with Box2D
            const sf::Vector2f& position,  // starting position of object
            b2BodyType bodyType = b2_staticBody  // Box2D body type
  );
  virtual ~Box2DBase();

  void syncObject(const sf::Window& referTo);  // syncs Box2D body attributes
                                               // with SFML shape
  void draw(sf::RenderTarget& target,
            sf::RenderStates states = sf::RenderStates::Default) const;

  static b2World world;

  b2Body* body;
  sf::Shape* drawShape;

 private:
  std::vector<sf::CircleShape*> m_objectPath;

  // Removes all dots from path drawing
  void clearPath();
};
