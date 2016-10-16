#include "data.h"
#include <vector>
class Analyze
{
    private:
        vector <matchup_t> matchups;
        float avgYdsAllowedPerGame;
        float avgPassYdsAllowedPerGame;
        float avgRunYdsAllowedPerGame;
        float avgTotalYdsPerGame;
        float stdDevYdsAllowedPerGame;
        float stdDevPassYdsAllowedPerGame;
        float stdDevRunYdsAllowedPerGame;
        float stdDevTotalYdsPerGame;
        void getAvg();
        void getStdDev();
        bool isEliteDefense(float);
        bool isBadPassDefense(float);
        bool isBadRunDefense(float);
        bool isEliteOffense(float);
        
    public:
        Analyze(vector <matchup_t> );
        void analyzeOffense(matchup_t &);
        void analyzeDefense(matchup_t &);
        void analyzeSpecialTeams(matchup_t &);
        void homeField(matchup_t &);


};
