William White
PA2 Web Server

Included Files:
Server.c
Readme.md
Native root (www file)

Special Notes for Graders:
I noticed that when loading my website typeing "http://localhost:8888/www/" would result in the correct GET request from the html website once it loads. However,
If I type "http://localhost:8888/www" without the "/" at the very end the GET requests passed do not include the "www" file in the URL request. I spoke to Pradyumnna during Office hours and he agreed that it was odd and wasn't inside the scope of PA2. 

My "check_file_type" function may look odd but I am keeping it the way it is simply due to the effort I put into it. Before TA Aditya Srivastava posted on Canvas the 
link to the tar containing the "www" file that the default root of the server is contained in I was worried that the files given to us may not have extensions. For 
example,in PA1 there was a fil3 "foo3" which was an image file but did not have the .png extension. In order to accomidate this my "check_file_type" function 
originally looked at the first few bytes of every file to determine thier type. Most file types such as GIFs and .HTML files start with the same signatures and thus 
they can be identifies without a file extension. This isn't true owever for js and css files. After obtaining the "www" tar file from Aditya I then used file extensions to differentiate between a few file types. 

Client.C:
Simply an way to test requests, can pass strings to the Server.c Always uses Port 8888 and the IP of my Server when I ran it on my comp. Not meant to be used by grader simply for Testing during development. 

Server.c:
- First, the program determines the portno from the arguments provided.
- Uses socket() to create a socket using TCP
- Prepares the socket address then bind(), listen() and accept() connection.

- Now we enter the main while loop of Server.c. The main thread of the program loops on an accept() request and if an apropriate connection is made the primary thread calls a new thread to run my Connection_handler function.

- Connection_handler: Another massive While
    * Process Client Message:
        - Tokenize client message with strtok. 
        - Determine RequestMethod, RequestURL and RequestVersion
        - Also determine if the Client Message contains "Connection: Closed"
    * Method Check
        - Determine if method is GET, if not send error 405
    * Version Check
        - Determine if Version is HTTP/1.1 or HTTP/1.0, if not send error 505
    * Stat
        - This portion of the code uses the stat() function to determine if the URL passed is a directory or if it is a file.
        - If File, simply continue
        - If Dir then verify if correct DIR, if so then add /index.html to the file.
    * Determine File
        - Opens file and sends errors in responce
        - Determines file length and determines message out length
        - If insufficient perms send 403
        - If no File send 404
    * Call check_file_type()
        - If image set isimage to 1;
    * Process Request
        - Builds the Response to the HTTP Request
        - if image then clear the char* arrays as they sometimes clip into each other and the memory cannot be cleanly used again

Handling Multiple Connections:
- Done via Pthread, calls a new function for ever accept()

Error Handling:
- All Errors present, handled with fopen and DIR

Persistent connections
- My default behavior for my server is to keep conections open for a period of time untill I recive ANY kind of message then I simply extend the time in which the socket can be open.
- If the connection recieves a "Connection: Close" it will close
- If the connection recieves a "Connection: Keep-alive" it will follow the defualt behavior.


    
