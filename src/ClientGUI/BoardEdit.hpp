#pragma once
#include <memory>
#include "SFML\Graphics.hpp"
#include "HandleBoardElems.hpp"
#include "json.hpp"

using json = nlohmann::json;

class BoardEdit
{
private:
    //sfml event
    sf::Event _event;

    //board size
    int _height, _width;
    float _cell_size;

    // buttons textures & sprites
    sf::Texture _submit_button;
    sf::Sprite  _submit_sprite;

    //board ui
    sf::Texture _board_elems_texture;
    sf::Sprite  _board_elems_sprite;

    //board texture & sprite
    sf::Texture _board_texture;
    sf::Sprite  _board_sprite;

    //npc & terrain textures
    std::vector<std::shared_ptr<sf::Texture>> _npc_textures;
    std::vector<std::shared_ptr<sf::Texture>> _terrain_textures;

    //npc & terrain sprites in menu
    std::vector<sf::Sprite>  _npc_sprites;
    std::vector<sf::Sprite>  _terrain_sprites;

    // npc & terrains on board
    int _elems_unique_id_on_board;
    int _selected_elem_on_board;
    json _elems_json;
    std::map<int, HandleBoardElems> _elems_on_board;

    // mouse position and state handlers
    int mouseX;
    int mouseY;
    bool mouseInsideRect;
    bool dragging;
    sf::Vector2f mouseRectOffset;
    //

    // NPC & Terrain Loader
    void fLoadNpcs(sf::RenderWindow &window);

public:
    bool draw_window;

    BoardEdit(const int &height, const int &width, const sf::Event &event, sf::RenderWindow &window);
    void fUpdate(sf::RenderWindow &window);
    void fDraw(sf::RenderWindow &window);

    //// Convert npcs and terrains on board to json
    //json fToJson();
};

