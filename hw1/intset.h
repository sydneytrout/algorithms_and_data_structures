#ifndef INTSET_H 
#define INTSET_H

class Intset {

 private: 
  int N; 
  int M; 

  struct Node {
    int key;
    Node *next;
    Node (int k, Node *n) { key = k; next = n; }
  };
  Node **table;  
  void free_table(void);
  int myhash(int key, int table_size);
  int myhash(int key);
  
 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
