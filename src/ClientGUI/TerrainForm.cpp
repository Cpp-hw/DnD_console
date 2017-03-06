#include "Includes/TerrainForm.hpp"

void TerrainForm::fInitUIElements()
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

    _terrain_img = std::make_shared<tgui::Picture>();
    _terrain_img->setSize(64,64);
    _terrain_img->setPosition(windowWidth / 2 + 440, 205);
    _terrain_img->hide();
    _gui.add(_terrain_img);

    _terrain_name = _theme->load("EditBox");
    _terrain_name->setPosition(windowWidth/2+50, 105);
    _terrain_name->setSize(180, 40);
    _terrain_name->setDefaultText("Name");
    _gui.add(_terrain_name, "Name");

    _type_l = _theme->load("Label");
    _type_l->setText("Type: ");
    _type_l->setPosition(windowWidth / 2 + 380, 115);
    _gui.add(_type_l);

    _terrain_type = _theme->load("ComboBox");
    _terrain_type->setPosition(windowWidth / 2 + 440, 105);
    _terrain_type->setSize(150, 40);
    _gui.add(_terrain_type, "Type");

    _terrain_width = _theme->load("EditBox");
    _terrain_width->setPosition(windowWidth / 2 + 50, 185);
    _terrain_width->setSize(160,40);
    _terrain_width->setDefaultText("Width");
    _gui.add(_terrain_width, "Width");

    _terrain_height = _theme->load("EditBox");
    _terrain_height->setPosition(windowWidth / 2 + 50, 265);
    _terrain_height->setSize(160, 40);
    _terrain_height->setDefaultText("Height");
    _gui.add(_terrain_height, "Height");

    _terrain_description = _theme->load("TextBox");
    _terrain_description->setPosition(windowWidth / 2 + 50, 385);
    _terrain_description->setSize(560, 200);
    _gui.add(_terrain_description, "Description");

    _create_btn = _theme->load("Button");
    _create_btn->setSize(160, 40);
    _create_btn->setPosition(windowWidth / 2 + 440, 605);
    _create_btn->setText("Create Terrain");
    _gui.add(_create_btn);

    _back_btn = _theme->load("Button");
    _back_btn->setSize(160, 50);
    _back_btn->setPosition(50, windowHeight - 150);
    _back_btn->setText("Back");
    _gui.add(_back_btn);

    _refresh_btn = _theme->load("Button");
    _refresh_btn->setSize(400, 50);
    _refresh_btn->setPosition(50, 375);
    _refresh_btn->setText("Refresh Terrain List");
    _gui.add(_refresh_btn);

    _terrain_list = _theme->load("ListBox");
    _terrain_list->setPosition(50, 85);
    _terrain_list->setSize(400, 240);
    _gui.add(_terrain_list);

    _create_btn->connect("pressed", &TerrainForm::fCreateTerrain, this, _terrain_name, _terrain_type, _terrain_width, _terrain_height, _terrain_description);
    _refresh_btn->connect("pressed", &TerrainForm::fRefresh, this);
    _back_btn->connect("pressed", &TerrainForm::fDisable, this);

    _combo_box = true;
}

void TerrainForm::fCreateTerrain(tgui::EditBox::Ptr name, tgui::ComboBox::Ptr type, tgui::EditBox::Ptr width, tgui::EditBox::Ptr height, tgui::TextBox::Ptr description)
{
    Terrain terrain(_game_session, name->getText().toAnsiString(), type->getSelectedItem().toAnsiString(), width->getText().toAnsiString(), height->getText().toAnsiString(), description->getText().toAnsiString());
    std::string response;
    std::string request = terrain.fTerrain_To_Json().dump();
    std::cout << request << std::endl;
    _http_client->fSendRequest(HttpClient::_POST, "/api/addterrain", request);
    _http_client->fGetResponse(response);

    if (response.find("success"))
    {
        fRefresh();
    }
    _combo_box = false;
}

void TerrainForm::fLoadTerrainListBox()
{

    std::string response;
    auto request = UserActions::fLoadMyTerrains(_game_session).dump();
    std::cout << request << std::endl;

    _http_client->fSendRequest(HttpClient::_POST, "/api/loadmyterrainslist", request);
    _http_client->fGetResponse(response);
    _terrain_data = json::parse(response);
    std::cout << _terrain_data;
}

void TerrainForm::fLoadTerrainTypeList()
{
    json request;
    std::string response;
    request["session_id"] = _game_session;
    _http_client->fSendRequest(HttpClient::_POST, "/api/loadtypesterrain", request.dump());
    _http_client->fGetResponse(response);
    _terrain_types = json::parse(response.c_str());
    std::cout << _terrain_types;
}

void TerrainForm::fLoadData()
{
    fLoadTerrainListBox();
    fLoadTerrainTypeList();
    _updated = false;
}

void TerrainForm::fLoadTerrainImages()
{
    _terrain_picture[0].loadFromFile("Interface/terrains/rock.png");
    _terrain_picture[1].loadFromFile("Interface/terrains/water.png");
    _terrain_picture[2].loadFromFile("Interface/terrains/snow.png");
    _terrain_picture[3].loadFromFile("Interface/terrains/lava.png");
    _terrain_picture[4].loadFromFile("Interface/terrains/shadow.png");
    _terrain_picture[5].loadFromFile("Interface/terrains/tree.png");

}

void TerrainForm::fRefresh()
{
    _terrain_type->removeAllItems();
    _terrain_list->removeAllItems();
    _load_terrains_data.launch();
}

void TerrainForm::fDisable()
{
    display_window = false;
}

TerrainForm::TerrainForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client):
    _load_terrains_data(&TerrainForm::fLoadData,this)
{

    display_window = true;
    _game_session = game_session;
    _http_client = http_client;
    _gui.setWindow(window);
    _updated = true;
    fInitUIElements();
    this->_event = event;
    _load_terrains_data.launch();
    fLoadTerrainImages();
}

void TerrainForm::fUpdate(sf::RenderWindow  &window)
{

    while (window.pollEvent(_event))
    {

        if (_combo_box)
        {
            std::string elem_type = _terrain_type->getSelectedItem();

            if (elem_type == "blocking")
            {
                _terrain_img->setTexture(_terrain_picture[0]);
                _terrain_img->show();
            }
            else if (elem_type == "difficult")
            {
                _terrain_img->setTexture(_terrain_picture[1]);
                _terrain_img->show();
            }
            else if (elem_type == "challenging")
            {
                _terrain_img->setTexture(_terrain_picture[2]);
                _terrain_img->show();
            }
            else if (elem_type == "hindering")
            {
                _terrain_img->setTexture(_terrain_picture[3]);
                _terrain_img->show();
            }
            else if (elem_type == "obscured")
            {
                _terrain_img->setTexture(_terrain_picture[4]);
                _terrain_img->show();
            }
            else if (elem_type == "cover")
            {
                _terrain_img->setTexture(_terrain_picture[5]);
                _terrain_img->show();
            }
        }
        else
        {
            _terrain_name->setText("");
            _terrain_height->setText("");
            _terrain_width->setText("");
            _terrain_description->setText("");
            _terrain_type->deselectItem();
            _terrain_img->hide();

            _combo_box = true;
        }

        try
        {
            if (_terrain_data["status"]=="success" && _terrain_types["status"] == "success" && !_updated)
            {
                std::string type_quan = _terrain_types["types_quantity"];
                for (int i = 0; i < std::stoi(type_quan); i++)
                {

                    _terrain_type->addItem(_terrain_types["list"][i]["type"], _terrain_types["list"][i]["type_id"]);
                }

                std::string terrains_quan = _terrain_data["terrains_quantity"];
                for (int i = 0; i < std::stoi(terrains_quan); i++)
                {

                    _terrain_list->addItem(_terrain_data["list"][i]["terrain"], _terrain_data["list"][i]["terrain_id"]);
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
void TerrainForm::fDraw(sf::RenderWindow & window)
{
    _gui.draw();
}
