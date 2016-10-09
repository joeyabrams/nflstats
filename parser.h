#include <vector>
#include <string>

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
        vector<matchup_t> matchups;
    public:
        Parser(vector<string>);
        void findMatchups();
        void findPassingStats();
        void printMatchups();
        string getDataFromColumn(string, int);
        void getYDSAllowed();

};
