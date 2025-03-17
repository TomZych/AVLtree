#include <iostream>
#include <sstream>
#include <queue>
#include <algorithm>


using namespace std;

struct Node{
    int data;
    Node* left;
    Node* right;
    int height;

    Node(int val) : data(val),left(nullptr),right(nullptr),height(1) {}
};
class AVLtree{

private:

    Node* root;

    int getHeight(Node* node){
        return node ? node->height : 0;
    }

    int getBalanceFactor(Node* node){
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    Node* rotateLeft(Node* A){ //Saying root is node "A"
        Node* B = A->right;
        Node* y = B->left;

        B->left = A;
        A->right = y;

        A->height = max(getHeight(A->left), getHeight(A->right)) + 1;
        B->height = max(getHeight(B->left), getHeight(B->right)) + 1;

        return B;
    }
    Node* rotateRight(Node* B){ //Saying root is node "B"
        Node* A = B->left;
        Node* x = A->right;

        A->right = B;
        B->left = x;

        B->height = max(getHeight(B->left), getHeight(B->right)) + 1;
        A->height = max(getHeight(A->left), getHeight(A->right)) + 1;

        return A;
    }


    Node* insert(Node* node, int value){
        if (node == NULL){
            return new Node(value);
        }
        if (value < node->data){
            node->left = insert(node->left, value);
        }
        else if (value > node->data){
            node->right = insert(node->right, value);
        }
        else{
            return node;
        }
        node->height = max(getHeight(node->left),getHeight(node->right))+1;



        int bf = getBalanceFactor(node);

        if (bf > 1 && value < node->left->data){ //Left,Left
            return rotateRight(node);
        }
        if (bf > 1 && value > node->left->data){ //Left,Right
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1 && value > node->right->data){ //Right,Right
            return rotateLeft(node);
        }
        if (bf < -1 && value < node->right->data){ //Right,Left
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    
        return node;
    
    }

    Node* minValNode(Node* node){
        Node* current = node;
        while (current->left){
            current = current->left;
        }
        return current;
    }

    Node* deleteNode(Node* root, int value){
        if (root == NULL) return root;
        else if (value < root->data){
            root->left = deleteNode(root->left, value);
        }
        else if (value > root->data){
            root->right = deleteNode(root->right, value);
        }
        else{
            if (root->left == NULL && root->right == NULL){ //No children
                delete root;
                root = NULL;
            }
            else if(root->left == NULL){ //One child on right
                Node* temp = root;
                root = root->right;
                delete temp;
            }
            else if (root->right == NULL){ //One child on left
                Node* temp = root;
                root = root->left;
                delete temp;
            }
            else { //2 children
                Node* temp = minValNode(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right,temp->data);
            }
        }
        if (root == NULL) return root;

        root->height = max(getHeight(root->left),getHeight(root->right)) + 1;

        int bf = getBalanceFactor(root);

        if (bf > 1 && getBalanceFactor(root->left) >= 0){ //left, left
            return rotateRight(root);
        }
        if (bf > 1 && getBalanceFactor(root->left) < 0){ //left right
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        if (bf < -1 && getBalanceFactor(root->right) <= 0){ //right right
            return rotateLeft(root);
        }
        if (bf < - 1 && getBalanceFactor(root->right) > 0){
            root->right =  rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

public:
    AVLtree() : root(nullptr) {}

    void insert(int value){
        root = insert(root, value);
    }
    void remove(int value){
        root = deleteNode(root,value);
    }

    string serialize() {
        if (root == NULL) return "X";

        stringstream ss;
        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();

            if (node) {
                ss << node->data << ",";
                q.push(node->left);
                q.push(node->right);
            } else {
                ss << "X,";
            }
        }

        string result = ss.str();
        result.pop_back();
        return result;
    }

};

int main() {
    AVLtree tree;
    
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    tree.insert(10);

    cout << tree.serialize() <<endl;

    tree.remove(6);
    cout<< tree.serialize() <<endl;
    return 0;

}