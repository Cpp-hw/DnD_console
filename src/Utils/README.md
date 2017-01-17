## Logger System info
- Usage
```c++
#include "Includes/Logger.hpp"
string msg = "ERROR!";
Logger::fLog(msg, Logger::type::error);
```
- Available types
  1. Logger::type::error
  2. Logger::type::warning
  3. Logger::type::info
 
- Description 
   Log system adds messages to log file, checks file size, and if size more than 10mb renames it into pattern 
   \*filename\*\_\*date\*\_-\_\*time\*.log
   __*Demonstration:*__ ![](http://i.imgur.com/Kefov2w.gif)

