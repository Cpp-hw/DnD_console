#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <TGUI/TGUI.hpp>
#include "UserActions.hpp"
#include "HttpClient.hpp"

class CharacterForm
{
    sf::Event _event;
    tgui::Gui _gui;
    HttpClient* _http_client;
    sf::Thread _http_thread;

    //UI elements 
    tgui::Theme::Ptr    _theme;
    tgui::EditBox::Ptr  _character_name;
    tgui::ComboBox::Ptr _character_race_cbox;
    tgui::ComboBox::Ptr _character_class_cbox;
    tgui::ComboBox::Ptr _strength;
    tgui::ComboBox::Ptr _dexterity;
    tgui::ComboBox::Ptr _constitution;
    tgui::ComboBox::Ptr _inteligence;
    tgui::ComboBox::Ptr _wisdom;
    tgui::ComboBox::Ptr _charisma;
    tgui::EditBox::Ptr  _strength_mod;
    tgui::EditBox::Ptr  _dexterity_mod;
    tgui::EditBox::Ptr  _constitution_mod;
    tgui::EditBox::Ptr  _inteligence_mod;
    tgui::EditBox::Ptr  _wisdom_mod;
    tgui::EditBox::Ptr  _charisma_mod;
    tgui::Label::Ptr    _strength_l;
    tgui::Label::Ptr    _dexterity_l;
    tgui::Label::Ptr    _constitution_l;
    tgui::Label::Ptr    _inteligence_l;
    tgui::Label::Ptr    _wisdom_l;
    tgui::Label::Ptr    _charisma_l;
    tgui::Label::Ptr    _race_l;
    tgui::Label::Ptr    _class_l;
    tgui::Label::Ptr    _hitpoints;
    tgui::Label::Ptr    _level;
    tgui::Label::Ptr    _experience;
    tgui::ListBox::Ptr  _character_list;
    tgui::Button::Ptr   _create_btn;
    tgui::Button::Ptr   _back_btn;
    tgui::Button::Ptr   _save_chg;
    tgui::Button::Ptr   _refresh_btn;
    tgui::Button::Ptr   _delete_btn;
    tgui::Button::Ptr   _edit_btn;
    tgui::Picture::Ptr  _back;

    std::string _data_race;
    std::string _data_class;
    std::string _data_strength;
    std::string _data_dexterity;
    std::string _data_constitution;
    std::string _data_intelligence;
    std::string _data_wisdom;
    std::string _data_charisma;
    std::string _data_hitpoints;
    std::string _data_level;
    std::string _data_experience;
    std::string _character_id;

    std::string _game_session;
    json _character_data;
    json _character_races;
    json _character_classes;

    bool _updated;
    bool _combo_box;
    bool _create_button_click;
    bool _save_changes_click;
    bool _edit_btn_click;
    bool _is_loaded;

    std::string fSetAbilityMod(std::string ability);
    void fInitUIElements();
    void fCreateCharacter(tgui::EditBox::Ptr name);
    void fDeleteCharacter(tgui::ListBox::Ptr character_list);
    void fEditCharacter(tgui::ListBox::Ptr character_list);
    void fLoadCharacterListBox();
    void fLoadClasses();
    void fLoadRaces();
    void fRefresh();
    void fDisable();
    void fSaveChanges(tgui::ListBox::Ptr character_list);
public:
    bool display_window;
    CharacterForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client);
    void fUpdate(sf::RenderWindow  &window);
    void fDraw(sf::RenderWindow & window);
};
