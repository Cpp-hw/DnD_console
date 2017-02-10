#pragma once
#include <SFML/Graphics.hpp>
class HandleBoardElems
{
public:
    sf::Vector2i position;
    sf::Vector2i position_on_board;
    sf::Sprite   elem_sprite;
    int          elem_id;
    bool         is_on_board;
    HandleBoardElems();
    HandleBoardElems(const sf::Sprite &sprite);
};

