#include "web.h"
#include "parser.h"
/*
* pass in the host("www.nfl.com") to connect to with an optional port
* the constructor will populate the private members sockfd and sockaddr_in which will
* be used in the connect() method as well as retrieve the dotted quad IP 
* which can be retrieved via get_ip()
*/
Web::Web(string host, int port)
{
    struct addrinfo hints = {0};
    struct addrinfo *p, *lladdrs;

    hostname = host;
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(host.c_str(), "http", &hints, &lladdrs);
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
vector<string> Web::get( string page )
{
    char send_buf[128];
    char recv_buf[1024];
    char *all_data;
    vector<string> err;
    int chunkedLen;
    int i;
    int res, bytesRead = 0;


    memset(send_buf, 0, sizeof(send_buf));
    memset(recv_buf, 0, sizeof(recv_buf));
    snprintf( send_buf, sizeof( send_buf ), "GET %s HTTP/1.0\r\n", page.c_str() );
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


    bytesRead += res;
    all_data = (char *) malloc (sizeof(char) * res);
    memcpy(all_data, recv_buf, res);
    
    while ( res > 0  )
    {
        memset( recv_buf, 0x0, sizeof( recv_buf ) );
        
        res = recv( sockfd, recv_buf, sizeof( recv_buf ) - 1, 0 );
        if (res > 0 )
        {
            all_data =  (char *) realloc(all_data, (( sizeof(char) * res) + bytesRead ));
            memcpy(all_data+bytesRead, recv_buf, res);
            bytesRead += res;
        }

    }

    parse_by_newlines(all_data);
#ifdef DEBUG
    puts(all_data);
    printf("bytesRead = %d\n", bytesRead);
#endif

    return data;
}
