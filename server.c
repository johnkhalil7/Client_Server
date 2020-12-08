/*
 * server.c
 */
 //Program By: Hadi Daana and Johnathan Khalil
 //Program Name: CIS 427 Project 1

#include <stdio.h>
#include <iostream>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

#define SERVER_PORT 1572
#define MAX_PENDING 5
#define MAX_LINE 256

string convertToString(char* , int );

int main(int argc, char** argv) {

	struct sockaddr_in sin;
	socklen_t addrlen;
	char buf[MAX_LINE];
	char get[MAX_LINE];
	int len;
	int size;
	int s;
	int new_s;

	string login = "LOGIN"; //user commands
	string logout = "LOGOUT";
	string quit = "QUIT";
	string msgget = "MSGGET";
	string msgstore = "MSGSTORE";
	string stdown = "SHUTDOWN";

	string messages[20]; //stores the messages from txt file in array
	string temp;
	string addmsg;
	
	int itotal = 0; //limiter for number of messages stored
	int i = 0;

	ifstream ifile;
	ofstream ofile;

	
	bool loggedIn = false; //conditions
	bool shutdown = false;
	bool rootLogIn = false;

	string acc0 = "LOGIN ROOT ROOT01"; //users for acceptable logins
	string acc1 = "LOGIN JOHN JOHN01";
	string acc2 = "LOGIN DAVID DAVID01";
	string acc3 = "LOGIN MARY MARY01";


	ifile.open("messages.txt");
	int j = 0;
	if (ifile.is_open()) //opens txt file and grabs each line from the file, stores that line in the array
	{
		getline(ifile, messages[j]);
		messages[j] += '\n';
		messages[j] += '\0';
		while (ifile)
		{
			++j;
			++itotal; //keeps track of the total number of messages stored
			getline(ifile, messages[j]);
			messages[j] += '\n';
			messages[j] += '\0';
		}
	}
	ifile.close();

	ofile.open("messages.txt", ios::app); //ensures that when msgstore is called, message added on newline
	ofile << "\n";
	ofile.close();

	/* build address data structure */
	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	/* setup passive open */
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	if ((bind(s, (struct sockaddr*) & sin, sizeof(sin))) < 0) {
		perror("bind");
		exit(1);
	}

	listen(s, MAX_PENDING);

	addrlen = sizeof(sin);
	cout << "The server is up, waiting for connection" << endl;
	

	/* wait for connection, then receive and print text */
	while (1) {
		if ((new_s = accept(s, (struct sockaddr*) & sin, &addrlen)) < 0) {
			perror("accept");
			exit(1);
		}
		cout << "new connection from " << inet_ntoa(sin.sin_addr) << endl;

		while (!shutdown)
		{
			while (len = recv(new_s, buf, sizeof(buf), 0))
			{
				cout << buf;
				if (strcmp(buf, msgget.c_str()) == 10) //MSGGET Command Begins
				{
					temp = "s: 200 OK\n   ";
					temp += messages[i];
					//temp += "\n";
					strcpy(buf, temp.c_str());
					i++;
					if (i == itotal) //resets incrementer for msgget calls
					{
						i = 0;
					}
					send(new_s, buf, strlen(buf) + 1, 0);
					break;
				} //MSGGET Command Ends

				if (strcmp(buf, msgstore.c_str()) == 10) //MSGSTORE Command Begins
				{
					if (loggedIn == false) //error condition if user is not logged in
					{
						temp = "s: 401 You are not currently logged in, login first";
						temp += "\n";
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
						break;
					}
					if (loggedIn == true)
					{
						temp = "s: 200 OK\n";
						temp += "s: Enter a message of the day\n"; //added message for user friendliness
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0); //sends 200 OK, then waits to  receive the user's message
						recv(new_s, get, sizeof(get), 0);
						size = sizeof(get) / sizeof(char);
						addmsg = convertToString(get, size); //converts the message into a string and stores it in a variable
						
						ofile.open("messages.txt", ios::app);
						ofile << addmsg; //writes the user's message at the first available newline
						ofile.close();
						messages[itotal] = addmsg;
						itotal++; //increments since there is a new total
						cout << addmsg;
						temp = "s: 200 OK\n";
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
						break;
					} //MSGSTORE Command Ends
					
				}else if (strcmp(buf, acc1.c_str()) == 10 || strcmp(buf, acc2.c_str()) == 10 || strcmp(buf, acc3.c_str()) == 10)//LOGIN Command Begins
				{
						temp = "s: 200 OK\n"; 
						loggedIn = true; //for users that are not the root
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
						break;
				}else if (strcmp(buf, acc0.c_str()) == 10) //if the user is the root
				{
				
					temp = "s: 200 OK\n";
					strcpy(buf, temp.c_str());
					send(new_s, buf, strlen(buf) + 1, 0);
					loggedIn = true;
					rootLogIn = true;
					break;
				}
				else if (strcmp(buf, login.c_str()) == 32) //error case for any other users
				{
						temp = "s: 410 Wrong UserID or Password\n";
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
						break;
						
				}//LOGIN Command Ends

				else if (strcmp(buf, logout.c_str()) == 10) //LOGOUT Command Begins
				{
					if (loggedIn == true)
					{
						loggedIn = false; //user is logged out, preventing user from using msgstore
						temp = "s: 200 OK\n";
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
					}
					if (rootLogIn)
					{
						rootLogIn = false; //added condition that has to be changed if user is the root
					}
				} //LOGOUT Command Ends

				else if (strcmp(buf, quit.c_str()) == 10) //QUIT Command Begins
				{
					temp = "s: 200 OK\n";
					strcpy(buf, temp.c_str()); //simply sends 200 OK, termination process on client side 
					send(new_s, buf, strlen(buf) + 1, 0);
					break;
				} //QUIT Command Ends

				else if (strcmp(buf, stdown.c_str()) == 10) //SHUTDOWN Command Begins
				{
					if (rootLogIn) //only can be used if the user is the root
					{
						temp = "s: 200 OK\n";
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
						shutdown = true;
						cout << "System has shutdown\n"; //outputs message, closes socket, terminates server
						close(new_s);
						exit(1);
						break;
					}
					else {
						temp = "s: 402 User not allowed\n";
						strcpy(buf, temp.c_str());
						send(new_s, buf, strlen(buf) + 1, 0);
						break;
					}
				} //SHUTDOWN Command Ends

				else{
					temp = "s: Enter a valid command\n"; //error message if user enters any other command
					strcpy(buf, temp.c_str());
					send(new_s, buf, strlen(buf) + 1, 0);
					break;
				}

			}
		
		}
		
		close(new_s);
	}
}
string convertToString(char* a, int size)
{
		string strng = a;
		return strng;
}

