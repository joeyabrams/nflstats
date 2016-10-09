//test
#include "web.h"
#include "parser.h"


int main( int argc, char **argv )
{
    vector<matchup_t> matchups;
    Web *web = new Web("www.nfl.com");
    web->conn();
    Parser *p = new Parser(web->get("/schedules/2016/REG6"));
    p->findMatchups(matchups);
    p->printMatchups(matchups);
    matchup_t m;

    web = new Web("www.sportingcharts.com");
    web->conn();
    
    p = new Parser(web->get("/nfl/stats/team-passing-statistics/2016/"));

    //p = new Parser(web->get("/nfl/stats/team-yards-against-per-game/2016/"));
    //web->peek();
    //p->findPassingStats();

    p->getYDSAllowed(matchups[3]);
}
