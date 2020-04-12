#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum token { NUM, ADD, SUB, MUL } TokenType;

typedef struct node {
  TokenType type;
  int val, size;
  struct node *left, *right, *parent;
} Node;

Node* nodeArr[300010];
int len;
Node* BuildSyntaxTree(Node* Parent) {
  char input = getchar();
  Node* root = (Node*)calloc(1, sizeof(Node));
  nodeArr[++len] = root;
  root->parent = Parent;
  switch (input) {
    case '+':
      root->type = ADD;
      root->left = BuildSyntaxTree(root);
      root->right = BuildSyntaxTree(root);
      root->size = root->left->size + root->right->size + 1;
      root->val = root->left->val + root->right->val;
      break;
    case '-':
      root->type = SUB;
      root->left = BuildSyntaxTree(root);
      root->right = BuildSyntaxTree(root);
      root->size = root->left->size + root->right->size + 1;
      root->val = root->left->val - root->right->val;
      break;
    case '*':
      root->type = MUL;
      root->left = BuildSyntaxTree(root);
      root->right = BuildSyntaxTree(root);
      root->size = root->left->size + root->right->size + 1;
      root->val = root->left->val * root->right->val;
      break;
    default:
      root->type = NUM;
      root->left = root->right = NULL;
      root->size = 1;
      root->val = input - '0';
      break;
  }
  root->val %= 10007;
  return root;
}
void FindValAt(Node* root, int a) {
  if (a == 1)
    printf("%d\n", root->val);
  else if (a - 1 <= root->left->size)
    FindValAt(root->left, a - 1);
  else
    FindValAt(root->right, a - root->left->size - 1);
}
void Travel() {
  Node* tmp;
  for (int i = 1; i <= len; i++) {
    tmp = nodeArr[i];
    if (tmp->type == NUM)
      printf("[%d, %d]: %d ", tmp->val, tmp->size, tmp->val);
    else if (tmp->type == MUL)
      printf("[%d, %d]: *  ", tmp->val, tmp->size);
    else if (tmp->type == ADD)
      printf("[%d, %d]: +  ", tmp->val, tmp->size);
    else if (tmp->type == SUB)
      printf("[%d, %d]: -  ", tmp->val, tmp->size);
  }
}
void UpdateIdx(int a, int b) {
  int l, r;
  // I think the most time consuming part.
  // Is there a method can quickly checkup like arr,
  // and maintain easily like linkedlist ?
  for (l = a, r = b + 1; r <= len; l++, r++) nodeArr[l] = nodeArr[r];
}
void UpdateVal(Node* root) {
  if (root == NULL) return;
  int value;
  switch (root->type) {
    case ADD:
      value = root->left->val + root->right->val;
      root->size = root->left->size + root->right->size + 1;
      break;
    case SUB:
      value = root->left->val - root->right->val;
      root->size = root->left->size + root->right->size + 1;
      break;
    case MUL:
      value = root->left->val * root->right->val;
      root->size = root->left->size + root->right->size + 1;
      break;
    default:
      value = root->val;
      break;
  }
  if (value != root->val) {
    root->val = value % 10007;
    UpdateVal(root->parent);
  }
}
void DeleteSubTreeAt(int a, int b) {
  Node *node_a, *node_b;
  // find the node before node a.
  node_a = nodeArr[a]->parent;
  // find the node after node b.
  node_b = nodeArr[b + 1];
  // link them up.
  if (node_a == NULL)
    nodeArr[0] = node_b;
  else if (node_a->left == nodeArr[a])
    node_a->left = node_b;
  else
    node_a->right = node_b;

  node_b->parent = node_a;
  UpdateIdx(a, b);
  UpdateVal(node_b->parent);
  len -= b - a + 1;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.txt", "r", stdin);
#endif
  // Node* root = NULL;
  int N, input_a, input_b;
  char instruction;
  scanf(" %d\n", &N);
  nodeArr[0] = BuildSyntaxTree(NULL);
  Travel();
  printf("\n");
  while (N--) {
    scanf(" %c", &instruction);
    if (instruction == 'Q') {
      scanf(" %d", &input_a);
      FindValAt(nodeArr[0], input_a);
    } else if (instruction == 'D') {
      scanf(" %d %d", &input_a, &input_b);
      DeleteSubTreeAt(input_a, input_b);
      Travel();
      printf("\n");
    }
  }

  return 0;
}