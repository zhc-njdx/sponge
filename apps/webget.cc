#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;
/*
 * the example: visit http://cs144.keithw.org/hello
 * telnet cs144.keithw.org http
 * GET /hello HTTP/1.1
 * Host: cs144.keithw.org
 * 
 * in this example the "host" is "cs144.keithw.org" and "path" is "/hello"
 * -------------------- HINTS -------------------------------
 * 1.in HTTP each line must be ended with "\r\n"
 * 2.don't forget to include the "Connection: close" line in client's request
 * 3.make sure read and print all the output from the server until the socket reaches "EOF", a single call to read is not enough
 * 4.expect to write about ten lines of code
 * ----------------------------------------------------------
 *
 * */
void get_URL(const string &host, const string &path) {
    // Your code here.

    // You will need to connect to the "http" service on
    // the computer whose name is in the "host" string,
    // then request the URL path given in the "path" string.

    // Then you'll need to print out everything the server sends back,
    // (not just one call to read() -- everything) until you reach
    // the "eof" (end of file).
    
	Address host_addr(host, "80");
	TCPSocket client;
	client.connect(host_addr);

	client.write("GET "+path+" HTTP/1.1\r\nHost: "+ host +"\r\nConnection: close\r\n\r\n");
	string response;
	while(!client.eof()){
		client.read(response);
		std::cout << response;
	}

//	 cerr << "Function called: get_URL(" << host << ", " << path << ").\n";
//     cerr << "Warning: get_URL() has not been implemented yet.\n";
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
