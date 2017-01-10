//
//  Database.hpp
//  DBConnection
//
//  Created by Olha Leskovska on 1/5/17.
//  Copyright © 2017 Olha Leskovska. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include "json.hpp"
#include <string>

using json = nlohmann::json;

class Database
{
    MYSQL* connection; // Database Connection Handle/Decryptor
    
public:
    Database();
    
    json fConnection();
    
    json fExecuteQuery(std::string sql_statement);
    
    json fGetData(std::string sql_statement);
    
    json fPutData(std::string sql_statement);
    
    ~Database();
}; // class Database

#endif /* Database_hpp */
