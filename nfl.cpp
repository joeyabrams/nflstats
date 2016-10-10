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
    web = new Web("www.sportingcharts.com");
    web->conn();
    p = new Parser(web->get("/nfl/stats/team-overall-yards-statistics/2016/"));


    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getTotalYDS(*it);


    web = new Web("www.sportingcharts.com");
    web->conn();
    p = new Parser(web->get("/nfl/stats/team-takeaway-giveaway-statistics/2016/"));


    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getTakeAways(*it);

    web = new Web("www.sportingcharts.com");
    web->conn();
    p = new Parser(web->get("/nfl/stats/team-rushing-yards-against-per-game/2016/"));

    for (it=matchups.begin(); it != matchups.end(); it++)
        p->getRushYDSAllowed(*it);

    cout << "Yards Allowed: " << endl;
    for (it=matchups.begin(); it != matchups.end(); it++)
    {
        cout << it->awayTeam.teamName << " " << it->awayTeam.ydsAllowedPerGame << endl;
        cout << it->homeTeam.teamName << " " << it->homeTeam.ydsAllowedPerGame << endl;
    } 
    
    cout << "Total Yards: " << endl;
    for (it=matchups.begin(); it != matchups.end(); it++)
    {
        cout << it->awayTeam.teamName << " " << it->awayTeam.totalYdsPerGame << endl;
        cout << it->homeTeam.teamName << " " << it->homeTeam.totalYdsPerGame << endl;
    } 

    cout << "Run %: " << endl;
    for (it=matchups.begin(); it != matchups.end(); it++)
    {
        cout << it->awayTeam.teamName << " " << it->awayTeam.rushPercentage << endl;
        cout << it->homeTeam.teamName << " " << it->homeTeam.rushPercentage << endl;
    }

    cout << "Takeaways: " << endl;
    for (it=matchups.begin(); it != matchups.end(); it++)
    {
        cout << it->awayTeam.teamName << " " << it->awayTeam.takeAways << endl;
        cout << it->homeTeam.teamName << " " << it->homeTeam.takeAways << endl;
    }
}
