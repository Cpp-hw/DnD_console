CREATE DATABASE DnD;
USE DnD;

CREATE TABLE Users (
    id INT NOT NULL AUTO_INCREMENT,
    username VARCHAR(30) NOT NULL,
    password VARCHAR(255) NOT NULL,
    email VARCHAR(30),
    is_active TINYINT(1) NOT NULL,
    PRIMARY KEY (id),
    UNIQUE (username, password)
    );

CREATE TABLE Characters (
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(20) NOT NULL,
    race VARCHAR(50) NOT NULL,
    class VARCHAR(30) NOT NULL,
    experience TINYINT UNSIGNED NOT NULL,
    hitpoints TINYINT UNSIGNED NOT NULL,
    level TINYINT UNSIGNED NOT NULL,
    id_user INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (id_user) REFERENCES Users (id)
    );

CREATE TABLE Terrain (
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    type VARCHAR(50) NOT NULL,
    width TINYINT UNSIGNED NOT NULL,
    height TINYINT UNSIGNED NOT NULL,
    description TEXT NOT NULL,
    id_owner INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (id_owner) REFERENCES Users (id)
    );

CREATE TABLE NPCs (
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    type VARCHAR(50) NOT NULL,
    hitpoints TINYINT UNSIGNED NOT NULL,
    level TINYINT UNSIGNED NOT NULL,
    strength TINYINT UNSIGNED NOT NULL,
    dexterity TINYINT UNSIGNED NOT NULL,
    constitution TINYINT UNSIGNED NOT NULL,
    intelligence TINYINT UNSIGNED NOT NULL,
    wisdom TINYINT UNSIGNED NOT NULL,
    charisma TINYINT UNSIGNED NOT NULL,
    id_owner INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (id_owner) REFERENCES Users (id)
    );

CREATE TABLE Abilities (
    id INT NOT NULL AUTO_INCREMENT,
    strength TINYINT UNSIGNED NOT NULL,
    str_mod TINYINT,
    dexterity TINYINT UNSIGNED NOT NULL,
    dex_mod TINYINT,
    constitution TINYINT UNSIGNED NOT NULL,
    con_mod TINYINT,
    intelligence TINYINT UNSIGNED NOT NULL,
    int_mod TINYINT,
    wisdom TINYINT UNSIGNED NOT NULL,
    wis_mod TINYINT,
    charisma TINYINT UNSIGNED NOT NULL,
    cha_mod TINYINT,
    id_character INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (id_character) REFERENCES Characters (id)
    );

CREATE TABLE Sessions (
    id INT NOT NULL AUTO_INCREMENT,
    id_user INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (id_user) REFERENCES Users (id)
    );

CREATE TABLE Boards (
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    width TINYINT UNSIGNED NOT NULL,
    height TINYINT UNSIGNED NOT NULL,
    description TEXT NOT NULL,
    id_owner INT NOT NULL,
    PRIMARY KEY (id),
    FOREIGN KEY (id_owner) REFERENCES Users (id)
    );

CREATE TABLE BT_Map (
    id_board INT NOT NULL,
    id_terrain INT NOT NULL,
    terrain_x TINYINT UNSIGNED NOT NULL,
    terrain_y TINYINT UNSIGNED NOT NULL,
    FOREIGN KEY (id_board) REFERENCES Boards (id),
    FOREIGN KEY (id_terrain) REFERENCES Terrain (id)
    );

CREATE TABLE BN_Map (
    id_board INT NOT NULL,
    id_npc INT NOT NULL,
    npc_x TINYINT UNSIGNED NOT NULL,
    npc_y TINYINT UNSIGNED NOT NULL,
    FOREIGN KEY (id_board) REFERENCES Boards (id),
    FOREIGN KEY (id_npc) REFERENCES NPCs (id)
    );
