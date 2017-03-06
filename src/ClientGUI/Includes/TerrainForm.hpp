#pragma once 

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "UserActions.hpp"
#include "HttpClient.hpp"

class TerrainForm
{
    sf::Event _event;
    sf::Thread _load_terrains_data;
    tgui::Gui _gui;

    HttpClient *_http_client;

    // UI elements
    tgui::Theme::Ptr    _theme;
    tgui::EditBox::Ptr  _terrain_name;
    tgui::ComboBox::Ptr _terrain_type;
    tgui::EditBox::Ptr  _terrain_width;
    tgui::EditBox::Ptr  _terrain_height;
    tgui::TextBox::Ptr  _terrain_description;
    tgui::Label::Ptr    _description_l;
    tgui::Label::Ptr    _width_l;
    tgui::Label::Ptr    _height_l;
    tgui::Label::Ptr    _type_l;
    tgui::Picture::Ptr  _terrain_img;
    sf::Texture         _terrain_picture[6];
    tgui::ListBox::Ptr  _terrain_list;
    tgui::Button::Ptr   _create_btn;
    tgui::Button::Ptr   _back_btn;
    tgui::Button::Ptr   _refresh_btn;
    
    tgui::Picture::Ptr _back;

    std::string _game_session;

    json _terrain_data;
    json _terrain_types;

    bool _updated;
    bool _combo_box;

    void fInitUIElements();
    void fCreateTerrain(tgui::EditBox::Ptr name, tgui::ComboBox::Ptr type, tgui::EditBox::Ptr width, tgui::EditBox::Ptr height, tgui::TextBox::Ptr description);
    void fLoadTerrainListBox();
    void fLoadTerrainTypeList();
    void fLoadData();
    void fLoadTerrainImages();
    void fRefresh();
    void fDisable();
public:
    bool display_window;
    TerrainForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client);
    void fUpdate(sf::RenderWindow  &window);
    void fDraw(sf::RenderWindow & window);
};