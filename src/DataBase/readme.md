# Interaction with the database

## Start to work
### Direct download link
- First off you need to download the relevant server package: http://dev.mysql.com/downloads/mysql/.
- If MySQL is already installed you should download MySQL Connector/C++. You can find it here: https://dev.mysql.com/downloads/connector/cpp/1.1.html

#### How add MySQL Connector/C++ libraries to Xcode
- First you need to add the following search paths in your xcode project.
In the build settings [**Project -> Edit Project Settings -> Build**] add the following locations to the search paths settings:
    - Header Search Paths: **/usr/local/mysql/include/**
    - Library Search Paths: **/usr/local/mysql/lib/**
- In the same menu you now need to add the linker flags in the Linking section:
    - Other Linker Flags: **-lmysqlclient -lm -lz**
- Then you just need to include the header file:
```c++ #include <mysql.h> ```

## General information
This Database class consists of the following:
- method for openning the connection;
- method for getting data from the database;
- method for putting data into the database.

It receives some SQL query and returns JSON-formatted response. 

**Inportant information:** 
Use: 
```sql
SELECT id, username, password, is_active FROM Users;
```
instead of:
```sql
SELECT * FROM Users;
```
