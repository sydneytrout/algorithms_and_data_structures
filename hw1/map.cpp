#include <iostream>
#include <fstream>
#include <cstring>
#include "map.h"

using namespace std;


Map::Map(){  
  N = 0;  // start with no elements present  
  M = 4;  // start with a 4-element array   
  table = new Node *[M]();
}

Map::~Map(){  
  free_table(true);  
}


void Map::free_table(bool remove){  
  for (int i=0; i<M; i++)    
    while (table[i] != NULL) {
      Node *to_delete = table[i];      
      table[i] = table[i]->next;   
      if(remove){
        while(to_delete->locs != NULL){
          LocNode *loc_delete = to_delete->locs;      
          to_delete->locs = to_delete->locs->next; 
          delete loc_delete;
        }
      }   
      delete to_delete;    
    }  
    delete [] table;
}

int Map::myhash(int key, int table_size){
    return ((unsigned)key * 1791321 + 76543) % table_size;
}

int Map::myhash(int key){ 
   return myhash(key, M);
}

bool Map::find(int key){
  for (Node *n = table[myhash(key)]; n != NULL; n = n->next)    
    if (n->key == key) return true;  
  return false;
}

void Map::insert(int key, LocNode *location){
  if (find(key)) return;  
  
  if (N == M) {    
    Node **newtbl = new Node *[M*2]();
    for (int i=0; i<M; i++)      
      for (Node *n = table[i]; n != NULL; n = n->next) {
      int h = myhash(n->key, M*2);
      newtbl[h] = new Node(n->key, newtbl[h], n->locs);
      }    
      free_table(false);     
      M *= 2;    
      table = newtbl;  
  }  
  N++;  
  int h = myhash(key);
    table[h] = new Node(key, table[h], location);
}


LocNode *&Map::operator[] (int key){
    if (!find(key)){
      insert(key, NULL);
    }
    Node *temp = table[myhash(key)];
    while(temp->key != key){
      temp = temp->next;
    }
    return temp->locs;

}


