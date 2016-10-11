//test
#include "web.h"
#include "parser.h"


int main( int argc, char **argv )
{
    vector<matchup_t> matchups;
    vector<matchup_t>::iterator it;

    Web *web = new Web("www.nfl.com");
    web->conn();
    Parser *p = new Parser(web->get("/schedules/2016/REG6"));
    p->findMatchups(matchups);
    p->printMatchups(matchups);
    matchup_t m;

    web = new Web("www.sportingcharts.com");
    web->conn();
    
    //p = new Parser(web->get("/nfl/stats/team-passing-statistics/2016/"));

    p = new Parser(web->get("/nfl/stats/team-yards-against-per-game/2016/"));
    //web->peek();
    //p->findPassingStats();


    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getYDSAllowed(*it);
    
    delete web;

    web = new Web("www.sportingcharts.com");
    web->conn();
    p = new Parser(web->get("/nfl/stats/team-overall-yards-statistics/2016/"));


    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getTotalYDS(*it);

    delete web;

    web = new Web("www.sportingcharts.com");
    web->conn();
    p = new Parser(web->get("/nfl/stats/team-takeaway-giveaway-statistics/2016/"));


    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getTakeAways(*it);

    delete web;

    web = new Web("www.sportingcharts.com");
    web->conn();
    p = new Parser(web->get("/nfl/stats/team-rushing-yards-against-per-game/2016/"));

    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getRushYDSAllowed(*it);

    m = matchups[5];

    cout << "==================" << endl;
    cout << m.awayTeam.teamName << endl;
    cout << "Defense" << endl;
    cout << "Total YDS Allowed per Game: " << m.awayTeam.ydsAllowedPerGame << endl;
    cout << "Run YDS Allowed per Game: " << m.awayTeam.runYdsAllowedPerGame << endl;
    cout << "Offsense" << endl;
    cout << "Total YDS Gained per Game: " << m.awayTeam.totalYdsPerGame << endl;
    cout << "Rush %: " << m.awayTeam.rushPercentage << "\t" << "Pass %: " << m.awayTeam.passPercentage << endl;
    cout << "Special Teams" << endl;
    cout << "Takeaways +/-: " << m.awayTeam.takeAways << endl;








}
