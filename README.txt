CIS-427 Project - 1
Program By: Hadi Daana and Johnathan Khalil

README file

This project is created by Hadi Daana and Johnathan Khalil. This
project contains two programs; client and server, which work in coordination of
one another. The server program should be running before the client is executed.
The client program requires an IP address as a commandline argument of the server.

How to Compile:
1. Create a directory for the programs.
2. When creating a directory, make sure it has the following files:
	Client.c
	Server.c
	Makefile
3. Go to the directory in terminal and type in "make clean" command
4. Now type in "make" command
5. It should output: 
	g++ -g -c client.c
	g++  -o client client.o
	g++ -g -c server.c
	g++  -o server server.o

6. Apply these rules to both the client and server programs

How to run:
1. Open two window terminals
2. In the first terminal window, type in the command, "./server"
3. It should the send message: "The server is up, waiting for connection"
4. In the second terminal window, type in the next command: "./client 127.0.0.1"
5. Instructions will print on the client window.

Commands: 
1. msgget
This command from client sends a message to server to return the
message of the day. The server has access to file messages.txt and 
returns each message in that file in chronological order.
		 
2. shutdown
It will allow the user on the client side to shutdown the server.
ONLY ROOT has access to shutdown the server. Will display a message stating that the server
has shutdown.

3. login username password
It will allow the user at client side to log in to access the server.
The login command is followed by username and password. If the credentials
are authenticated, the user logs in, otherwise, it will display an error message.

4. logout
It will allow the logged in user to logout from the server

5. quit 
It will terminate the client connection from server. WIll display message indicating that client has been
terminated.

6. msgstore
It will allow the user to upload one message to the server. The server will save
the message to the messages.txt file. The limit for messages is 20.

Sample Output for Client:
c: msgget
s: 200 OK
   You always pass failure on the way to success
c: msgstore
s: 401 You are not currently logged in, login first
c: login john john01
s: 200 OK
c: MSGSTORE
s: 200 OK
s: Enter a message of the day
c: Hello World
s: 200 OK
c: msgget
s: 200 OK
   No one is perfect, that is why pencils have erasers
c: msgget
s: 200 OK
   It always seems impossible until it is done
c: msgget
s: 200 OK
   If opportunity does not knock, build a door
c: msgget
s: 200 OK
   The difference between ordinary and extraordinary is that little extra
c: msgget
s: 200 OK
   Hello World
c: logout
s: 200 OK
c: shutdown
s: 402 User not allowed
c: login root ROOT01
s: 200 OK
c: shutdown
s: 200 OK
c: quit
s: 200 OK
Client has terminated
----------------------------------------------------------------------------------------------------
Sample Output for Server:
MSGGET
MSGSTORE
LOGIN JOHN JOHN01
MSGSTORE
Hello World
MSGGET
MSGGET
MSGGET
MSGGET
MSGGET
LOGOUT
SHUTDOWN
LOGIN ROOT ROOT01
SHUTDOWN
System has shutdown



