#include <string.h>
#include <set>
#include <iostream>
//#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <fcntl.h>
#include <vector>
#include <iterator>
#include <errno.h>

using namespace std;

class Web
{
    private:
        int sockfd;
        struct sockaddr_in addr_in;
        string str_ip;
        string hostname;
        vector <string> data;
        int getChunkedLength(char *, int, int *);
        int parse_by_newlines(char *);
        
    public:

        Web(string host, int port=80);
        string get_ip ();
        int conn();
        vector<string> get(string);
        void peek();
};
