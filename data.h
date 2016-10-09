#include <string>
using namespace std;
typedef struct teamData
{
    string teamName;
    float ydsAllowedPerGame;
    float totalYdsPerGame;
    float rushPercentage;
    float passPercentage;
} teamData_t;

typedef struct matchup
{
    teamData_t awayTeam;
    teamData_t homeTeam;
} matchup_t;
