//=============================================================================
//File Name: Base.h
//Description: Holds declarations for Base class
//Author: Tyler Veness
//=============================================================================

#ifndef GUI_BASE_H_
#define GUI_BASE_H_

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/Network/IpAddress.hpp>

#include "GUI/GUISettings.h"

/* ===== STATIC VAR DECLARATIONS ===== */
extern sf::RenderWindow mainWin;
/* =================================== */

bool keyPressed( sf::Event& event , sf::Keyboard::Key );
bool keyReleased( sf::Event& event , sf::Keyboard::Key );
bool mouseButtonReleased( sf::Event& event , sf::Mouse::Button button );
bool pressedControl( sf::Event& event );
bool pressedShift( sf::Event& event );
bool pressedAlt( sf::Event& event );

#endif /* GUI_BASE_H_ */
