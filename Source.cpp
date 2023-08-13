#include <iostream>
using namespace std;
#include <vector>
#include <fstream>
#include <queue>
#include <string>

struct node
{
	char symbol;
	int count;
	node* left;
	node* right;
	node()
	{
		symbol = count = NULL;
		left = right = NULL;
	}
	node(char val, int c) : symbol(val), count(c), left(NULL), right(NULL) {}
};
class minHeap
{
public:
	minHeap() // default constructor
	{
		node* n = new node(NULL, NULL);
		v.push_back(n);
	}
	minHeap(int* freq, vector<char> chars, int N, int build) // parameterized constructor that will take an array of random numbers and its size in parameters and initialize the heap with random values.It will call the buildMinHeap() function to convert the random values into a heap.
	{
		node* n = new node(NULL, NULL);
		v.push_back(n);
		for (int i = 0; i < N; i++)
		{
			node* n = new node(chars[i], freq[i]);
			v.push_back(n);
		}
		buildMinHeap();
	}
	void buildMinHeap() // It will generate heap from random values stored in the object
	{
		int N = v.size();
		node** tempArr = new node * [N];
		for (int i = 0; i < N; i++)
		{
			tempArr[i] = v[i];
		}
		v.clear();
		for (int i = 0; i < N; i++)
		{
			insert(tempArr[i]);
		}
	}
	void insert(node* n) // Inserts the key value in the heap array such that, the resultant heap tree is a complete binary treeand it follows min heap order
	{
		v.push_back(n);
		bubble_up(v.size() - 1);
	}
	bool isEmpty() const // returns true if it is empty
	{
		if (v.size() == 1)
			return true;
		return false;
	}
	bool oneNodeLeft() const
	{
		if (v.size() == 2)
			return true;
		return false;
	}
	node* getMin() const //returns minimum value this operation should be performed in O(1)
	{
		return v.at(1);
	}
	void deleteMin() //always from root node
	{
		//swap root node with last leaf node
		node* temp = v.at(1);
		v.at(1) = v.at(v.size() - 1);
		v.at(v.size() - 1) = temp;

		//delete the leaf node
		v.pop_back();

		//bubble down
		bubble_down(1);
	}
	void encode(node* root, char key, fstream& outfile, string code)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->symbol == key)
		{
			outfile << code;
		}
		encode(root->left, key, outfile, code + "0");
		encode(root->right, key, outfile, code + "1");
	}
	void decode(node* root, ifstream& infile, char c, ofstream& outfile)
	{
		node* parent = returnRoot();
		while ((c = infile.get()) != EOF)
		{
			//cout << c;
			if (c == '0')
			{
				parent = parent->left;
			}
			else if (c == '1')
			{
				parent = parent->right;
			}
			if (parent->right == NULL && parent->left == NULL)
			{
				outfile << parent->symbol;
				//cout << parent->symbol;
				parent = returnRoot();
			}
		}
	}
	node* returnRoot()
	{
		return v.at(1);
	}
	void printCodes()
	{
		printCodesR(v.at(1), "");
	}
	void printCodesR(node* root, string code)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->symbol != '*')
		{
			cout << root->symbol << ": ";
			cout << code << endl;
		}
		printCodesR(root->left, code + "0");
		printCodesR(root->right, code + "1");
	}
	void display()
	{
		for (int i = 1; i < v.size(); i++)
		{
			cout << v.at(i)->symbol << ": " << v.at(i)->count << endl;
		}
		cout << endl;
	}
private:
	vector<node*> v;
	void bubble_up(int i) // A recursive method to heapify a subtree with the root at given index.It maintains heap property during insertion
	{
		if (i == 0)
			return;
		else
		{
			if (v.at(i)->count < v.at(i / 2)->count)
			{
				node* temp = v.at(i);
				v.at(i) = v.at(i / 2);
				v.at(i / 2) = temp;
				bubble_up(i / 2);
				// send recursive call only if it swaps 
			}
		}
	}
	void bubble_down(int i) // It maintains heap property during deletion
	{
		if (i * 2 > v.size() || i * 2 + 1 > v.size()) // if they have reached the leaf node
			return;
		else
		{
			if (2 * i < v.size() && 2 * i + 1 < v.size()) // if both children exist
			{
				if (v.at(2 * i)->count < v.at(2 * i + 1)->count) // it will CHECK right and left child based on which one is smaller
				{
					if (v.at(2 * i)->count < v.at(i)->count)  // THEN it compares that (smaller) child with than the current node
					{
						node* temp = v.at(i);		// if yes then it swaps
						v.at(i) = v.at(2 * i);
						v.at(2 * i) = temp;
						bubble_down(2 * i);
					}
				}
				else  // otherwise if the other child was smaller then it would have swapped with that if needed
				{
					if (v.at(2 * i + 1)->count < v.at(i)->count)
					{
						node* temp = v.at(i);
						v.at(i) = v.at(2 * i + 1);
						v.at(2 * i + 1) = temp;
						bubble_down(2 * i + 1);
					}
				}
			}
			else
			{
				if (v.at(2 * i)->count < v.at(i)->count) // since heap is always a complete tree, if both children dont exist, the only
				{							//other possibility is that it has a left child, so we compare with that
					node* temp = v.at(i);
					v.at(i) = v.at(2 * i);
					v.at(2 * i) = temp;
					bubble_down(2 * i);
				}
			}
		}
	}
};
int main()
{
	ifstream file;
	string name;
	cout << "Enter file to encode: ";
	cin >> name;
	name += ".txt";
	file.open(name, ios::in);

	if (!file)
	{
		cout << "file didnt open1\n";
		system("pause");
		return 0;
	}
	//calculating frequency of each character in the file
	char c = NULL;
	vector<char> uniqueChars;
	while ((c = file.get()) != EOF)//first storing all the unique characters in vector
	{
		int flag = 0;
		for (int i = 0; i < uniqueChars.size(); i++)
		{
			if (uniqueChars.at(i) == c)
			{
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			uniqueChars.push_back(c);
		}
	}
	int N = uniqueChars.size();
	int* freq = new int[N] {0};
	for (int i = 0; i < N; i++)//then calculating their frequency
	{
		file.clear();	//EOF flag shud be cleared evrytime to read again
		file.seekg(0, ios::beg);//point to beginning of file 
		while ((c = file.get()) != EOF)
		{
			if (c == uniqueChars.at(i))
			{
				freq[i] = freq[i] + 1;
			}
		}
	}
	//creating a leaf node for each character and adding it to the priority queue (min heap)
	minHeap priorityQ(freq, uniqueChars, N, 1);
	//priorityQ.display();

	node* child1;	node* child2;	node* parent = NULL;
	//generating codes
	while (!priorityQ.oneNodeLeft())//traverse till theres only one node left in the heap
	{
		//remove two nodes from queue one by one
		child1 = priorityQ.getMin();
		priorityQ.deleteMin();
		child2 = priorityQ.getMin();
		priorityQ.deleteMin();

		//create new node with these two nodes as children and its freq as that of their sum
		parent = new node('*', child1->count + child2->count);
		parent->left = child1;
		parent->right = child2;

		priorityQ.insert(parent); //insert it into the min heap
	}
	//codes have been generated
	//printing them
	priorityQ.printCodesR(parent, "");

	//encoding file
	fstream encfile;
	name = "encodedFile.txt";
	encfile.open(name, ios::out);
	if (!encfile)
	{
		cout << "file didnt open2\n";
		system("pause");
		return 0;
	}
	file.clear();
	file.seekg(0, ios::beg);
	cout << "\nEncoded Result on " << name;
	while ((c = file.get()) != EOF)
	{
		priorityQ.encode(parent, c, encfile, "");
	}
	cout << "\nDecoded Result on " << name;
	encfile.close();

	//decoding file
	//using the encoded file as input for decoding
	ofstream decfile;
	ifstream infile;
	infile.open(name);
	decfile.open("decodedFile.txt", ios::out);
	if (!infile || !decfile)
	{
		cout << "file didnt open3\n";
		system("pause");
		return 0;
	}
	priorityQ.decode(parent, infile, c, decfile);

	//closing files :<
	decfile.close();
	file.close();
	cout << "\n\n\n";
	system("pause");
	return 0;
}
