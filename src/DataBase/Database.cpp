//
//  Database.cpp
//  DBConnection
//
//  Created by Olha Leskovska on 1/5/17.
//  Copyright © 2017 Olha Leskovska. All rights reserved.
//

#include <my_global.h>
#include <mysql.h>
#include <iostream>
#include "Database.hpp"
#include <vector>
#include <string.h>

using namespace std;

Database::Database()
{
    connection = NULL;
}

json Database::fExecuteQuery(std::string sql_statement)
{
    fConnection();
    
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
    error["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
    error["Message"] = "Unknown MySQL Query"; // insert "Message" as the json key and message as its value
    return error;
}

json Database::fConnection()
{
    json connection_result;
    // Get the Database Connection Handle
    connection = mysql_init(NULL);
    if (connection == NULL)
    {
        // If we can't create the Database Connection Handle
        connection_result["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
        connection_result["Message"] = "Can't create a MySQL Connection Handle"; // insert "Message" as the json key and message as its value
    }
    // Connecting to a Database Server
    else if(!mysql_real_connect(connection, // address of an existing MYSQL structure.
                                "localhost", // host name or an IP address
                                "DnD_db", // user's MySQL login ID
                                "root", // password for user
                                "DnD", // database name
                                NULL, // port number for the TCP/IP connection
                                NULL, //  socket or named pipe to use
                                0 // client flag
                                ))
    {
        connection_result["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
        connection_result["Message"] = mysql_error(connection); // insert "Message" as the json key and message as its value
    }
    else
    {
        connection_result["Result"] = "Success"; // insert "Result" as the json key and "Success" as its value
        connection_result["Message"] = "Open connection"; // insert "Message" as the json key and message as its value
    }
    
    return connection_result;
}

json Database::fGetData(string sql_statement)
{
    json get_data_result;  // json result
    
    //Execute SQL-query
    if (mysql_query(connection, sql_statement.c_str()) != 0)
    {
        get_data_result["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
        get_data_result["Message"] = mysql_error(connection); // insert "Message" as the json key and message as its value
        return get_data_result;
    }
    
    MYSQL_RES*         result; // Resulting Table Handle
    MYSQL_ROW          row; // Rows Handle
    
    // Get a handle to the resulting table
    result = mysql_store_result(connection);
    if (result == NULL)
    {
        get_data_result["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
        get_data_result["Message"] = "Can't get the result description"; // insert "Message" as the json key and message as its value
        return get_data_result;
    }
    else
    {
        if (mysql_num_rows(result) != 0)
        {
            int values_number = mysql_num_fields(result); // Number of values in a row
            json final_json; // json for final data
            json jarr; //json array
            
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
                    get_data_result[name] = row[value_number]; // insert field_name as the json key and table fields data as its value
                }
                jarr.push_back(get_data_result); // push json into json_array
            }
            mysql_free_result(result);
            final_json["Rows"] = mysql_num_rows(result); // insert "Rows" as the json key and rows_number as its value
            final_json["Result"] = "Success"; // insert "Result" as the json key and "Success" as its value
            final_json["Data"] = jarr; // insert "Data" as the json key and json_array as its value
            return final_json;
        }
        else if (mysql_num_rows(result) == 0)
        {
            get_data_result["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
            get_data_result["Message"] = "No fields with these data"; // insert "Message" as the json key and message as its value
        }
    }
    
    // Free up the memory used by the result table
    mysql_free_result(result);
    
    return get_data_result;
}

json Database::fPutData(string sql_statement)
{
    json put_data_result;
    
    //Execute SQL-query
    if (mysql_query(connection, sql_statement.c_str()) != 0)
    {
        put_data_result["Message"] = mysql_error(connection); // insert "Message" as the json key and message as its value
        put_data_result["Result"] = "Error"; // insert "Result" as the json key and "Error" as its value
    }
    else
    {
        put_data_result["Result"] = "Success"; // insert "Result" as the json key and "Success" as its value
        put_data_result["Data"] = ""; // insert "Data" as the json key and message as its value
        
    }
    
    return put_data_result;
}

Database::~Database()
{
    // Closing the connection
    mysql_close(connection);
}
