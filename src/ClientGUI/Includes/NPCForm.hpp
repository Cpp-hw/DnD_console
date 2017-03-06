#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <TGUI/TGUI.hpp>
#include "UserActions.hpp"
#include "HttpClient.hpp"

class NPCForm
{
    sf::Event _event;
    tgui::Gui _gui;
    sf::Thread _load_data;

    HttpClient *_http_client;

    // UI elements
    tgui::Theme::Ptr    _theme;
    tgui::EditBox::Ptr  _npc_name;
    tgui::EditBox::Ptr  _hitpoints_;
    tgui::EditBox::Ptr  _level_;
    tgui::ComboBox::Ptr _npc_type;
    tgui::ComboBox::Ptr _strength;
    tgui::ComboBox::Ptr _dexterity;
    tgui::ComboBox::Ptr _constitution;
    tgui::ComboBox::Ptr _inteligence;
    tgui::ComboBox::Ptr _wisdom;
    tgui::ComboBox::Ptr _charisma;
    tgui::Label::Ptr    _strength_l;
    tgui::Label::Ptr    _dexterity_l;
    tgui::Label::Ptr    _constitution_l;
    tgui::Label::Ptr    _inteligence_l;
    tgui::Label::Ptr    _wisdom_l;
    tgui::Label::Ptr    _charisma_l;
    tgui::Label::Ptr    _hitpoints;
    tgui::Label::Ptr    _level;
    tgui::Label::Ptr    _npc_type_l;
    tgui::ListBox::Ptr  _npc_list;
    tgui::Button::Ptr   _create_btn;
    tgui::Button::Ptr   _back_btn;
    tgui::Button::Ptr   _refresh_btn;
    tgui::Button::Ptr   _edit_btn;
    tgui::Button::Ptr   _save_chg;
    tgui::Button::Ptr   _delete_btn;
    tgui::Picture::Ptr  _back;
    tgui::Picture::Ptr  _npc_img;
    sf::Texture         _npc_picture[8];

    std::string _data_strength;
    std::string _data_dexterity;
    std::string _data_constitution;
    std::string _data_intelligence;
    std::string _data_wisdom;
    std::string _data_charisma;
    std::string _data_hitpoints;
    std::string _data_level;
    std::string _npc_id;

    std::string _game_session;
    json _npc_data;
    json _npc_types;

    bool _updated;
    bool _combo_box;

    void fInitUIElements();
    void fCreateNPC(tgui::EditBox::Ptr name, tgui::EditBox::Ptr level, tgui::EditBox::Ptr hitpoints, tgui::ComboBox::Ptr type);
    void fLoadNPCListBox();
    void fLoadNPCType();
    void fLoadNPCImages();
    void fLoadData();
    void fEditNPC(tgui::ListBox::Ptr npc_list);
    void fSaveChanges(tgui::ListBox::Ptr npc_list);
    void fRefresh();
    void fDisable();
    void fDeleteNPC(tgui::ListBox::Ptr npc_list);

public:
    bool display_window;
    NPCForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client);
    void fUpdate(sf::RenderWindow  &window);
    void fDraw(sf::RenderWindow & window);
};