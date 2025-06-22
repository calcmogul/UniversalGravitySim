// Copyright (c) Tyler Veness

#include "planet.hpp"

Planet::Planet(const sf::Vector2f& position, float radius,
               const sf::Color& color)
    : Box2DBase(&shape, position, b2_dynamicBody), shape(radius * 30.f) {
  b2CircleShape circle;
  circle.m_p.Set(0.f, 0.f);
  circle.m_radius = radius;

  // Add circle fixture to the ground body.
  body->CreateFixture(&circle, 1.f);

  body->SetAngularVelocity(20.f);

  shape.setFillColor(color);
  shape.setOrigin(shape.getGeometricCenter());
  shape.setPosition(position);

  shader.setUniform("radius", shape.getRadius());
  shader.setUniform("center_color", sf::Glsl::Vec4{shape.getFillColor()});

  m_shader_state.shader = &shader;
}

void Planet::draw_on(sf::RenderTarget& target) {
  shader.setUniform("current_pos",
                    (shape.getPosition() - target.getView().getCenter())
                            .componentWiseMul({1.0, -1.0}) +
                        sf::Vector2f{target.getSize()} / 2.f);

  target.draw(*this, m_shader_state);
}
