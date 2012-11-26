//=============================================================================
//File Name: Clickable.cpp
//Description: Deriving from this class gives derived class clickable nature
//Author: Tyler Veness
//=============================================================================

#include "Clickable.h"
#include <SFML/Window/Mouse.hpp>

Clickable::Clickable( const sf::Vector2f& size , std::vector<sf::Color> borders ) : TaperRectangleShape( size , borders ) {
	m_visible = false;
}

Clickable::~Clickable() {

}

bool Clickable::isHovered( sf::Window& referTo ) {
	if ( m_visible && sf::Mouse::getPosition( referTo ).x >= getPosition().x && sf::Mouse::getPosition( referTo ).x <= getPosition().x + getSize().x && sf::Mouse::getPosition( referTo ).y >= getPosition().y && sf::Mouse::getPosition( referTo ).y <= getPosition().y + getSize().y )
		return true;
	else
		return false;
}

bool Clickable::isClicked( sf::Window& referTo ) {
	return isHovered( referTo ) && sf::Mouse::isButtonPressed( sf::Mouse::Left );
}

void Clickable::setVisible( bool see ) {
	if ( m_visible != see )
		m_visible = see;
}

bool Clickable::isVisible() {
	return m_visible;
}
