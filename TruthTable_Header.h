#include<iostream>
#include<string>
#include<iomanip>
using namespace std;
///------------------------------------------COLOR TEXT------------------------------------------

void redColor();
void greenColor();
void blackColor();
void resetColor();
void grayColor();

///------------------------------------------NODE Class------------------------------------------

struct Node
{
	string premise;		//hold the premise
	int* arr;			//array to hold result of inputs
	int size;			//size of array (total possible combinations)
	Node* next;
public:
	//copy constructor
	Node(Node* obj);
	Node(string data, Node* ptr);
	Node(int size = 0, string premise = "");
	void printNode();
};

///------------------------------------------STACK Class------------------------------------------

class Stack
{
public:
	Node* top;

	Stack();
	bool isEmpty();
	string topData();
	void push(string str);
	void push(Node* newNode);
	string pop();
	Node* popNode();
};

///------------------------------------------GLOBAL FUNCTIONS------------------------------------------

bool isOperator(char c);
int precedence(string c);
bool isEquivalent(const Node* exp1, const Node* exp2);
bool containsInvalidChars(string str);

///------------------------------------------TRUTH TABLE Class------------------------------------------

class TruthTable
{
	Node* head;	//this head points to linked list of arrays of variables (it points to the truth table each premise is inserted after the other)
	int totalVariables;
	int possibleCombinations;	//size of array
public:
	//constructor of truth table, it counts the variables of the expression and generate possible combination 
	TruthTable(string postfix);

	//counts the total number of variable in string to generate input lines
	int countTotalVariable(string str);

	//create input lines/possible combinations according to number of varaibles
	void createInputLines(string expression);

	//fills the array with zero and ones
	void fillArrayWith01(int*& arr, int possibleCombinations, int consecutiveZeroOnes);

	//generates truth table
	Node* generateTruthTable(string expression);

	//prints the truth table Linked List 
	void printTruthTable(Node* result);

	//inserts the premise and array in linked list
	void insertInTruthTable(Node* newArray);

	//this function return the Node with matching string/premise is found
	Node* findNodeWithPremise(string premise);

	//this function return the true if matching string/premise is found
	bool premiseAlreadyAvailable(string premise);

	//function to evaluate posftix expreesion and generate result of truth table 
	Node* evaluatePostfix(string postfix);

	//converts infix to postfix
	string infixToPostfix(string infix);

	///------------------------------------------LOGICAL OPERATIONS FUNCTIONS------------------------------------------

	//computes result of AND function
	Node* AND(Node* op1, Node* op2, string resultPremis);

	//computes result of OR function
	Node* OR(Node* op1, Node* op2, string resultPremis);

	//computes result of NOT function
	Node* NOT(Node* op1, string resultPremis);

	//computes result of IMPLICATION function
	Node* IMPLICATION(Node* op1, Node* op2, string resultPremis);

	//computes result of BIIMPLICATION function
	Node* BIIMPLICATION(Node* op1, Node* op2, string resultPremis);

	//computes result of XOR function
	Node* XOR(Node* op1, Node* op2, string resultPremis);
};
