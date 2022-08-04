#include <iostream>
#include "intset.h"

using namespace std;

Intset::Intset()
{
  N = 0;  
  M = 4;  
  table = new Node *[M]();
}

Intset::~Intset()
{
  free_table();  
}


void Intset::free_table(void)
{
  for (int i=0; i<M; i++)
    while (table[i] != NULL) {
      Node *to_delete = table[i];
      table[i] = table[i]->next;
      delete to_delete;
    }
  delete [] table;
}

int Intset::myhash(int key, int table_size)
{
  return ((unsigned)key * 1791321 + 76543) % table_size;
}

int Intset::myhash(int key)
{
  return myhash(key, M);
}

bool Intset::find(int key)
{
  for (Node *n = table[myhash(key)]; n != NULL; n = n->next)
    if (n->key == key) return true;
  return false;
}


void Intset::insert(int key)
{
  if (find(key)) return;

  if (N == M) {
    Node **newtbl = new Node *[M*2]();
    for (int i=0; i<M; i++)
      for (Node *n = table[i]; n != NULL; n = n->next) {
	int h = myhash(n->key, M*2);
	newtbl[h] = new Node(n->key, newtbl[h]);
      }
    free_table(); 
    M *= 2;
    table = newtbl;
  }

  N++;
  int h = myhash(key);
  table[h] = new Node(key, table[h]);
}

void Intset::remove(int key)
{
  if (!find(key)) return;
  N--;
  int h = myhash(key);
  Node *prev = NULL, *to_del = table[h];
  while (to_del->key != key) { prev = to_del; to_del = to_del->next; }
  (prev!=NULL ? prev->next : table[h]) = to_del->next;
  delete to_del;
}

void Intset::print(void)
{
  for (int i=0; i<M; i++)
    for (Node *n = table[i]; n != NULL; n = n->next) 
      cout << n->key << " ";
  cout << "\n";
}
