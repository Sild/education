#include <iostream>

struct Node {
	int val;
	struct Node* left;
	struct Node* right;
};

struct Node* add_node(int data)
{
	Node* node = new Node();
	node->val = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void Morris(Node* root) {
    auto cur = root;
    while (cur != nullptr) {
        if (!cur->left) {
            std::cout << cur->val << ", ";
            cur = cur->right;
            continue;
        }

        auto prev = cur->left;
        while (prev->right != nullptr && prev->right != cur) {
            prev = prev->right;
        }
        if (prev->right == nullptr) {
            prev->right = cur;
            cur = cur->left;
        } else {
            std::cout << cur->val << ", ";
            prev->right = nullptr;
            cur = cur->right;
        }
    }
    std::cout << std::endl;
}

int main()
{

	Node* root = add_node(4);
	root->left = add_node(2);
	root->right = add_node(5);
	root->left->left = add_node(1);
	root->left->right = add_node(3);
	Morris(root);
    delete root->left->right;
    delete root->left->left;
    delete root->left;
    delete root->right;
    delete root;
	return 0;
}
