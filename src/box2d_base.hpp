// Copyright (c) Tyler Veness

#pragma once

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window/Window.hpp>
#include <box2d/box2d.h>

inline sf::Vector2f box2d_to_sfml(const b2Vec2& vec, float box_height) {
  return {vec.x * 30.f, box_height - vec.y * 30.f};
}

inline b2Vec2 sfml_to_box2d(const sf::Vector2f& vec, float box_height) {
  return {vec.x / 30.f, (box_height - vec.y) / 30.f};
}

class Box2DBase : public sf::Drawable {
 public:
  /**
   * Constructs a Box2D base object.
   *
   * @param sf_shape SFML shape to sync with Box2D.
   * @param position Starting position of object.
   * @param body_type Box2D body type.
   */
  Box2DBase(sf::Shape* sf_shape, const sf::Vector2f& position,
            b2BodyType body_type = b2_staticBody) {
    b2BodyDef body_def;
    body_def.type = body_type;
    body_def.position = sfml_to_box2d(position, 600);

    body = world.CreateBody(&body_def);

    draw_shape = sf_shape;
  }

  virtual ~Box2DBase() {
    world.DestroyBody(body);
    clear_path();
  }

  /**
   * Syncs Box2D body attributes with SFML shape.
   */
  void sync_object(const sf::Window& refer_to) {
    draw_shape->setPosition(
        box2d_to_sfml(body->GetPosition(), refer_to.getSize().y));
    draw_shape->setRotation(sf::radians(-body->GetAngle()));

    m_object_path.emplace_back(1.f, 4);
    m_object_path.back().setPosition(draw_shape->getPosition());
    m_object_path.back().setFillColor(sf::Color(255, 255, 0));

    if (m_object_path.size() > 500) {
      m_object_path.erase(m_object_path.begin());
    }
  }

  void draw(sf::RenderTarget& target,
            sf::RenderStates states = sf::RenderStates::Default) const {
    // Draw object at current position
    target.draw(*draw_shape, states);

    // Draw previous path
    for (unsigned int index = 0; index < m_object_path.size(); index++) {
      target.draw(m_object_path[index]);
    }
  }

  inline static b2World world{{0.f, 0.f}};

  b2Body* body;
  sf::Shape* draw_shape;

 private:
  std::vector<sf::CircleShape> m_object_path;

  /**
   * Removes all dots from path drawing.
   */
  void clear_path() { m_object_path.clear(); }
};
