#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

int main (void){
    vector <pair<int,string> > offense_and_team;
    vector <pair<int,int> > offense_and_defense;
    string team;
    int offense;
    int defense;

    while(cin >> team >> offense >> defense){
        offense_and_team.push_back(make_pair(offense,team));
        offense_and_defense.push_back(make_pair(offense, defense));
    }

    sort(offense_and_team.rbegin(), offense_and_team.rend());
    sort(offense_and_defense.rbegin(), offense_and_defense.rend());

    set<string> safe;
    int best = 0;

    for(int it = 0; it < offense_and_defense.size(); it++){
        if(offense_and_defense[it].second > best){
            best = offense_and_defense[it].second;
        }
        if(offense_and_defense[it].second >= best) {
            safe.insert(offense_and_team[it].second);
        }
    }

    cout << "Teams that are safe:" << endl;
    for(auto it = safe.begin(); it != safe.end(); it++){
        cout << *it << endl;
    }
}

