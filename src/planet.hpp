// Copyright (c) Tyler Veness

#pragma once

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "box2d_base.hpp"

class Ship;

class Planet : public Box2DBase {
 public:
  virtual ~Planet();

  static void drawAll(const Ship& ship, sf::RenderTarget& target,
                      sf::RenderStates states = sf::RenderStates::Default);
  static void syncObjects(const sf::Window& referTo);

  static void add(const sf::Vector2f& position, const float& radius,
                  const sf::Color& color);

  static void applyUnivGravity();
  static float getUnivGravity(b2Body* body1, b2Body* body2);

  static const Planet* getPlanet(size_t index);

  sf::CircleShape shape;
  sf::Shader shader{std::string_view{R"(
#version 330

// radius must be greater than 7

uniform float radius;
uniform vec2 currentPos;
uniform vec4 centerColor;

const float startFade = 7.0;

void main() {
  gl_FragColor = centerColor;

  float distance = distance(gl_FragCoord.xy, currentPos.xy);

  if (distance >= radius) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, gl_FragColor.a);
  } else if (distance > startFade) {
    // if outside of range, start gradienting color to black
    float factor = (1.0 - (distance - startFade) / (radius - startFade));
    gl_FragColor = vec4(factor * vec3(gl_FragColor), gl_FragColor.a);
  }
})"},
                    sf::Shader::Type::Fragment};

 private:
  Planet(const sf::Vector2f& position, const float& radius,
         const sf::Color& color);

  static std::vector<Planet*> m_planets;

  sf::RenderStates m_shaderState;
};
