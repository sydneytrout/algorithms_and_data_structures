//Sydney Trout
//Dr Dean
//CPSC 2150 lab
//29 August 2021
//Lab 1

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Sequence {
string dna;
bool is_delta;
};

Sequence *sequences;
Sequence probe;
Sequence comp;
Sequence bestProbe;

//function that compares characters of the probe and sequence
bool compareString(Sequence &a, Sequence &b){
  int index = 0;
  for (int i = 0; i < 100 && index < 2; i++){
    if(a.dna[i] != b.dna[i]){
      index++;
    }
  }
  return index < 2;
}

int main(){

    ifstream input ("covid-smaller.txt");

    int N = 0;
    string label;
    string dnaseq;


    //reads input file and determines number of sequences and average length
    while(input >> label >> dnaseq){
      N++;
    }

    sequences = new Sequence[N];

    //allows the file to be reread
    input.clear();
    input.seekg(0, ios_base::beg);

    int numD = 0;
    int numO = 0;

    //stores the sequences in an array of structs
    for(int i = 0; i < N; i++){
      input >> label;
      input >> dnaseq;
      if (label == "delta_variant"){
        sequences[i].dna = dnaseq;
        sequences[i].is_delta = true;
        numD++;
      }

      else{
        sequences[i].dna = dnaseq;
        sequences[i].is_delta = false;
        numO++;
      }
    }

    double numFalseNeg = 0;
    double numFalsePos = 0;
    double fpr;
    double fnr;
    double error;
    double bestError = 50.00;
    double bestFpr;
    double bestFnr;
    double bestNumFP;
    double bestNumFN;
    bool match;


    int i = 0;
    //loops through all the sequences
    while(i < N){
      //if the sequence is delta, continue
      if (sequences[i].is_delta == true){
        //loops through all characters of given delta sequence making probes
        for(unsigned int k = 0; k < (sequences[i].dna.length()-100); k++){
          numFalseNeg = 0;
          numFalsePos = 0;
          probe.dna = sequences[i].dna.substr(k, 100);
          //running probe against all other sequences
          for(int z = 0; z < N; z++){
            //if the other sequences are delta
            if (sequences[z].is_delta == true){
              match = false;
              //runs through every set of 100 in other sequence
              for (unsigned int y = 0; y < (sequences[z].dna.length()-100); y++){
                comp.dna = sequences[z].dna.substr(y, 100);
                //running through each character and comparing probe and 100 from other sequence
                if (compareString(probe, comp) == true){
                  match = true;
                  break;
                }
              }
              //if after running through every 100 option in the other sequence it never gets 99+, its a false neg
              if (match == false){
                numFalseNeg++;
              }
            }
            //running probe against non deltas
            else {
              match = false;
              //runs through every set of 100 in other sequence
              for (unsigned int r = 0; r < (sequences[z].dna.length()-100); r++){
                comp.dna = sequences[z].dna.substr(r, 100);
                //running through each character and comparing probe and 100 from other sequence
                if (compareString(probe, comp) == true){
                  match = true;
                  break;
                }
              }
              //if there is a match, it is a false positive
              if (match == true){
                numFalsePos++;
              }
            }
            //calculates the error rate and breaks if it is greater than the current best error
            fpr = (double)numFalsePos/numO;
            fnr = (double)numFalseNeg/numD;
            error = (2.0 * fpr) + (1.0 * fnr);
            if (error > bestError){
              break;
            }
        }

          //if error for given probe is better than best yet, set all these things
          if (error < bestError){
            bestError = error;
            bestFpr = fpr;
            bestFnr = fnr;
            bestNumFP = numFalsePos;
            bestNumFN = numFalseNeg;
            bestProbe.dna = probe.dna.substr(0, 100);
          }
        }
      }
      i++;
    }

    cout << "The best probe: " << bestProbe.dna << endl;
    cout << "The number of false positives: " << bestNumFP << ". The false positive rate: " << bestFpr << endl;
    cout << "The number of false negatives: " << bestNumFN << ". The false negative rate: " << bestFnr << endl;
    cout << "The error rate: " << bestError << endl;
    cout << "The worst case running time: O((N)^2(M-K)^2(K))" << endl;
    cout << "The anticipated running time: O((N)^2(M-K)^2)" << endl;


    delete[]sequences;

    return 0;
}
