typedef struct Node{
  char* word;
  struct Node* left;
  struct Node* right;
}NODE;

extern NODE* root;

void insert_node(char* c);

void print_tree(NODE* r);
