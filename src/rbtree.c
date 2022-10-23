#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // rbtree *p =malloc(sizeof(rbtree));
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  // node_t *nillNode = malloc(sizeof(node_t));
  nilNode -> color = RBTREE_BLACK; 
  // nillNode -> color = 1;로 해도 똑같다. (node_t를 선언할 때 enum으로 선언했고 0일때는 red, 1일때는 black으로 정의했음)
  // (*nillNode).color = 1;으로 해도 똑같다. (->의 의미가 nillNode로 가! 거기서 color를 찾아)
  // 레드로 안 넣고 블랙으로 넣는 이유는 루트노드는 블랙으로 넣어야하니까 그러는 건가?
  p ->root = nilNode;
  p ->nil = nilNode;
  return p;
}

void free_node(rbtree *t, node_t *x) {
  // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
  // 왼쪽, 오른쪽 없애고, 루트 없애는 순서로 메모리 반환 중 (재귀를 돌면서)
  if (x->left != t->nil) 
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL; //이건 왜하는지..?
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t-> root != t->nil) //root없애고 
    free_node(t,t->root);
  free(t->nil); //nil도 없애고
  free(t); //마지막으로 t 자체가 차지하고 있는 공간도 없애준다
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *x = t->root; //굳이 rbtree t의 루트노드의 주소를 x에 넣은 것.(물론 t->root 다치기 귀찮으니까)
  node_t *y = t->nil;
  
  //insert될 노드가 Z임
  node_t *z = t->(node_t *)calloc(1, sizeof(node_t));
  z ->key = key;

  while (x != t->nil) //루트노드의 주소가 nil과 다르면 아래 수행 
  {
    y = x;
    if (z ->key < x->key)
      x = x->left
    else
      x = x-> right

  }
}

int main(void)
{
  new_rbtree();
  delete_rbtree(new_rbtree());
  // delete_rbtree(*(new_rbtree()));
  // printf("%p\n", new_rbtree()->root);
}