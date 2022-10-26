#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>
​
​
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // rbtree 구조체 1개의 크기만큼 메모리를 할당한 후 rbtree 포인터형으로 타입 변환(캐스팅) 
  // TODO: initialize struct if needed
  node_t *t_nil = (node_t *)calloc(1, sizeof(node_t));  // node_t 구조체 1개의 크기만큼 메모리를 할당한 후 node_t 포인터형으로 타입 변환(캐스팅) 
  
  t_nil->color = RBTREE_BLACK; // 경계노드는 BLACK
  p-> nil  = t_nil;  // 트리의 nil노드는 경계노드를 가리킨다
  p-> root = t_nil;  // 트리의 Root는 경계노드를 가리킨다
  return p;
}
​
void free_node(rbtree *t, node_t *x) {
  if (x->left != t->nil) 
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL;
}
​
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // delete_tree(tree): RB tree 구조체가 차지했던 메모리 반환
    if (t->root != t->nil)
    free_node(t, t->root);
  free(t->nil);
  free(t);
}
​
void left_rotate(rbtree *t, node_t *x) {
  /* 노드 x에 대해 좌회전 수행 */
  node_t *t_nil = t->nil;
  node_t *y = x->right;
  x->right = y->left; 
  if (y->left != t_nil)
    y->left->parent = x;
  y->parent = x->parent ; 
  if (x->parent == t_nil)
    t->root = y ;
  else if (x == x->parent->left) 
    x->parent->left = y;
  else
    x->parent->right = y;
  x->parent = y ;
  y->left = x ; 
}
void right_rotate(rbtree *t, node_t *x) {
  /* 노드 x에 대해 우회전 수행 */
  node_t *t_nil = t->nil;
  node_t *y = x->left;
  x->left = y->right; 
  if (y->right != t_nil)
    y->right->parent = x;
  y->parent = x->parent ; 
  if (x->parent == t_nil)
    t->root = y ;
  else if (x == x->parent->left) 
    x->parent->left = y;
  else
    x->parent->right = y;
  x->parent = y ;
  y->right = x ; 
}
void rb_insert_fixup(rbtree *t, node_t *z) {
  node_t *uncle ;
  /* 
  위반될 수 있는 rb트리의 특징은 2번과 4번.
  2번은 새 노드(z)가 루트노드일 때 위반되며, 이는 root의 색깔을 BLACK으로 바꿔줌으로써 쉽게 해결할 수 있다.
  4번은 새 노드(z)의 부모노드가 RED일 때 위반되며, 삼촌노드가 적색인지 흑색인지에 따라 해결할 수 있는 방법이 다르다. 
  z의 부모노드가 RED인 조건에서 while 반복문을 수행하며 아래와 같이 해결한다. 
  Case 1. z의 부모노드가 조부모 노드의 왼쪽 노드일 때 
   (a) 부모 RED 삼촌 RED : 부모, 조부모, 삼촌 노드를 RECOLOR : 부모와 삼촌은 RED에서 BLACK, 조부모는 BLACK에서 RED. 
                         RECOLORING 후에는 조부모까지는 RBTree의 조건을 만족하지만, 조부모 노드가 RED 노드로 색이 바뀌면서 다시 RBTree의 조건을 위반할 가능성 존재.
                         따라서 z를 조부모 노드로 바꾼 후 다시 RB트리의 조건을 위반하는 노드가 없는지 반복문의 다음 루프에서 확인+해결한다.
   (b) 부모 RED 삼촌 BLACK 
        (b-1) Z가 오른쪽 노드인 경우 : z를 z의 부모노드로 바꾼 후 z에 대해 좌회전해서 (b-2)의 케이스와 동일하게 만들어준다. (이후 b-2의 해결과정에 따라 해결한다.)
        (b-2) Z가 왼쪽 노드인 경우 : 부모, 조부모 노드를 RECOLOR : 부모는 RED에서 BLACK, 조부모는 BLACK에서 RED 
                                 RECOLORING 후에는 조부모 노드에 대해 우회전한다. 우회전 후 Z의 부모노드가 조부모 노드의 위치로 가게 되는데, 부모노드는 흑색이므로 반복문이 더 실행되지 않는다. 
  Case 2. z의 부모노드가 조부모 노드의 오른쪽 노드일 때 (왼쪽 노드일 때와 대칭으로 해결하면 된다.)
    (a) 부모 RED 삼촌 RED : 부모, 조부모, 삼촌 노드를 RECOLOR : 부모와 삼촌은 RED에서 BLACK, 조부모는 BLACK에서 RED. 
                         RECOLORING 후에는 조부모까지는 RBTree의 조건을 만족하지만, 조부모 노드가 RED 노드로 색이 바뀌면서 다시 RBTree의 조건을 위반할 가능성 존재.
                         따라서 z를 조부모 노드로 바꾼 후 다시 RB트리의 조건을 위반하는 노드가 없는지 반복문의 다음 루프에서 확인+해결한다.
   (b) 부모 RED 삼촌 BLACK 
        (b-1) Z가 왼쪽 노드인 경우 : z를 z의 부모노드로 바꾼 후 z에 대해 우회전해서 (b-2)의 케이스와 동일하게 만들어준다. (이후 b-2의 해결과정에 따라 해결한다.)
        (b-2) Z가 오른쪽 노드인 경우 : 부모, 조부모 노드를 RECOLOR : 부모는 RED에서 BLACK, 조부모는 BLACK에서 RED 
                                 RECOLORING 후에는 조부모 노드에 대해 좌회전한다. 좌회전 후 Z의 부모노드가 조부모 노드의 위치로 가게 되는데, 부모노드는 흑색이므로 반복문이 더 실행되지 않는다. 
  */
 while (z->parent->color == RBTREE_RED) {
  if (z->parent == z->parent->parent->left) // 부모가 조부모의 오른쪽 자식일 때
  {
  uncle = z->parent->parent->right;
    if (uncle->color == RBTREE_RED) { // 삼촌이 RED
      z->parent->color = RBTREE_BLACK;
      uncle->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      z = z->parent->parent; 
      // printf("src says: z is %d,z's color is %d, z's parent is %d, and z's  parent color is %d. \n\n", z->key, z->color, z->parent->key, z->parent->color);
    }
    else { // 삼촌이 BLACK
      if (z== z->parent->right) 
      { // b-1 케이스. b-2케이스로 만든다
        z = z->parent; 
        left_rotate(t,z);
      };
      //b-2 케이스 해결 시작
      // printf("src says: z is %d,z's color is %d, z's parent is %d, and z's  parent color is %d. \n\n", z->key, z->color, z->parent->key, z->parent->color);
      z->parent->color = RBTREE_BLACK;
      // printf("src says: z is %d,z's color is %d, z's parent is %d, and z's  parent color is %d\n\n", z->key, z->color, z->parent->key, z->parent->color);
      z->parent->parent-> color = RBTREE_RED; 
      right_rotate(t,z->parent->parent);
    }
  }
  else { // 부모가 조부모의 오른쪽 자식일 때
  uncle = z->parent->parent->left;
      if (uncle->color == RBTREE_RED) { // 삼촌이 RED // 여기서 segmentation fault 에러가 난다!?
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent; 
        // printf("src says: z is %d,z's color is %d, z's parent is %d, and z's  parent color is %d. \n\n", z->key, z->color, z->parent->key, z->parent->color);
        }
      else { // 삼촌이 BLACK
        if (z== z->parent->left) { // b-1 케이스. b-2케이스로 만든다
            z = z->parent; 
            right_rotate(t,z);
          };
        //b-2 케이스 해결 시작
        z->parent->color = RBTREE_BLACK;
        // printf("src says: z is %d,z's color is %d, z's parent is %d, and z's  parent color is %d\n\n", z->key, z->color, z->parent->key, z->parent->color);
        z->parent->parent-> color = RBTREE_RED; 
        left_rotate(t,z->parent->parent);
      }
 } // else 끝
 } // while 끝
  t->root->color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *t_nil = t->nil ;
  node_t *x = t->root ;  // x는 새 노드가 들어갈 자리를 찾을 변수
  node_t *y = t_nil ;  // y는 새 노드가 들어갈 자리의 부모 변수를 찾을 변수 
  node_t *z = (node_t *)calloc(1, sizeof(node_t));  // node_t 구조체 1개의 크기만큼 메모리를 할당한 후 node_t 포인터형으로 타입 변환(캐스팅) 
  z->key = key;
  while (x!= t_nil) {
    y = x;
    if (key < x->key)
      x = x->left;
    else 
      x = x->right;
  }
     /* 반복문이 끝났다는 것은 x가 nil에 도달했고, y는 nil 부모 노드에 도달해있다는 것 
     확실한 것은 z의 부모는 y라는 것이고, 이제 y와 크기를 비교해서 왼쪽 또는 오른쪽 자식 노드로 넣어줘야 한다 */
  z-> parent = y; 
  if (y==t_nil)
    t->root = z; 
  else if (key< y->key)
    y->left = z;
  else
    y->right = z;
  
  z->color = RBTREE_RED; // z는 새로 삽입하는 노드이므로 색깔은 RED
  z->left = t_nil;  // z는 새로 삽입되는 노드이므로 왼쪽 자식은 nil
  z->right = t_nil;  // z는 새로 삽입되는 노드이므로 오른쪽 자식은 nil
  
  rb_insert_fixup(t, z);
  printf("key: %d, color:%d, left:%d, right:%d, parent: %d \n \n", z->key, z->color, z->left->key, z->right->key, z->parent->key);
  return z;
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *t_nil = t->nil ;
  node_t *x = t->root ;  
  while (x != t_nil) {
    if (key == x-> key)
      return x ;
    else if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  return NULL;
}
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // ptr = tree_min(tree): RB tree 중 최소 값을 가진 node pointer 반환
  node_t *t_nil = t->nil ;
  node_t *x = t->root ;  
  while (x->left != t_nil) {
    x = x->left;
  }
  return x;
}
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  // ptr = tree_max(tree): 최대값을 가진 node pointer 반환
  node_t *t_nil = t->nil ;
  node_t *x = t->root ;  
  while (x->right != t_nil) {
    x = x->right;
  }
  return x;
}
​
​
void rb_delete_fixup(rbtree *t,node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    // CASE 1 ~ 4 : x가 부모노드의 왼쪽 자식인 경우 
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      
      // CASE 1 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right; // 새로운 형제 노드로 업데이트. 계속 진행되면서 경우 2,3,4 에 따라 해결되도록 한다
      } 
​
      // CASE 2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      // x와 w에서 흑색을 하나씩 제거하면서 x는 일반 흑색노드로, w는 적색노드로 변경하고 x->parent에 extra black을 추가해서 다음 반복문을 통해 처리
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      // 형제노드와 형제노드의 적색 왼쪽 자식의 색을 바꾸고 w에 대해 우회전하여 case 4로 만든다.
      else{ 
        if (w->right->color == RBTREE_BLACK) {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotate(t, w);
            w = x->parent->right;
        }
​
        // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        // 부모, 형제, 형제의 오른쪽 자식의 색을 바꾼 후 x->parent에 대해 좌회전
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root; // 더이상 문제의 여지가 없으므로 x를 root로 만들어서 반복문 탈출 
      }
    }
    // CASE 5 ~ 8 : x가 부모노드의 왼쪽 자식인 경우 
    else {
      node_t *w = x->parent->left;
​
      // CASE 5 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
​
      // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
​
      // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else 
      {
          if (w->left->color == RBTREE_BLACK) {
            w->right->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            left_rotate(t, w);
            w = x->parent->left;
            }
​
        // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}
void rb_transplant(rbtree *t, node_t *u, node_t * v) { // u가 루트인 서브트리를 v가 루트인 서브트리로 교체하며 u를 분리(삭제)
  node_t *t_nil = t->nil ;
  if (u->parent == t_nil) {
    t->root = v;
  }
  else if (u == u->parent->left){ // u가 왼쪽 자식이라면 
    u->parent->left = v ;
  }
  else { // u가 오른쪽 자식이라면 
    u->parent->right = v;
  }
  v->parent = u->parent ;
  }  
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // tree_erase(tree, ptr): RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
  node_t *y = p ;  // p의 자식이 두개일 경우 p대신 '실제로' 삭제되는 노드를 의미 (= p의 직후원소)
  node_t *x ;
  node_t *k ;
  node_t *t_nil = t-> nil ;
  color_t y_original_color =  y->color; // 삭제할 노드의 색을 기록 (p가 삭제될수도 있고, p의 직후원소가 삭제될 수도 있다. p의 직후 원소가 삭제되는 케이스에서는 한 번 더 색을 기록해줘야 한다.)
​
  // 자식이 없거나 하나인 케이스
  if (p->left ==t_nil) { //삭제할 노드의 왼쪽 자식이 없다면 (자식이 없거나 하나) transplant로 p를 제거하고 p의 자식과 p의 부모를 연결한다
    x = p->right ;  // x는 p를 대체하는 노드를 가리키는 포인터 (fixup에서 extra black이 붙을 자리)
    rb_transplant(t,p,p->right); // p는 분리(삭제) 된다. 이제 p 자리에는 p->right이 있다. 
  }
  else if (p->right == t_nil) { //삭제할 노드의 오른쪽 자식이 없다면 (왼쪽 자식만 있음) transplant로 p를 제거하고 p의 자식(nil)과 p의 부모를 연결한다
    x = p->left ;  // x는 p를 대체하는 노드를 가리키는 포인터 (fixup에서 extra black이 붙을 자리)
    rb_transplant(t,p,p->left); // p는 분리(삭제) 된다. 이제 p 자리에는 p->left이 있다. 
  }
  // 자식이 두개인 케이스 (직후원소를 찾는 케이스)
  else {
    k = p->right ;  // 직후원소 (p의 오른쪽 자식을 루트로 하는 서브트리에서 최솟값을 찾기)
    while (k->left != t_nil) 
      k = k->left;
    y = k ; // y는 p의 직후원소로서 삭제될 
    y_original_color =  y->color; // 삭제할 노드의 색을 기록 
    x = y->right ; // x는 y를 대체할 노드(y의 오른쪽 자식)를 가리키는 포인터 (fixup에서 extra black이 붙을 자리)
    if (y->parent == p) { 
      x->parent = y ; 
    }
    else {
      rb_transplant(t, y, y->right); // y는 우선 트리에서 떨어져나옴 
      y->right = p->right ; // y의 오른쪽 자식이 p의 오른쪽 자식을 가리키게 함으로서 y는 p를 대체할 준비 1 
      y->right->parent = y ; // p의 오른쪽 자식도 부모로 y를 가리키게 함으로서 y가 p를 대체할 준비 2  
    }
    rb_transplant(t, p, y); // p를 y로 대체함으로서 날려버린다
​
    y->left = p->left;  // y의 왼쪽 자식이 p의 왼쪽 자식을 가리키게 함으로서 y는 p를 대체할 준비 3
    y->left->parent = y ;  // y의 왼쪽 자식도 부모로 y를 가리키게 함으로서 y는 p를 대체할 준비 4
    y->color = p->color ;  // y의 색깔이 p의 색깔을 이어받음으로서 y가 p를 대체할 준비 완료
  }
  if (y_original_color == RBTREE_BLACK) {
    rb_delete_fixup(t, x);
  }
  free(p);
  return 0;
}
​
void dfs(const rbtree *t, node_t *x, size_t *i, key_t *arr, size_t n) {
  // x부터 t를 dfs 해 나간다
	if(x != t->nil){
		dfs(t, x->left, i, arr, n);
    if (*i < n) {
    printf("key: %d, color:%d, parent: %d, left:%d, right:%d,  \n \n", x->key, x->color, x->parent->key, x->left->key, x->right->key);
    arr[(*i)++]= x->key;
    }
    else return;
		dfs(t, x->right, i, arr, n);
	}
  }
​
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root ==t->nil) {
    return 0 ; //빈 트리이므로 배열로 변환할 게 없음 
  }
  size_t i= 0 ;
  printf("Root key: %d, color:%d, parent: %d, left:%d, right:%d,  \n \n", t->root->key, t->root->color, t->root->parent->key, t->root->left->key, t->root->right->key);
  dfs(t, t->root, &i, arr, n); //트리의 루트부터 dfs를 시작 
  return 0;
}