// Copyright (c) 2018 Tyler Veness. All Rights Reserved.

#ifndef UNIVERSALGRAVITYSIM_SRC_UIFONT_HPP_
#define UNIVERSALGRAVITYSIM_SRC_UIFONT_HPP_

#include <SFML/Graphics/Font.hpp>

class UIFont {
public:
    // @return a global instance of the resources available
    static UIFont* getInstance();

    sf::Font& segoeUI();

protected:
    UIFont();

private:
    static UIFont* m_instance;

    static sf::Font m_segoeUI;

    // disallow copy and assignment
    UIFont(const UIFont&);
    void operator=(const UIFont&);
};

#endif  // UNIVERSALGRAVITYSIM_SRC_UIFONT_HPP_
