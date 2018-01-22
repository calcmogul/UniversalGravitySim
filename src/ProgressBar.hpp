// Copyright (c) 2012-2018 Tyler Veness. All Rights Reserved.

#ifndef UNIVERSALGRAVITYSIM_SRC_PROGRESSBAR_HPP_
#define UNIVERSALGRAVITYSIM_SRC_PROGRESSBAR_HPP_

#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Text.hpp>

#include "UIFont.hpp"

class ProgressBar : public sf::RectangleShape {
   public:
    ProgressBar(const sf::Vector2f& size, std::string message,
                const sf::Color& fullFillColor, const sf::Color& emptyFillColor,
                const sf::Color& outlineColor, float percentFull = 100.f);

    void setPercent(float percentFull);
    float getPercent();

    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);

    void setSize(const sf::Vector2f& size);
    void setSize(float width, float height);

    void setString(const std::string& message);
    const sf::String& getString();

    void setBarFillColor(const sf::Color& fill);
    const sf::Color& getBarFillColor();

    sf::Shader shader;

   protected:
    sf::RenderStates shaderState;

   private:
    sf::RectangleShape barFill;
    float percent;

    sf::Text sfText;

    void draw(sf::RenderTarget& target,
              sf::RenderStates states = sf::RenderStates::Default) const;
};

#endif  // UNIVERSALGRAVITYSIM_SRC_PROGRESSBAR_HPP_
