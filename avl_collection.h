                  #ifndef AVL_COLLECTION_H
#define AVL_COLLECTION_H

#include "collection.h"

template<typename K,typename V>
class AVLCollection: public Collection<K,V>
{
public:
AVLCollection();
AVLCollection(const AVLCollection<K,V>&rhs);
~AVLCollection();
AVLCollection& operator=(const AVLCollection<K,V>&rhs);
void add(const K& a_key, const V& a_val);
void remove(const K& a_key);
bool find(const K& search_key, V& the_val) const;
void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(ArrayList<K>& all_keys) const;
void sort(ArrayList<K>& all_keys_sorted) const;
size_t size() const;
size_t height()const;
private:
struct Node {
K key;
V value;
int height;
Node* left;
Node* right;
};
// root node of the tree
Node* root;
// number of k-v pairs stored in the collection
size_t node_count;
// rotate right helper
Node* rotate_right(Node* k2);
// rotate left helper
Node* rotate_left(Node* k2);
// rebalance the subtree rooted at subtree_root
Node* rebalance(Node* subtree_root);
//empty AVL tree
void make_empty(Node* subtree_root);
// copy helper
void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
// remove helper
Node* remove(Node* subtree_root, const K& a_key);
//helper to add
Node* add(Node * subtree_root,const K& a_key, const V& a_val);
// helper to recursively build up key list
void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
// helper to recursively build sorted list of keys
void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
// helper to get height of the tree
size_t height(const Node* subtree_root) const;
void print_tree(std::string indent, Node* subtree_root);
};


template<typename K,typename V>
AVLCollection<K,V>::AVLCollection(){
root = nullptr;
node_count =0;
}

template<typename K,typename V>
AVLCollection<K,V>::AVLCollection(const AVLCollection<K,V>&rhs){
root = nullptr;
*this = rhs;


}

template<typename K,typename V>
AVLCollection<K,V>::~AVLCollection(){
if(root!=nullptr){
make_empty(root);
root = nullptr;
}
}

template<typename K,typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::rotate_right(Node* k2){
size_t heightl =0;
size_t heightr =0;
if(k2 == nullptr){
return nullptr;
}
Node * k1 = k2->left;
k2->left = k1->right;
k1->right = k2;
//update height
if(k2!=nullptr){
  //get the larger height of its left and right subtree
   if(height(k2->left) >= height(k2->right)){
   k2->height = 1+ height(k2->left);
   }
   else if(height(k2->right) > height(k2->left)){
   k2->height = 1+ height(k2->right);
   }
}
if(k1!=nullptr){
   //get the larger height of its left and right subtree
  if(height(k1->left) >= height(k1->right)){
  k1->height = 1 + height(k1->left);
  }
  else if(height(k1->right) > height(k1->left)){
  k1->height = 1+ height(k1->right);
  }
}
return k1;
}

template<typename K,typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::rotate_left(Node* k2){
if(k2 == nullptr){
return nullptr;
}
Node * k1 = k2->right;
k2->right = k1->left;
k1->left = k2;
//update height

if(k2!=nullptr){
  //get the larger height of its left and right subtree
  if(height(k2->left) >= height(k2->right)){ 
  k2->height = 1 + height(k2->left);
  }
  else if(height(k2->right) > height(k2->left)){
  k2->height = 1+ height(k2->right);
  }
}

if(k1!=nullptr){
  if(height(k1->left) >= height(k1->right)){
  k1->height = 1+ height(k1->left);
  }
  else if(height(k1->right) > height(k1->left)){
  k1->height = 1 + height(k1->right);
  }
}

return k1;
}

template <typename K,typename V>
AVLCollection<K,V>&AVLCollection<K,V>::operator=(const AVLCollection<K,V>&rhs){
if(this!=&rhs){
if(root != nullptr){
make_empty(root);
}
//copy head pointer
root = new Node();
root->left = nullptr;
root->right= nullptr;
root->key = rhs.root->key;
root->value = rhs.root->value;
//copy left and right pointer
copy(root,rhs.root);
node_count = rhs.size();
}
return *this;
}





template<typename K,typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::add(Node * subtree_root,const K& a_key, const V& a_val){
if(subtree_root == nullptr){
Node * newNode = new Node();
newNode->left = nullptr;
newNode->right = nullptr;
newNode->height = 1;
newNode->key = a_key;
newNode->value = a_val;
//size_t heightl = 0;
//size_t heightr = 0;

subtree_root = newNode;
return subtree_root;
}
else{
 if (a_key < subtree_root->key){
 subtree_root->left = add(subtree_root->left, a_key, a_val);
 }
 else{
 subtree_root->right = add(subtree_root->right, a_key, a_val);
 }
}
// backtracking:
//... update height of subtree_root as needed ...
//update height
if(height(subtree_root->left) >= height(subtree_root->right)){
 subtree_root->height = 1+ height(subtree_root->left);
}
else if(height(subtree_root->right) > height(subtree_root->left)){
 subtree_root->height = 1+ height(subtree_root->right);
}
// backtracking: rebalance at the subtree_root (as needed)
return rebalance(subtree_root);

}

template<typename K,typename V>
void AVLCollection<K,V>::remove(const K& a_key){
root = remove(root,a_key);
if(root!=nullptr){
root->height = height(root);
}
}

template<typename K,typename V>
size_t AVLCollection<K,V>::height(const Node * subtree_root)const{

if(subtree_root == nullptr){  
   return 0;  
}
//return height at tree no need for recursion
else{ 
   return subtree_root->height;
}

}

template<typename K,typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::rebalance(Node* subtree_root){
size_t heightl =0;
size_t heightr =0;
 
 if(subtree_root == nullptr){ // nothing to balance
 return subtree_root;
 }

Node* lptr = subtree_root->left;
Node* rptr = subtree_root->right;

// left but no right pointer (special case)
if((lptr != nullptr)&&(rptr == nullptr)&&(height(lptr) > 1)){
  if(height(subtree_root->left->left) > height(subtree_root->left->right)){
//rotate right
  return rotate_right(subtree_root);
  }
//... check if need double rotation ..
 else{
   if(subtree_root->left !=nullptr){
   subtree_root->left = rotate_left(subtree_root->left);
   return rotate_right(subtree_root);
   }
 }
}

// right but no left pointer (special case)
//right heavy
else if((rptr != nullptr)&&(lptr == nullptr)&&(height(rptr) > 1)){
 if(height(subtree_root->right->right ) > height(subtree_root->right->left)){
  //rotate left
    if(subtree_root !=nullptr){
    return rotate_left(subtree_root);
    }
  }
//... check if need double rotation ..
  else if(height(rptr) > 1){
    if(subtree_root->right !=nullptr){
     subtree_root->right = rotate_right(subtree_root->right);
     return rotate_left(subtree_root);
    }
  }
}

else if((lptr != nullptr) && (rptr!= nullptr)){
size_t balance = height(lptr) - height(rptr);

//left heavy
if((height(lptr) > height(rptr))&&(balance > 1)){
size_t ll = 0;
size_t lr =0;
if(lptr->right != nullptr){
 lr = height(lptr->right);
}
if(lptr->left !=nullptr){
 ll = height(lptr->left);
}

if(ll >= lr){
                                                                                                                                                                                                                                         
  if(subtree_root !=nullptr){
  //rotate right
  return rotate_right(subtree_root);
  }
}
//... check if need double rotation ..

else if(lr > ll){
  if(subtree_root->left !=nullptr){
  subtree_root->left = rotate_left(subtree_root->left);
  return rotate_right(subtree_root);
  }
}

}

//right heavy
else if((height(rptr) > height(lptr))&&(balance < -1)){
 size_t rr = 0;
 size_t rl =0;
 if(rptr->right != nullptr){
   rr = height(rptr->right);
 }
  if(rptr->left !=nullptr){ 
  rl = height(rptr->left);
  }
  if(rr >= rl){
    if(subtree_root !=nullptr){
    //rotate left
    return rotate_left(subtree_root);
    }
  }
//... check if need double rotation ..

else if(rl > rr){
  if(subtree_root->right !=nullptr){
  subtree_root->right = rotate_right(subtree_root->right);
  return rotate_left(subtree_root);
  }
}

}
}
return subtree_root;
}

template<typename K,typename V>
void AVLCollection<K,V>::add(const K& a_key, const V& a_val){
root = add(root,a_key,a_val);
root->height = height(root);
node_count++;
}



template<typename K,typename V>
bool AVLCollection<K,V>::find(const K& search_key, V& the_val) const{
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
void AVLCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const{
 if(root != nullptr){
 find(root,k1,k2,keys);
 }
}

template<typename K,typename V>
void AVLCollection<K,V>::keys(ArrayList<K>& all_keys) const{
keys(root,all_keys);
}

template<typename K,typename V>
void AVLCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const{
keys(root,all_keys_sorted);

}


template<typename K,typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root){
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
void AVLCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root){

if(rhs_subtree_root == nullptr){
return;
}

else{
//copy left subtree
 if(rhs_subtree_root->left != nullptr) { 
 lhs_subtree_root->left = new Node();
 lhs_subtree_root->left->key = rhs_subtree_root->left->key;
 lhs_subtree_root->left->value = rhs_subtree_root->left->value;
 lhs_subtree_root->left->height = rhs_subtree_root->left->height;
 copy(lhs_subtree_root->left,rhs_subtree_root->right); 
 }
//copy right subtree
 if(rhs_subtree_root->right != nullptr) {
lhs_subtree_root->right = new Node();
 lhs_subtree_root->right->key = rhs_subtree_root->right->key;
 lhs_subtree_root->right->value = rhs_subtree_root->right->value;
 lhs_subtree_root->right->height = rhs_subtree_root->right->height;
 copy(lhs_subtree_root->right,rhs_subtree_root->right); 
 
 }

}

}



template<typename K,typename V>
typename AVLCollection<K,V>::Node* AVLCollection<K,V>::remove(Node* subtree_root, const K& a_key){
Node * prev = nullptr;
size_t heightl=0;
size_t heightr =0;

//navigate to find node to remove
if(subtree_root == nullptr){
return nullptr;
}
//find the node to remove
if((subtree_root!=nullptr)&&(subtree_root->key > a_key)){ //key to delete is smaller, so we search tree left
prev = subtree_root;
subtree_root->left = remove(subtree_root->left,a_key);
//subtree_root->height = height(subtree_root);

}
else if((subtree_root!=nullptr)&&(subtree_root->key < a_key)){ //key to delete is larger, so we search tree right
prev = subtree_root;
subtree_root->right = remove(subtree_root->right,a_key);
//subtree_root->height = height(subtree_root);

}
else{//keys are equal

	 if (subtree_root->left == nullptr) 
        { 
            Node *temp = subtree_root->right; 
            delete subtree_root;
	    subtree_root = nullptr;
            node_count = node_count-1; 	
            return temp; 
        } 
        else if(subtree_root->right == nullptr) 
        { 
            Node *temp = subtree_root->left; 
            delete subtree_root; 
	    subtree_root = nullptr;
            node_count = node_count-1;
            return temp; 
         }
	else{
                //step 2,3,4
		Node * hlp = subtree_root;
		//go right
		hlp = hlp->right;
		//go most left
		while(hlp->left !=nullptr){
		hlp = hlp->left;
		}
		//copy inorder values
		K tempkey= subtree_root->key;
                V tempValue = subtree_root->value;
		subtree_root->key = hlp->key;
		subtree_root->value = hlp->value;
		//delete 
		hlp->key = tempkey;
                hlp->value = tempValue;
		subtree_root->right = remove(subtree_root->right,hlp->key);
		
			
	} 	
}
//update height

if(height(subtree_root->left) >= height(subtree_root->right)){
subtree_root->height = 1+ height(subtree_root->left);
}
else if(height(subtree_root->right) > height(subtree_root->left)){
subtree_root->height = 1+ height(subtree_root->right);
}

//rebalance tree
return rebalance(subtree_root);

}

template<typename K,typename V>
void AVLCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const{
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
void AVLCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const{
if(subtree_root == nullptr){
return;
}

keys(subtree_root->left,all_keys);
all_keys.add(subtree_root->key);
keys(subtree_root->right,all_keys);

}

template<typename K,typename V>
size_t AVLCollection<K,V>::size() const{
return node_count;
}



template<typename K,typename V>
size_t AVLCollection<K,V>::height() const{
return height(root);
}

template<typename K, typename V>
void AVLCollection<K,V>::print_tree(std::string indent, Node* subtree_root)
{
if(subtree_root == nullptr){
return;
}
std::cout << indent << subtree_root->key << " (h=" << subtree_root->height << ")" << std::endl;
print_tree(indent + " ", subtree_root->left);
print_tree(indent + " ", subtree_root->right);
}

#endif



