#include "web.h"
#include "parser.h"
/*
* pass in the host("www.nfl.com") to connect to with an optional port
* the constructor will populate the private members sockfd and sockaddr_in which will
* be used in the connect() method as well as retrieve the dotted quad IP 
* which can be retrieved via get_ip()
*/
Web::Web(char *host, int port)
{
    struct addrinfo hints = {0};
    struct addrinfo *p, *lladdrs;

    hostname = host;
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(host, "http", &hints, &lladdrs);
    memcpy(&addr_in, lladdrs->ai_addr, sizeof (struct sockaddr_in ) );
    str_ip = inet_ntoa(addr_in.sin_addr);

}
/*
* loop through and print out all data read from the given webpage in get()
* function. The data should be tokenized by "\r\n"
*
*/
void Web::peek()
{
    vector <string>::const_iterator it;

    for(it=data.begin(); it != data.end(); it++)
        cout << *it << endl;
}

string Web::get_ip()
{
    return str_ip;
}

/*
*  HTTP 1.1 allows a CHUNKED response, this function attempts
* to find the chunk size. 
* returns: numbers of bytes read up to the chunk size in the http header
* returns the chunked size in chunkedLen parameter.
* 
*/
int Web::getChunkedLength(char *header, int size, int *chunkedLen)
{
    char *p;
    char tmp[size];
    int i;

    strncpy(tmp, header, size);

    p = strtok(tmp, "\r\n");
    while ( p != NULL )
    {
        if (isdigit(*p))
        {
            i += strlen(p) + 2;
            *chunkedLen= strtol(p, NULL, 16);
            return i;
        }
        i += strlen(p) + 2;
        p = strtok(NULL, "\r\n");
    }
    
    return -1;
}
/*
* buf is the raw data retrieved from HTTP GET request performed by
* get() function -- parse it into the private vector of strings data by 
* using strtok to tokenize it by newlines.
*
* returns the number of lines/tokens processed
*/
int Web::parse_by_newlines(char *buf)
{
    char *p = buf;
    char *t;
    int i = 0;
    int flag = 0;

    if (data.size() > 0)
    {
        /*
        * if data already exists in our vector then we need to reassemble
        * the very last line/string of data as it most likely was broken up in 
        * the middle of a new line
        */
        string tmp = data.back();

        /*
        * convert the last string in our vector to a c style string.
        * copy into a buffer and then pop off the string from the vector
        * we then parse out the first line out of the data buffer which should be
        * the remaining portion of the last line we parsed out and reassemble
        * it up to the /r/n by concatenating them together and then pushing it 
        * back into place on the vector.
        *
        * BUG: On the off chance that the data in our buffer was read in perfectly
        * and finished reading exactly on /r/n this will end up concatenating two
        * lines together which should have been seperate strings in the vector 
        */
        if (tmp.length() > 0 )

        {
            flag++; //set a flag so that we know we started strtok() process an next
                    // call should be passed NULL as an argument
            char cstr[tmp.length()];
#ifdef DEBUG
            printf("parse_by_newlines():size=%d\n", tmp.length());
#endif
            strcpy(cstr, tmp.c_str());
            data.pop_back();
            p = strtok(buf, "\r\n");
            int len = strlen(p) + tmp.length() + 1;
            char combined_str[len];
            memset(combined_str, 0, sizeof(combined_str));
            strcpy(combined_str, cstr);
            strncat(combined_str, p, len);
            data.push_back(combined_str);
        }
    }
    /*
    * if flag = 0 it's our first call to this function so we haven't called
    * strtok() above to reassemble the last string in our vector so call it with
    * buf as parameter otherwise we've already started the strtok() process so call
    * it with NULL and continue
    */
    if (!flag)
        p = strtok(buf, "\r\n");
    while (p != NULL)
    {
        i++;
        data.push_back(p);
        p = strtok(NULL, "\r\n");
    }
    
    return i;
}

int Web::conn()
{
    return connect( sockfd, (struct sockaddr *) &addr_in, sizeof ( addr_in ) );
}
/*
* page is a webpage to read on the server. ie: "/index.html" 
* returns the webpage as a vector of strings
* empty vector returned on error
*/
vector<string> Web::get( char *page )
{
    char send_buf[128];
    char recv_buf[1024];
    vector<string> err;
    int chunkedLen;
    int i;
    int res, bytesRead = 0;


    memset(send_buf, 0, sizeof(send_buf));
    memset(recv_buf, 0, sizeof(recv_buf));
    snprintf( send_buf, sizeof( send_buf ), "GET %s HTTP/1.0\r\n", page );
#ifdef DEBUG
    printf( "req: [%s]\n", send_buf );
#endif
    res = send( sockfd, send_buf, strlen( send_buf ), 0 );
    
    if ( res < 0 )
        return err;
    
    memset(send_buf, 0, sizeof(send_buf));
    snprintf(send_buf, sizeof(send_buf), "Host: %s\r\n\r\n", hostname.c_str());
    //res = send( sockfd, "Host: www.nfl.com\r\n\r\n", strlen("Host: www.nfl.com\r\n\r\n"), 0 );

    res = send( sockfd, send_buf, strlen(send_buf), 0);
    
    if ( res < 0 )
        return err;
    
    res = recv( sockfd, recv_buf, sizeof( recv_buf ) - 1, 0 );

    if ( res < 0 )
        return err;
#ifdef DEBUG
    printf("%d bytes received\n", res);
#endif

    i = parse_by_newlines(recv_buf);



    bytesRead += res;

    while ( res > 0  )
    {
        memset( recv_buf, 0x0, sizeof( recv_buf ) );
        
        res = recv( sockfd, recv_buf, sizeof( recv_buf ) - 1, 0 );
        if ( res > 0 )
            parse_by_newlines(recv_buf);
        bytesRead += res;
    }
#ifdef DEBUG
    printf("bytesRead = %d\n", bytesRead);
#endif

    return data;
}
