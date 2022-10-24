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
  node_t *x = t->root; //t->root 치기 귀찮으니까 x라고 바꿈
  node_t *y = t->nil;
  
  //삽입 될 노드를 Z라고 하고
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z ->key = key;

  //1. 노드가 nil이 아닐때까지 while돌려
    while (x != t->nil) 
    {
      //왜 y=x를 해줘야하는가 ->x가 다음 노드로 내려가기 전에 미래의 자기 부모의 위치를 기록하는 것  
      //https://www.youtube.com/watch?v=ESqeK-ACHkU&list=PLjq20pMYInLLWi5JBplcRZ6Z8_X5d3Tvb&index=14 12:34분 참고
      y = x; 

      //새로 넣을 노드의 숫자가 루트 노드의 숫자보다 작으면 왼쪽이지?
      //이진탐색 트리구조 성질임.
      if (z ->key < x->key) 
        x = x->left; //x의 왼쪽인놈이 이제 루트임. 
      else
        x = x-> right;//x의 오른쪽인놈이 이제 루트임. 
    }

  //2. x가 드디어 트리의 맨 끝에 도달했으면(x= nil인 상황) 아래를 해.
    z->parent = y; //y에는 현재 x의 부모노드 주소가 들어있고 그게 곧 z의 부모임.
    
    if (y == t->nil) //y도 nil인 경우가 하나 있음. 바로 트리가 비어있어서 z가 최초 삽입인 경우
    {
      t->root = z; //트리에 최초 삽입인 경우 root에 z를 넣는다.
    }
    else //y가 nil이 아니면 (즉 트리가 비어있지 않아서 x가 타고타고 내려간 상황)
    {
      if(z->key < y->key) //z의 key와 부모 y의 key를 비교해서 이진탐색트리 구현
      {
        y->left = z;
      }
      else
      {
        y->right = z;
      }
    }
    //삽입은 레드로한다는 규칙에 따라 z를 red로하고
    z->color = RBTREE_RED;

    //레드블랙은 nil노드도 명시해줘야하니까 z의 오른쪽 왼쪽도 nil노드 셋팅
    z->left = t->nil; 
    z->right = t->nil;

    // 삽입 후 조정은 이따가 하자~
    // rbtree_insert_fixup(t, z);
    return z;
}

int main(void)
{
  // RB트리 생성 완료
  rbtree *new = new_rbtree();
  rbtree_insert(new, 1024);

  // RB트리 삭제 완료
  // delete_rbtree(new);

  // printf("%p\n", new_rbtree()->root);
}