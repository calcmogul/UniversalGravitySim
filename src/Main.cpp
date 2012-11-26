//=============================================================================
//File Name: Main.cpp
//Description: Runs main game
//Author: Tyler Veness
//=============================================================================

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Sleep.hpp>
#include <cmath>

#include "Ship.hpp"
#include "Planet.hpp"
#include "ProgressBar.hpp"
#include "GUI/Button.h"
#include "GUI/GUISettings.h"

int main() {
    sf::RenderWindow mainWin( sf::VideoMode::getDesktopMode()  , "Universal Gravitation Simulator" , sf::Style::Fullscreen | sf::Style::Resize | sf::Style::Close );
    mainWin.setMouseCursorVisible( false );
    mainWin.setVerticalSyncEnabled( true );

    sf::Image appIcon;
    if ( !appIcon.loadFromFile( "Resources/GalagaShip.png" ) ) {
        exit( 1 );
    }
    mainWin.setIcon( appIcon.getSize().x , appIcon.getSize().y , appIcon.getPixelsPtr() );

    sf::Texture backgroundTexture;

    if ( !backgroundTexture.loadFromFile( "Resources/SpaceBackground.png" ) ) {
        exit( 1 );
    }

    backgroundTexture.setRepeated( true ); // Tiling background

    sf::Sprite backgroundSprite( backgroundTexture );
    backgroundSprite.setTextureRect( sf::IntRect( -86 , -86 , mainWin.getSize().x + 2 * 86 , mainWin.getSize().y + 2 * 86 ) );
    backgroundSprite.setPosition( 0.f , 0.f );

    sf::RectangleShape HUDBackground( sf::Vector2f( mainWin.getSize().x , 45.f ) );
    HUDBackground.setFillColor( sf::Color( 90 , 90 , 90 , 170 ) );

    Button resetButton( "Reset" , "Ctrl + N" , "" , -1 , 40 , []{

    } );

    Ship myShip( sf::Vector2f( 400.f , 0.f ) , 100.f );

    Planet::add( sf::Vector2f( 0.f , 0.f ) , 140.f / 30.f , sf::Color( 0 , 128 , 0 ) );

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 1; //6
    int32 positionIterations = 1; //2

    mainWin.setView( sf::View( sf::FloatRect( 0.f , 0.f , mainWin.getSize().x , mainWin.getSize().y ) ) );
    //mainWin.setView( sf::View( sf::FloatRect( 0.f - mainWin.getSize().x / 2.f , 0.f - mainWin.getSize().y / 1.f , mainWin.getSize().x , mainWin.getSize().y ) ) );

    sf::Event event;

    bool isPaused = false;

    sf::View myView( mainWin.getView() );

    while ( mainWin.isOpen() ) {
        while ( mainWin.pollEvent( event ) ) {
            if ( event.type == sf::Event::Closed ) {
                mainWin.close();
            }
            else if ( event.type == sf::Event::Resized ) {
                backgroundSprite.setTextureRect( sf::IntRect( -86.f , -86.f , mainWin.getSize().x + 2.f * 86.f , mainWin.getSize().y + 2.f * 86.f ) );

                HUDBackground.setScale( mainWin.getSize().x / 50.f , HUDBackground.getScale().y );
            }

            else if ( event.type == sf::Event::KeyReleased ) {
                if ( event.key.code == sf::Keyboard::Space ) {
                    isPaused = !isPaused;
                }
            }
        }

        // Handles all hotkeys that are assigned to buttons
        for ( unsigned int index = 0 ; index < Button::allButtons.size() ; index++ ) {
            if ( Button::allButtons[index]->hotKeyActivated( event ) ) {
                if ( Button::allButtons[index]->func != NULL )
                    Button::allButtons[index]->func();
            }
        }

        // Change colors of all buttons when necessary
        for ( unsigned int index = 0 ; index < Button::allButtons.size() ; index++ ) {
            // If button color is wrong for current hover state, switch it to other color
            if ( Button::allButtons[index]->isHovered( mainWin ) ^ ( Button::allButtons[index]->getFillColor().r == BUTTON_HOVER ) ) {
                // "r" chosen is arbitrary since r, g, and b components are all equal
                Button::allButtons[index]->setFillColor( Button::allButtons[index]->getFillColor().r ^ Button::colorXOR );
            }
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) ) {
            mainWin.close();
        }

        if ( !isPaused ) {
            // Instruct the world to perform a single step of simulation.
            // It is generally best to keep the time step and iterations fixed.
            Box2DBase::world.Step( timeStep , velocityIterations , positionIterations );

            myShip.syncObject( mainWin );
            Planet::syncObjects( mainWin );

            Planet::applyUnivGravity();
        }

        /* ===== Move view around according to arrow keys ===== */
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ) {
            myView.move( -5.f , 0.f );
            mainWin.setView( myView );
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ) {
            myView.move( 5.f , 0.f );
            mainWin.setView( myView );
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ) {
            myView.move( 0.f , -5.f );
            mainWin.setView( myView );
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ) {
            myView.move( 0.f , 5.f );
            mainWin.setView( myView );
        }
        /* ==================================================== */

        /* ===== Handle background texture shifting with ship ===== */
        // Move background left
        if ( mainWin.getView().getCenter().x - backgroundSprite.getPosition().x < mainWin.getSize().x / 2 + 86.f ) {
            backgroundSprite.setPosition( backgroundSprite.getPosition().x - 86.f , backgroundSprite.getPosition().y );
        }

        // Move background right
        if ( mainWin.getView().getCenter().x - backgroundSprite.getPosition().x > mainWin.getSize().x / 2 + 86.f ) {
            backgroundSprite.setPosition( backgroundSprite.getPosition().x + 86.f , backgroundSprite.getPosition().y );
        }

        // Move background up
        if ( mainWin.getView().getCenter().y - backgroundSprite.getPosition().y < mainWin.getSize().y / 2 + 86.f ) {
            backgroundSprite.setPosition( backgroundSprite.getPosition().x , backgroundSprite.getPosition().y - 86.f );
        }

        // Move background down
        if ( mainWin.getView().getCenter().y - backgroundSprite.getPosition().y > mainWin.getSize().y / 2 + 86.f ) {
            backgroundSprite.setPosition( backgroundSprite.getPosition().x , backgroundSprite.getPosition().y + 86.f );
        }
        /* ======================================================== */

        HUDBackground.setPosition( mainWin.getView().getCenter().x - mainWin.getSize().x / 2.f , mainWin.getView().getCenter().y + mainWin.getSize().y / 2.f - HUDBackground.getSize().y );

        mainWin.clear( sf::Color( 10 , 10 , 10 ) );

        mainWin.draw( backgroundSprite );
        Planet::drawAll( myShip , mainWin );
        mainWin.draw( myShip );

        mainWin.draw( HUDBackground );

        mainWin.display();

        //mainWin.setView( sf::View( sf::FloatRect( myShip.shape.getPosition().x - mainWin.getSize().x / 2 , myShip.shape.getPosition().y - mainWin.getSize().y / 2 , mainWin.getSize().x , mainWin.getSize().y ) ) );
    }

    Planet::cleanup();

    return 0;
}
