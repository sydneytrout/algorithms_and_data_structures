#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

struct Candy {
    int weight;
    int taste;
    double value;
    int bag;
    Candy(int w, int t){
        weight = w;
        taste = t;
        value = (double)t / (double)w;
        bag = 3;
    }
};

struct Bag{
    int weight;
    int total_taste;
    Bag(int w, int t){
        weight = w;
        total_taste = t;
    }
};

vector <Candy> candies;
vector <Bag> bags;

bool sort_by_value(const Candy &x, const Candy &y){
    return x.value > y.value;
}

void clear_bags(){
    for(int i = 0; i < 3; i++){
         bags[i].weight=0;
         bags[i].total_taste=0;
    }
    for(int i = 0; i < candies.size(); i++){
        candies[i].bag = 3;
    }
}

int greedy(){
    sort(candies.begin(), candies.end(), sort_by_value);
    for(int i = 0; i < candies.size(); i++){
        int w = candies[i].weight;
        int t = candies[i].taste;
        for(int j=0;j<3;j++){
            if(bags[j].weight + w <= 2000){
                bags[j].weight += w;
                bags[j].total_taste += t;
                break;
            }else continue;
        }
    }
    int total = 0;
    for(auto x: bags){
        total = total + x.total_taste;
    }
    return total;
}

void move_bag(int index, int bag){
    if(bag == 0){
        if(bags[1].weight < bags[2].weight){
            candies[index].bag = 1;
        }
        else{
            candies[index].bag = 2;
        }
    }
    else if(bag == 1){
        if(bags[0].weight < bags[2].weight){
            candies[index].bag = 0;
        }
        else{
            candies[index].bag = 2;
        }
    }
    else{
        if(bags[0].weight < bags[1].weight){
            candies[index].bag = 0;
        }
        else{
            candies[index].bag = 1;
        }
    }
}

void repair_bags(int s, int e){
    int other = 3 - (s + e);
    for(int i = candies.size()-1; i >= 0; i--){
        while(bags[e].weight > 2000){
            if(candies[i].bag == e){
                bags[e].weight -= candies[i].weight;
                bags[e].total_taste -= candies[i].taste; 
                candies[i].bag = 3; 
            }
        }
    }
    for(int i = 0; i < candies.size(); i++){
        if(candies[i].bag == 3){
            if(bags[s].weight + candies[i].weight <= 2000){
                bags[s].weight += candies[i].weight;
                bags[s].total_taste += candies[i].taste; 
                candies[i].bag = s; 
            }
            else if(bags[other].weight + candies[i].weight <= 2000){
                bags[other].weight += candies[i].weight;
                bags[other].total_taste += candies[i].taste; 
                candies[i].bag = other;
            }
            else continue;
        }
    }
}

int refine(){
    int best_total = 0;
    int T = 1000;
    for(int z = 0; z < T; z++){
        random_shuffle(candies.begin(), candies.end());
        for(int i = 0; i < candies.size(); i++){
            int w = candies[i].weight;
            int t = candies[i].taste;
            for(int j = 0; j < 3; j++){
                if(bags[j].weight+w<=2000){
                    bags[j].weight+=w;
                    bags[j].total_taste+=t;
                    candies[i].bag=j;
                    break;
                }
                else continue;
            }
        }
        vector<Candy> tempc = candies;
        vector<Bag> tempb = bags;
        for(int i = 0; i < candies.size(); i++){
            int start_bag;
            int end_bag;
            if(candies[i].bag != 3){
                start_bag = candies[i].bag;
                move_bag(i, start_bag);
                end_bag = candies[i].bag;
            }
            sort(candies.begin(), candies.end(), sort_by_value);
            repair_bags(start_bag, end_bag);
        }
        int total = 0;
        for(auto x: bags){
            total = total + x.total_taste;
        }
        if(total > best_total){
            best_total = total;
        }
        else{
            candies = tempc;
            bags = tempb;
        }
        clear_bags();
    }
    return best_total;
}

bool check_valid(int candy, int bag){
    if (bag == 3) return true;
    if(bags[bag].weight + candies[candy].weight >= 2000){
        return false;
    }
    return true;
}

int etotal = 0;
int ebesttotal = 0;

int get_upperbound(int index){
    int ub = 0;
    for(int i = index; i < candies.size(); i++){
        ub += candies[i].taste;
    }
    return ub;
}

int get_hvd(int index){
    int weight = 0;
    for(int i = 0; i < 3; i++){
            weight = weight + bags[i].weight;
    }
    return candies[index+1].value * (6000 - weight);
}

void exhaustive(int index){
    if(index == candies.size()){
        etotal = 0;
        for(auto x: bags){
            etotal = etotal + x.total_taste;
        }
        if(etotal > ebesttotal){
            ebesttotal = etotal;
        }
        return;
    }
    int cv = 0;
    for(auto x: bags){
            cv = cv + x.total_taste;
    }
    if(cv + get_upperbound(index) < ebesttotal) return;
    if((index < candies.size() - 1) && (cv + get_hvd(index) < ebesttotal)) return;
    if(index == 0){
        for(int j = 2; j < 4; j++){
            if(check_valid(index,j)){
                candies[index].bag = j;
                bags[j].weight += candies[index].weight;
                bags[j].total_taste += candies[index].taste;
                exhaustive(index+1);
                candies[index].bag = 3;
                bags[j].weight -= candies[index].weight;
                bags[j].total_taste -= candies[index].taste;
            }
        }
    }
    else{
        for(int j = 0; j < 4; j++){
            if(check_valid(index,j)){
                candies[index].bag = j;
                bags[j].weight += candies[index].weight;
                bags[j].total_taste += candies[index].taste;
                exhaustive(index+1);
                candies[index].bag = 3;
                bags[j].weight -= candies[index].weight;
                bags[j].total_taste -= candies[index].taste;
            }
        }
    }

}

int main(){
    int w;
    int t;
    int i = 0;
    while(cin>>w>>t){
        candies.push_back(Candy(w,t));
    }
    for(int i = 0; i < 3; i++){
        bags.push_back(Bag(0,0));
    }
    int sum = greedy();
    cout<<"Greedy: "<<sum<<endl;
    clear_bags();

    sum = refine();
    cout<<"Refinement: "<<sum<<endl;

    clear_bags();
    sort(candies.begin(), candies.end(), sort_by_value);
    exhaustive(0);
    cout<<"Exhaustive: " << ebesttotal <<endl;
}