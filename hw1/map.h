#ifndef MAP_H 
#define MAP_H

struct LocNode{
    int seqid; 
    int index;
    LocNode *next;
    LocNode (int seq, int in, LocNode *n) { seqid = seq; index = in; next = n;}  
};



class Map { 
  private:   
    int N; 
    int M;  
    struct Node {    
      int key;  
      LocNode *locs;  
      Node *next;    
      Node (int k, Node *n, LocNode *l) { key = k; next = n; locs = l;}  
    };  
    Node **table;  
    
    public:  
    Map();  
    ~Map();  
    bool find(int key);  
    void insert(int key, LocNode *location);  
    int myhash(int key, int table_size);  
    int myhash(int key); 
    void free_table(bool remove);  
    LocNode *&operator[] (int key);
};

#endif