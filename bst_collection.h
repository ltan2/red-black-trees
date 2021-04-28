#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include "collection.h"

template<typename K,typename V>
class BSTCollection: public Collection<K,V>
{
public:
BSTCollection();
BSTCollection(const BSTCollection<K,V>&rhs);
~BSTCollection();
BSTCollection& operator=(const BSTCollection<K,V>&rhs);
void add(const K& a_key, const V& a_val);
void remove(const K& a_key);
bool find(const K& search_key, V& the_val) const;
void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
void keys(ArrayList<K>& all_keys) const;
void sort(ArrayList<K>& all_keys_sorted) const;
size_t size() const;
size_t height()const;
private:
// tree node
struct Node {
K key;
V value;

Node* left;
Node* right;
};
// root node of the tree
Node* root;
// number of k-v pairs stored in the collection
size_t node_count;
// remove all elements in the bst
void make_empty(Node* subtree_root);
// copy helper
void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root);
// remove helper
Node* remove(Node* subtree_root, const K& a_key);
// helper to recursively build up key list
void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const;
// helper to recursively build sorted list of keys
void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;
// helper to recursively find height of the tree
size_t height(const Node* subtree_root) const;
};

template<typename K,typename V>
BSTCollection<K,V>::BSTCollection(){
root = nullptr;
node_count =0;
}

template<typename K,typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection<K,V>&rhs){
root = nullptr;
*this = rhs;

}

template<typename K,typename V>
BSTCollection<K,V>::~BSTCollection(){
if(node_count > 0){
	if(root!=nullptr){
	// call make empty function to delete all the contents of the array and the array itself
	make_empty(root);
	}
}
root = nullptr;
}


template <typename K,typename V>
BSTCollection<K,V>&BSTCollection<K,V>::operator=(const BSTCollection<K,V>&rhs){
if(this!=&rhs){
if(root!=nullptr){
// call make empty function to delete all the contents of the array and the array itself
  make_empty(root);
  root = nullptr;
  node_count =0;
 }
//if(this!=&rhs){
root = new Node();
root->left = nullptr;
root->right= nullptr;
root->key = rhs.root->key;
root->value = rhs.root->value;
copy(root,rhs.root);
node_count = rhs.size();
}
return *this;
}


template<typename K,typename V>
void BSTCollection<K,V>::add(const K& a_key, const V& a_val){
Node * newNode = new Node();
newNode->key = a_key;
newNode->value = a_val;
newNode->left = nullptr;
newNode->right = nullptr;
bool mleft = false;
bool mright = false;

//if tree is empty
if(root == nullptr){
root = newNode;
node_count++;
}
//tree not empty
else{
Node * hlp = root;
Node * prev = hlp;
	while(hlp!= nullptr){
	mleft = false;
	mright = false; 
		if(hlp->key < a_key){
		prev = hlp;
		mright = true;
		hlp = hlp->right;
		}
		else{
		mleft = true;
		prev = hlp;
		hlp = hlp->left;
		}
	}
//hlp is null
if(mright == true){
prev->right = newNode;
node_count++;
}
if(mleft == true){
prev->left = newNode;
node_count++;
}

}
}

template<typename K,typename V>
void BSTCollection<K,V>::remove(const K& a_key){
//find then remove
V val;
bool found = find(a_key,val);
if(found == true){
root = remove(root,a_key);
}

}

template<typename K,typename V>
bool BSTCollection<K,V>::find(const K& search_key, V& the_val) const{
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
void BSTCollection<K,V>::find(const K& k1, const K& k2, ArrayList<K>& keys) const{
if(root != nullptr){
find(root,k1,k2,keys);
}
}

template<typename K,typename V>
void BSTCollection<K,V>::keys(ArrayList<K>& all_keys) const{
keys(root,all_keys);
}

template<typename K,typename V>
void BSTCollection<K,V>::sort(ArrayList<K>& all_keys_sorted) const{
keys(root,all_keys_sorted);

}


template<typename K,typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root){
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
void BSTCollection<K,V>::copy(Node* lhs_subtree_root, const Node* rhs_subtree_root){
if(rhs_subtree_root == nullptr){
return;
}

else{

 if(rhs_subtree_root->left != nullptr) { 
 lhs_subtree_root->left = new Node();
 lhs_subtree_root->left->key = rhs_subtree_root->left->key;
 lhs_subtree_root->left->value = rhs_subtree_root->left->value;
 copy(lhs_subtree_root->left,rhs_subtree_root->right); 
 }

 if(rhs_subtree_root->right != nullptr) {
 lhs_subtree_root->right = new Node();
 lhs_subtree_root->right->key = rhs_subtree_root->right->key;
 lhs_subtree_root->right->value = rhs_subtree_root->right->value;
 copy(lhs_subtree_root->right,rhs_subtree_root->right); 
 
 }

}

}

template<typename K,typename V>
typename BSTCollection<K,V>::Node* BSTCollection<K,V>::remove(Node* subtree_root, const K& a_key){
Node * prev = nullptr;

//navigate to find node to remove
if(subtree_root == nullptr){
return nullptr;
}
//find the node to remove
if((subtree_root!=nullptr)&&(subtree_root->key > a_key)){ //key to delete is smaller, so we search tree left
prev = subtree_root;
subtree_root->left = remove(subtree_root->left,a_key);
}
else if((subtree_root!=nullptr)&&(subtree_root->key < a_key)){ //key to delete is larger, so we search tree right
prev = subtree_root;
subtree_root->right = remove(subtree_root->right,a_key);
}
else{//keys are equal

	 if (subtree_root->left == nullptr) 
        { 
            Node *temp = subtree_root->right; 
            delete subtree_root;
            node_count = node_count-1; 
            return temp; 
        } 
        else if(subtree_root->right == nullptr) 
        { 
            Node *temp = subtree_root->left; 
            delete subtree_root; 
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
return subtree_root;
}

template<typename K,typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys) const{
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
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys) const{
if(subtree_root == nullptr){
return;
}

keys(subtree_root->left,all_keys);
all_keys.add(subtree_root->key);
keys(subtree_root->right,all_keys);

}

template<typename K,typename V>
size_t BSTCollection<K,V>::size() const{
return node_count;
}

template<typename K,typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root) const{
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
size_t BSTCollection<K,V>::height() const{
return height(root);
}

#endif
