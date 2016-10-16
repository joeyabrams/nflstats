#include <string>
using namespace std;
#ifndef DATA_H
#define DATA_H
typedef struct teamData
{
    string teamName;
    float ydsAllowedPerGame;
    float runYdsAllowedPerGame;
    float passYdsAllowedPerGame;
    float totalYdsPerGame;
    float rushPercentage;
    float passPercentage;
    int takeAways;
    float confidence = 0.5;
} teamData_t;

typedef struct matchup
{
    teamData_t awayTeam;
    teamData_t homeTeam;
} matchup_t;
#endif
