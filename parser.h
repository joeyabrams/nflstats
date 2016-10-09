#include <vector>
#include <string>
#include <map>

using namespace std;

typedef struct matchup
{
    string awayTeam;
    string homeTeam;
} matchup_t;

class Parser 
{
    private:
        vector<string> data;
        map<string, string> teamAbbreviations;
    public:
        Parser(vector<string>);
        void findMatchups(vector<matchup_t> &);
        void findPassingStats();
        void printMatchups(vector<matchup_t>);
        string getDataFromColumn(string, int);
        void getYDSAllowed(matchup_t);
        

};
