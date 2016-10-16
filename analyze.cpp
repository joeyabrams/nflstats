#include "analyze.h"
#include <math.h>
#include <iostream>

using namespace std;

#define HOME_FIELD_ADV  0.05

Analyze::Analyze(vector<matchup_t> matchups)
{
    this->matchups = matchups;

    avgYdsAllowedPerGame = 0;
    avgPassYdsAllowedPerGame = 0;
    avgRunYdsAllowedPerGame = 0;
    avgTotalYdsPerGame = 0;
    stdDevYdsAllowedPerGame = 0;
    stdDevPassYdsAllowedPerGame = 0;
    stdDevRunYdsAllowedPerGame = 0;
    stdDevTotalYdsPerGame = 0;
    getAvg();
    getStdDev();
}

/*void dump(){
    m = matchup;
    m.awayTeam.confidence -= HOME_FIELD_ADV;
    m.homeTeam.confidence += HOME_FIELD_ADV;
    

#ifdef DEBUG
    cout << "Away Confidence: " << m.awayTeam.confidence << endl;
    cout << "Home Confidence: " << m.homeTeam.confidence << endl;
#endif
}*/

void Analyze::homeField(matchup_t &m)
{
    m.homeTeam.confidence += 0.02;
    m.awayTeam.confidence -= 0.02;
}
bool Analyze::isEliteOffense(float totalYds)
{
    if (totalYds > ( avgTotalYdsPerGame + stdDevTotalYdsPerGame ) )
        return true;
    return false;
}
void Analyze::analyzeSpecialTeams(matchup_t &m)
{
    if (m.homeTeam.takeAways > 0 )
    {
        m.homeTeam.confidence += 0.02;
        m.awayTeam.confidence -= 0.02;
    }

    if (m.awayTeam.takeAways > 0 )
    {
        m.awayTeam.confidence += 0.02;
        m.homeTeam.confidence -= 0.02;
    }
}
void Analyze::analyzeOffense(matchup_t &m)
{
    if (m.homeTeam.totalYdsPerGame > m.awayTeam.totalYdsPerGame)
    {
        cout << "Better Overall Offense: " << m.homeTeam.teamName << endl;
        m.homeTeam.confidence += 0.05;
        m.awayTeam.confidence -= 0.05;

        if (isEliteOffense(m.homeTeam.totalYdsPerGame))
        {
            cout << "Elite Offense: " << m.homeTeam.teamName << endl;
            m.homeTeam.confidence += 0.05;
            m.awayTeam.confidence -= 0.05;
        }
    }

    else 
    {
        cout << "Better Overall Offense: " << m.awayTeam.teamName << endl;
        m.awayTeam.confidence += 0.05;
        m.homeTeam.confidence -= 0.05;

        if (isEliteOffense(m.awayTeam.totalYdsPerGame))
        {
            cout << "Elite Offense: " << m.awayTeam.teamName << endl;
            m.awayTeam.confidence += 0.05;
            m.homeTeam.confidence -= 0.05;
        }
    }

}

bool Analyze::isBadPassDefense(float ydsAllowed)
{
    if (ydsAllowed > (avgPassYdsAllowedPerGame + stdDevPassYdsAllowedPerGame ) )
        return true;
    return false;
}

bool Analyze::isBadRunDefense(float ydsAllowed)
{
    if (ydsAllowed > (avgRunYdsAllowedPerGame + stdDevRunYdsAllowedPerGame ) )
        return true;
    return false;
}

bool Analyze::isEliteDefense(float ydsAllowed)
{
    cout << "Yds Allowed: " << ydsAllowed << "Std Dev: " << (avgYdsAllowedPerGame - stdDevYdsAllowedPerGame ) << endl;
    if (ydsAllowed <= ( avgYdsAllowedPerGame - stdDevYdsAllowedPerGame ) )
        return true;
    else
        return false;
}
void Analyze::analyzeDefense(matchup_t &m)
{
    if (m.homeTeam.ydsAllowedPerGame > m.awayTeam.ydsAllowedPerGame)
    {
        cout << "Better Overall Defense: " << m.awayTeam.teamName << endl;
        m.awayTeam.confidence += 0.05;
        m.homeTeam.confidence -= 0.05;
                
        if (isEliteDefense(m.awayTeam.ydsAllowedPerGame) )
        {
            //elite defense, below 1 stddeviation
            m.awayTeam.confidence += 0.05;
            m.homeTeam.confidence -= 0.05;
            cout << "Elite Defense: " << m.awayTeam.teamName << endl;
            if (isEliteDefense(m.homeTeam.ydsAllowedPerGame))
            {
                m.awayTeam.confidence -= 0.05;
                m.homeTeam.confidence += 0.05;
                cout << "Elite Defense: " << m.homeTeam.teamName << " Cancels out Adv. " << endl;
            }
        } 
    }

    else
    {
        cout << "Better Overall Defense: " << m.homeTeam.teamName << endl;
        m.homeTeam.confidence += 0.05;
        m.awayTeam.confidence -= 0.05;

        if (isEliteDefense(m.homeTeam.ydsAllowedPerGame) )
        {
            //elite defense, below 1 stddeviation
            m.homeTeam.confidence += 0.05;
            m.awayTeam.confidence -= 0.05;
            cout << "Elite Defense: " << m.awayTeam.teamName << endl;
            if (isEliteDefense(m.awayTeam.ydsAllowedPerGame))
            {
                m.homeTeam.confidence -= 0.05;
                m.awayTeam.confidence += 0.05;
                cout << "Elite Defense: " << m.awayTeam.teamName << " Cancels out Adv. " << endl;
            }
        } 
    }


    if ( ( m.homeTeam.passPercentage > 60 )  && isBadPassDefense(m.awayTeam.passYdsAllowedPerGame))
    {
        cout <<m.homeTeam.teamName << "Passing Team playing poor Pass Defense: " << m.awayTeam.teamName << endl;
        m.homeTeam.confidence += 0.05;
        m.awayTeam.confidence -=0.05;
    }

    if ( ( m.awayTeam.passPercentage > 60 )  && isBadPassDefense(m.homeTeam.passYdsAllowedPerGame))
    {
        cout << m.awayTeam.teamName << "Passing Team playing poor Pass Defense: " << m.homeTeam.teamName << endl;
        m.awayTeam.confidence += 0.05;
        m.homeTeam.confidence -=0.05;
    }


    if ( (m.homeTeam.rushPercentage > 60 ) && isBadRunDefense(m.awayTeam.runYdsAllowedPerGame))
    {
        cout << "Running Team playing poor Run Defense: " << m.awayTeam.teamName << endl;
        m.homeTeam.confidence += 0.05;
        m.awayTeam.confidence -= 0.05;
    }

    if ( (m.awayTeam.rushPercentage > 60 ) && isBadRunDefense(m.homeTeam.runYdsAllowedPerGame))
    {
        cout << "Running Team playing poor Run Defense: " << m.homeTeam.teamName << endl;
        m.awayTeam.confidence += 0.05;
        m.homeTeam.confidence -= 0.05;
    }
    
}

void Analyze::getAvg()
{
    vector<matchup_t>::const_iterator it;

    for (it=matchups.begin(); it != matchups.end(); it++)
    {

        avgYdsAllowedPerGame += it->homeTeam.ydsAllowedPerGame;
        avgPassYdsAllowedPerGame += it->homeTeam.passYdsAllowedPerGame;
        avgRunYdsAllowedPerGame += it->homeTeam.runYdsAllowedPerGame;
        avgTotalYdsPerGame += it->homeTeam.totalYdsPerGame;

        avgYdsAllowedPerGame += it->awayTeam.ydsAllowedPerGame;
        avgPassYdsAllowedPerGame += it->awayTeam.passYdsAllowedPerGame;
        avgRunYdsAllowedPerGame += it->awayTeam.runYdsAllowedPerGame;
        avgTotalYdsPerGame += it->awayTeam.totalYdsPerGame;
#ifdef DEBUG
        cout << "analyze::getAvg Added: " << it->awayTeam.totalYdsPerGame << " and " << it->homeTeam.totalYdsPerGame << endl;
#endif
    }


    avgYdsAllowedPerGame = avgYdsAllowedPerGame / (matchups.size() * 2);
    avgPassYdsAllowedPerGame = avgPassYdsAllowedPerGame / (matchups.size() * 2);
    avgRunYdsAllowedPerGame = avgRunYdsAllowedPerGame / (matchups.size() * 2);
    avgTotalYdsPerGame = avgTotalYdsPerGame / (matchups.size() * 2 );


    cout << "Avg. yds allowed per game: " << avgYdsAllowedPerGame << endl;
    cout << "Avg. pass yds allowed per game: " << avgPassYdsAllowedPerGame << endl;
    cout << "Avg. run yds allowed per game: " << avgRunYdsAllowedPerGame << endl;
    cout << "Avg. yds per game: " << avgTotalYdsPerGame << endl;
}

void Analyze::getStdDev()
{
    vector<matchup_t>::const_iterator it;
    float sum = 0;

    vector<float>::const_iterator itf;

    vector<float> total_ydsAllowedPerGame;
    vector<float> total_passYdsAllowedPerGame;
    vector<float> total_runYdsAllowedPerGame;
    vector<float> total_totalYdsPerGame;

    for (it=matchups.begin(); it != matchups.end(); it++)
    {

        total_ydsAllowedPerGame.push_back( pow ( ( it->homeTeam.ydsAllowedPerGame - avgYdsAllowedPerGame ), 2 ) );
        total_passYdsAllowedPerGame.push_back( pow ( ( it->homeTeam.passYdsAllowedPerGame - avgYdsAllowedPerGame ), 2 ) );
        total_runYdsAllowedPerGame.push_back( pow ( ( it->homeTeam.runYdsAllowedPerGame - avgYdsAllowedPerGame ), 2 ) );
        total_totalYdsPerGame.push_back( pow ( ( it->homeTeam.totalYdsPerGame - avgYdsAllowedPerGame ), 2 ) );

        total_ydsAllowedPerGame.push_back( pow ( ( it->awayTeam.ydsAllowedPerGame - avgYdsAllowedPerGame ), 2 ) );
        total_passYdsAllowedPerGame.push_back( pow ( ( it->awayTeam.passYdsAllowedPerGame - avgYdsAllowedPerGame ), 2 ) );
        total_runYdsAllowedPerGame.push_back( pow ( ( it->awayTeam.runYdsAllowedPerGame - avgYdsAllowedPerGame ), 2 ) );
        total_totalYdsPerGame.push_back( pow ( ( it->awayTeam.totalYdsPerGame - avgYdsAllowedPerGame ), 2 ) );
    }

    
    for (itf=total_ydsAllowedPerGame.begin(); itf != total_ydsAllowedPerGame.end(); itf++)
        sum += *itf;
    stdDevYdsAllowedPerGame = sqrt ( (sum / total_ydsAllowedPerGame.size() ) );

    sum = 0;


    for (itf=total_passYdsAllowedPerGame.begin(); itf != total_passYdsAllowedPerGame.end(); itf++)
        sum += *itf;
    stdDevPassYdsAllowedPerGame = sqrt ( (sum / total_passYdsAllowedPerGame.size() ) );

    sum = 0;


    for (itf=total_runYdsAllowedPerGame.begin(); itf != total_runYdsAllowedPerGame.end(); itf++)
        sum += *itf;
    stdDevRunYdsAllowedPerGame = sqrt ( (sum / total_runYdsAllowedPerGame.size() ) );


    sum = 0;

    for (itf=total_totalYdsPerGame.begin(); itf != total_totalYdsPerGame.end(); itf++)
        sum += *itf;
    stdDevTotalYdsPerGame = sqrt ( (sum / total_totalYdsPerGame.size() ) );

    cout << "Std. Dev. Pass YDS Allowed: " << stdDevPassYdsAllowedPerGame << endl;
    cout << "Std. Dev. Run YDS Allowed: " << stdDevRunYdsAllowedPerGame << endl;
    cout << "Std. Dev. YDS Allowed: " << stdDevYdsAllowedPerGame << endl;
    cout << "Std. Dev. Total YDS: " << stdDevTotalYdsPerGame << endl;
}
