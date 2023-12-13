#include"TruthTable_Header.h"

///------------------------------------------COLOR TEXT FUNCTIONS------------------------------------------

void redColor()
{
	printf("\033[1;31m");
}

void greenColor()
{
	printf("\033[1;32m");
}

void blackColor()
{
	printf("\033[1;30m");
}

void resetColor()
{
	printf("\033[0m");
}

void grayColor()
{
	printf("\033[1;30m");
}

///------------------------------------------NODE Class------------------------------------------

Node::Node(Node* obj)
{
	this->premise = obj->premise;
	this->arr = obj->arr;
	this->size = obj->size;
	this->next = nullptr;
}

Node::Node(string data, Node* ptr)
{
	this->premise = data;
	next = ptr;
}

Node::Node(int size, string premise)
{
	this->premise = premise;
	arr = new int[size];
	this->size = size;
	next = nullptr;
}

int& Node::operator[](int index)
{
	if (index >= 0 && index < size)
		return arr[index];
}

void Node::printNode()
{
	cout << "\n----------------------\n";
	cout << "Premis: " << premise << endl;

	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}

	cout << "\n----------------------\n";
}

///------------------------------------------STACK Class------------------------------------------

Stack::Stack() : top(nullptr) {}

bool Stack::isEmpty()
{
	return (top == nullptr) ? true : false;
}

string Stack::pop()
{
	if (isEmpty())
		return "";
	else
	{
		string str;

		Node* temp = top;
		str = top->premise;
		top = top->next;
		delete temp;

		return str;
	}
}

string Stack::topData()
{
	if (!isEmpty())
		return top->premise;
}

void Stack::push(string str)
{
	Node* newNode = new Node(str, top);
	top = newNode;
}

void Stack::push(Node* newNode)
{
	newNode->next = top;
	top = newNode;
}

Node* Stack::popNode()
{
	if (isEmpty())
		return nullptr;
	else
	{
		Node* temp = top;
		top = top->next;

		Node* poppedNode(temp);
		poppedNode->next = nullptr;
		temp = nullptr;
		return poppedNode;
	}
}


///------------------------------------------GLOBAL FUNCTIONS------------------------------------------

bool isOperator(char c)
{
	/*
	&	AND
	|	OR
	!	NOT
	-	IMPLICATION
	=	BIIMPLICATION
	+	XOR
	*/
	if (c == '&' || c == '|' || c == '!' || c == '-' || c == '=' || c == '+')
		return true;
	else
		return false;
}

int precedence(string c)
{
	/*! NOT
	& AND
	| OR
	+ XOR
	- IMPLICATION
	= BIIMPLICATION
	*/
	if (c == "!")
		return 3;
	else if (c == "&" || c == "|" || c == "+")
		return 2;
	else if (c == "-" || c == "=")
		return 1;
	else
		return -1;
}

bool isEquivalent(const Node* exp1, const Node* exp2)
{
	bool equal = false;

	//if size is not equal
	if (exp1->size != exp2->size)
		return false;

	for (int i = 0; i < exp1->size; i++)
		if (exp1->arr[i] != exp2->arr[i]) //if any value is different than it return false
			return false;

	return  true;
}

bool containsInvalidChars(string str)
{
	bool containAtleastOneVariable = false;

	for (int i = 0; i < str.length(); i++)
	{
		//if contain wrong char
		if (!(isalpha(str[i]) || str[i] == '&' || str[i] == '|' || str[i] == '!' || str[i] == '-' || str[i] == '=' || str[i] == '+' || str[i] == '(' || str[i] == ')'))
			return true;

		if (isalpha(str[i]))
			containAtleastOneVariable = true;

		//if there are two consecutive chars
		if (isalpha(str[i]) && isalpha(str[i + 1]))
			return true;
	}
	//the expression invalid
	if (containAtleastOneVariable == false)
		return true;

	//else is don't contain invalid char
	return false;
}

///------------------------------------------TRUTH TABLE Class------------------------------------------

//constructor of truth table, it counts the variables of the expression and generate possible combination 
TruthTable::TruthTable(string postfix)
{
	head = nullptr;
	totalVariables = countTotalVariable(postfix);
	this->possibleCombinations = pow(2, totalVariables);
	createInputLines(postfix);
}

//counts the total number of variable in string to generate input lines
int TruthTable::countTotalVariable(string str)
{
	/*the function iterates throught every index of  string and creates a new string of letters with no
	repitition and the length of that string are total variables in expression*/
	string resultString;
	//removing repeative letters/variables from string
	for (int i = 0; i < str.length(); ++i)
	{
		//checks if the variable is alpha and is not already present in ResultString, than adds it to result String,
		//find() return -1 if the letter is not found
		if (isalpha(str[i]) && resultString.find(str[i]) == -1)
			resultString += str[i];
	}
	//the length of resultString is the total number of variables
	return resultString.length();
}

//create input lines/possible combinations according to number of varaibles
void TruthTable::createInputLines(string expression)
{
	string resultString;
	//removing repeative letters/variables from string
	for (int i = 0; i < expression.length(); ++i)
	{
		//find() return -1 is the letter is not found
		if (isalpha(expression[i]) && resultString.find(expression[i]) == -1)
		{
			resultString += expression[i];
		}
	}
	expression = resultString;

	for (int i = 1; i <= totalVariables; i++)
	{
		int consecutiveZeroOnes = pow(2, i - 1);

		Node* newArray = new Node(possibleCombinations);
		fillArrayWith01(newArray->arr, possibleCombinations, consecutiveZeroOnes);


		//filling name of premise of that term/array/combination 
		for (int j = expression.length(); j >= 0; j--)
		{

			if ((expression[j] >= 'a' && expression[j] <= 'z') || (expression[j] >= 'A' && expression[j] <= 'Z'))
			{
				if (!premiseAlreadyAvailable(string(1, expression[j])))
				{
					newArray->premise = expression[j];
					j--;
					break;;
				}

			}
		}

		//adds to the linked list of truth table
		if (!head)
		{
			head = newArray;
		}
		else
		{
			newArray->next = head;
			head = newArray;
		}
	}
}

//fills the array with zero and ones
void TruthTable::fillArrayWith01(int*& arr, int possibleCombinations, int consecutiveZeroOnes)
{
	int countZero = 0;	//counts how many consecuitive 0's are places until next is to be 1
	int countOnes = 0;	//counts how many consecuitive 1's are places until next is to be 0
	for (int j = 0; j < possibleCombinations; j++)
	{
		//first places zero's
		if (countZero < consecutiveZeroOnes)
		{
			arr[j] = 0;
			countZero++;
		}
		//when zero's are placed than places 1 
		else if (countOnes < consecutiveZeroOnes)
		{
			arr[j] = 1;
			countOnes++;
		}

		if (countOnes == consecutiveZeroOnes && countZero == consecutiveZeroOnes)
			countOnes = countZero = 0;
	}
}

//generates truth table
Node* TruthTable::generateTruthTable(string expression)
{
	//converts the expression to postfix, than evalute that postfix and print the whole truth table
	string postfix = infixToPostfix(expression);
	Node* finalResult = evaluatePostfix(postfix);
	printTruthTable(finalResult);

	//return the finalResult array
	return finalResult;
}

//prints the truth table Linked List 
void TruthTable::printTruthTable(Node* result)
{
	for (int len = 0; len < result->premise.length(); len++)
	{

		int i = 0;
		Node* curr = head;
		while (curr)
		{
			//deciding the num value for setw(num) according to length of premise
			int length = curr->premise.length();
			length = length + (curr->premise.length() / 2) - 1;

			//deciding the color of text
			(i % 2 == 0) ? redColor() : resetColor();
			i++;

			//first print all array except the final result
			if (curr->premise != result->premise)
				if (curr->premise.length() == len)
					cout << setw(length) << left << curr->premise << " ";

			curr = curr->next;

		}
	}
	int resultLength = result->premise.length();
	int resultLengthCopy = resultLength;
	resultLength = resultLength + (result->premise.length() / 2) - 1;

	//prints result premise 
	greenColor();
	cout << setw(resultLength) << left << result->premise << " ";
	resetColor();
	cout << endl << endl;


	for (int i = 0; i < possibleCombinations; i++)
	{
		//print the truth table according to the length of premise, (A+B) is printed before (A+B)|C
		for (int len = 0; len < resultLengthCopy; len++)
		{
			int j = 0;
			Node* curr = head;
			while (curr)
			{
				//deciding the num value for setw(num) according to length of premise
				int length = curr->premise.length();
				length = length + (curr->premise.length() / 2) - 1;

				//deciding the color of text 
				(j % 2 == 0) ? redColor() : resetColor();
				j++;

				//prints every premise array except the result array
				if (curr->premise != result->premise)
				{
					if (curr->premise.length() == len)
					{
						cout << setw(length);
						cout << curr->operator[](i) << " ";
					}
				}
				curr = curr->next;
			}
		}

		//print result array with different color
		greenColor();
		cout << setw(resultLength) << result->operator[](i) << " ";
		resetColor();


		cout << endl;

	}
	resetColor();
}

//inserts the premise and array in linked list
void TruthTable::insertInTruthTable(Node* newArray)
{
	if (!newArray)
		return;

	Node* curr = head;

	while (curr->next)
	{
		//if same premise has been calculated again than don't add it again
		if (curr->premise == newArray->premise)
			return;

		curr = curr->next;
	}
	//if same premise has been calculated again than don't add it again
	if (curr->premise == newArray->premise)
		return;

	curr->next = newArray;
	newArray->next = nullptr;
	redColor();
	resetColor();
}

//this function return the Node with matching string/premise is found
Node* TruthTable::findNodeWithPremise(string premise)
{
	Node* curr = head;
	while (curr)
	{
		if (curr->premise == premise)
			return curr;
		curr = curr->next;
	}

}

//this function return the true if matching string/premise is found
bool TruthTable::premiseAlreadyAvailable(string premise)
{
	Node* curr = head;
	while (curr)
	{
		if (curr->premise == premise)
			return true;
		curr = curr->next;
	}
	return false;

}

//function to evaluate posftix expreesion and generate result of truth table 
Node* TruthTable::evaluatePostfix(string postfix)
{
	string resultPremis;
	int length = postfix.length();

	Node* curr = head;
	Node* operand1 = nullptr;
	Node* operand2 = nullptr;
	Stack stack;
	Node* newResultArray{};

	for (int i = 0; i < length; i++)
	{
		if ((postfix[i] >= 'a' && postfix[i] <= 'z') || (postfix[i] >= 'A' && postfix[i] <= 'Z'))
		{
			//return node with the matching string and push that node to stack, now the stack hold that node
			newResultArray = new Node(findNodeWithPremise(string(1, postfix[i])));	//calling copy constructor
			stack.push(newResultArray);
		}
		else
		{
			operand2 = stack.popNode();

			//if not operator is encounter than pop only one operand
			if (postfix[i] != '!')
				operand1 = stack.popNode();


			switch (postfix[i])
			{
				//AND
			case '&':
			{
				//calculates premise name
				resultPremis = '(' + operand1->premise + '&' + operand2->premise + ')';

				//calculate the result and it is stored in stack 
				newResultArray = AND(operand1, operand2, resultPremis);
				stack.push(newResultArray);
				break;
			}
			//OR
			case '|':
			{
				//calculates premise name
				resultPremis = '(' + operand1->premise + '|' + operand2->premise + ')';

				//calculate the result and it is stored in stack 
				newResultArray = OR(operand1, operand2, resultPremis);
				stack.push(newResultArray);
				break;
			}

			//NOT
			case '!':
			{
				//calculates premise name
				//operand 2 is used beacuse it is poped first
				resultPremis = "(!" + operand2->premise + ')';

				//calculate the result and it is stored in stack 
				newResultArray = NOT(operand2, resultPremis);
				stack.push(newResultArray);
				break;
			}
			//IMPLICATION
			case '-':
			{
				//calculates premise name
				resultPremis = '(' + operand1->premise + '-' + operand2->premise + ')';

				//calculate the result and it is stored in stack 
				newResultArray = IMPLICATION(operand1, operand2, resultPremis);
				stack.push(newResultArray);
				break;
			}

			//BIIMPLICATION
			case '=':
			{
				//calculates premise name
				resultPremis = '(' + operand1->premise + '=' + operand2->premise + ')';

				//calculate the result and it is stored in stack 
				newResultArray = BIIMPLICATION(operand1, operand2, resultPremis);
				stack.push(newResultArray);
				break;
			}
			//XOR
			case '+':
			{
				//calculates premise name
				resultPremis = '(' + operand1->premise + '+' + operand2->premise + ')';

				//calculate the result and it is stored in stack 
				newResultArray = XOR(operand1, operand2, resultPremis);
				stack.push(newResultArray);
				break;
			}
			}
		}
	}

	Node* finalResult = new Node(newResultArray);
	//returns the specific node with final result
	return finalResult;
}

//converts infix to postfix
string TruthTable::infixToPostfix(string infix)
{
	int length = infix.length();
	string postfix;
	Stack stack;

	for (int i = 0; i < length; i++)
	{
		//if char is found add it to posfix
		if ((infix[i] >= 'a' && infix[i] <= 'z') || (infix[i] >= 'A' && infix[i] <= 'Z'))
			postfix += infix[i];

		else if (infix[i] == '(')
			stack.push(string(1, infix[i]));

		else if (infix[i] == ')')
		{
			while (stack.topData() != "(")
				postfix += stack.pop();

			stack.pop(); //pop left bracket '('
		}

		else if (isOperator(infix[i]))
		{
			//string(1,infix[i]) converts char to string
			while (!stack.isEmpty() && precedence(string(1, infix[i])) <= precedence(stack.topData()))
				postfix += stack.pop();

			stack.push(string(1, infix[i]));
		}
	}

	while (!stack.isEmpty())
		postfix += stack.pop();

	return postfix;
}

///------------------------------------------LOGICAL OPERATIONS FUNCTIONS------------------------------------------

//computes result of AND function
Node* TruthTable::AND(Node* op1, Node* op2, string resultPremis)
{
	//declares a node, the array in node is of size of possible combinations
	Node* newResultArray = new Node(this->possibleCombinations, resultPremis);

	//array's of op1 and op2 are performed AND operation logic and the result is stored in newResultArray
	for (int i = 0; i < possibleCombinations; i++)
	{
		if (op1->operator[](i) == 1 && op2->operator[](i) == 1)
			newResultArray->operator[](i) = 1;
		else
			newResultArray->operator[](i) = 0;
	}

	//the node with the result is than added to the linked list of truth table which contains all premises
	insertInTruthTable(newResultArray);

	//it returns the computed result
	return newResultArray;
}

//computes result of OR function
Node* TruthTable::OR(Node* op1, Node* op2, string resultPremis)
{
	//declares a node, the array in node is of size of possible combinations
	Node* newResultArray = new Node(this->possibleCombinations, resultPremis);

	//array's of op1 and op2 are performed OR operation logic and the result is stored in newResultArray
	for (int i = 0; i < possibleCombinations; i++)
	{
		if (op1->operator[](i) == 1 || op2->operator[](i) == 1)
			newResultArray->operator[](i) = 1;
		else
			newResultArray->operator[](i) = 0;
	}

	//the node with the result is than added to the linked list of truth table which contains all premises
	insertInTruthTable(newResultArray);

	//it returns the computed result
	return newResultArray;
}

//computes result of NOT function
Node* TruthTable::NOT(Node* op1, string resultPremis)
{
	//declares a node, the array in node is of size of possible combinations
	Node* newResultArray = new Node(this->possibleCombinations, resultPremis);

	//array's of op1 and op2 are performed NOT operation logic and the result is stored in newResultArray
	for (int i = 0; i < possibleCombinations; i++)
	{
		if (op1->operator[](i) == 1)
			newResultArray->operator[](i) = 0;
		else
			newResultArray->operator[](i) = 1;
	}

	//the node with the result is than added to the linked list of truth table which contains all premises
	insertInTruthTable(newResultArray);

	//it returns the computed result
	return newResultArray;
}

//computes result of IMPLICATION function
Node* TruthTable::IMPLICATION(Node* op1, Node* op2, string resultPremis)
{
	//declares a node, the array in node is of size of possible combinations
	Node* newResultArray = new Node(this->possibleCombinations, resultPremis);

	//array's of op1 and op2 are performed IMPLICATION operation logic and the result is stored in newResultArray
	for (int i = 0; i < possibleCombinations; i++)
	{
		if (op1->operator[](i) == 1 && op2->operator[](i) == 0)
			newResultArray->operator[](i) = 0;
		else
			newResultArray->operator[](i) = 1;
	}

	//the node with the result is than added to the linked list of truth table which contains all premises
	insertInTruthTable(newResultArray);

	//it returns the computed result
	return newResultArray;
}

//computes result of BIIMPLICATION function
Node* TruthTable::BIIMPLICATION(Node* op1, Node* op2, string resultPremis)
{
	//declares a node, the array in node is of size of possible combinations
	Node* newResultArray = new Node(this->possibleCombinations, resultPremis);

	//array's of op1 and op2 are performed BIIMPLICATION operation logic and the result is stored in newResultArray
	for (int i = 0; i < possibleCombinations; i++)
	{
		if (op1->operator[](i) == op2->operator[](i))
			newResultArray->operator[](i) = 1;
		else
			newResultArray->operator[](i) = 0;
	}

	//the node with the result is than added to the linked list of truth table which contains all premises
	insertInTruthTable(newResultArray);

	//it returns the computed result
	return newResultArray;
}

//computes result of XOR function
Node* TruthTable::XOR(Node* op1, Node* op2, string resultPremis)
{
	//declares a node, the array in node is of size of possible combinations
	Node* newResultArray = new Node(this->possibleCombinations, resultPremis);

	//array's of op1 and op2 are performed XOR operation logic and the result is stored in newResultArray
	for (int i = 0; i < possibleCombinations; i++)
	{
		if (op1->operator[](i) == op2->operator[](i))
			newResultArray->operator[](i) = 0;
		else
			newResultArray->operator[](i) = 1;
	}

	//the node with the result is than added to the linked list of truth table which contains all premises
	insertInTruthTable(newResultArray);

	//it returns the computed result
	return newResultArray;
}



