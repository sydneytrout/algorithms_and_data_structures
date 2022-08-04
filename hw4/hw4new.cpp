#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <cmath>
#include <vector>
#include <queue>
#include <set>

using namespace std;

struct Point{
    int x, y;
    bool operator< (const Point &p) const{
        if(x == p.x){
            return (y < p.y);
        }
        return (x < p.x);
    }

     bool operator!= (const Point &p) const{
        if(x == p.x){
            return !(y == p.y);
        }
        return !(x == p.x);
    }
    bool operator== (const Point &p) const{
        if(x == p.x){
            return (y == p.y);
        }
        return (x == p.x);
    }
};

struct pixel{
    unsigned char r, g, b;
};

struct Node{
    Point curr;
    pixel color;
    Point prev;
    uint8_t passengers;
    bool operator< (const Node &p) const {
        if(passengers == p.passengers){
            if(curr.x == p.curr.x){
                if(curr.y == p.curr.y){
                    if(prev.x == p.prev.x){
                        return prev.y < p.prev.y;
                    }
                    return prev.x < p.prev.x;
                }
                return curr.y < p.curr.y;
            }
            return curr.x < p.curr.x;
        }
        return passengers < p.passengers;
    }

    bool operator!= (const Node &p) const{
        if(curr.x == p.curr.x){
            return !(curr.y == p.curr.y);
        }
        return !(curr.x == p.curr.x);
    }

    bool operator== (const Node &p) const{
        if(curr.x == p.curr.x){
            return (curr.y == p.curr.y);
        }
        return (curr.x == p.curr.x);
    }
};

struct config{
    char ppmFile[25];
    Point start, goal, dean, lin, perez, reeder;
    int T;
    double minE;
    double maxE;
    double M;
    double D;
};

pixel *ppm;
int width, height;
config id;
pixel orange = {245,102,0};

vector<Node> all_nodes;
map<Node, double> dist;
map<Node, Node> pred;
map<pair<Node,Node>, double> edge_wt;
double dpComp;
double length;
vector<double> unit1;
vector<int> second;
double length2;
vector<double> unit2;
double product;

bool dotProduct(vector<double> &unit1, Point &curr, Point &next){
    second = {(next.x - curr.x), (next.y - curr.y)};
    length2 = sqrt(pow(second[0], 2.0) + pow(second[1],2.0));
    unit2 = {(second[0] / length2), (second[1] / length2)};
    product = (unit1[0] * unit2[0]) + (unit1[1] * unit2[1]);
    if(product >= dpComp)return true;
    return false;
}

int dean = 1;
int lin = (1 << 1);
int john = (1 << 2);
int inf = 999999;  
typedef pair<int,Node> pin;
uint8_t hasAll = dean | lin | john;

double get_dist(Point &x, Point &y){
    return sqrt(pow((x.x-y.x),2.0) + pow((x.y-y.y),2.0));
}

vector<Node> get_nbrs(Node current){
    Node nb;
    vector<Node> nbrs;
    int j = current.curr.y;
    int i = current.curr.x;
    int point1;
    int point2;
    double dist;
    length = get_dist(current.curr, current.prev);
    unit1 = {((current.curr.x - current.prev.x) / length), ((current.curr.y - current.prev.y) / length)};
    for(int s = -id.maxE; s <= id.maxE; s++){
        for(int t = -id.maxE; t <= id.maxE; t++){
            point1 = s+i;
            point2 = t+j;
            int access = (point2)*width + (point1);
            if(point1 > width || point1 < 0 || point2 > height || point2 < 0) continue;
            nb = all_nodes[access];
            nb.passengers = current.passengers;
            dist = sqrt((s*s) + (t*t));
            if(nb.color.r + nb.color.g + nb.color.b <= id.T){
                if(dist <= id.maxE && dist >= id.minE){
                   if(current.curr != id.start){
                       if(dotProduct(unit1, current.curr, nb.curr)){
                            nb.prev = current.curr;
                            if(get_dist(nb.curr,id.dean) <= id.D){
                                nb.passengers |= dean;
                            }
                            if(get_dist(nb.curr,id.lin) <= id.D){
                                nb.passengers |= lin;
                            }
                            if(get_dist(nb.curr,id.perez) <= id.D){
                                nb.passengers |= john;
                            }
                            if(get_dist(nb.curr,id.reeder) <= id.D){
                                nb.passengers |= john;
                            }
                            nbrs.push_back(nb);
                            edge_wt[make_pair(current, nb)] = dist;
                       }
                    }
                    else{
                        nb.prev = id.start;
                        nbrs.push_back(nb);
                        edge_wt[make_pair(current, nb)] = dist;
                    }
                }
            }
        }
    }
    return nbrs;
}


void dijkstra(Node source, Node dest)
{
  dist[source] = 0;
  priority_queue<pin, vector<pin>, greater<pin>> to_visit; 
  to_visit.push(make_pair(0, source));
  while (!to_visit.empty()) {
    Node x = to_visit.top().second;
    to_visit.pop();
    if((get_dist(x.curr, dest.curr)  <= id.D) && (x.passengers == hasAll) && x!=dest){
        pred[dest] = x;
        return;
    }
    for (Node n : get_nbrs(x)) {
        if (dist.find(n) == dist.end()){
            dist[n] = inf;
        }
        double weight = edge_wt[make_pair(x,n)];
        if (dist[x] + weight < dist[n]) {
            dist[n] = dist[x] + weight;
            pred[n] = x;
            to_visit.push(make_pair(dist[n], n));
        }
    }
  }
}


void writePPM(FILE* &ppm){
    fprintf(ppm, "P6\n%d %d\n255\n", width, height);
    for(int i = 0; i < width * height; i++){
        fprintf(ppm, "%c%c%c", all_nodes[i].color.r,all_nodes[i].color.g,all_nodes[i].color.b);
    }
}

bool check_color(int x, int y){
    return ((all_nodes[y*width + x].color.r == orange.r && all_nodes[y*width + x].color.g == orange.g && all_nodes[y*width + x].color.b == orange.b) && (all_nodes[y*width + x].color.r == 255 && all_nodes[y*width + x].color.g == 255 && all_nodes[y*width + x].color.b == 255));
}

void color(Node &dest, pixel &c){
    for(int i = -3; i <= 3; i++){
        for(int j = -3; j <= 3; j++){
            if(!check_color(dest.curr.x+j, dest.curr.y+i)){
                all_nodes[(dest.curr.y+i) * width + (dest.curr.x+j)].color.r = c.r;
                all_nodes[(dest.curr.y+i) * width + (dest.curr.x+j)].color.g = c.g;
                all_nodes[(dest.curr.y+i) * width + (dest.curr.x+j)].color.b = c.b;
            }
        }
    }
}

void colorPeople(){
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            if(!check_color(id.dean.x+j, id.dean.y+i)){
                all_nodes[(id.dean.y+i) * width + (id.dean.x+j)].color.r = 148;
                all_nodes[(id.dean.y+i) * width + (id.dean.x+j)].color.g = 0;
                all_nodes[(id.dean.y+i) * width + (id.dean.x+j)].color.b = 211;
            }
        }
    }
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            if(!check_color(id.lin.x+j, id.lin.y+i)){
                all_nodes[(id.lin.y+i) * width + (id.lin.x+j)].color.r = 148;
                all_nodes[(id.lin.y+i) * width + (id.lin.x+j)].color.g = 0;
                all_nodes[(id.lin.y+i) * width + (id.lin.x+j)].color.b = 211;
            }
        }
    }
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            if(!check_color(id.perez.x+j, id.perez.y+i)){
                all_nodes[(id.perez.y+i) * width + (id.perez.x+j)].color.r = 148;
                all_nodes[(id.perez.y+i) * width + (id.perez.x+j)].color.g = 0;
                all_nodes[(id.perez.y+i) * width + (id.perez.x+j)].color.b = 211;
           }
        }
    }
    for(int i = -5; i <= 5; i++){
        for(int j = -5; j <= 5; j++){
            if(!check_color(id.reeder.x+j, id.reeder.y+i)){
                all_nodes[(id.reeder.y+i) * width + (id.reeder.x+j)].color.r = 148;
                all_nodes[(id.reeder.y+i) * width + (id.reeder.x+j)].color.g = 0;
                all_nodes[(id.reeder.y+i) * width + (id.reeder.x+j)].color.b = 211;
            }
        }
    }
}

void markPath(FILE* &txt, Node &source, Node &dest){
    if(source != dest) markPath(txt, source, pred[dest]);
    color(dest, orange);
    if(get_dist(id.goal, dest.curr) < id.D) return;
    fprintf(txt, "%d %d\n", dest.curr.x, dest.curr.y);
}

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    if(fscanf(input, "%s", id.ppmFile));
    if(fscanf(input, "%d %d", &id.start.x, &id.start.y));
    if(fscanf(input, "%d %d", &id.goal.x, &id.goal.y));
    if(fscanf(input, "%d %d", &id.dean.x, &id.dean.y));
    if(fscanf(input, "%d %d", &id.lin.x, &id.lin.y));
    if(fscanf(input, "%d %d", &id.perez.x, &id.perez.y));
    if(fscanf(input, "%d %d", &id.reeder.x, &id.reeder.y));
    if(fscanf(input, "%d", &id.T));
    if(fscanf(input, "%lf", &id.minE));
    if(fscanf(input, "%lf", &id.maxE));
    if(fscanf(input, "%lf", &id.M));
    if(fscanf(input, "%lf", &id.D));
    FILE *img = fopen(id.ppmFile, "r");
    if(fscanf (img, "P6\n%d %d\n255%*c", &width, &height));
    ppm = new pixel[width * height];
    if(fread (ppm, width * height, sizeof(pixel), img));
    for(int i = 0; i < height; i ++){
        for(int j = 0;j < width; j++){
            Node temp;
            temp.curr.x = j;
            temp.curr.y = i;
            temp.passengers = 0;
            int place = i*width + j;
            temp.color.r = ppm[place].r;
            temp.color.g = ppm[place].g;
            temp.color.b = ppm[place].b;
            all_nodes.push_back(temp);
        }
    }
    dpComp = cos(id.M * (M_PI /180.0));
    // fclose(img);
    FILE *ppm = fopen("path.ppm", "w");
    FILE *txt = fopen("path.txt", "w");
    dijkstra(all_nodes[id.start.y * width + id.start.x], all_nodes[id.goal.y * width + id.goal.x]);
    markPath(txt, all_nodes[id.start.y * width + id.start.x], all_nodes[id.goal.y * width + id.goal.x]);
    colorPeople();
    writePPM(ppm);
}