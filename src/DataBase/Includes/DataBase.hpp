//
//  DataBase.hpp
//  Olha Leskovs'ka
//


#ifndef DataBase_hpp
#define DataBase_hpp

using json = nlohmann::json;

class DataBase
{
private:
    MYSQL* _connection; // Database Connection Handle/Decryptor
    
public:
    DataBase();
    
    json fConnection(std::string host, std::string user_name, std::string password, std::string database_name); // for connection with Database
    
    json fExecuteQuery(std::string sql_statement); // for executing a query
    
    json fGetData(std::string sql_statement); // for getting Data from Database
    
    json fPutData(std::string sql_statement); // for putting Data into Database
    
    ~DataBase();
}; // class DataBase

#endif /* DataBase_hpp */
