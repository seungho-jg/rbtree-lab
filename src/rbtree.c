#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->root = p->nil = nil;
  return p;
}
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left;
  y->left = x->right;
  if (x->right != t->nil)
  {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent == t->nil)
  {
    t->root = x;
  }
  else if (y == y->parent->right)
  {
    y->parent->right = x;
  }
  else
  {
    y->parent->left = x;
  }

  x->right = y;
  y->parent = x;
}
void delete_node(rbtree *t, node_t *node)
{
  if (node->left != t->nil){
    delete_node(t,node->left);
  }
  if (node->right != t->nil){
    delete_node(t,node->right);
  }
    free(node);
    // node=NULL;
}
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if(t != NULL){
    if(t->nil != t->root){
      delete_node(t, t->root);
    }
    // free(t->root);
    // t->root = NULL;
    free(t->nil);
    free(t);
    // t=NULL;
  }
}

void rb_insert_fixed(rbtree *t, node_t *new_node)
{
  node_t *y;
  while (new_node->parent->color == RBTREE_RED)
  {
    if (new_node->parent == new_node->parent->parent->left)
    {
      // y
      y = new_node->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        // y가 빨간색일 경우 색만 바꿔주고 n->p->p 를 new_node로 바꿔주고 다시
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      else{
        if (new_node == new_node->parent->right)
        {
          new_node = new_node->parent;
          left_rotate(t, new_node);
        }
          new_node->parent->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          right_rotate(t, new_node->parent->parent);
      }
    }
    else
    // right <-> left
    {
      y = new_node->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        // y가 빨간색일 경우 색만 바꿔주고 n->p->p 를 new_node로 바꿔주고 다시
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;

      }
      else{
        if (new_node == new_node->parent->left)
        {
          new_node = new_node->parent;
          right_rotate(t, new_node);
        }
          new_node->parent->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          left_rotate(t, new_node->parent->parent);
      }
    }
    // free(y);
  }
  // 루트색 블랙으로 바꾸기
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 노드 초기화
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  if (new_node == NULL)
  {
    return NULL;
  }
  new_node->key = key;
  // 탐색
  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil)
  {
    y = x;
    if (new_node->key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  new_node->parent = y;
  // 처음 노드일 경우
  if (y == t->nil)
  {
    t->root = new_node;
  }
  else if (new_node->key < y->key)
  {
    y->left = new_node;
  }
  else
  {
    y->right = new_node;
  }

  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;
  rb_insert_fixed(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *cur = t->root;
  
  while(cur != t->nil){
    if(cur->key > key){
      cur = cur->left;
    }
    else if(cur->key <key) {
      cur = cur->right;
    }
    else {
      printf("find: %d\n", cur->key);
      return cur;
    }
  }

  

  return NULL;
}

node_t *find_min(rbtree *t, node_t *n)
{
  node_t *min = n;
  while (min->left !=t->nil)
  {
    min = min->left;
  }
  return min;
}

node_t *rbtree_min(const rbtree *t)
{
  node_t *min = t->root;
  while (min->left !=t->nil)
  {
    min = min->left;
  }
  return min;
}

node_t *rbtree_max(const rbtree *t)
{
  node_t *max = t->root;
  while (max->right !=t->nil)
  {
    max = max->right;
  }
  return max;
}
void rb_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil){
    t->root = v;
  } else if (u==u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}
void erase_fixup(rbtree *t, node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color =RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
    if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
      w->color = RBTREE_RED;
      x= x->parent;
    } else {
      if(w->right->color == RBTREE_BLACK){
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t,w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->right->color = RBTREE_BLACK;
      left_rotate(t,x->parent);
      x =t->root;
    }
    } else {
      // 좌우
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color =RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
    if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
      w->color = RBTREE_RED;
      x= x->parent;
    } else {
      if(w->left->color == RBTREE_BLACK){
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t,w);
        w = x->parent->left;
      }
      w->color = x->parent->color;//부
      x->parent->color = RBTREE_BLACK;//깜
      w->left->color = RBTREE_BLACK;//깜
      right_rotate(t,x->parent);
      x =t->root;
    }
    }
  }

  x->color = RBTREE_BLACK;
}
// void erase_fixup(rbtree *t, node_t *x){
//   node_t *w = NULL;

//   while (x != t->root && x->color == RBTREE_BLACK)
//   {
//     if (x == x->parent->left)
//     {
//       w = x->parent->right;
//       // case 1
//       if (w->color == RBTREE_RED)
//       {
//         w->color = RBTREE_BLACK;
//         x->parent->color = RBTREE_RED;
//         left_rotate(t, x->parent);
//         w = x->parent->right;
//       }
//       //case 2
//       if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
//       {
//         w->color = RBTREE_RED;
//         x = x->parent;
//       }
//       //case 3
//       else {
//        if (w->right->color == RBTREE_BLACK){
//         w->left->color = RBTREE_BLACK;
//         w->color = RBTREE_RED;
//         right_rotate(t, w);
//         w = x->parent->right;
//         }
//         //case 4
//         w->color = x->parent->color;
//         x->parent->color = RBTREE_BLACK;
//         w->right->color = RBTREE_BLACK;
//         left_rotate(t, x->parent);
//         x = t->root;
//         }
//     }
//     else{
//       w = x->parent->left;
//       if (w->color == RBTREE_RED)
//       {
//         w->color = RBTREE_BLACK;
//         x->parent->color = RBTREE_RED;
//         right_rotate(t, x->parent);
//         w = x->parent->left;
//       }
//       if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
//       {
//         w->color = RBTREE_RED;
//         x = x->parent;
//       }
//       else {
//         if (w->left->color == RBTREE_BLACK)
//         {
//           w->right->color = RBTREE_BLACK;
//           w->color = RBTREE_RED;
//           left_rotate(t, w);
//           w = x->parent->left;
//         }
//         w->color = x->parent->color;
//         x->parent->color = RBTREE_BLACK;
//         w->left->color = RBTREE_BLACK;
//         right_rotate(t, x->parent);
//         x = t->root;
//       }
//     }
//   }
//   x->color = RBTREE_BLACK;
// }
int rbtree_erase(rbtree *t, node_t *z)
{
  node_t *y = z;
  node_t *x = NULL;
  color_t y_original_color = y->color;
  if (z->left == t->nil){
    x = z->right;
    rb_transplant(t,z,z->right);
  } else if (z->right == t->nil){
    x = z->left;
    rb_transplant(t,z,z->left);
  } else {
    y = find_min(t,z->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z){
      x->parent = y;
    } else {
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK){
    erase_fixup(t,x);
  }
  printf("erase: %d\n", z->key);

  free(z);
  z=NULL;
  return 0;
}

int currentIndex = 0;

void inorder_tree(const rbtree *t, node_t *n, key_t *arr)
{
  if (n == t->nil){
    return;
  }
  inorder_tree(t, n->left, arr);
  arr[currentIndex++] = n->key;
  inorder_tree(t, n->right, arr);
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{ 
  currentIndex = 0;
  inorder_tree(t, t->root, arr);
  return 0;
}

void rbtree_to_print(node_t *t, node_t *nil)
{
  // TODO: implement to_print
  if (t == nil)
  {
    printf("노드 바닥이에용\n");
    return;
  }
  printf("key = %d, color = %d \n", t->key, t->color);
  rbtree_to_print(t->left, nil);
  rbtree_to_print(t->right, nil);
}