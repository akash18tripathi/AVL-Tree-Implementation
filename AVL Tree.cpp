#include <iostream>
#include <cstring>

using namespace std;

template <class T>
bool cmprtr(T a,T b,int type){
    if(type==0) return (a < b); // Less than
    if(type==1) return (a > b); // Greater than
    if(type==2) return (a == b); // Equal to

    return false;
}



template <class T>
class AVL{
    public:

    class Node{
        public:
        
        T val;
        Node* left;
        Node* right;
        int freq;
        int height;
        int subTreeSize;
        int lNodes;
        int rNodes;

        Node(T v){
            this->val=v;
            left=NULL;
            right=NULL;
            this->freq=1;
            this->height=1;
            this->subTreeSize=0;
            this->lNodes=0;
            this->rNodes=0;
        }
    };

    Node* root;
    
    AVL(){
        this->root=NULL;
    }

    void inorder(Node* n){
        if(n==NULL){
            return;
        }
        inorder(n->left);
        cout << n->val << " ";
        inorder(n->right);
    }
    void preorder(Node* n){
        if(n==NULL){
            return;
        }
        cout << n-> val << " ";
        preorder(n->left);
        preorder(n->right);
    }
    void postorder(Node* n){
        if(n==NULL){
            return;
        }
        postorder(n->left);
        postorder(n->right);
        cout << n->val << " ";
    }
    int height(Node* n){
        if(n==NULL){
            return 0;
        }
        return n->height;
    }
    int stNodes(Node* n){
        if(n==NULL){
            return 0;
        }
        return n->lNodes+n->rNodes+n->freq;
    }
    int balanceFactor(Node* node){
        if(node==NULL){
            return 0;
        }
        return height(node->left)-height(node->right);
    }
    Node* R_Rotation(Node* n){
        if(n==NULL){
            return NULL;
        }
        Node* l = n->left;
        Node* tmp = l->right;
        l->right = n;
        n->left=tmp;
        n->lNodes= this->stNodes(n->left);
        l->rNodes = this->stNodes(l->right);

        n->height = 1+ max(height(n->left),height(n->right));
        l->height = 1+ max(height(l->left),height(l->right));
        
        return l;
    }

    Node* L_Rotation(Node* n){
        if(n==NULL){
            return NULL;
        }
        Node* r = n->right;
        Node* tmp = r->left;
        r->left = n;
        n->right=tmp;
        n->rNodes= this->stNodes(n->right);
        r->lNodes = this->stNodes(r->left);

        n->height = 1+ max(height(n->left),height(n->right));
        r->height = 1+ max(height(r->left),height(r->right));
        
        return r;
    }

    Node* findMin(Node* node){
        if(node==NULL){
            return NULL;
        }
        Node* tmp = node;
        while(tmp->left!=NULL){
            tmp=tmp->left;
        }
        return tmp;
    }

    Node* insertInTree(Node* node,T key){
        if(node==NULL){
            Node* tmp = new Node(key);
            return tmp;
        }
        // cmprtr(left,right)
        if(cmprtr(node->val,key,0)){
            node->right = insertInTree(node->right,key);
        }else if(cmprtr(key,node->val,0)){
            node->left = insertInTree(node->left,key);
        }else{
            node->freq++;
        }
        int lh = this->height(node->left);
        int rh = this->height(node->right);
        node->height = 1+max(lh,rh);
        node->lNodes= this->stNodes(node->left);
        node->rNodes = this->stNodes(node->right);

        int balancingFactor = balanceFactor(node);
        //left- left rotation
        if(balancingFactor>1 && (cmprtr(key,node->left->val,0)) ){
            return R_Rotation(node);
        }
        //left- right rotation
        if(balancingFactor>1 && (cmprtr(node->left->val,key,0)) ){
            node->left = L_Rotation(node->left);
            Node* lr = R_Rotation(node);
            return lr;
        }
        //right- left rotation
        if(balancingFactor<-1 && (cmprtr(key,node->right->val,0)) ){
            node->right = R_Rotation(node->right);
            Node* rl = L_Rotation(node);
            return rl;
        }
        //right- right rotation
        if(balancingFactor<-1 && (cmprtr(node->right->val,key,0)) ){
            return L_Rotation(node);
        }
        //If no rotation
        return node;
    }

    void insert(T k){
        //cout << k <<endl;
        this->root  = this->insertInTree(this->root,k);
    }

    Node* deleteInTree(Node* node,T key){
        if(node==NULL){
            return NULL;
        }

        Node* child;
        if(cmprtr(node->val,key,0)){
            node->right = deleteInTree(node->right,key);
        }else if(cmprtr(key,node->val,0)){
            node->left = deleteInTree(node->left,key);
        }else{
            //key found
                if(node->left==NULL || node->right==NULL){
                    child = node->left!=NULL?node->left:node->right;
                    //No Child
                    if(child==NULL){
                        node=NULL;
                    }else{
                        //One Child
                        node = child;
                    }
                }else{
                    //Has 2 children
                    Node* toDel = findMin(node->right);
                    node->val = toDel->val;
                    node->right = deleteInTree(node->right,toDel->val);
                }

            //}
        }
        if(node==NULL){
            return NULL;
        }

        node->height = 1+ max(height(node->left),height(node->right));
        node->rNodes = this->stNodes(node->right);
        node->lNodes = this->stNodes(node->left);

        int balancingFactor = balanceFactor(node);
        //Left
        if(balancingFactor>1){
            int bf2 = balanceFactor(node->left);
            if(bf2>=0){
                //Left
                return R_Rotation(node);
            }else{
                //Right
                node->left = L_Rotation(node->left);
                return R_Rotation(node);
            }
        }else if(balancingFactor<-1){
            //Right
            int bf2 = balanceFactor(node->right);
            if(bf2>=0){
                //Left
                node->right = R_Rotation(node->left);
                return L_Rotation(node);
            }else{
                //Right
                return L_Rotation(node);
            }
        }

        return node;
    }

    void deleteNode(T k){
        this->root = this->deleteInTree(this->root,k);
    }

    bool searchInTree(Node* node,T k){
        if(node==NULL){
            return false;
        }
        if(cmprtr(node->val,k,0)){
            return searchInTree(node->right,k);
        }else if(cmprtr(k,node->val,0)){
            return searchInTree(node->left,k);
        }else{
            return true;
        }
        return false;
    }
    bool search(T k){
        return searchInTree(this->root,k);
    }
    int count(Node* node,T k){
        if(node==NULL){
            return 0;
        }
        if(cmprtr(node->val,k,0)){
            return count(node->right,k);
        }else if(cmprtr(k,node->val,0)){
            return count(node->left,k);
        }else{
            return node->freq;
        }
    }
    int count_occurence(T k){
        return count(this->root,k);
    }

    T lowerBound(Node* node,T k,T lb){
        if(node==NULL){
            return lb;
        }
        if(!(cmprtr(node->val,k,0))){
            return lowerBound(node->left,k,node->val);
        }else{
            return lowerBound(node->right,k,lb);
        }
    }
    T lower_bound(T k){
        return lowerBound(this->root,k,T());
    }
    T upperBound(Node* node,T k,T lb){
        if(node==NULL){
            return lb;
        }
        if(cmprtr(k,node->val,0)){
            return upperBound(node->left,k,node->val);
        }else{
            return upperBound(node->right,k,lb);
        }
    }
    T upper_bound(T k){
        return upperBound(this->root,k,T());
    }

    Node* lowerBoundNode(Node* node,T k,Node* lb){
        if(node==NULL){
            return lb;
        }
        if(!(cmprtr(node->val,k,0))){
            return lowerBoundNode(node->left,k,node);
        }else{
            return lowerBoundNode(node->right,k,lb);
        }
    }
    Node* reverseLowerBound(Node* node,T k,Node* lb){
        if(node==NULL){
            return lb;
        }
        if(cmprtr(k,node->val,0)){
            return reverseLowerBound(node->left,k,lb);
        }else{
            return reverseLowerBound(node->right,k,node);
        }
    }
    T closest_element(T k){
        T lb = this->lower_bound(k);
        Node* n = this->reverseLowerBound(this->root,k,NULL);
        if(n==NULL){
            return lb;
        }
        if(abs(lb-k)<=abs(n->val-k)){
            return lb;
        }else{
            return n->val;
        }
    }
    T kthLargest(Node* n,int k){
        if(n==NULL || k<=0){
            return T();
        }
        //Found
        if( k> n->rNodes && k<= n->rNodes+n->freq){
            return n->val;
        }
        //Go right
        if(k <= n->rNodes){
            return kthLargest(n->right,k);
        }
        return kthLargest(n->left,k-n->rNodes-n->freq);
    }
    T Kth_largest(int k){
        return kthLargest(this->root,k);
    }
    
    int findNumOfElementsLTX(Node* n,T x){
        if(n==NULL){
            return 0;
        }
        if(cmprtr(x,n->val,0)){
            return findNumOfElementsLTX(n->left,x);
        }else{
            return (n->lNodes+n->freq)+findNumOfElementsLTX(n->right,x); 
        }
    }
    
    int count_range(T l,T r){
        if(cmprtr(r,l,0)){
            return 0;
        }
        int rRange = this->findNumOfElementsLTX(root,r);
        Node* n = this->lowerBoundNode(this->root,l,NULL);
        int lRange;
        if(n!=NULL){
            lRange = this->findNumOfElementsLTX(root,n->val);
        }else{
            lRange = this->findNumOfElementsLTX(root,l);
        }        
        int ans = rRange-lRange;
         if(n!=NULL){
            ans+= n->freq;
         }
        return ans;

    }

};
