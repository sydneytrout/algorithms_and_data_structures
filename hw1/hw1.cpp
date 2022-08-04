#include <iostream>
#include <fstream>
#include <string>
#include "map.h"
#include "intset.h"


using namespace std;

int N, numD, K =100;
long long x=2;
long long xN=1; 
int p=1000000007;
Map ht = Map();

string *sequence;
bool *is_delta;

struct probe_info {  
    int seq_index, pos;
    int fp_count, fn_count;
    double error_rate;
};

int Hash(int id, int index){
    int h=0;                
    for(int i=index; i<index + K/2; i++){
        h = (((long long)h * x)%p + sequence[id][i]) % p;
    }
    return h;
}

int rollingHash(int oh,int i, int n){
    int nh = oh;
    nh = (((((p+nh-(((long long)sequence[i][n]*xN)%p))%p)*x)% p)+sequence[i][(K/2)+n])% p;  
    return nh;
}


bool is_match(int i, int p, int j, int q){

    if(q < 0 || q + (K - 1) > sequence[j].length()){
        return false;
    }
    int mismatched = 0;  
    for (int k=0; k<K; k++){    
        if (sequence[i][p+k] != sequence[j][q+k]){      
            mismatched ++;      
            if (mismatched > 1){
                return false;
            }    
        }  
    }
    return true;
}

probe_info eval_probes(){
    probe_info temp;
    probe_info best;
    best.error_rate = 99999;
    for (int i=0; i<N; i++) {
        if (is_delta[i]){
            int ph1 = Hash(i,0);
            int ph2 = Hash(i,(K/2));
            for (int j=0; j<sequence[i].length()-K; j++) {
                Intset check;
                temp.fp_count = 0;
                temp.fn_count = numD;
                temp.seq_index = i;
                temp.pos = j;
                LocNode *m1= ht[ph1];
                LocNode *m2= ht[ph2];
                while(m1!= NULL){
                    if(check.find(m1->seqid)){
                        m1=m1->next;
                        continue;
                    }
                    if(is_match(i,j,m1->seqid,m1->index)){
                        if (is_delta[m1->seqid]) temp.fn_count --;
                        if (!is_delta[m1->seqid]) temp.fp_count ++;
                        check.insert(m1->seqid);
                    }
                    m1=m1->next;
                }
        
                while(m2!= NULL){
                    if(check.find(m2->seqid)){
                        m2=m2->next;
                        continue;
                    }
                    if(is_match(i,j,m2->seqid,(m2->index)-K/2)){
                        if (is_delta[m2->seqid]) temp.fn_count --;
                        if (!is_delta[m2->seqid]) temp.fp_count ++;
                        check.insert(m2->seqid);
                    }
                    m2=m2->next;
                }
                double FPR = (double)temp.fp_count / (N-numD);
                double FNR = (double)temp.fn_count / numD;
                temp.error_rate = 2.0 * FPR + 1.0 * FNR;

                if (temp.error_rate < best.error_rate){
                    best = temp;
                }
                
                ph1=rollingHash(ph1,i,j);
                ph2=rollingHash(ph2,i,(K/2)+j);
            }
        }
  }
  return best;
}


int main(){

    ifstream input("covid-smaller.txt");
    string label, seq;  
    while (input >> label >> seq){
        N++;
    }  
    input.clear();
    input.seekg(0);
    sequence = new string[N];
    is_delta = new bool[N]; 
    for (int i=0; i<N; i++) {
        input >> label >> sequence[i];
        is_delta[i] = label == "delta_variant";
        if (is_delta[i]) numD++;
    }

    int z = (K/2)-1;

    while(z != 0){
        xN = ((long long)xN*x) % p;
        z--;
    }

    for (int i=0; i<N; i++) {
        int rh=Hash(i,0);
        for (int n=0; n<sequence[i].length()-(K/2); n++){
            ht[rh] = new LocNode(i, n, ht[rh]);
            rh=rollingHash(rh,i,n);
        }
    }

    probe_info probe = eval_probes();

    cout << "Best probe: " << sequence[probe.seq_index].substr(probe.pos, K) << "\n";
    cout << "False positives: " << probe.fp_count << "\n";
    cout << "False negatives: " << probe.fn_count << "\n";
    cout << "Error_rate: " << probe.error_rate << "\n";


    delete [] sequence;
    delete [] is_delta;

    return 0;

}
