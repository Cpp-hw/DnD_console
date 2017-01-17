//
//  Database.cpp
//  Olha Leskovs'ka
//


#include "Includes/stdafx.hpp"
#include "Includes/DataBase.hpp"

DataBase::DataBase()
{
    _connection = NULL;
}

json DataBase::fExecuteQuery(std::string sql_statement)
{
    int wordsFound = 0;
    string select = "SELECT";
    for(string::iterator it = sql_statement.begin(); it != sql_statement.end(); ++it) // finding substring in a string
    {
        if(wordsFound >= select.size())
            return fGetData(sql_statement); // method of getting data
        
        if(*it == select[wordsFound])
            ++wordsFound;
    }
    
    string insert = "INSERT";
    string update = "UPDATE";
    for(string::iterator it = sql_statement.begin(); it != sql_statement.end(); ++it) // finding substring in a string
    {
        if(wordsFound >= insert.size() || wordsFound >= update.size() )
            return fPutData(sql_statement); // method of putting data
        
        if(*it == insert[wordsFound] || *it == update[wordsFound])
            ++wordsFound;
    }
    json error;
    error["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
    error["message"] = "Unknown MySQL Query"; // inserts "Message" as the json key and message as its value
    return error;
}

json DataBase::fConnection(string host, string user_name, string password, string database_name)
{
    json connection_result;
    // Get the Database Connection Handle
    _connection = mysql_init(NULL);
    if (_connection == NULL)
    {
        // If we can't create the Database Connection Handle
        connection_result["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
        connection_result["message"] = "Can't create a MySQL Connection Handle"; // inserts "Message" as the json key and message as its value
    }
    // Connecting to a Database Server
    else if(!mysql_real_connect(_connection, // address of an existing MYSQL structure.
                                host.data(), // host name or an IP address
                                user_name.data(), // user's MySQL login ID
                                password.data(), // password for user
                                database_name.data(), // database name
                                NULL, // port number for the TCP/IP connection
                                NULL, //  socket or named pipe to use
                                0 // client flag
                                ))
    {
        connection_result["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
        connection_result["message"] = mysql_error(_connection); // inserts "Message" as the json key and message as its value
    }
    else
    {
        connection_result["result"] = "Success"; // inserts "Result" as the json key and "Success" as its value
        connection_result["message"] = "Open connection"; // inserts "Message" as the json key and message as its value
    }
    return connection_result;
}

json DataBase::fGetData(string sql_statement)
{
    json get_data_result;  // json result
    
    //Execute SQL-query
    if (mysql_query(_connection, sql_statement.c_str()) != 0)
    {
        get_data_result["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
        get_data_result["message"] = mysql_error(_connection); // inserts "Message" as the json key and message as its value
        return get_data_result;
    }
    
    MYSQL_RES*         result; // Resulting Table Handle
    MYSQL_ROW          row; // Rows Handle
    
    // Get a handle to the resulting table
    result = mysql_store_result(_connection);
    if (result == NULL)
    {
        get_data_result["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
        get_data_result["message"] = "Can't get the result description"; // inserts "Message" as the json key and message as its value
        return get_data_result;
    }
    else
    {
        if (mysql_num_rows(result) != 0)
        {
            int values_number = mysql_num_fields(result); // Number of values in a row
            json final_json; // json for final data
            json jarr; // json array
            
            //fields = mysql_num_fields(result); // Number of fields
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                MYSQL_FIELD* field; // for Field names
                string       name; // for current field name
                mysql_field_seek(result, 0);
                for(unsigned int value_number = 0; value_number < values_number; value_number++)
                {
                    field = mysql_fetch_field(result);
                    name = field->name;
                    get_data_result[name] = row[value_number]; // inserts field_name as the json key and table fields data as its value
                }
                jarr.push_back(get_data_result); // pushes json into json_array
            }
            mysql_free_result(result);
            final_json["rows"] = mysql_num_rows(result); // insert "Rows" as the json key and rows_number as its value
            final_json["result"] = "Success"; // inserts "Result" as the json key and "Success" as its value
            final_json["data"] = jarr; // inserts "Data" as the json key and json_array as its value
            return final_json;
        }
        else if (mysql_num_rows(result) == 0)
        {
            get_data_result["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
            get_data_result["message"] = "No fields with these data"; // inserts "Message" as the json key and message as its value
        }
    }
    
    // Free up the memory used by the result table
    mysql_free_result(result);
    return get_data_result;
}

json DataBase::fPutData(string sql_statement)
{
    json put_data_result;
    
    //Execute SQL-query
    if (mysql_query(_connection, sql_statement.c_str()) != 0)
    {
        put_data_result["message"] = mysql_error(_connection); // inserts "Message" as the json key and message as its value
        put_data_result["result"] = "Error"; // inserts "Result" as the json key and "Error" as its value
    }
    else
    {
        put_data_result["result"] = "Success"; // inserts "Result" as the json key and "Success" as its value
        put_data_result["data"] = "New user"; // inserts "Data" as the json key and message as its value
    }
    
    return put_data_result;
}

DataBase::~DataBase()
{
    // Closing the connection
    mysql_close(_connection);
}
