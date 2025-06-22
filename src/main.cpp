// Copyright (c) Tyler Veness

#include <format>
#include <vector>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "globals.hpp"
#include "planet.hpp"
#include "ship.hpp"

static constexpr double G = 6.6738480f;

int main() {
  constexpr float FRAME_RATE = 60.f;

  sf::RenderWindow main_window(sf::VideoMode::getDesktopMode(),
                               "Universal Gravitation Simulator",
                               sf::Style::Resize | sf::Style::Close);
  main_window.setMouseCursorVisible(false);
  main_window.setFramerateLimit(FRAME_RATE);

  main_window.setIcon(sf::Image{"resources/GalagaShip.png"});

  sf::Texture background_texture{"resources/SpaceBackground.png"};
  background_texture.setRepeated(true);

  sf::Sprite background_sprite{background_texture};
  background_sprite.setTextureRect(
      sf::IntRect{{-86, -86},
                  {static_cast<int>(main_window.getSize().x + 2 * 86),
                   static_cast<int>(main_window.getSize().y + 2 * 86)}});
  background_sprite.setPosition({0.f, 0.f});

  Ship ship{{400.f, 0.f}, 100.f};

  std::vector<Planet> planets;
  planets.emplace_back(sf::Vector2f{0.f, 0.f}, 140.f / 30.f,
                       sf::Color{0, 128, 0});

  main_window.setView(
      sf::View{sf::FloatRect{{0.f, 0.f}, sf::Vector2f{main_window.getSize()}}});

  bool is_paused = false;

  sf::Text mass_planet{global_font(), "", 16};
  sf::Text mass_ship{global_font(), "", 16};
  sf::Text force{global_font(), "", 16};

  while (main_window.isOpen()) {
    while (auto event = main_window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        main_window.close();
      } else if (event->is<sf::Event::Resized>()) {
        background_sprite.setTextureRect(sf::IntRect{
            {-86, -86},
            {static_cast<int>(main_window.getSize().x + 2.f * 86.f),
             static_cast<int>(main_window.getSize().y + 2.f * 86.f)}});
      } else if (auto key_event = event->getIf<sf::Event::KeyReleased>()) {
        if (key_event->code == sf::Keyboard::Key::Space) {
          is_paused = !is_paused;
        }
      }
    }

    mass_planet.setString(
        std::format("Planet mass = {} kg", planets[0].body->GetMass()));
    mass_planet.setPosition(ship.get_position() + sf::Vector2f{5.f, 50.f});

    mass_ship.setString(std::format("Ship mass = {} kg", ship.body->GetMass()));
    mass_ship.setPosition(ship.get_position() + sf::Vector2f{5.f, 50.f + 18.f});

    // Shouldn't apply universal gravitation on same body
    b2Vec2 delta =
        planets[0].body->GetWorldCenter() - ship.body->GetWorldCenter();
    float r = delta.Length();

    delta.Normalize();
    force.setString(std::format(
        "Force = {} N",
        G * planets[0].body->GetMass() * ship.body->GetMass() / (r * r)));
    force.setPosition(ship.get_position() + sf::Vector2f{5.f, 50.f + 36.f});

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
      main_window.close();
    }

    if (!is_paused) {
      // Instruct the world to perform a single step of simulation.
      // It is generally best to keep the time step and iterations fixed.
      Box2DBase::world.Step(1.f / FRAME_RATE, 1, 1);

      ship.sync_object(main_window);
      for (auto& planet : planets) {
        planet.sync_object(main_window);
      }

      // Applies universal gravitation to all combinations of bodies
      for (b2Body* startBody = Box2DBase::world.GetBodyList();
           startBody != nullptr; startBody = startBody->GetNext()) {
        for (b2Body* moveBody = startBody->GetNext(); moveBody != nullptr;
             moveBody = moveBody->GetNext()) {
          // Shouldn't apply universal gravitation on same body
          if (moveBody != startBody) {
            b2Vec2 delta =
                startBody->GetWorldCenter() - moveBody->GetWorldCenter();
            float r = delta.Length();

            float force =
                G * moveBody->GetMass() * startBody->GetMass() / (r * r);

            delta.Normalize();
            startBody->ApplyForceToCenter(-force * delta, true);
            moveBody->ApplyForceToCenter(force * delta, true);
          }
        }
      }

      ship.control();
    }

    /* ===== Handle background texture shifting with ship ===== */
    // Move background left
    if (main_window.getView().getCenter().x -
            background_sprite.getPosition().x <
        main_window.getSize().x / 2.f + 86.f) {
      background_sprite.setPosition({background_sprite.getPosition().x - 86.f,
                                     background_sprite.getPosition().y});
    }

    // Move background right
    if (main_window.getView().getCenter().x -
            background_sprite.getPosition().x >
        main_window.getSize().x / 2.f + 86.f) {
      background_sprite.setPosition({background_sprite.getPosition().x + 86.f,
                                     background_sprite.getPosition().y});
    }

    // Move background up
    if (main_window.getView().getCenter().y -
            background_sprite.getPosition().y <
        main_window.getSize().y / 2.f + 86.f) {
      background_sprite.setPosition({background_sprite.getPosition().x,
                                     background_sprite.getPosition().y - 86.f});
    }

    // Move background down
    if (main_window.getView().getCenter().y -
            background_sprite.getPosition().y >
        main_window.getSize().y / 2.f + 86.f) {
      background_sprite.setPosition({background_sprite.getPosition().x,
                                     background_sprite.getPosition().y + 86.f});
    }
    /* ======================================================== */

    main_window.clear({10, 10, 10});

    main_window.draw(background_sprite);
    for (auto& planet : planets) {
      planet.draw_on(main_window);
    }
    main_window.draw(ship);

    main_window.draw(mass_planet);
    main_window.draw(mass_ship);
    main_window.draw(force);

    main_window.display();

    main_window.setView(sf::View{
        sf::FloatRect{{ship.get_position().x - main_window.getSize().x / 2.f,
                       ship.get_position().y - main_window.getSize().y / 2.f},
                      sf::Vector2f{main_window.getSize()}}});
  }
}
