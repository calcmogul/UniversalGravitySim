// Copyright (c) 2016-2018 Tyler Veness. All Rights Reserved.

#include "Planet.hpp"

#include <cmath>

#include "Ship.hpp"

std::vector<Planet*> Planet::m_planets;

static constexpr double kG = 6.6738480f;

Planet::~Planet() {
    m_planets.erase(std::remove(m_planets.begin(), m_planets.end(), this),
                    m_planets.end());
}

void Planet::drawAll(const Ship& ship, sf::RenderTarget& target,
                     sf::RenderStates states) {
    for (unsigned int index = 0; index < m_planets.size(); index++) {
        // Realign shading with position of given ship's current position
        sf::Glsl::Vec2 vec;
        vec.x = m_planets[index]->shape.getPosition().x -
                target.getView().getCenter().x + target.getSize().x / 2.f;
        vec.y = target.getView().getCenter().y -
                m_planets[index]->shape.getPosition().y +
                target.getSize().y / 2.f + 2.f /* offset for shading */;
        m_planets[index]->shader.setUniform("currentPos", vec);

        // Redraw planet
        target.draw(*m_planets[index], m_planets[index]->m_shaderState);
    }
}

void Planet::syncObjects(const sf::Window& referTo) {
    for (unsigned int index = 0; index < m_planets.size(); index++) {
        m_planets[index]->syncObject(referTo);
    }
}

void Planet::add(const sf::Vector2f& position, const float32& radius,
                 const sf::Color& color) {
    m_planets.push_back(new Planet(position, radius, color));
}

void Planet::applyUnivGravity() {
    // Applies universal gravitation to all combinations of bodies
    for (b2Body* startBody = Box2DBase::world.GetBodyList(); startBody != NULL;
         startBody = startBody->GetNext()) {
        for (b2Body* moveBody = startBody->GetNext(); moveBody != NULL;
             moveBody = moveBody->GetNext()) {
            if (moveBody != startBody) {  // shouldn't apply universal
                                          // gravitation on same body
                b2Vec2 delta =
                    startBody->GetWorldCenter() - moveBody->GetWorldCenter();
                float r = delta.Length();

                float force =
                    kG * moveBody->GetMass() * startBody->GetMass() / (r * r);

                delta.Normalize();
                startBody->ApplyForceToCenter(-force * delta, true);
                moveBody->ApplyForceToCenter(force * delta, true);
            }
        }
    }
}

float Planet::getUnivGravity(b2Body* body1, b2Body* body2) {
    if (body1 != body2) {  // shouldn't apply universal gravitation on same body
        b2Vec2 delta = body1->GetWorldCenter() - body2->GetWorldCenter();
        float r = delta.Length();

        float force = kG * body1->GetMass() * body2->GetMass() / (r * r);

        delta.Normalize();

        return std::hypot((force * delta).x, (force * delta).y);
    } else {
        return 0;
    }
}

const Planet* Planet::getPlanet(size_t index) { return m_planets[index]; }

Planet::Planet(const sf::Vector2f& position, const float32& radius,
               const sf::Color& color)
    : Box2DBase(&shape, position, b2_dynamicBody), shape(radius * 30.f) {
    b2CircleShape earthCircle;

    earthCircle.m_p.Set(0.f, 0.f);
    earthCircle.m_radius = radius;

    // Add the Earth fixture to the ground body.
    body->CreateFixture(&earthCircle, 1.f);

    body->SetAngularVelocity(20.f);

    // Create SFML shape
    shape.setFillColor(color);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);

    // Create the shader
    if (!shader.loadFromFile("Resources/circleShade.frag",
                             sf::Shader::Fragment)) {
        std::exit(1);
    }
    shader.setUniform("radius", shape.getRadius());
    shader.setUniform("startFade", 7.f);
    sf::Glsl::Vec4 fillColor{shape.getFillColor()};
    shader.setUniform("centerColor", fillColor);

    m_shaderState.shader = &shader;
}
