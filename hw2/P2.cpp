#include <iostream>
#include <cstdlib>
#include <tuple>

using namespace std;

struct Node {
  int key;
  int size;  
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = NULL; }
};

void update_size(Node *n){
    n->size = 1;  
    if (n->left != NULL) n->size += n->left->size;
    if (n->right != NULL) n->size += n->right->size;
}

int getSize(Node *s){
  if(s == NULL){
    return 0;
  }
  return s->size;
}

Node *insert(Node *T, int r, int v)
{
  if (T == NULL){
     return new Node(v);
  }
  int rootRank = getSize(T->left);
  if (r <= rootRank){
    T->left  = insert(T->left, r, v);
  } 
  else{
    T->right = insert(T->right, r - rootRank - 1, v);
  }                
  update_size(T);
  return T;
}

int get_rand(int N)
{
  return 123456789 % N;
}

void print_inorder(Node *root){
    if (root == NULL) return;
    print_inorder(root->left);
    cout << root->key << "\n";
    print_inorder(root->right);
}

int main(){
    int n;
    cin>>n;
    int rand;
    Node *temp = NULL;
    for(int i = n; i > 0; i--){
        temp = insert(temp, get_rand(n-i+1), i);
    }
    cout << n << endl;
    print_inorder(temp);
}
