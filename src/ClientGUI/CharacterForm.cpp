#include "Includes/CharacterForm.hpp"

void CharacterForm::fInitUIElements()
{
    _theme = std::make_shared<tgui::Theme>("Interface/Game.txt");

    auto windowWidth = tgui::bindWidth(_gui);
    auto windowHeight = tgui::bindHeight(_gui);

    _theme = std::make_shared<tgui::Theme>("Interface/Game.txt");
    _back = std::make_shared<tgui::Picture>("Interface/Back.png");
    _back->setSize(tgui::bindMax(1280, windowWidth), tgui::bindMax(800, windowHeight));
    _gui.add(_back);

    _character_name = _theme->load("EditBox");
    _character_name->setPosition(610, 85);
    _character_name->setSize(160, 50);
    _character_name->setDefaultText("Name");
    _gui.add(_character_name);

    _character_class = _theme->load("EditBox");
    _character_class->setPosition(820, 85);
    _character_class->setSize(160, 50);
    _character_class->setDefaultText("Class");
    _gui.add(_character_class);


    _character_race = _theme->load("EditBox");
    _character_race->setPosition(1030, 85);
    _character_race->setSize(160, 50);
    _character_race->setDefaultText("Race");
    _gui.add(_character_race);


    _strength_l = _theme->load("Label");
    _strength_l->setPosition(610, 145);
    _strength_l->setText("Strength");
    _gui.add(_strength_l);

    _dexterity_l = _theme->load("Label");
    _dexterity_l->setPosition(820, 145);
    _dexterity_l->setText("Dexterity");
    _gui.add(_dexterity_l);

    _constitution_l = _theme->load("Label");
    _constitution_l->setPosition(1030, 145);
    _constitution_l->setText("Constitution");
    _gui.add(_constitution_l);

    _strength = _theme->load("ComboBox");
    _strength->setSize(160, 50);
    _strength->setPosition(610, 195);
    for (int i = 1; i <= 20; ++i)
    {
        _strength->addItem(std::to_string(i));
    }
    _gui.add(_strength);

    _dexterity = _theme->load("ComboBox");
    _dexterity->setSize(160, 50);
    _dexterity->setPosition(820, 195);
    for (int i = 1; i <= 20; ++i)
    {
        _dexterity->addItem(std::to_string(i));
    }
    _gui.add(_dexterity);

    _constitution = _theme->load("ComboBox");
    _constitution->setSize(160, 50);
    _constitution->setPosition(1030, 195);
    for (int i = 1; i <= 20; ++i)
    {
        _constitution->addItem(std::to_string(i));
    }
    _gui.add(_constitution);

    _inteligence_l = _theme->load("Label");
    _inteligence_l->setPosition(610, 245);
    _inteligence_l->setText("Intelligence");
    _gui.add(_inteligence_l);

    _wisdom_l = _theme->load("Label");
    _wisdom_l->setPosition(820, 245);
    _wisdom_l->setText("Wisdom");
    _gui.add(_wisdom_l);

    _charisma_l = _theme->load("Label");
    _charisma_l->setPosition(1030, 245);
    _charisma_l->setText("Charisma");
    _gui.add(_charisma_l);

    _inteligence = _theme->load("ComboBox");
    _inteligence->setSize(160, 50);
    _inteligence->setPosition(610, 295);
    for (int i = 1; i <= 20; ++i)
    {
        _inteligence->addItem(std::to_string(i));
    }
    _gui.add(_inteligence);

    _wisdom = _theme->load("ComboBox");
    _wisdom->setSize(160, 50);
    _wisdom->setPosition(820, 295);
    for (int i = 1; i <= 20; ++i)
    {
        _wisdom->addItem(std::to_string(i));
    }
    _gui.add(_wisdom);

    _charisma = _theme->load("ComboBox");
    _charisma->setSize(160, 50);
    _charisma->setPosition(1030, 295);
    for (int i = 1; i <= 20; ++i)
    {
        _charisma->addItem(std::to_string(i));
    }
    _gui.add(_charisma);

    _create_btn = _theme->load("Button");
    _create_btn->setSize(160, 50);
    _create_btn->setPosition(1030, 375);
    _create_btn->setText("Create Character");
    _gui.add(_create_btn);

    _back_btn = _theme->load("Button");
    _back_btn->setSize(160, 50);
    _back_btn->setPosition(50, 665);
    _back_btn->setText("Back");
    _gui.add(_back_btn);

    _refresh_btn = _theme->load("Button");
    _refresh_btn->setSize(400, 50);
    _refresh_btn->setPosition(50, 375);
    _refresh_btn->setText("Refresh Character List");
    _gui.add(_refresh_btn);

    _character_list = _theme->load("ListBox");
    _character_list->setPosition(50, 85);
    _character_list->setSize(400, 240);
    _gui.add(_character_list);

    _combo_box = true;

    _create_btn->connect("pressed", &CharacterForm::fCreateCharacter, this, _character_name, _character_class, _character_race);
    _refresh_btn->connect("pressed", &CharacterForm::fRefresh, this);
    _back_btn->connect("pressed", &CharacterForm::fDisable, this);
}

void CharacterForm::fCreateCharacter(tgui::EditBox::Ptr name, tgui::EditBox::Ptr _class, tgui::EditBox::Ptr race)
{
    //character.fSetOwner(_game_session);

    Character character(name->getText().toAnsiString(), race->getText().toAnsiString(), _class->getText().toAnsiString(), "0", "100", "1", _data_strength, _data_dexterity, _data_constitution, _data_intelligence, _data_wisdom, _data_charisma, _game_session);

    std::string response;
    std::string request = character.fToJson().dump();

    _http_client->fSendRequest(HttpClient::_POST, "/api/addcharacter", request);
    _http_client->fGetResponse(response);
    std::cout << response << std::endl;
    _combo_box = false;
}

void CharacterForm::fLoadCharacterListBox()
{

    std::string response;

    auto request = UserActions::fLoadMyCharacters(_game_session).dump();

    _http_client->fSendRequest(HttpClient::_POST, "/api/loadmycharacterslist", request);
    _http_client->fGetResponse(response);
    _character_data = json::parse(response);
    std::cout << _character_data;
}

void CharacterForm::fRefresh()
{
    fLoadCharacterListBox();
    _character_list->removeAllItems();
    _updated = false;
}

void CharacterForm::fDisable()
{
    display_window = false;
}

CharacterForm::CharacterForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client)
{

    display_window = true;
    _game_session = game_session;
    _gui.setWindow(window);
    _http_client = http_client;
    fLoadCharacterListBox();
    _updated = false;
    //_menu_option = NONE;
    fInitUIElements();
    this->_event = event;
}

void CharacterForm::fUpdate(sf::RenderWindow  &window)
{

    while (window.pollEvent(_event))
    {
        if (_combo_box)
        {
            _data_strength = _strength->getSelectedItem();
            _data_dexterity = _dexterity->getSelectedItem();
            _data_constitution = _constitution->getSelectedItem();
            _data_intelligence = _inteligence->getSelectedItem();
            _data_wisdom = _wisdom->getSelectedItem();
            _data_charisma = _charisma->getSelectedItem();

        }
        else
        {
            _strength->deselectItem();
            _dexterity->deselectItem();
            _constitution->deselectItem();
            _inteligence->deselectItem();
            _wisdom->deselectItem();
            _charisma->deselectItem();
            _character_name->setDefaultText(" ");
            _character_race->setDefaultText(" ");
            _character_class->setDefaultText(" ");
            _combo_box = true;

        }

        try
        {
            if (_character_data["status"] == "success" && !_updated)
            {
                std::string quan = _character_data["character_quantity"];
                for (int i = 0; i < std::stoi(quan); i++)
                {

                    _character_list->addItem(_character_data["list"][i]["character"], std::to_string(i));
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
void CharacterForm::fDraw(sf::RenderWindow & window)
{
    _gui.draw();
}
