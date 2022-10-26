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

node_t *rbtree_min(const rbtree *t){
  if(t->root == t->nil){
    return NULL;
  }
  node_t *curr = t->root;
  while(curr->left != t->nil){
    curr = curr->left;
  }
  return curr;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil) {
    return NULL;
  }

  node_t * curr = t->root;

  while (curr->right != t->nil) {
    curr = curr->right;
  }
  return curr;
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

void left_rotate(rbtree *t, node_t *x){
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

void right_rotate(rbtree *t, node_t *x){
  //left와 정반대
  node_t *y;
  y = x->left;
  x->left = y->right;
  if (y->right != t->nil) {
      y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
      t->root = y;
  else if (x == x->parent->right)
      x->parent->right = y;
  else
      x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z)
{
  // 이게 뭘까? 포인터로 아무것도 지정 안해줫으니 y는 nil인 상황임.
  node_t *y;

  //삽입은 항상 레드로 하기 때문에 while문에 무조건 걸리게 되어있다.
  while(z->parent->color == RBTREE_RED)
  {
    //아빠가 할아버지 왼쪽에 있는 경우(나는 왼쪽일수도 오른쪽일 수도 있음)
    if (z->parent == z->parent->parent->left)
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
      else{
        //case2: 나 레드+오른쪽 / 아빠 레드+왼쪽 / 삼촌 블랙 
        //아빠를 기준으로 잡고, 왼쪽으로 회전하고, case3방식으로 해결 
        //어차피 case3로 해결할거라 if문으로 case2인지만 체크해 case2면 case3로 만들고 if문 탈출하고 case3해결 시작 
        if(z== z->parent->right){
          // 아빠를 기준으로 잡고
          z = z->parent;
          // 왼쪽으로 회전
          left_rotate(t,z);
        }
      
        //case3: 나 레드+왼쪽/ 아빠 레드+왼쪽/ 삼촌 블랙
        //아빠를 검은색이로 바꾸고 할아버지를 레드로 바꿔 그리고 오른쪽 회전
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    //아빠가 할아버지 오른쪽에 있는 경우
    else{
      y = z->parent->parent->left;
      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }       
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else 
      {
        if (z == z->parent->left) 
        {
          z = z->parent;
          right_rotate(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
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
      if (z ->key < x->key){ 
        x = x->left; //x의 왼쪽인놈이 이제 루트임. 
      }else{
        x = x-> right;//x의 오른쪽인놈이 이제 루트임.
      } 
    }

  //2. x가 드디어 트리의 맨 끝에 도달했으면(x= nil인 상황) 아래를 해.
    z->parent = y; //y에는 현재 x의 부모노드 주소가 들어있고 그게 곧 z의 부모임.
    
    if (y == t->nil) //y도 nil인 경우가 하나 있음. 바로 트리가 비어있어서 z가 최초 삽입인 경우
    {
      t->root = z; //트리에 최초 삽입인 경우 root에 z를 넣는다.
    }
    else if(z->key < y->key){//y가 nil이 아니면 (즉 트리가 비어있지 않아서 x가 타고타고 내려간 상황)
       y->left = z;
    }else{
      y->right = z;
    }

    //레드블랙은 nil노드도 명시해줘야하니까 z의 오른쪽 왼쪽도 nil노드 셋팅
    z->left = t->nil; 
    z->right = t->nil;
    
    //삽입은 레드로한다는 규칙에 따라 z를 red로하고
    z->color = RBTREE_RED;

    rbtree_insert_fixup(t, z);
    
    //디버깅이 잘 안되니 이렇게 print로 찍어보는 방법 좋다!
    printf("key: %d, color:%d, left:%d, right:%d, parent: %d \n \n", z->key, z->color, z->left->key, z->right->key, z->parent->key);
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
  }
    return NULL;    
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  //U를 V가 교체하는 상황 CLRS 299에 잘 설명해놓음.
  
  //u가 루트노드이면 센티넬이니까 루트노드의 parent는 nil노드를 가리키고 있을거임.p311그림b 26번이 t.nil가리키는 중
  if(u->parent == t->nil){
    t->root = v;
  }
  //u가 부모의 왼쪽 자식이면
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x){
  node_t *w; ////w는 x의 형제를 의미함

  //x는 삭제된 놈을 의미함(extra black을 가지고 있는 놈이 x임)
  while (x != t->root && x->color == RBTREE_BLACK){
    //노션 정글개발일지 5주차에 삭제 케이스 상세 정리해놨음.
    //case1~4 left-case 삭제한 x가 왼쪽 자식인 경우
    if (x == x->parent->left){
      //w는 x의 형제를 의미함
      w = x->parent->right;

      //경우1) 형제가 레드인 경우
      if( w->color == RBTREE_RED){
        //형제의 색을 블랙으로 바꾸고
        w->color = RBTREE_BLACK;
        //부모의 색을 레드로 바꿈 (한마디로 회전하기 전에 형제와 부모의 색을 바꿈)
        x->parent->color = RBTREE_RED;
        //부모를 기준으로 좌회전
        left_rotate(t, x->parent);
        //회전해서 x의 형제가 바뀜
        w= x->parent->right;
      }

      //경우2) 형제가 블랙인 경우
      //case 2: 형제의 자녀 모두 블랙
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color= RBTREE_RED;
        x=x->parent;
      }
      //case3: 형제의 왼쪽 자녀가 레드
      // else if (w->right->color == RBTREE_BLACK){
      //    //형제의 왼쪽 자식을 블랙으로 바꿔주고 (한마디로 형제와 형제 왼쪽 자식의 색을 바꿔)
      //     w->left->color = RBTREE_BLACK;
      //     //회전할거니까 검은색이던 형제의 색을 레드로 바꿔주고
      //     w->color = RBTREE_RED;
      //     //우회전
      //     right_rotate(t,w);
      //     //회전해서 x의 형제가 바뀜
      //     w=x->parent->right;
      // }
      else{ //형제의 자녀중 한명 또는 모두 레드인 상황
        if(w->right->color == RBTREE_BLACK){ //왼쪽은 자동으로 레드
          //형제의 왼쪽 자식을 블랙으로 바꿔주고 (한마디로 형제와 형제 왼쪽 자식의 색을 바꿔)
          w->left->color = RBTREE_BLACK;
          //회전할거니까 검은색이던 형제의 색을 레드로 바꿔주고
          w->color = RBTREE_RED;
          //우회전
          right_rotate(t,w);
          //회전해서 x의 형제가 바뀜
          w=x->parent->right;
        }
      
        //case 4: 오른쪽 자녀가 레드
        w->color = x->parent->color;
        x->parent->color =RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x= t->root;
      }
    }
    //case5~8 right-case 삭제한 x가 오른쪽 자식인 경우
    else
    {
      w = x->parent->left;

      // CASE 5 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      // else if (w->left->color == RBTREE_BLACK){
      //     w->right->color = RBTREE_BLACK;
      //     w->color = RBTREE_RED;
      //     left_rotate(t, w);
      //     w = x->parent->left;
      // }
      // w->color = x->parent->color;
      // x->parent->color = RBTREE_BLACK;
      // w->left->color = RBTREE_BLACK;
      // right_rotate(t, x->parent);
      // x = t->root;
      else{
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

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

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y; 
  node_t *x;
  color_t yOriginalColor; //color_t는 enum이고 0(레드) 또는 1(블랙)
  y = p; //굳이 삭제할 노드를 p라고 부르다가 y라고 부르겠대..
  yOriginalColor = y->color;

  if(p->left == t->nil){
    x = p-> right;
    rbtree_transplant(t, p, p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    rbtree_transplant(t,p,p->left);
  }
  else{
    //삭제하려는 노드의 양쪽 자손이 다 있는 경우
    //sucessor찾는 로직(우측 노드의 젤 작은 노드) clrs 326 9번행

    // rbtree subt = {p->right, t->nil};
    // y = rbtree_min(&subt);

    y= p->right;
    while(y->left != t->nil){
      y= y->left;
    }

    //10행
    yOriginalColor = y->color;
    
    //11행
    //y가 sucessor니까 사라질텐데 그 y를 대체할 sucessor x를 지정 
    x = y->right; 

    //12행
    //p를 대체할 Y가 p의 자식인 경우
    if (y->parent==p){
      
      //13행 (x의 부모는 y라고 굳이 한번 더 선언하는 이유는 만약 x가 nil인 경우
      //fixup(x)로 x가 전달 되어 x의 형제들을 봐야하는데 그러려면 x의 부모를 통해서 봐야하지?
      //근데 센티넬이기 때문에 nil을 여러 노드들이 돌려쓴단 말이야? 그럼 현재 nil의 부모는 뭐가 들어있을지 모름 
      //따라서 fixup하면 x의 부모를 통해 형제를 봐야하는데 전혀 무관한 부모를 보게 될수도 있다.
      //따라서 nil노드 x의 parent에 y라고 초기화? 명시적으로 선언해주는 것
      x->parent =y;
    }
    //14행 p를 대체할 y가 p의 자식이 아닌 경우 clrs 326에 그림 그려놓음
    else
    {
      //14행
      rbtree_transplant(t, y, y->right);
      
      //15~16행 (p의 오른쪽 자식과의 관계를 y가 차지하는 과정)
      y->right = p->right;
      y->right->parent = y;
    }
    //17행 (p의 부모와의 관계를 y가 차지하는 과정)
    rbtree_transplant(t,p,y);
    
    //18~19행(p의 왼쪽 자식과의 관계를 y가 차지하는 과정)
    y->left = p->left;
    y->left->parent = y;

    //20행 (p의 색깔마저 y가 차지하는 과정)
    y->color = p->color;
  }

  //21행
  if(yOriginalColor == RBTREE_BLACK){
    rbtree_erase_fixup(t,x);
  }

  //모든 사회적 관계를 y에게 빼앗긴 p는 free로 없애줌
  free(p);
  return 0;
}

// void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count) {
//   if (curr == t->nil) {
//     return;
//   }
//   subtree_to_array(t, curr->left, arr, n, count);
//   if (*count < n) {
//     arr[(*count)++] = curr->key;
//   }
//   else return;
//   subtree_to_array(t, curr->right, arr, n, count);
// }

// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
//   // TODO: implement to_array
//   if (t->root == t->nil) {
//     return 0;
//   }

//   size_t cnt = 0;
//   subtree_to_array(t, t->root, arr, n, &cnt); 
//   return 0;
// }


// 강욱이 코드
void ascending_array_walk(node_t *x, node_t *nil, key_t *arr, int *idx, const size_t n){
  if (x != nil && *idx < n) {
    //트리의 몇번째까지 배열로 만들어라라고 정해주는게 n이니 n까지만 배열로 만들기
    // 중위순열도는 중
    ascending_array_walk(x->left, nil, arr, idx, n); 
    // idx가 0일 때 x key값이 arr에 들어가고 ascending_right가 시작 
    arr[(*idx)++] = x->key;
    ascending_array_walk(x->right, nil, arr, idx, n);
  }
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int i = 0;
  int * idx = &i; //i의 주소를 가리키고있는 포인터 idx를 만들어
//   printf("Root key: %d, color:%d, parent: %d, left:%d, right:%d,  \n \n", t->root->key, t->root->color, t->root->parent->key, t->root->left->key, t->root->right->key);
  ascending_array_walk(t->root, t->nil, arr, idx, n);
  return 0;
}

