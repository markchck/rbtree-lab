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

void left_rotation(rbtree *t, node_t *x){
  //y구초제 생성
  node_t *y;

  //y를 x의 오른쪽에
  y = x->right; 

  //clrs p315 그림 13.2참고하면서 봐라

  //1. 좌회전하면서 아빠가 y에서 x로 바뀌는 베타를 처리하는 부분
    //1-1) 베타의 부모단에서 서열 정리하는 부분 (베타는 이제부터 제 오른쪽 자식이에요-x왈..)
    x->right = y-> left;
    
    //1-2) 베타가 제 부모는 이제부터 x이에요 라고 적는 부분
    if (y->left != t ->nil) //베타가 혹시 nil이면 옮길 것도 없으니까 nil이 아닌 경우만 하면 됨.
    {
      y->left->parent = x;
    }
  
  //2. 할아버지 세대에서도 이제부터 y가 내 자식이구나..라고 족보 정리가 필요함.
    //2-1) 먼저 y가 할아버지(x의 parent) 이제 제가 할아버지 자식이에요라고 기록 하는 중. 
    y->parent = x->parent;

    //2-2)할아버지도 '그래 내 자식은 이제부터 y다' 라고 인정해주는 부분이 필요함
    //근데 y가 대체하게될 x가 할아버지의 왼쪽 자식이었나 오른쪽 자식이었나에 따라 달라짐.
      
      //2-2-1) 만약 애당초 x가 할아버지가 없었으면 
      if (x->parent == t->nil) 
        //x가 루트였다는 소리이고 이젠 y가 root가 되면 됨.
        t->root = y;

      //2-2-2) 만약 x가 할아버지 왼쪽 자식이었으면
      else if (x == x->parent->left) 
        //할아버지의 왼쪽 자식에 y가 적히면 됨.
        x->parent->left =y; 
      
      //2-2-3) 만약 x가 할아버지 오른쪽 자식이었으면
      else
        //할아버지의 오른쪽 자식에 y가 적히면 됨.
        x->parent->right = y;

  //3. x와 y의 관계도 바뀌니까 서열 정리 해줘야 함.
    //3-1) y가 x는 이제 내 왼쪽 자식이다라고 기록함.
      y->left = x;
    //3-2) x도 내 부모는 이제 y야라고 기록함
      x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  // 이게 뭘까? 포인터로 아무것도 지정 안해줫으니 y는 nil인 상황임.
  node_t *y;

  //삽입은 항상 레드로 하기 때문에 while문에 무조건 걸리게 되어있다.
  while(z->parent->color == RBTREE_RED){

    //아빠가 할아버지 왼쪽에 있는 경우(나는 왼쪽일수도 오른쪽일 수도 있음)
    if (z->parent == z->parent->parent->left);
    {
      //빈포인터 y에다가 삼촌(아빠 오른쪽)을 담아
      y = z->parent->parent->right;
      
      //case1: 나, 아빠, 삼촌 모두 빨간색 (아빠가 흑색인 경우는 위반 아님) 
      if(y->color == RBTREE_RED)
      {
        // 아빠와 삼촌을 블랙으로 바꾸고
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;

        // 할아버지를 레드로 바꾸고
        z->parent->parent->color = RBTREE_RED;

        // 할아버지에서 다시 확인 시작 (이거 코드 재밌네 ㅋㅋ z를 할아버지로 바꿔주면 할아버지에서 다시 확인하는 꼴임)
        z = z->parent->parent;
      }
      
      //case2: 나 레드+오른쪽 / 아빠 레드+왼쪽 / 삼촌 블랙 
      //아빠를 기준으로 잡고, 왼쪽으로 회전하고, case3방식으로 해결
      else
      {
        if(z== z->parent->right){
          // 아빠를 기준으로 잡고
          z = z->parent;
          // 왼쪽으로 회전
          left_rotation(t,z);
        }
        //Case3는 rotation하고난 뒤에 하자
      }
    }
  }
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

    rbtree_insert_fixup(t, z);
    return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *current = t-> root;
  while (current != t->nil)
  {
    if (current ->key == key)
    {
      return current;
    }
    
    if (current->key < key){
      current = current->right;
    }else{
      current = current->left;
    }
    return NULL;
  }
}

int main(){
  rbtree *new = new_rbtree();
  rbtree_insert(new, 1024);
  // rbtree_find(new, 1025);

  // RB트리 삭제 완료
  // delete_rbtree(new);

  // printf("%p\n", new_rbtree()->root);
}