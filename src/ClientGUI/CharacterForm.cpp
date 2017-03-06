#include "Includes/CharacterForm.hpp"
#include "Includes/IniParser.hpp"

void CharacterForm::fInitUIElements()
{
    _theme = std::make_shared<tgui::Theme>("Interface/Game.txt");

    auto windowWidth = tgui::bindWidth(_gui);
    auto windowHeight = tgui::bindHeight(_gui);

    _theme = std::make_shared<tgui::Theme>("Interface/Game.txt");
    _back = std::make_shared<tgui::Picture>("Interface/Back.png");
    _back->setSize(tgui::bindMax(1280, windowWidth), tgui::bindMax(800, windowHeight));
    _gui.add(_back);

    tgui::Picture::Ptr border = std::make_shared<tgui::Picture>("Interface/WindowBorder.png");
    border->setSize(windowWidth, windowHeight);
    _gui.add(border);

    _character_name = _theme->load("EditBox");
    _character_name->setPosition(windowWidth/2+50, 105);
    _character_name->setSize(180, 40);
    _character_name->setDefaultText("Name");
    _character_name->setInputValidator("[0-9]{1,5}");
    _gui.add(_character_name);

    _class_l = _theme->load("Label");
    _class_l->setText("Class: ");
    _class_l->setSize(60, 40);
    _class_l->setPosition(windowWidth/2+440,95);
    _gui.add(_class_l);

    _character_class_cbox = _theme->load("ComboBox");
    _character_class_cbox->setPosition(windowWidth/2+500, 85);
    _character_class_cbox->setSize(150, 40);
    _gui.add(_character_class_cbox);

    _race_l = _theme->load("Label");
    _race_l->setText("Race: ");
    _race_l->setSize(60, 40);
    _race_l->setPosition(windowWidth / 2 + 440, 155);
    _gui.add(_race_l);

    _character_race_cbox = _theme->load("ComboBox");
    _character_race_cbox->setPosition(windowWidth/2+500, 145);
    _character_race_cbox->setSize(150, 40);
    _gui.add(_character_race_cbox);

    _data_level = "1";
    _data_experience = "0";
    _data_hitpoints = "100";
    _hitpoints = _theme->load("Label");
    _hitpoints->setPosition(windowWidth / 2 + 440, 225);
    _hitpoints->setText("HitPoints: " + _data_hitpoints);
    _hitpoints->setTextColor("red");
    _gui.add(_hitpoints);

    _experience = _theme->load("Label");
    _experience->setPosition(windowWidth / 2 + 440, 285);
    _experience->setText("Experience: " + _data_experience);
    _experience->setTextColor("blue");
    _gui.add(_experience);

    _level = _theme->load("Label");
    _level->setPosition(windowWidth / 2 + 440, 345);
    _level->setText("Level: " + _data_level);
    _level->setTextColor("yellow");
    _gui.add(_level);

    _strength_l = _theme->load("Label");
    _strength_l->setPosition(windowWidth/2+50, 235);
    _strength_l->setText("Strength");
    _gui.add(_strength_l);

    _dexterity_l = _theme->load("Label");
    _dexterity_l->setPosition(windowWidth / 2 + 50, _strength_l->getSize().y+275);
    _dexterity_l->setText("Dexterity");
    _gui.add(_dexterity_l);

    _constitution_l = _theme->load("Label");
    _constitution_l->setPosition(windowWidth / 2 + 50, _dexterity_l->getSize().y + 335);
    _constitution_l->setText("Constitution");
    _gui.add(_constitution_l);

    _inteligence_l = _theme->load("Label");
    _inteligence_l->setPosition(windowWidth / 2 + 50, _constitution_l->getSize().y+395);
    _inteligence_l->setText("Intelligence");
    _gui.add(_inteligence_l);

    _wisdom_l = _theme->load("Label");
    _wisdom_l->setPosition(windowWidth / 2 + 50, _inteligence_l->getSize().y+455);
    _wisdom_l->setText("Wisdom");
    _gui.add(_wisdom_l);

    _charisma_l = _theme->load("Label");
    _charisma_l->setPosition(windowWidth / 2 + 50, _wisdom_l->getSize().y+515);
    _charisma_l->setText("Charisma");
    _gui.add(_charisma_l);

    _strength = _theme->load("ComboBox");
    _strength->setSize(80, 40);
    _strength->setPosition(windowWidth / 2 + 200, 225);
    _gui.add(_strength);
    
    _dexterity = _theme->load("ComboBox");
    _dexterity->setSize(80, 40);
    _dexterity->setPosition(windowWidth / 2 + 200, 285);
    _gui.add(_dexterity);
    
    _constitution = _theme->load("ComboBox");
    _constitution->setSize(80, 40);
    _constitution->setPosition(windowWidth / 2 + 200, 345);
    _gui.add(_constitution);
    
    _inteligence = _theme->load("ComboBox");
    _inteligence->setSize(80, 40);
    _inteligence->setPosition(windowWidth / 2 + 200, 405);
    _gui.add(_inteligence);
    
    _wisdom = _theme->load("ComboBox");
    _wisdom->setSize(80, 40);
    _wisdom->setPosition(windowWidth / 2 + 200, 465);
    _gui.add(_wisdom);
    
    _charisma = _theme->load("ComboBox");
    _charisma->setSize(80, 40);
    _charisma->setPosition(windowWidth / 2 + 200, 525);
    _gui.add(_charisma);

    tgui::Label::Ptr _mods_l = _theme->load("Label");
    _mods_l->setPosition(windowWidth / 2 + 300, 185);
    _mods_l->setText("Mods");
    _gui.add(_mods_l);

    _strength_mod = _theme->load("EditBox");
    _strength_mod->setSize(80, 40);
    _strength_mod->setPosition(windowWidth / 2 + 300, 225);
    _strength_mod->disable();
    _gui.add(_strength_mod);

    _dexterity_mod = _theme->load("EditBox");
    _dexterity_mod->setSize(80, 40);
    _dexterity_mod->setPosition(windowWidth / 2 + 300, 285);
    _dexterity_mod->disable();
    _gui.add(_dexterity_mod);

    _constitution_mod = _theme->load("EditBox");
    _constitution_mod->setSize(80, 40);
    _constitution_mod->setPosition(windowWidth / 2 + 300, 345);
    _constitution_mod->disable();
    _gui.add(_constitution_mod);

    _inteligence_mod = _theme->load("EditBox");
    _inteligence_mod->setSize(80, 40);
    _inteligence_mod->setPosition(windowWidth / 2 + 300, 405);
    _inteligence_mod->disable();
    _gui.add(_inteligence_mod);

    _wisdom_mod = _theme->load("EditBox");
    _wisdom_mod->setSize(80, 40);
    _wisdom_mod->setPosition(windowWidth / 2 + 300, 465);
    _wisdom_mod->disable();
    _gui.add(_wisdom_mod);

    _charisma_mod = _theme->load("EditBox");
    _charisma_mod->setSize(80, 40);
    _charisma_mod->setPosition(windowWidth / 2 + 300, 525);
    _charisma_mod->disable();
    _gui.add(_charisma_mod);

    _create_btn = _theme->load("Button");
    _create_btn->setSize(160, 50);
    _create_btn->setPosition(windowWidth / 2 + 490, _wisdom_l->getSize().y + 575);
    _create_btn->setText("Create Character");
    _gui.add(_create_btn);

    _save_chg = _theme->load("Button");
    _save_chg->setSize(160, 50);
    _save_chg->setPosition(windowWidth / 2 + 490, _wisdom_l->getSize().y + 575);
    _save_chg->setText("Save Changes");
    _save_chg->hide();
    _gui.add(_save_chg);

    _back_btn = _theme->load("Button");
    _back_btn->setSize(160, 50);
    _back_btn->setPosition(50, windowHeight-150);
    _back_btn->setText("Back");
    _gui.add(_back_btn);

    _refresh_btn = _theme->load("Button");
    _refresh_btn->setSize(400, 50);
    _refresh_btn->setPosition(50, 375);
    _refresh_btn->setText("Refresh Character List");
    _gui.add(_refresh_btn);

    _delete_btn = _theme->load("Button");
    _delete_btn->setSize(400, 50);
    _delete_btn->setPosition(50, 445);
    _delete_btn->setText("Delete Character");
    _gui.add(_delete_btn);

    _edit_btn = _theme->load("Button");
    _edit_btn->setSize(400, 50);
    _edit_btn->setPosition(50, 515);
    _edit_btn->setText("Edit Character");
    _gui.add(_edit_btn);

    _character_list = _theme->load("ListBox");
    _character_list->setPosition(50, 85);
    _character_list->setSize(400, 240);
    _gui.add(_character_list);

    for (int i = 1; i <= 20; ++i)
    {
        _strength->addItem(std::to_string(i));
        _dexterity->addItem(std::to_string(i));
        _constitution->addItem(std::to_string(i));
        _inteligence->addItem(std::to_string(i));
        _charisma->addItem(std::to_string(i));
        _wisdom->addItem(std::to_string(i));
    }

    _combo_box = true;

    _create_btn->connect("pressed", &CharacterForm::fCreateCharacter, this, _character_name);
    _refresh_btn->connect("pressed", &CharacterForm::fRefresh, this);
    _save_chg->connect("pressed", &CharacterForm::fSaveChanges, this, _character_list);
    _delete_btn->connect("pressed", &CharacterForm::fDeleteCharacter, this, _character_list);
    _edit_btn->connect("pressed", &CharacterForm::fEditCharacter, this, _character_list);
    _back_btn->connect("pressed", &CharacterForm::fDisable, this);
}

std::string CharacterForm::fSetAbilityMod(std::string ability)
{
    int ability_ = std::stoi(ability);

    if (ability_ >= 4 && ability_ <= 6)
    {
        return "-2";
    }
    else if (ability_ >= 7 && ability_ <= 9)
    {
        return "-1";
    }
    else if (ability_ == 10)
    {
        return "0";
    }
    else if (ability_ >= 11 && ability_ <= 13)
    {
        return "1";
    }
    else if (ability_ >= 14 && ability_ <= 16)
    {
        return "2";
    }
    else if (ability_ >= 17 && ability_ <= 19)
    {
        return "3";
    }
    else if (ability_ == 20)
    {
        return "5";
    }
    else
    {
        return "-3";
    }
}

void CharacterForm::fCreateCharacter(tgui::EditBox::Ptr name)
{
    Character character(name->getText().toAnsiString(), _data_race, _data_class,_data_experience, _data_hitpoints, _data_level, _data_strength, _data_dexterity, _data_constitution, _data_intelligence, _data_wisdom, _data_charisma, _game_session);

    std::string response;
    std::string request = character.fToJson().dump();

    _http_client->fSendRequest(HttpClient::_POST, "/api/addcharacter", request);
    _http_client->fGetResponse(response);
    std::cout << response << std::endl;
    _combo_box = false;
}

void CharacterForm::fDeleteCharacter(tgui::ListBox::Ptr character_list)
{
    auto request = UserActions::fDeleteCharacter(_game_session, character_list->getSelectedItemId().toAnsiString()).dump();
    std::string response;
    _http_client->fSendRequest(HttpClient::_POST, "/api/deletecharacter", request);
    _http_client->fGetResponse(response);
    std::cout << response;
 
    if (response.find("success"))
    {
        fRefresh();
    }
}

void CharacterForm::fEditCharacter(tgui::ListBox::Ptr character_list)
{
    _create_btn->hide();
    _save_chg->show();

    json request;
    request["session_id"] = _game_session;
    request["character"] = character_list->getSelectedItem().toAnsiString();
    
    std::string response;
   
    _http_client->fSendRequest(HttpClient::_POST, "/api/loaddefinedcharacter", request.dump());
    _http_client->fGetResponse(response);
    std::cout << response;

    json temp_character;
   
    temp_character = json::parse(response.c_str());
   
    _character_name->setText(temp_character["character"]);
    _character_race_cbox->setSelectedItem(temp_character["race"]);
    _character_class_cbox->setSelectedItem(temp_character["class"]);
    _strength->setSelectedItem(temp_character["strength"]);
    _dexterity->setSelectedItem(temp_character["dexterity"]);
    _constitution->setSelectedItem(temp_character["constitution"]);
    _inteligence->setSelectedItem(temp_character["intelligence"]);
    _wisdom->setSelectedItem(temp_character["wisdom"]);
    _charisma->setSelectedItem(temp_character["charisma"]);

}

void CharacterForm::fLoadCharacterListBox()
{
   std::string response;
   auto request = UserActions::fLoadMyCharacters(_game_session).dump();
   _http_client->fSendRequest(HttpClient::_POST, "/api/loadmycharacterslist", request);
   _http_client->fGetResponse(response);
   _character_data = json::parse(response.c_str());
   std::cout << _character_data;
   fLoadRaces();
   fLoadClasses();
   _updated = false;
}
void CharacterForm::fLoadRaces()
{
    json request;
    std::string response;
    request["session_id"] = _game_session;
    std::cout << request.dump();
    _http_client->fSendRequest(HttpClient::_POST, "/api/loadraces", request.dump());
    _http_client->fGetResponse(response);
    _character_races = json::parse(response.c_str());
    std::cout << _character_races;
}

void CharacterForm::fLoadClasses()
{
    json request;
    std::string response;
    request["session_id"] = _game_session;
    std::cout << request.dump();
    _http_client->fSendRequest(HttpClient::_POST, "/api/loadclasses", request.dump());
    _http_client->fGetResponse(response);
    _character_classes = json::parse(response.c_str());
    std::cout << _character_classes;
}

void CharacterForm::fRefresh()
{
    _character_list->removeAllItems();
    _character_race_cbox->removeAllItems();
    _character_class_cbox->removeAllItems();
    _http_thread.launch();
}

void CharacterForm::fSaveChanges(tgui::ListBox::Ptr character_list)
{
    json request;
    std::string response;
    Character character(_character_name->getText().toAnsiString(), _data_race, _data_class, _data_experience, _data_hitpoints, _data_level, _data_strength, _data_dexterity, _data_constitution, _data_intelligence, _data_wisdom, _data_charisma, _game_session);
    request = character.fToJson();
    request["character_id"] = _character_id;
    std::cout << request;
    _http_client->fSendRequest(HttpClient::_POST, "/api/editcharacter", request.dump());
    _http_client->fGetResponse(response);
     std::cout << response;

     if (response.find("success"))
     {
         _save_chg->hide();
         _create_btn->show();
         fRefresh();
     }
     _combo_box = false;

}
void CharacterForm::fDisable()
{
    display_window = false;
}


CharacterForm::CharacterForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client)
    : _http_thread(&CharacterForm::fLoadCharacterListBox, this)
{
    display_window = true;
    _gui.setWindow(window);
    _game_session = game_session;
    _http_client = http_client;
    _updated = true;
    fInitUIElements();
    this->_event = event;
    _http_thread.launch();
}

void CharacterForm::fUpdate(sf::RenderWindow  &window)
{

    while (window.pollEvent(_event))
    {

        try
        {
            if (_character_data["status"] == "success"&& _character_races["status"] == "success" && _character_classes["status"] == "success" && !_updated)
            {

                std::string race_quan = _character_races["races_quantity"];
                for (int i = 0; i < std::stoi(race_quan); i++)
                {
                    _character_race_cbox->addItem(_character_races["list"][i]["race"], _character_races["list"][i]["race_id"]);
                }

                std::string class_quan = _character_classes["classes_quantity"];

                for (int i = 0; i < std::stoi(class_quan); i++)
                {
                    _character_class_cbox->addItem(_character_classes["list"][i]["class"], _character_classes["list"][i]["class_id"]);
                }


                std::string quan = _character_data["character_quantity"];
                for (int i = 0; i < std::stoi(quan); i++)
                {
                    _character_list->addItem(_character_data["list"][i]["character"], _character_data["list"][i]["character_id"]);
                }

                _updated = true;
            }
        }
        catch (const std::exception&)
        {

        }

        if (_combo_box)
        {
            _data_strength = _strength->getSelectedItem();
            _data_dexterity = _dexterity->getSelectedItem();
            _data_constitution = _constitution->getSelectedItem();
            _data_intelligence = _inteligence->getSelectedItem();
            _data_wisdom = _wisdom->getSelectedItem();
            _data_charisma = _charisma->getSelectedItem();
            _data_race = _character_race_cbox->getSelectedItem();
            _data_class = _character_class_cbox->getSelectedItem();
            _character_id = _character_list->getSelectedItemId();
           
            if (_data_strength.size() > 0)
            {
                _strength_mod->setText(fSetAbilityMod(_data_strength));
            }

            if (_data_dexterity.size() > 0)
            {
                _dexterity_mod->setText(fSetAbilityMod(_data_dexterity));
            }

            if (_data_constitution.size() > 0)
            {
                _constitution_mod->setText(fSetAbilityMod(_data_constitution));
            }

            if (_data_intelligence.size() > 0)
            {
                _inteligence_mod->setText(fSetAbilityMod(_data_intelligence));
            }

            if (_data_wisdom.size() > 0)
            {
                _wisdom_mod->setText(fSetAbilityMod(_data_wisdom));
            }

            if (_data_charisma.size() > 0)
            {
                _charisma_mod->setText(fSetAbilityMod(_data_charisma));
            }

        }
        else
        {
            _strength->deselectItem();
            _dexterity->deselectItem();
            _constitution->deselectItem();
            _inteligence->deselectItem();
            _wisdom->deselectItem();
            _charisma->deselectItem();
            _character_name->setText("");
            _strength_mod->setText("");
            _dexterity_mod->setText("");
            _constitution_mod->setText("");
            _inteligence_mod->setText("");
            _wisdom_mod->setText("");
            _charisma_mod->setText("");
            _character_race_cbox->deselectItem();
            _character_class_cbox->deselectItem();
            _combo_box = true;
        }


        if (_event.type == sf::Event::Closed)
            window.close();

        _gui.handleEvent(_event);
    }
}
void CharacterForm::fDraw(sf::RenderWindow & window)
{
    _gui.draw();
}
