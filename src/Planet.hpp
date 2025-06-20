// Copyright (c) 2012-2025 Tyler Veness. All Rights Reserved.

#ifndef UNIVERSALGRAVITYSIM_SRC_PLANET_HPP_
#define UNIVERSALGRAVITYSIM_SRC_PLANET_HPP_

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "Box2DBase.hpp"

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
    sf::Shader shader;

private:
    Planet(const sf::Vector2f& position, const float& radius,
           const sf::Color& color);

    static std::vector<Planet*> m_planets;

    sf::RenderStates m_shaderState;
};

#endif  // UNIVERSALGRAVITYSIM_SRC_PLANET_HPP_
