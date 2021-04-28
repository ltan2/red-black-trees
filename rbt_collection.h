//----------------------------------------------------------------------
// Name: Lin Ai Tan 
// File: rbt_collection.h
// Date: Fall, 2020
// Desc: A Key-Value Collection implementation using a Red-Black tree.
//----------------------------------------------------------------------


#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H


#include "string.h"
#include "collection.h"
#include "array_list.h"


template<typename K, typename V>
class RBTCollection : public Collection<K,V>
{
public:

  // create an empty collection
  RBTCollection();
  
  // copy constructor
  RBTCollection(const RBTCollection<K,V>& rhs);

  // assignment operator
  RBTCollection<K,V>& operator=(const RBTCollection<K,V>& rhs);

  // delete collection
  ~RBTCollection();
  
  // add a new key-value pair into the collection 
  void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collectiona
  void remove(const K& a_key);

  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;

  // find and return each key >= k1 and <= k2 
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  
  // return all of the keys in the collection 
  void keys(ArrayList<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
  void sort(ArrayList<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
  size_t size() const;

  // return the height of the tree
  size_t height() const;

  // for testing:

  // check if tree satisfies the red-black tree constraints
  bool valid_rbt() const;
  
  // pretty-print the red-black tree (with heights)
  void print() const;

  
private:
  
  // RBT node structure
  enum color_t {RED, BLACK};
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
    Node* parent;
    color_t color;
  };

  // root node
  Node* root;

  // number of k-v pairs stored in the collection
  size_t node_count;

  // helper to empty entire hash table
  void make_empty(Node* subtree_root);

  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root); 
    
  // helper to recursively find range of keys
  void find(const Node* subtree_root, const K& k1, const K& k2,
            ArrayList<K>& keys) const;

  // helper to build sorted list of keys (used by keys and sort)
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;

  // rotate right helper
  void rotate_right(Node* k2);

  // rotate left helper
  void rotate_left(Node* k2);

  // restore red-black constraints in add
  void add_rebalance(Node* x);

  // restore red-black constraints in remove
  void remove_rebalance(Node* x, bool going_right);
  
  // height helper
  size_t height(Node* subtree_root) const;
  
  // ------------
  // for testing:
  // ------------
  
  // validate helper
  bool valid_rbt(Node* subtree_root) const;

  // validate helper
  size_t black_node_height(Node* subtree_root) const;
  
  // recursive pretty-print helper
  void print_tree(std::string indent, Node* subtree_root) const;
};


// TODO: Finish the above functions below
template<typename K,typename V>
RBTCollection<K,V>::RBTCollection(){
root = nullptr;
node_count =0;
}

template<typename K,typename V>
RBTCollection<K,V>::RBTCollection(const RBTCollection<K,V>& rhs){
root = nullptr;
node_count = 0;
*this = rhs;
}


template<typename K,typename V>
RBTCollection<K,V>&::RBTCollection<K,V>::operator=(const RBTCollection<K,V>& rhs){
if(this!=&rhs){
if(root != nullptr){
make_empty(root);
}
root = new Node();
root->left = nullptr;
root->right= nullptr;
root->key = rhs.root->key;
root->value = rhs.root->value;
root->color = rhs.root->color;
 //copy left and right pointer
 copy(this->root,rhs.root);
 this->node_count = rhs.size();
}
return *this;
}

template<typename K,typename V>
RBTCollection<K,V>::~RBTCollection(){
 if(root!=nullptr){
 make_empty(root);
 root = nullptr;
 }
}

template<typename K,typename V>
void RBTCollection<K,V>::add(const K& a_key, const V& a_val){
Node * newNode = new Node();
newNode->key = a_key;
newNode->value = a_val;
newNode->color = RED;
newNode->left = nullptr;
newNode->right = nullptr;
newNode->parent = nullptr;
Node * x = nullptr;
x = root; //x is curr node
Node * p = nullptr;
//print_tree("",root);
while(x!= nullptr){
 add_rebalance(x);
  p = x;
  if(a_key < x->key){
   x = x->left;
  }
  else{
   x = x->right;
  }
}
  if(p==nullptr){
  root = newNode;
  //root->left = nullptr;
  //root->right = nullptr;
  newNode->parent = nullptr;
  }
  else if(a_key < p->key){
  p->left = newNode;
  newNode->parent = p;
  }
  else{
  p->right = newNode;
  newNode->parent = p;
  }
  add_rebalance(newNode);
  root->color = BLACK;
  node_count++;
 // print_tree("",root);
}

template<typename K,typename V>
void RBTCollection<K,V>::remove(const K& a_key){
//print();

//std::cout << "key is" << a_key << std::endl;
Node * sentinel = new Node();
sentinel->right = root;
sentinel->left = nullptr;
sentinel->color = RED;
Node * x = root;
if(x==nullptr){//empty tree
return;
}
x->parent = sentinel;
Node * p = sentinel;
bool found = false;
//bool isVal = valid_rbt();

while((x!=nullptr)&&(found ==false)){
 if(a_key < x->key){
  remove_rebalance(x,false);
  //std::cout << "x->key is " << x->key << "x-color is " << x->color << std::endl;
  x = x->left;
 }
 else if(a_key > x->key){
  remove_rebalance(x,true);
  //std::cout << "x->key is " << x->key << "x-color is " << x->color << std::endl;
  x = x->right;
 }
 else{
  // if(x!=sentinel->right){
   if(x->left!=nullptr){ //we are moving towards right
    remove_rebalance(x,true); 
    //std::cout << "x->key is " << x->key << "x-color is " << x->color << std::endl;
   }
   else{ // we are moving towards left
    remove_rebalance(x,false);
    //std::cout << "x->key is " << x->key << "x-color is " << x->color << std::endl;
  }
 // }
   found = true;
 }
 if(x!=nullptr){
 p = x->parent;
 }
  
}
//if(x->parent->parent!=nullptr){
//print_tree("",x->parent->parent->parent->parent);
//}
// no such node exist to delete
if(found != false){
//return;
//}

if(x!=nullptr){
//std::cout << "x key is 8 " << std::endl;
if(x->left == nullptr){
//one child case
    if(x == sentinel->right){
  //  std::cout <<"1"<<std::endl;
    sentinel->right = x->right;
    }
    else{
    //std::cout <<"2 is here"<<std::endl;
    Node * ptemp = x->parent; 
    if(ptemp->right ==x){
      ptemp->right = x->right;
      if(x->right !=nullptr){
       x->right->parent = ptemp;
      }
       //else{
       //std::cout << "hi";
       //}
    }
    else{//ptemp left is x
      ptemp->left = x->right;
      if(x->right!=nullptr){
      x->right->parent = ptemp;  
      }
      else{
      //print_tree("",x->parent->parent->parent->parent->parent);
     // std::cout << "x to be deleted" << x->key << std::endl;
      //std::cout << "x color is " << x->color << std::endl;
      }
    }
    
  }
    delete x;
    x =nullptr;
    
} 
else if(x->right == nullptr){
//std::cout << "hi2";
   if(x == sentinel->right){
    //std::cout <<"3"<<std::endl;
    sentinel->right = x->right;
    }
   else{ 
   //std::cout <<"4"<<std::endl;
   Node *ptemp = x->parent;
   if(ptemp->right ==x){
    ptemp->right = x->left;
     if(x->right!=nullptr){
     x->right->parent = ptemp;  
     }
    }
    else{
    ptemp->left = x->right;
      if(x->right!=nullptr){
      x->right->parent = ptemp;  
      }
    }
    
   } 
   delete x; 
   x = nullptr;
   //return;
}
//two child case
else{
   //print();
   //std::cout << "hi3";
   Node * s = x->right;
   remove_rebalance(s,false); // we are going left
   while(s->left != nullptr){
   s = s->left;
   remove_rebalance(s,false);
   }
   x->key = s->key;
   x->value = s->value;
   if(s!=nullptr){
     if(s->left == nullptr){
     //std::cout << "hi1";
     //print();
     //one child case
      if(s == sentinel->right){
       //std::cout <<"1"<<std::endl;
       sentinel->right = s->right;
      }
      else{
      // std::cout <<"hyaa"<<std::endl;
      // std::cout <<s->parent->key<<std::endl;
      // print();
       Node * ptemp = s->parent; 
       if(ptemp->right ==s){
       ptemp->right = s->left;
         if(s->right!=nullptr){
          s->right->parent = ptemp;  
         }
       }
      else{
       ptemp->left = s->right;
        if(s->left!=nullptr){
         s->left->parent = ptemp;  
        }
      }
      
     }
     //print();
     delete s; 
     s =nullptr; 
    } 
    else if(s->right == nullptr){
    // std::cout << "hi2";
     if(s == sentinel->right){
      //std::cout <<"3"<<std::endl;
      sentinel->right = s->right;
     }
     else{ 
      //std::cout <<"4"<<std::endl;
      Node *ptemp = s->parent;
      if(ptemp->right ==s){
       ptemp->right = s->left;
         if(s->right!=nullptr){
          s->right->parent = ptemp;  
         }
      }
      else{
       ptemp->left = s->right;
       if(s->left!=nullptr){
        s->left->parent = ptemp;  
      }
     }
      
     } 
     delete s; 
      s = nullptr;
    }
  }
}
}

}
root = sentinel->right;
if(root!=nullptr){
root->color = BLACK;
}
delete sentinel;
sentinel = nullptr;
//std::cout << "is valid rbt?" << valid_rbt() << std::endl;
node_count = node_count-1;
//print();

}
        

template<typename K,typename V>
bool RBTCollection<K,V>::find(const K& search_key, V& the_val) const{
Node * hlp = root;

while(hlp!= nullptr){
  if(hlp->key == search_key){//keys match
  the_val = hlp->value; 
  return true;
  }
  if(hlp->key > search_key){ //search key is smaller, so we search the tree left
  hlp = hlp->left;
  }
  else{
  hlp = hlp->right; //search key is larger, so we search tree right
  }
}

return false;
}


template<typename K,typename V>
void RBTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const{
 if(root != nullptr){
 find(root,k1,k2,keys);
 }
}

template<typename K,typename V>
void RBTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const{
keys(root,all_keys_sorted);
}

template<typename K,typename V>
size_t RBTCollection<K,V>::size() const{
return node_count;
}

template<typename K,typename V>
size_t RBTCollection<K,V>::height()const{
return height(root);
}

template<typename K,typename V>
void RBTCollection<K,V>::make_empty(Node* subtree_root){
 if(subtree_root== nullptr)
  {
    return;
  }

  make_empty(subtree_root->left);
  make_empty(subtree_root->right);

delete subtree_root;
subtree_root = nullptr; //neccessary

}

template<typename K,typename V>
void RBTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root){
if(rhs_subtree_root == nullptr){
return;
}

else{
//copy left subtree
 if(rhs_subtree_root->left != nullptr) { 
 lhs_subtree_root->left = new Node();
 lhs_subtree_root->left->key = rhs_subtree_root->left->key;
 lhs_subtree_root->left->value = rhs_subtree_root->left->value;
 lhs_subtree_root->left->color = rhs_subtree_root->left->color;
 copy(lhs_subtree_root->left,rhs_subtree_root->right); 
 }
//copy right subtree
 if(rhs_subtree_root->right != nullptr) {
lhs_subtree_root->right = new Node();
 lhs_subtree_root->right->key = rhs_subtree_root->right->key;
 lhs_subtree_root->right->value = rhs_subtree_root->right->value;
 lhs_subtree_root->right->color = rhs_subtree_root->right->color;
 copy(lhs_subtree_root->right,rhs_subtree_root->right); 
 
 }
 
}
}
template<typename K,typename V>
void RBTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2,ArrayList<K>& keys) const{
//base case
if(subtree_root == nullptr){
return;
}
if(subtree_root->key < k1){ //current node is smaller than the left bound, so we know all of its left won't be in the range and we search the right
find(subtree_root->right,k1,k2,keys);
}
if((subtree_root->key >= k1)&&(subtree_root->key <= k2)){//current node is between, so its left and right can also be.
keys.add(subtree_root->key);
find(subtree_root->left,k1, k2,keys);
find(subtree_root->right,k1, k2,keys);
}
if(subtree_root->key > k2){ //current node is larger than the right bound, so we all know all of its right won't be in the range and we searc the left
find(subtree_root->left,k1, k2,keys);
}

}

template<typename K,typename V>
void RBTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const{
if(subtree_root == nullptr){
return;
}

keys(subtree_root->left,all_keys);
all_keys.add(subtree_root->key);
keys(subtree_root->right,all_keys);

}

template<typename K,typename V>
void RBTCollection<K,V>::rotate_right(Node* k2){

if(k2 != nullptr){
Node * p = k2->parent;
Node * k1 = k2->left;
k2->left = k1->right;
//update k2 left node parent
if(k2->left!=nullptr){
k2->left->parent = k2;
}
//2. update k1 's parent
k1->parent = p;
// 3. set k1 's new parent to point to k1 ( note could be null )
if(k1->parent!=nullptr){
 if(k1->parent->key > k1->key){
 k1->parent->left = k1;
 }
 else{
 k1->parent->right = k1;
 }
}
else{
root = k1;
}

k1->right = k2;
k2-> parent = k1;
//print_tree("",root);
}
}

template<typename K,typename V>
void RBTCollection<K,V>::rotate_left(Node* k2){

if(k2 != nullptr){
Node * p = k2->parent;

Node * k1 = k2->right;
k2->right = k1->left;
//update k2 parent
if(k2->right!=nullptr){
k2->right->parent = k2;
}
//2. update k1 's parent
k1->parent = p;

// 3. set k1 's new parent to point to k1 ( note could be null )
if(k1->parent!=nullptr){
 if(k1->parent->key > k1->key){
 k1->parent->left = k1;
 }
 else{
 k1->parent->right = k1;
 }
}
else{
root = k1;
}
k1->left = k2;
k2-> parent = k1;
}

}

template<typename K,typename V>
void RBTCollection<K,V>::add_rebalance(Node* x){
if(x == nullptr){
return;
}
Node * p = x->parent;


if((x->color == BLACK)&&(x->left!=nullptr)&&(x->left->color == RED)&&(x->right!=nullptr)&&(x->right->color == RED)){
x->color = RED;
x->left->color= BLACK;
x->right->color= BLACK;
}

// case 2 and 3: rotations
if(p!=nullptr){

if((x->color== RED)&&(p->color ==RED)){
// handle p as the root
  if((p!=nullptr)&&(p->parent == nullptr)){
  //p->right = x;
   if(x->key < p->key){
   p->left = x;
   }
   else{
   p->right = x;
   }
  x->color = RED;
  x->parent =p;
  }

// handle right - right and right - left cases

 else if((p!=nullptr)&&(p->parent->right == p)){
 // handle inside (right - left ) case
  if(p->left == x){
  rotate_right(p);
  x =p;
  }
  // handle outside (right - right ) case
  rotate_left(x->parent->parent);
  x->parent->color = BLACK;
  x->parent->left->color= RED;
  }
  // handle left - left and left - right cases
  else {
    //handle inside (left - right ) case (see above )
    if(p->right ==x){
    rotate_left(p);
    x = p;
    }

    // handle ouside (left - left ) case (see above )
    rotate_right(x->parent->parent);
    x->parent->color = BLACK;
    x->parent->right->color = RED;
  }
 }
}

}

template<typename K,typename V>
void RBTCollection<K,V>::remove_rebalance(Node* x, bool going_right){

 if(x == nullptr){
 return;
 }
 if(x->color==RED){
  return;
 }


 Node * p = x->parent;
 Node * t = nullptr; // x's siblings
if(p!=nullptr){
 if(p->left == x){
 t = p->right;
 }
 else{
 t = p->left;
 }
}
if(t!=nullptr){
//std::cout << " t is" << t->color << std::endl;
}
if(x->left !=nullptr){
//std::cout << "left child is" << x->left->color << std::endl;
}
if(x->right!=nullptr){
//std::cout << "right child is" << x->right->color << std::endl;
}
//case 1
//std::cout << "jese";
if(((x->left!=nullptr)&&(x->left->color == RED))||((x->right!=nullptr)&&(x->right->color == RED))){
 if(x->left != nullptr){ // left side is red
   if(((x->right == nullptr)&&(x->left->color == RED))||((x->right->color == BLACK)&&(x->left->color == RED))){
       if(going_right == true){
       rotate_right(x);
       //update color
       x->color = RED;
       if((x->parent !=nullptr)&&(x->parent->parent !=nullptr)){
       x->parent->color = BLACK;
       }
      }
    }
 }

  if(x->right != nullptr){ //right side is red
     if(((x->left == nullptr)&&(x->right->color == RED))||((x->left->color == BLACK)&&(x->right->color==RED))){
       if(going_right == false){
       rotate_left(x);
       //update color
       x->color = RED;
       if((x->parent !=nullptr)&&(x->parent->parent !=nullptr)){
       x->parent->color = BLACK;
       }
     }
    }
 } 
}

//case 2
else if((t!=nullptr)&&((t->left==nullptr)||(t->left->color == BLACK))&&((t->right==nullptr)||(t->right->color == BLACK))){

  x->color = RED;
  x->parent->color = BLACK;
  t->color = RED;
}

//case 3 or case 4
else if(t!=nullptr){

 
   // case 4 right left case
   if((t->left !=nullptr)&&(t->parent->right == t)&&(t->left->color == RED)){

    //double rotation
    rotate_right(t);
    rotate_left(x->parent);
    x->parent->color = BLACK;
    x->color = RED;
   }
   //case 4 left right case
   else if((t->right!=nullptr)&&(t->parent->left == t)&&(t->right->color == RED)){

    //double rotation
    rotate_left(t);
    rotate_right(x->parent);
    x->parent->color = BLACK;
    x->color = RED;
   }
   else if((t->right!=nullptr)&&(t->parent->right == t)&&(t->right->color == RED)){//case 3 right
    rotate_left(t->parent);
    t->color = RED;
    t->right->color = BLACK;
    x->color = RED;
    x->parent->color = BLACK;
   }
  // case 3 left left
   else if(((t->left!=nullptr)&&(t->left->color == RED))||((t->right!=nullptr)&&(t->right->color == RED))){ 
   rotate_right(t->parent);
   t->color = RED;
   t->left->color = BLACK;
   x->color = RED;
   x->parent->color = BLACK;
   } 
  

   
   x->color = RED;
   p->color = BLACK;
 }
}   


template<typename K,typename V>
size_t RBTCollection<K,V>::height(Node* subtree_root) const{
if (subtree_root == nullptr){  
        return 0;  
}
    else{  
        /* compute the depth of each subtree */
        size_t lDepth = height(subtree_root->left);  
        size_t rDepth = height(subtree_root->right);  
      
        /* use the larger one */
        if (lDepth > rDepth){  
            return(lDepth + 1);
	}  
        else{
	 return(rDepth + 1);
	}
}
}

template<typename K,typename V>
void RBTCollection<K,V>::keys(ArrayList<K>& all_keys) const{
keys(root,all_keys);
}


//----------------------------------------------------------------------
// Provided Helper Functions:
//----------------------------------------------------------------------

template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt() const
{
  return !root or (root->color == BLACK and valid_rbt(root));
}


template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt(Node* subtree_root) const
{
  if (!subtree_root)
    return true;
  color_t rc = subtree_root->color;
  color_t lcc = subtree_root->left ? subtree_root->left->color : BLACK;
  color_t rcc = subtree_root->right ? subtree_root->right->color : BLACK;  
  size_t lbh = black_node_height(subtree_root->left);
  size_t rbh = black_node_height(subtree_root->right);
  bool lv = valid_rbt(subtree_root->left);
  bool rv = valid_rbt(subtree_root->right);
  // check equal black node heights, no two consecutive red nodes, and
  // left and right are valid RBTs
  return (lbh == rbh) and (rc != RED or (lcc != RED and rcc != RED)) and lv and rv;
}


template<typename K, typename V>
size_t RBTCollection<K,V>::black_node_height(Node* subtree_root) const
{
  if (!subtree_root)
    return 1;
  size_t hl = black_node_height(subtree_root->left);
  size_t hr = black_node_height(subtree_root->right);
  size_t h = hl > hr ? hl : hr;
  if (subtree_root->color == BLACK)
    return 1 + h;
  else
    return h;
}


template<typename K, typename V>
void RBTCollection<K,V>::print() const
{
  print_tree("", root);
}


template<typename K, typename V>
void RBTCollection<K,V>::print_tree(std::string indent, Node* subtree_root) const
{
  if (!subtree_root)
    return;
  std::string color = "[BLACK]";
  if (subtree_root->color == RED)
    color = "[RED]";
  std::cout << indent << subtree_root->key << " "
	    << color << " (h="
	    << height(subtree_root) << ")" << std::endl;
  print_tree(indent + "  ", subtree_root->left);
  print_tree(indent + "  ", subtree_root->right);
}


#endif
