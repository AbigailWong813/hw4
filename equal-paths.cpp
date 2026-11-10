#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    if (root == NULL) {
        return true;
    }

	//Get the heights of the left and right subtrees - helper func?
	//Determine if the left and right subtree have same height! If not ret false!
	int leftH = height(root->left);
    int rightH = height(root->right);

	//Check if there are subtrees under us
	//Are they the same height
	if (leftH != rightH) {
        if (leftH != 0 && rightH != 0){
            return false;
        }else if (leftH == 0){
            return equalPaths(root->right);
        }else{
            return equalPaths(root->left);
        }
    }

	//If all lead nodes are same height return true!
	return equalPaths(root->left) && equalPaths(root->right);

}

//helpper function
int height(Node *root) {
    if (root == NULL) return 0;
    
    int leftH = height(root->left);
    int rightH = height(root->right);
    
    return 1 + max(leftH, rightH);
}

