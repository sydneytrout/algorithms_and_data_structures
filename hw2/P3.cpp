#include <iostream>
#include <fstream>
#include <cstdlib>
#include <tuple>
#include <set>
#include <vector>
#include <cstdio>
#include <unordered_map>
#include <map>

using namespace std;

struct Node{
    string key;
    int size;
    Node *left;
    Node *right;
    Node *parent;
    Node (string s) {key = s; size = 1; left = right = parent = NULL;}
};

void update_size(Node *n){
    n->size = 1;  
    if (n->left != NULL) n->size += n->left->size;
    if (n->right != NULL) n->size += n->right->size;
    if (n->left != NULL) n->left->parent = n;
    if (n->right != NULL) n->right->parent = n;
}

int getSize(Node *s){
  if(s == NULL){
    return 0;
  }
  return s->size;
}


int numLines;
set<string> Dancers;
unordered_map <string,int> M;
vector<pair<string,string>> Steps;
map<string,Node*> fr;

int get_rank(Node *root, Node *n){
  int left_size_n = n->left == NULL ? 0 : n->left->size;
  if(root == n){
    return left_size_n;
  }
  if(n == n->parent->left){
    return get_rank(root, n->parent) - n->size + left_size_n;
  }
  else{
    return get_rank(root, n->parent) + left_size_n + 1;
  }
}

Node *select(Node *T, int r){
  int root_rank = T->left == NULL ? 0 : T->left->size;
  if (r == root_rank){
    return T;
  }
  if (r < root_rank){
     return select(T->left, r);
  }
  return select(T->right, r - root_rank - 1);

}

Node *join(Node *L, Node *R)
{
  if (L == NULL){
    return R;
  }
  if (R == NULL){
    return L;
  }
  int total = L->size + R->size;
  int randn = rand() % total;
  Node *T;
  if (randn < L->size) {
    T = L;
  } else {
    T = R;
  }
  if(T == L){
    L->right = join(L->right, R);
  }
  else{
    R->left = join(L, R->left);
  }
  update_size(T);
  return T;
}

Node *remove(Node *T, int r)
{
  if(T == NULL){
    return NULL;
  }
  int root_rank = T->left == NULL ? 0 : T->left->size;
  if(r == root_rank){
    Node *newT = join(T->left, T->right);
    delete T;
    return newT;
  }
  if(r <= root_rank){
    T->left = remove(T->left, r);
  }
  else{
    T->right = remove(T->right, r - root_rank - 1);
  }
  update_size(T);
  return T;
}

Node *insert(Node *T, int r, string v)
{
  if (T == NULL){
     return new Node(v);
  }
  int rootRank = getSize(T->left);
  if (r < rootRank){
    T->left  = insert(T->left, r, v);
  } 
  else{
    T->right = insert(T->right, r - rootRank - 1, v);
  }                
  update_size(T);
  return T;
}

pair<Node *, Node *> split(Node *T, int r)
{
  pair<Node *, Node *> pairT = make_pair(T,T);
  if (T == NULL){
    return pairT;
  }
  int root_rank = T->left == NULL ? 0 : T->left->size;
  if (r <= root_rank){
    tie(pairT.first, T->left) = split(T->left, r);
  }
  else {
    tie(T->right, pairT.second) = split(T->right, r - root_rank - 1);
  }
  update_size(T);
  return pairT;
}

Node *insert_keep_balanced(Node *T, int r, string v)
{
  if (T == NULL) return new Node(v);
  int root_rank = T->left == NULL ? 0 : T->left->size;
  if (rand() % (1 + T->size) == 0) {
    pair<Node *, Node *> result = split (T, r);
    T = new Node(v);
    T->left = result.first;
    T->right = result.second;
  } else {
    if (r <= root_rank) T->left  = insert_keep_balanced(T->left,  root_rank, v);
    else            T->right = insert_keep_balanced(T->right, r - root_rank - 1, v);
  }
  T->size = 1;
  if (T->left != NULL) T->size += T->left->size;
  if (T->right != NULL) T->size += T->right->size;
  if (T->left != NULL) T->left->parent = T;
  if (T->right != NULL) T->right->parent = T;
  return T;
}

void print_inorder(Node *T, unordered_map<string, int> M)
{
  if (T==NULL){
    return;
  }
  print_inorder(T->left, M);
  cout << T->key << " " << M[T->key] << endl;
  print_inorder(T->right, M);

}

int main(void){
    string dancer1;
    string dancer2;
    while(cin>>dancer1>>dancer2){
        Dancers.insert(dancer1);
        Dancers.insert(dancer2);
        Steps.push_back(make_pair(dancer1,dancer2));
    }
    Node *temp = NULL;
    Node *hold = NULL;
    int r = 0;
    for(auto s: Dancers){
        temp=insert_keep_balanced(temp,r,s);
        hold=insert_keep_balanced(hold,r,s);
        r++;
    }
    int x = 0;
    for(auto s: Dancers){
        fr[s] = select(temp,x);
        x++;
    }
    int rank1 = 0;
    int rank2 = 0;
    set<string>::iterator itr;
    int start = 0;
    int finish = 0;
    for(auto it = Steps.begin(); it != Steps.end(); it++){
        string t = it->first;
        string s = it->second;
        rank1 = get_rank(temp, fr[t]);
        rank2 = get_rank(temp, fr[s]);
        int check;
        if(rank2 < rank1){
          check = abs(rank2 - (rank1-1));
        }
        if ((rank1-rank2) == 1){
          check = 0;
        }
        if(rank2 > rank1){
          check = abs(rank2 - (rank1));
        }
        M[t] = M[t] + check;
        temp = remove(temp,rank1);
        rank2 = get_rank(temp, fr[s]);
        temp = insert(temp, rank2, it->first);  
        fr[t] = select(temp,rank2 + 1);
    }
     print_inorder(hold, M);
}