//=============================================================================
//File Name: Base.cpp
//Description: Holds definitions for Base class
//Author: Tyler Veness
//=============================================================================

#include "Base.h"
#include <sstream>
#include <fstream>

bool keyPressed( sf::Event& event , sf::Keyboard::Key key ) {
	return ( event.type == sf::Event::KeyPressed && event.key.code == key );
}

bool keyReleased( sf::Event& event , sf::Keyboard::Key key ) {
	return ( event.type == sf::Event::KeyReleased && event.key.code == key ); // true if key was released and it was the one asked for
}

bool mouseButtonReleased( sf::Event& event , sf::Mouse::Button button ) {
	return ( event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == button ); // true if mouse button was released and it was the one asked for
}

bool pressedControl( sf::Event& event ) {
	return ( keyPressed( event , sf::Keyboard::LControl ) || keyPressed( event , sf::Keyboard::RControl ) );
}

bool pressedShift( sf::Event& event ) {
	return ( keyPressed( event , sf::Keyboard::LShift ) || keyPressed( event , sf::Keyboard::RShift ) );
}

bool pressedAlt( sf::Event& event ) {
	return ( keyPressed( event , sf::Keyboard::LAlt ) || keyPressed( event , sf::Keyboard::RAlt ) );
}
