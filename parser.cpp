#include "parser.h"
#include "web.h"

Parser::Parser( vector<string> d )
{
    this->data = d;
}
/*
* just iterate through and print out matchups
* todo: overload ostream maybe to make it print more naturally
*/

void Parser::printMatchups()
{
    vector<matchup_t>::const_iterator it;

    for ( it=matchups.begin(); it != matchups.end(); it++)
        cout << it->awayTeam << " vs " << it->homeTeam << endl;
}
/*
* this function will search for "awayAbbr" and "homeAbbr" to extract
* the away and home teams -- they are in order on the webpage so home teams
* will follow away teams.
* There can be duplicates on the page so we do a set.count("teamname") to avoid
* processing duplicates.
*/

void Parser::findMatchups()
{
    vector<string>::const_iterator it;
    set<string> teams;
    int total_matches = 0;
    matchup_t match;

    for(it=data.begin(); it != data.end(); it++)
    {
        if ( it->find("awayAbbr") != string::npos )
        {
#ifdef DEBUG
            cout << *it << endl;
#endif
            string tmp = *it;
            int spos = tmp.find_first_of(':') + 2;
            int epos = tmp.find_first_of('-', spos) - 1;
#ifdef DEBUG
            cout << "SPOS = " << spos << "LEN = " << (epos - spos) << endl;
#endif
            string team =  tmp.substr(spos, (epos - spos));
            /*
            * Ensure we havent processed this team already by checking the set.
            * if count returns 0 then add the team to the set to avoid processing
            * it in the future and set match.awayTeam to teamName
            */
            if (!teams.count(team))
            {
                teams.insert(team);
                match.awayTeam = team;
            }

        }
        else if ( it->find("homeAbbr") != string::npos )
        {
#ifdef DEBUG
            cout << *it << endl;
#endif
            string tmp = *it;
            int spos = tmp.find_first_of(':') + 2;
            int epos = tmp.find_first_of('-', spos) - 1;
#ifdef DEBUG
            cout << "SPOS = " << spos << "LEN = " << (epos - spos) << endl;
#endif
            string team =  tmp.substr(spos, (epos - spos));

            /*
            * The home team always follows the away team so we wait until
            * we retrieve a home team before we add it to our vector
            * at this point match.awayTeam should already be set.
            */
            if (!teams.count(team))
            {
                teams.insert(team);
                match.homeTeam = team;

                matchups.push_back(match);
            }
        }
    }
}

void Parser::findStats()
{
    vector<string>::const_iterator it;

    for (it=data.begin(); it!=data.end(); it++)
    {
        if (it->find("td style") != string::npos)
        {
            int spos = 0;
            int epos = 0;

            string tmp;
#ifdef DEBUG
            cout << *it << endl;
#endif
            spos = it->find("'>", 0) + 2;


        }

    }

}

