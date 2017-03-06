#include "Includes/NPCForm.hpp"

void NPCForm::fInitUIElements()
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

    _npc_img = std::make_shared<tgui::Picture>();
    _npc_img->setSize(64, 64);
    _npc_img->setPosition(windowWidth / 2 + 380, 205);
    _npc_img->hide();
    _gui.add(_npc_img);

    _npc_name = _theme->load("EditBox");
    _npc_name->setPosition(windowWidth / 2 + 50, 105);
    _npc_name->setSize(180, 40);
    _npc_name->setDefaultText("Name");
    _gui.add(_npc_name);

    _npc_type_l = _theme->load("Label");
    _npc_type_l->setText("Type: ");
    _npc_type_l->setPosition(windowWidth / 2 + 380, 115);
    _gui.add(_npc_type_l);

    _npc_type = _theme->load("ComboBox");
    _npc_type->setPosition(windowWidth / 2 + 440, 105);
    _npc_type->setSize(150, 40);
    _gui.add(_npc_type);

    _strength_l = _theme->load("Label");
    _strength_l->setPosition(windowWidth / 2 + 50, 235);
    _strength_l->setText("Strength");
    _gui.add(_strength_l);

    _dexterity_l = _theme->load("Label");
    _dexterity_l->setPosition(windowWidth / 2 + 50, _strength_l->getSize().y + 275);
    _dexterity_l->setText("Dexterity");
    _gui.add(_dexterity_l);

    _constitution_l = _theme->load("Label");
    _constitution_l->setPosition(windowWidth / 2 + 50, _dexterity_l->getSize().y + 335);
    _constitution_l->setText("Constitution");
    _gui.add(_constitution_l);

    _inteligence_l = _theme->load("Label");
    _inteligence_l->setPosition(windowWidth / 2 + 50, _constitution_l->getSize().y + 395);
    _inteligence_l->setText("Intelligence");
    _gui.add(_inteligence_l);

    _wisdom_l = _theme->load("Label");
    _wisdom_l->setPosition(windowWidth / 2 + 50, _inteligence_l->getSize().y + 455);
    _wisdom_l->setText("Wisdom");
    _gui.add(_wisdom_l);

    _charisma_l = _theme->load("Label");
    _charisma_l->setPosition(windowWidth / 2 + 50, _wisdom_l->getSize().y + 515);
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

    _hitpoints = _theme->load("Label");
    _hitpoints->setPosition(windowWidth / 2 + 380, _inteligence_l->getSize().y + 455);
    _hitpoints->setText("HitPoints: ");
    _hitpoints->setTextColor("red");
    _gui.add(_hitpoints);


    _level = _theme->load("Label");
    _level->setPosition(windowWidth / 2 + 380, _wisdom_l->getSize().y + 515);
    _level->setText("Level: ");
    _level->setTextColor("yellow");
    _gui.add(_level);

    _hitpoints_ = _theme->load("EditBox");
    _hitpoints_->setSize(80, 40);
    _hitpoints_->setPosition(windowWidth / 2 + 510, 465);
    _gui.add(_hitpoints_);

    _level_= _theme->load("EditBox");
    _level_->setSize(80, 40);
    _level_->setPosition(windowWidth / 2 + 510, 525);
    _gui.add(_level_);

    tgui::Label::Ptr _mods_l = _theme->load("Label");
    _mods_l->setPosition(windowWidth / 2 + 50, 185);
    _mods_l->setText("Abilities: ");
    _gui.add(_mods_l);

    for (int i = 1; i <= 20; ++i)
    {
        _strength->addItem(std::to_string(i));
        _dexterity->addItem(std::to_string(i));
        _constitution->addItem(std::to_string(i));
        _inteligence->addItem(std::to_string(i));
        _charisma->addItem(std::to_string(i));
        _wisdom->addItem(std::to_string(i));
    }

    _create_btn = _theme->load("Button");
    _create_btn->setSize(160, 50);
    _create_btn->setPosition(windowWidth / 2 + 430, _wisdom_l->getSize().y + 585);
    _create_btn->setText("Create NPC");
    _gui.add(_create_btn);

    _back_btn = _theme->load("Button");
    _back_btn->setSize(160, 50);
    _back_btn->setPosition(50, windowHeight - 150);
    _back_btn->setText("Back");
    _gui.add(_back_btn);

    _refresh_btn = _theme->load("Button");
    _refresh_btn->setSize(400, 50);
    _refresh_btn->setPosition(50, 375);
    _refresh_btn->setText("Refresh NPCs List");
    _gui.add(_refresh_btn);

    _delete_btn = _theme->load("Button");
    _delete_btn->setSize(400, 50);
    _delete_btn->setPosition(50, 445);
    _delete_btn->setText("Delete NPC");
    _gui.add(_delete_btn);

    _edit_btn = _theme->load("Button");
    _edit_btn->setSize(400, 50);
    _edit_btn->setPosition(50, 515);
    _edit_btn->setText("Edit NPC");
    _gui.add(_edit_btn);

    _save_chg = _theme->load("Button");
    _save_chg->setSize(160, 50);
    _save_chg->setPosition(windowWidth / 2 + 430, _wisdom_l->getSize().y + 585);
    _save_chg->setText("Save Changes");
    _save_chg->hide();
    _gui.add(_save_chg);

    _npc_list = _theme->load("ListBox");
    _npc_list->setPosition(50, 85);
    _npc_list->setSize(400, 240);
    _gui.add(_npc_list);

    _create_btn->connect("pressed", &NPCForm::fCreateNPC, this, _npc_name, _level_,_hitpoints_, _npc_type);
    _refresh_btn->connect("pressed", &NPCForm::fRefresh, this);
    _delete_btn->connect("pressed", &NPCForm::fDeleteNPC, this, _npc_list);
    _edit_btn->connect("pressed", &NPCForm::fEditNPC, this, _npc_list);
    _save_chg->connect("pressed", &NPCForm::fSaveChanges, this, _npc_list);
    _back_btn->connect("pressed", &NPCForm::fDisable, this);

    _combo_box = true;

}

void NPCForm::fCreateNPC(tgui::EditBox::Ptr name, tgui::EditBox::Ptr level, tgui::EditBox::Ptr hitpoints, tgui::ComboBox::Ptr type)
{

    Npc npc(_game_session, name->getText().toAnsiString(), type->getSelectedItemId().toAnsiString(), level->getText().toAnsiString(), hitpoints->getText().toAnsiString(), _data_strength, _data_dexterity, _data_constitution, _data_intelligence, _data_wisdom, _data_charisma);

    std::string response;
    std::string request = npc.fToJson().dump();
    std::cout << request << std::endl;
    _http_client->fSendRequest(HttpClient::_POST, "/api/addnpc", request);
    _http_client->fGetResponse(response);
    std::cout << response << std::endl;
    _combo_box = false;

    if (response.find("success"))
    {
        fRefresh();
    }
}

void NPCForm::fLoadNPCType()
{
    json request;
    std::string response;
    request["session_id"] = _game_session;
    std::cout << request.dump();
    _http_client->fSendRequest(HttpClient::_POST, "/api/loadtypesnpc", request.dump());
    _http_client->fGetResponse(response);
    _npc_types = json::parse(response.c_str());
    std::cout << _npc_types;
}

void NPCForm::fLoadNPCListBox()
{
    std::string response;
    auto request = UserActions::fLoadMyNpcs(_game_session).dump();
    _http_client->fSendRequest(HttpClient::_POST, "/api/loadmynpcslist", request);
    _http_client->fGetResponse(response);
    _npc_data = json::parse(response.c_str());
    std::cout << _npc_data;
}

void NPCForm::fLoadData()
{
    fLoadNPCType();
    fLoadNPCListBox();
    _updated = false;
}

void NPCForm::fDeleteNPC(tgui::ListBox::Ptr npc_list)
{
    auto request = UserActions::fDeleteNpc(_game_session, npc_list->getSelectedItemId().toAnsiString()).dump();
    std::string response;
    _http_client->fSendRequest(HttpClient::_POST, "/api/deletenpc", request);
    _http_client->fGetResponse(response);
    std::cout << response;

    if (response.find("success"))
    {
        fRefresh();
    }

}

void NPCForm::fEditNPC(tgui::ListBox::Ptr npc_list)
{

    _create_btn->hide();
    _save_chg->show();
    try
    {
        json request;
        request["session_id"] = _game_session;
        request["npc_id"] = npc_list->getSelectedItemId().toAnsiString();

        std::string response;

        _http_client->fSendRequest(HttpClient::_POST, "/api/loadnpc", request.dump());
        _http_client->fGetResponse(response);
        std::cout << response;

        json temp_npc;

        temp_npc = json::parse(response.c_str());


        _npc_name->setText(temp_npc["npc"]);
        _npc_type->setSelectedItem(temp_npc["type"]);
        _strength->setSelectedItem(temp_npc["strength"]);
        _dexterity->setSelectedItem(temp_npc["dexterity"]);
        _constitution->setSelectedItem(temp_npc["constitution"]);
        _inteligence->setSelectedItem(temp_npc["intelligence"]);
        _wisdom->setSelectedItem(temp_npc["wisdom"]);
        _charisma->setSelectedItem(temp_npc["charisma"]);
        _hitpoints_->setText(temp_npc["hitpoints"]);
        _level_->setText(temp_npc["level"]);
    }
    catch (std::exception e)
    {
        std::cout << e.what();
    }
}

void NPCForm::fSaveChanges(tgui::ListBox::Ptr npc_list)
{
    json request;
    std::string response;
    Npc npc(_game_session, _npc_name->getText().toAnsiString(), _npc_type->getSelectedItemId().toAnsiString(), _data_level, _data_hitpoints, _data_strength, _data_dexterity, _data_constitution, _data_intelligence, _data_wisdom, _data_charisma);
    request = npc.fToJson();
    request["npc_id"] = _npc_id;
    request["type"] = _npc_type->getSelectedItem();
    std::cout << request;
    _http_client->fSendRequest(HttpClient::_POST, "/api/editnpc", request.dump());
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

void NPCForm::fRefresh()
{
    _npc_list->removeAllItems();
    _npc_type->removeAllItems();
    _load_data.launch();

}

void NPCForm::fLoadNPCImages()
{
    _npc_picture[0].loadFromFile("Interface/sprites/Creatures250/Goblin250.png");
    _npc_picture[1].loadFromFile("Interface/sprites/Creatures250/Fairy250.png");
    _npc_picture[2].loadFromFile("Interface/sprites/Creatures250/Golem250.png");
    _npc_picture[3].loadFromFile("Interface/sprites/Creatures250/Dragon250.png");
    _npc_picture[4].loadFromFile("Interface/sprites/Creatures250/Gremlin250.png");
    _npc_picture[5].loadFromFile("Interface/sprites/Creatures250/Monk250.png");
    _npc_picture[6].loadFromFile("Interface/sprites/Creatures250/Archer250.png");
    _npc_picture[7].loadFromFile("Interface/sprites/Creatures250/Troglodyte.png");
}

void NPCForm::fDisable()
{
    display_window = false;
}


NPCForm::NPCForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client):_load_data(&NPCForm::fLoadData,this)
{
    this->_event = event;
    _gui.setWindow(window);
    display_window = true;
    _http_client = http_client;
    _game_session = game_session;
    _updated = true;
    fInitUIElements();
    _load_data.launch();
    fLoadNPCImages();
}

void NPCForm::fUpdate(sf::RenderWindow  &window)
{

    while (window.pollEvent(_event))
    {
        if (_combo_box)
        {

            std::string elem_type = _npc_type->getSelectedItem();

            if (elem_type == "Elf")
            {
                _npc_img->setTexture(_npc_picture[0]);
                _npc_img->show();
            }
            else if (elem_type == "Fey")
            {
                _npc_img->setTexture(_npc_picture[1]);
                _npc_img->show();
            }
            else if (elem_type == "Giant")
            {
                _npc_img->setTexture(_npc_picture[2]);
                _npc_img->show();
            }
            else if (elem_type == "Dragon")
            {
                _npc_img->setTexture(_npc_picture[3]);
                _npc_img->show();
            }
            else if (elem_type == "Goblin")
            {
                _npc_img->setTexture(_npc_picture[4]);
                _npc_img->show();
            }
            else if (elem_type == "Human")
            {
                _npc_img->setTexture(_npc_picture[5]);
                _npc_img->show();
            }
            else if (elem_type == "Kobold")
            {
                _npc_img->setTexture(_npc_picture[6]);
                _npc_img->show();
            }
            else if (elem_type == "Orc")
            {
                _npc_img->setTexture(_npc_picture[7]);
                _npc_img->show();
            }

            _data_strength = _strength->getSelectedItem();
            _data_dexterity = _dexterity->getSelectedItem();
            _data_constitution = _constitution->getSelectedItem();
            _data_intelligence = _inteligence->getSelectedItem();
            _data_wisdom = _wisdom->getSelectedItem();
            _data_charisma = _charisma->getSelectedItem();
            _npc_id = _npc_list->getSelectedItemId();
            _data_hitpoints = _hitpoints_->getText();
            _data_level = _hitpoints_->getText();

        }
        else
        {
            _strength->deselectItem();
            _dexterity->deselectItem();
            _constitution->deselectItem();
            _inteligence->deselectItem();
            _wisdom->deselectItem();
            _charisma->deselectItem();
            _npc_name->setText("");
            _hitpoints_->setText("");
            _level_->setText("");
            _npc_type->deselectItem();
            _npc_img->hide();
            _combo_box = true;

        }

        try
        {
            if (_npc_data["status"] == "success" && _npc_types["status"] == "success" && !_updated)
            {
                std::string npc_quan = _npc_data["npcs_quantity"];
                for (int i = 0; i < std::stoi(npc_quan); i++)
                {

                    _npc_list->addItem(_npc_data["list"][i]["npc"], _npc_data["list"][i]["npc_id"]);
                }

                std::string npc_type_quan = _npc_types["types_quantity"];
                for (int i = 0; i < std::stoi(npc_type_quan); i++)
                {

                    _npc_type->addItem(_npc_types["list"][i]["type"], _npc_types["list"][i]["type_id"]);
                }

                _updated = true;
            }
        }
        catch (const std::exception&)
        {

        }

        if (_event.type == sf::Event::Closed)
            window.close();

        _gui.handleEvent(_event);
    }
}
void NPCForm::fDraw(sf::RenderWindow & window)
{
    _gui.draw();
}
