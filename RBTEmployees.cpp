// RBTemployees.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Used template from Algorithm Tutor
// Tutorial URL: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
// adapeted for purposes of this assignment
#include <iostream>
#include <chrono>
#include <random>
#include <string>
using namespace std;
using namespace std::chrono;
// data structure that represents a node in the tree
struct Node {
	int data; // holds the key
	Node* parent; // pointer to the parent
	Node* left; // pointer to left child
	Node* right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	string firstName;
	string lastName;
	string jobTitle;
};

typedef Node* NodePtr;

// class RBTree implements the operations in Red Black Tree
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(NodePtr node, NodePtr parent) {
		node->data = 1;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
		node->firstName = "";
		node->lastName = "";
		node->jobTitle = "";
	}

	void preOrderHelper(NodePtr node) {
		if (node != TNULL) {
			cout << node->data << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout << node->data << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->data << " ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, int key) {
		
		if (node == TNULL){
			return nullptr;
		}
		
		if (key == node->data) {
			return node;
		}
		
		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	

	// fix the rb tree modified by the delete operation
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v) {
		if (u->parent == nullptr) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL) {
			if (node->data == key) {
				z = node;
			}

			if (node->data <= key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		}
		else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0) {
			fixDelete(x);
		}
	}

	// fix the red-black tree
	void fixInsert(NodePtr k) {
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(NodePtr root, string indent, bool last) {
		// print the tree structure on the screen
		if (root != TNULL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			}
			else {
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->color ? "RED" : "BLACK";
			cout << root->data << "(" << sColor << ")" << endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// search the tree for the key k
	// and return the corresponding node
	NodePtr searchTree(int k) {
		return searchTreeHelper(this->root, k);
	}

	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	NodePtr predecessor(NodePtr x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(string firstName, string lastName, string jobTitle, int salary) {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->data = salary;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red
		node->firstName = firstName;
		node->lastName = lastName;
		node->jobTitle = jobTitle;


		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->data < y->data) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr) {
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	NodePtr getRoot() {
		return this->root;
		cout << this->root << endl;
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		if (root) {
			printHelper(this->root, "", true);
		}
	}

	//used to print all employees
	void printInorder(Node* r) // (left, current node, right)
	{
		if (r == TNULL)
			return;
		//first recur on left child
		printInorder(r->left);
		//then print the data of node
		cout << r->firstName << " " << r->lastName << " " << r->jobTitle << " " << r->data << " " << endl;
		//now recur on right child
		printInorder(r->right);
	}
	
	// Function to print employees within a salary range
	void printEmployeesInSalaryRange(Node* root, int low, int high) {
		if (root == nullptr)
			return;

		if (root->data < low) {
			printEmployeesInSalaryRange(root->right, low, high);
		}
		else if (root->data > high) {
			printEmployeesInSalaryRange(root->left, low, high);
		}
		else {
			// The current node falls within the range
			cout << root->firstName << " " << root->lastName << " " << root->jobTitle << " " << root->data << endl;

			// Check the left and right subtrees
			printEmployeesInSalaryRange(root->left, low, high);
			printEmployeesInSalaryRange(root->right, low, high);
		}
	}
	void searchEmployeesInSalaryRange(int low, int high)
	{
		printEmployeesInSalaryRange(this->root, low, high);
	}

	void deleteNEmployees(int numToDelete) {
		while (numToDelete > 0) {
			// Get the minimum salary employee and delete it
			NodePtr minNode = minimum(root);
			if (minNode != TNULL) {
				deleteNode(minNode->data);
				--numToDelete;
			}
			else {
				break; // If there are no more employees, break out of the loop
			}
		}
	}

};

int main()
{
	RBTree obj;

	Node* new_node = new Node();
	Node* employeeNode = nullptr;

	
	string firstName, lastName, jobTitle;
	int salary{};
	
	//generate random salaries between 30000 and 200000
	int lowSalGen = 30000;
	int highSalGen = 200000;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> salaryDist(lowSalGen, highSalGen);
	//make 10,000 employees
	cout << "Got here 1\n";
	for (int i = 0; i < 10000; i++){
		Node* new_node = new Node();
		new_node->firstName = "Employee";
		new_node->lastName = to_string(i + 1);
		new_node->jobTitle = "teacher";
		new_node->data = salaryDist(gen);
		obj.insert(new_node->firstName, new_node->lastName, new_node->jobTitle, new_node->data);
		//cout << new_node->firstName << " " << new_node->lastName << " " << new_node->jobTitle << " " << new_node->data << endl;
	}
	//print the default employees
	obj.printInorder(obj.getRoot());
	cout << "Here are 10,000 default employees" << endl;

    int option;
    
	do
	{ 
		//user menu
		cout << "What operation do you want to perform?"
			<< " Select Option number. Enter 0 to exit." << endl;
		cout << "1. Insert Employee's Info" << endl;
		cout << "2. Search Employee" << endl;
		cout << "3. Delete Employee" << endl;
		cout << "4. Print all employees" << endl;
		cout << "5. Search for employees in a salary range" << endl;
		cout << "0. Exit Program" << endl;

		//take user's selection
		cin >> option;
		switch (option)
		{
		case 0:
			break; //quit the program, go outside do...while loop
		case 1:
			cout << "INSERT EMPLOYEE INFO" << endl;
			new_node = new Node(); //create a new node

			//insert first name
			cout << "Enter employee's first name" << endl;
			cin.ignore();
			getline(cin, new_node->firstName);

			//insert last name
			cout << "Enter employee's last name" << endl;
			getline(cin, new_node->lastName);
		
			//insert job title
			cout << "Enter employee's job title" << endl;
			getline(cin, new_node->jobTitle);
			
			//insert salary
			cout << "Enter salary" << endl; //salary is node's key
			cin >> new_node->data;
			
			//stuff everything into the node
			obj.insert(new_node->firstName, new_node->lastName, new_node->jobTitle, new_node->data);
			cout << endl;
			break;
		case 2:
			cout << "SEARCH" << endl;
			cout << "Enter Employee's salary to search: ";
			cin >> salary;
			employeeNode = obj.searchTree(salary);

			if (employeeNode != nullptr)
			{
				cout << "Value found" << endl;
				//cout << employeeNode << "\n";
				cout << employeeNode->firstName << " " << employeeNode->lastName << " " << employeeNode->jobTitle << " " << employeeNode->data << endl;
			}
			else
			{
				cout << "Value NOT found" << endl;
			}
			break;
		case 3:
			cout << "DELETE" << endl;
			cout << "Enter Salary of Employee to DELETE in BST: ";
			cin >> salary;
			new_node = obj.searchTree(salary);
			if (new_node != NULL)
			{
				obj.deleteNode(salary);
				cout << "Value Deleted" << endl;
			}
			else
			{
				cout << "Value NOT found" << endl;
			}
			break;
		case 4:
			cout << "PRINT" << endl;
			obj.printInorder(obj.getRoot());
			break;

		case 5:
			cout << "Enter the range of salaries to print" << endl;
			cout << "low salary: " << endl;
			int lowSalary;
			cin >> lowSalary;
			cout << "high salary: " << endl;
			int highSalary;
			cin >> highSalary;
			obj.searchEmployeesInSalaryRange(lowSalary, highSalary);
			break;

		default:
			cout << "Enter Proper Option number" << endl;
		}
	}
	while (option != 0);

	//cout << "Testing commit one more time" << endl;
}

