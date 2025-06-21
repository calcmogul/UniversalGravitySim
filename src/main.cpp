// Copyright (c) Tyler Veness

#include <string>

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "globals.hpp"
#include "planet.hpp"
#include "ship.hpp"

int main() {
  sf::RenderWindow mainWin(sf::VideoMode::getDesktopMode(),
                           "Universal Gravitation Simulator",
                           sf::Style::Resize | sf::Style::Close);
  mainWin.setMouseCursorVisible(false);
  mainWin.setVerticalSyncEnabled(true);

  sf::Image appIcon{"resources/GalagaShip.png"};
  mainWin.setIcon(appIcon);

  sf::Texture backgroundTexture;

  if (!backgroundTexture.loadFromFile("resources/SpaceBackground.png")) {
    std::exit(1);
  }

  backgroundTexture.setRepeated(true);  // Tiling background

  sf::Sprite backgroundSprite(backgroundTexture);
  backgroundSprite.setTextureRect(sf::IntRect(
      {-86, -86}, {static_cast<int>(mainWin.getSize().x + 2 * 86),
                   static_cast<int>(mainWin.getSize().y + 2 * 86)}));
  backgroundSprite.setPosition({0.f, 0.f});

  sf::RectangleShape HUDBackground(sf::Vector2f(mainWin.getSize().x, 45.f));
  HUDBackground.setFillColor(sf::Color(90, 90, 90, 170));

  Ship myShip(sf::Vector2f(400.f, 0.f), 100.f);

  Planet::add(sf::Vector2f(0.f, 0.f), 140.f / 30.f, sf::Color(0, 128, 0));

  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality simulation
  // in most game scenarios.
  float timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 1;  // 6
  int32 positionIterations = 1;  // 2

  mainWin.setView(
      sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f{mainWin.getSize()})));
  // mainWin.setView( sf::View( sf::FloatRect( 0.f - mainWin.getSize().x / 2.f
  // , 0.f - mainWin.getSize().y / 1.f , mainWin.getSize().x ,
  // mainWin.getSize().y ) ) );

  bool isPaused = false;

  sf::Text massPlanet(global_font(), "", 16.f);
  massPlanet.setPosition(sf::Vector2f(5.f, 5.f));

  sf::Text massShip(global_font(), "", 16.f);
  massShip.setPosition(
      sf::Vector2f(5.f, massPlanet.getPosition().y + 16.f + 5.f));

  sf::Text force(global_font(), "", 16.f);
  force.setPosition(sf::Vector2f(5.f, massShip.getPosition().y + 16.f + 5.f));

  while (mainWin.isOpen()) {
    while (auto event = mainWin.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        mainWin.close();
      } else if (event->is<sf::Event::Resized>()) {
        backgroundSprite.setTextureRect(sf::IntRect(
            {-86, -86}, {static_cast<int>(mainWin.getSize().x + 2.f * 86.f),
                         static_cast<int>(mainWin.getSize().y + 2.f * 86.f)}));

        HUDBackground.setScale(
            {mainWin.getSize().x / 50.f, HUDBackground.getScale().y});
      } else if (auto key_event = event->getIf<sf::Event::KeyReleased>()) {
        if (key_event->code == sf::Keyboard::Key::Space) {
          isPaused = !isPaused;
        }
      }
    }

    massPlanet.setString("Planet mass = " +
                         std::to_string(Planet::getPlanet(0)->body->GetMass()) +
                         " kg");
    massShip.setString(
        "Ship mass   = " + std::to_string(myShip.body->GetMass()) + " kg");
    force.setString("Force       = " +
                    std::to_string(Planet::getUnivGravity(
                        Planet::getPlanet(0)->body, myShip.body)) +
                    " N");

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
      mainWin.close();
    }

    if (!isPaused) {
      // Instruct the world to perform a single step of simulation.
      // It is generally best to keep the time step and iterations fixed.
      Box2DBase::world.Step(timeStep, velocityIterations, positionIterations);

      myShip.syncObject(mainWin);
      Planet::syncObjects(mainWin);

      Planet::applyUnivGravity();

      myShip.controlShip();
    }

    /* ===== Handle background texture shifting with ship ===== */
    // Move background left
    if (mainWin.getView().getCenter().x - backgroundSprite.getPosition().x <
        mainWin.getSize().x / 2 + 86.f) {
      backgroundSprite.setPosition({backgroundSprite.getPosition().x - 86.f,
                                    backgroundSprite.getPosition().y});
    }

    // Move background right
    if (mainWin.getView().getCenter().x - backgroundSprite.getPosition().x >
        mainWin.getSize().x / 2 + 86.f) {
      backgroundSprite.setPosition({backgroundSprite.getPosition().x + 86.f,
                                    backgroundSprite.getPosition().y});
    }

    // Move background up
    if (mainWin.getView().getCenter().y - backgroundSprite.getPosition().y <
        mainWin.getSize().y / 2 + 86.f) {
      backgroundSprite.setPosition({backgroundSprite.getPosition().x,
                                    backgroundSprite.getPosition().y - 86.f});
    }

    // Move background down
    if (mainWin.getView().getCenter().y - backgroundSprite.getPosition().y >
        mainWin.getSize().y / 2 + 86.f) {
      backgroundSprite.setPosition({backgroundSprite.getPosition().x,
                                    backgroundSprite.getPosition().y + 86.f});
    }
    /* ======================================================== */

    HUDBackground.setPosition(
        {mainWin.getView().getCenter().x - mainWin.getSize().x / 2.f,
         mainWin.getView().getCenter().y + mainWin.getSize().y / 2.f -
             HUDBackground.getSize().y});

    mainWin.clear(sf::Color(10, 10, 10));

    mainWin.draw(backgroundSprite);
    Planet::drawAll(myShip, mainWin);
    mainWin.draw(myShip);

    mainWin.draw(HUDBackground);

    mainWin.draw(massPlanet);
    mainWin.draw(massShip);
    mainWin.draw(force);

    mainWin.display();

    mainWin.setView(sf::View(
        sf::FloatRect({myShip.shape.getPosition().x - mainWin.getSize().x / 2,
                       myShip.shape.getPosition().y - mainWin.getSize().y / 2},
                      sf::Vector2f{mainWin.getSize()})));
  }
}
