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


void Parser::getYDSAllowed()
{
    vector<string>::const_iterator it;
    string strData;
    int current_position = 0;

    for (it=data.begin(); it!=data.end(); it++)
    {
        strData =   getDataFromColumn(*it, 2);
        if (strData != "null")
            cout <<"Team: " <<  strData;
        strData = getDataFromColumn(*it, 5);
        if (strData != "null")
            cout << "Avg. YDS Allowed per Game: " << strData << endl;
    }

}

void Parser::findPassingStats()
{
    vector<string>::const_iterator it;

    for (it=data.begin(); it!=data.end(); it++)
    {
        if (it->find("td style") != string::npos)
        {
            int start_pos = 0;
            int end_pos = 0;
            int len = 0;

            string tmp;
#ifdef DEBUG
            cout << *it << endl;
#endif
            start_pos = it->find("'>", 0) + 2;
            end_pos = it->find ("<", start_pos);
            len = end_pos - start_pos;
            cout << it->substr(start_pos, len) << endl;

            start_pos = end_pos;
           
            for (int i = 0; i <= 3; i++)
                start_pos = it->find("</td>", start_pos) + 2;
           

            start_pos = it->find("\">", start_pos) + 2;
            end_pos = it->find("<", start_pos);
            len = end_pos - start_pos;

            cout << "Passing YDS: " << it->substr(start_pos, len) << endl;

            start_pos = end_pos;

            for (int i = 0; i <= 2; i++)
                start_pos = it->find("</td>", start_pos) + 2;

            start_pos = it->find("\">", start_pos) + 2;
            end_pos = it->find("<", start_pos);
            len = end_pos - start_pos;

            cout << "TDs: " << it->substr(start_pos, len) << endl;



        }

    }

}

