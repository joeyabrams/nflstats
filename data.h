#include <string>
using namespace std;
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
} teamData_t;

typedef struct matchup
{
    teamData_t awayTeam;
    teamData_t homeTeam;
} matchup_t;
