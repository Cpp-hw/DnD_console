# **D&D game SERVER**
This server consists of the following:
## **HTTP Server**
#### Short notes:
 - The server is intended to be cross-platform. Proper platform version will be created on build time automaticly, it is in TODO list. For now, some lines of the code have to be /*commented*/, and some of them have to be uncomented in order to switch from Windows to UNIX.
 - No need in additional libraries or headers, it is built on (win)sockets, using native posibilities of both UNIX and Windows systems
 - The server is easyly built in Microsoft Visual Studio 2012 (for now, it is only environmet I've tested the build in).
 - For now, it is early version with basic functionality only
 - More possibilities and functionality are comming
 - If you want to test or try it:
   1. build it
   2. create folder `Root` in the same directory where the exe file is placed
   3. run it
   4. type in your Internet browser following "127.0.0.1:27015" - the browser will probably receive "bad request responce" (it is indendeed to be), while the server will inform about succesfull client connection, its request and other information.
   5. you may also use simple GET requests (in terminal or your application) to communicate with the server, in the following format:
   `GET URI HTTP version CRLFCRLF`
    e.g.:
   `GET /some/url/to.file HTTP/1.1\r\n\r\n`

#### Change log:
 - (31.12.2016) The server is CROSS-PLATFORM, it still does not use any third party libraries, native sockets only!!!