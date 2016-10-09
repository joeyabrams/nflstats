#include "parser.h"
#include "web.h"

Parser::Parser( vector<string> d )
{

    teamAbbreviations = { {"SD", "New York Giants"}, 
    { "DEN", "Denver Broncos"},
    { "SF", "San Francisco 49ers"},
    { "BUF", "Buffalo Bills"},
    { "PHI", "Philadelphia Eagles"},
    { "WAS", "Washington Redskins"},
    { "CLE", "Cleveland Browns"},
    { "TEN", "Tennessee Titans"},
    { "BAL", "Baltimore Ravens"},
    { "NYG", "New York Giants"},
    { "CAR", "Carolina Panthers"},
    { "NO", "New Orleans Saints"},
    { "JAX", "Jacksonville Jaguars"},
    { "CHI", "Chicago Bears"},
    { "LA", "Los Angeles Rams"},
    { "DET", "Detroit Lions"},
    { "PIT", "Pittsburgh Steelers"},
    { "MIA", "Miami Dolphins"},
    { "CIN", "Cincinnati Bengals"},
    { "NE", "New England Patriots"},
    { "KC", "Kansas City Chiefs"},
    { "OAK", "Oakland Raiders"},
    { "ATL", "Atlanta Falcons"},
    { "SEA", "Seattle Seahawks"},
    { "DAL", "Dallas Cowboys"},
    { "GB", "Green Bay Packers"},
    { "IND", "Indianapolis Colts"},
    { "HOU", "Houston Texans"},
    { "NYJ", "New York Jets"},
    { "ARI", "Arizona Cardinals"}};
    this->data = d;
}


/*
* just iterate through and print out matchups
* todo: overload ostream maybe to make it print more naturally
*/

void Parser::printMatchups(vector<matchup_t> matchups)
{
    vector<matchup_t>::const_iterator it;

    for ( it=matchups.begin(); it != matchups.end(); it++)
        cout << it->awayTeam.teamName << " vs " << it->homeTeam.teamName << endl;
}
/*
* this function will search for "awayAbbr" and "homeAbbr" to extract
* the away and home teams -- they are in order on the webpage so home teams
* will follow away teams.
* There can be duplicates on the page so we do a set.count("teamname") to avoid
* processing duplicates.
*/

void Parser::findMatchups(vector<matchup_t> & matchups)
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
                match.awayTeam.teamName = team;
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
                match.homeTeam.teamName = team;

                matchups.push_back(match);
            }
        }
    }
}

/*
* strbuf = html table/tags to parse
* row = column to retrieve data from
* returns data from the specified row
*
* handles edge cases where there is a link embedded inside
* the row ie: <td><a href..</a></td> instread of <td> data </td>
*
*/

string Parser::getDataFromColumn(string strbuf, int row)
{
    int start_pos = 0;
    int end_pos = 0;
    int len = 0;


    if (strbuf.find("td style") != string::npos)
    {
        for ( int i = 0; i < row; i++)
            start_pos = strbuf.find("td style", start_pos) + 1;

        start_pos = strbuf.find(">", start_pos) +1;
        if (strbuf[start_pos+1] == 'a')
            start_pos = strbuf.find(">", start_pos) + 1;
            
        end_pos = strbuf.find("<", start_pos);
        len = end_pos - start_pos;
        
        return strbuf.substr(start_pos, len);

    }

    return "null";

}


void Parser::getYDSAllowed(matchup_t &m)
{
    vector<string>::const_iterator it;
    string strData;
    int current_position = 0;

    for (it=data.begin(); it!=data.end(); it++)
    {
        strData =   getDataFromColumn(*it, 2);
        if (strData == teamAbbreviations[m.awayTeam.teamName])
            m.awayTeam.ydsAllowedPerGame = stof(getDataFromColumn(*it, 5));
        else if (strData == teamAbbreviations[m.homeTeam.teamName])
            m.homeTeam.ydsAllowedPerGame = stof(getDataFromColumn(*it, 5));
        
    }

}

string Parser::remove_comma(string buf)
{
    if (buf.find(",") != string::npos)
    {
        int pos = buf.find(",");
        string tmp = buf.substr(0, pos);
        tmp += buf.substr(pos+1);
        return tmp;
    }
    
    else
        return buf;
}
void Parser::getTotalYDS(matchup_t &m)
{
    vector<string>::const_iterator it;
    string strData;
    int current_position = 0;

    for (it=data.begin(); it!=data.end(); it++)
    {
        strData =   getDataFromColumn(*it, 2);
        if (strData == teamAbbreviations[m.awayTeam.teamName])
            m.awayTeam.totalYdsPerGame = stof(remove_comma(getDataFromColumn(*it, 3)));
        else if (strData == teamAbbreviations[m.homeTeam.teamName])
            m.homeTeam.totalYdsPerGame = stof(remove_comma(getDataFromColumn(*it, 3)));
        
    }

}
