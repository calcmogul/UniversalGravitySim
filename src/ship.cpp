// Copyright (c) Tyler Veness

#include "ship.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

static sf::Texture& get_ship_texture() {
  static sf::Texture texture{"resources/GalagaShip.png"};
  return texture;
}

Ship::Ship(const sf::Vector2f& position, float health)
    : Box2DBase(&shape, position, b2_dynamicBody), health{health} {
  b2PolygonShape ship_triangle;

  auto& texture = get_ship_texture();

  // all shifted left by 15 pixels and down by 20 pixels
  b2Vec2 ship_vertices[6];
  ship_vertices[0].Set(-1.f / 30.f, (texture.getSize().y - 20.f) / 30.f);
  ship_vertices[1].Set(-15.f / 30.f, (texture.getSize().y - 36.f) / 30.f);
  ship_vertices[2].Set(-15.f / 30.f, (texture.getSize().y - 51.f) / 30.f);
  ship_vertices[3].Set(14.f / 30.f, (texture.getSize().y - 51.f) / 30.f);
  ship_vertices[4].Set(14.f / 30.f, (texture.getSize().y - 36.f) / 30.f);
  ship_vertices[5].Set(0.f / 30.f, (texture.getSize().y - 20.f) / 30.f);

  ship_triangle.Set(ship_vertices, 6);

  // Add the shape to the body
  body->CreateFixture(&ship_triangle, 1.f);

  // Create SFML shape
  for (size_t i = 0; i < 6; ++i) {
    shape.setPoint(i, {ship_vertices[i].x * 30.f,
                       texture.getSize().y - ship_vertices[i].y * 30.f});
  }
  shape.setOrigin({0.f, static_cast<float>(texture.getSize().y)});
  shape.setTexture(&texture);
}

void Ship::control() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    body->SetAngularVelocity(body->GetAngularVelocity() + 0.1f);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    body->SetAngularVelocity(body->GetAngularVelocity() - 0.1f);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    body->ApplyForceToCenter({7.5f * std::cos(body->GetAngle() + b2_pi / 2.f),
                              7.5f * std::sin(body->GetAngle() + b2_pi / 2.f)},
                             true);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    body->ApplyForceToCenter({-7.5f * std::cos(body->GetAngle() + b2_pi / 2.f),
                              -7.5f * std::sin(body->GetAngle() + b2_pi / 2.f)},
                             true);
  }
}
