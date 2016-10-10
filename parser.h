#include <vector>
#include <string>
#include <map>
#include "data.h"

using namespace std;


class Parser 
{
    private:
        vector<string> data;
        map<string, string> teamAbbreviations;
        string remove_comma(string);
    public:
        Parser(vector<string>);
        void findMatchups(vector<matchup_t> &);
        void findPassingStats();
        void printMatchups(vector<matchup_t>);
        string getDataFromColumn(string, int);
        
        
        void getYDSAllowed(matchup_t &);
        void getTotalYDS(matchup_t &);

        void getTakeAways(matchup_t &);
        void getRushYDSAllowed(matchup_t &);
};
