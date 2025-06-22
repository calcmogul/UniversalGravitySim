// Copyright (c) Tyler Veness

#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "box2d_base.hpp"

class Planet : public Box2DBase {
 public:
  Planet(const sf::Vector2f& position, float radius, const sf::Color& color);

  virtual ~Planet() = default;

  Planet(Planet&&) = default;
  Planet& operator=(Planet&&) = default;

  void draw_on(sf::RenderTarget& target);

  sf::CircleShape shape;
  sf::Shader shader{std::string_view{R"(
#version 330

// radius must be greater than 7

uniform float radius;
uniform vec2 current_pos;
uniform vec4 center_color;

const float START_FADE = 7.0;

void main() {
  gl_FragColor = center_color;

  float distance = distance(gl_FragCoord.xy, current_pos.xy);

  if (distance >= radius) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, gl_FragColor.a);
  } else if (distance > START_FADE) {
    // if outside of range, start gradienting color to black
    float factor = (1.0 - (distance - START_FADE) / (radius - START_FADE));
    gl_FragColor = vec4(factor * vec3(gl_FragColor), gl_FragColor.a);
  }
})"},
                    sf::Shader::Type::Fragment};

 private:
  sf::RenderStates m_shader_state;
};
