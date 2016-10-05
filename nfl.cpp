#include "web.h"
#include "parser.h"

int main( int argc, char **argv )
{
    Web *web = new Web("www.nfl.com");
    web->conn();
    Parser *p = new Parser(web->get("/schedules/2016/REG5"));
    p->findMatchups();
    p->printMatchups();

    web = new Web("www.sportingcharts.com");
    web->conn();
    

    p = new Parser(web->get("/nfl/stats/team-passing-statistics/2016/"));
    //web->peek();
    p->findStats();
}
