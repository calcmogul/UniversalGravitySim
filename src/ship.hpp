// Copyright (c) Tyler Veness

#pragma once

#include <SFML/Graphics/ConvexShape.hpp>

#include "box2d_base.hpp"

class Ship : public Box2DBase {
 public:
  Ship(const sf::Vector2f& position, float health);

  virtual ~Ship() = default;

  void control();

  sf::Vector2f get_position() const { return shape.getPosition(); }

  float get_health() const { return health; }

 private:
  static constexpr float MAX_SPEED = 10.f;

  float health;
  sf::ConvexShape shape{6};
};
