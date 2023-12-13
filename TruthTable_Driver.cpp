#include"TruthTable_Header.h"
int main()
{
	while (true)
	{
		int choice;
		cout << "1. Generate Truth Table\n";
		cout << "2. Check Equivalence\n";
		cout << "3. Exit\nInput: ";
		cin >> choice;
		while (choice > 3 || choice < 1 || !cin)
		{
			cin.clear();
			cin.ignore();
			cout << "Enter 1-3: ";
			cin >> choice;
		}
		system("cls");
		grayColor();
		cout << "! NOT\n& AND\n| OR\n+ XOR\n- IMPLICATION\n= BIIMPLICATION\n";
		cout << "Enter Expression like A|B or (A|B)\n";
		resetColor();

		switch (choice)
		{
			//Generate TruthTable of 1 expression
		case 1:
		{
			///---------------Expression input------------------------
			string expression1;
			cout << "Enter Expression 1: ";
			cin.clear();
			cin.ignore();
			getline(cin, expression1);

			//if expression is invalid
			if (containsInvalidChars(expression1) == true)
			{
				cout << "Invalid Enter again\n";
				cout << "Don't enter spaces, enter with proper or no brackets, don't enter number or symbols\n";
				system("pause");
				system("cls");
				main();
			}

			cout << "\n--------------------------------------------------------\n";
			///---------------Expression Evaluate------------------------

			TruthTable exp1(expression1);
			Node* result1 = exp1.generateTruthTable(expression1);

		}
		break;

		//Check equivalence of two expression
		case 2:
		{
			///---------------Expression input------------------------
			string expression1;
			cout << "Enter Expression 1: ";
			cin.clear();
			cin.ignore();
			getline(cin, expression1);

			//if expression is invalid
			if (containsInvalidChars(expression1) == true)
			{
				cout << "Invalid Enter again\n";
				cout << "Don't enter spaces, enter with proper or no brackets, don't enter number or symbols\n";

				system("pause");
				system("cls");
				main();
			}

			string expression2;
			cout << "Enter Expression 2: ";
			getline(cin, expression2);

			//if expression is invalid
			if (containsInvalidChars(expression2) == true)
			{
				cout << "Invalid Enter again\n";
				cout << "Don't enter spaces, enter with proper or no brackets, don't enter number or symbols\n";

				system("pause");
				system("cls");
				main();
			}

			cout << "\n--------------------------------------------------------\n";
			///---------------Expression 1 Evaluate------------------------

			TruthTable exp1(expression1);
			Node* result1 = exp1.generateTruthTable(expression1);

			cout << "--------------------------------------------------------\n";
			///---------------Expression 2 Evaluate------------------------

			TruthTable exp2(expression2);
			Node* result2 = exp2.generateTruthTable(expression2);


			cout << "--------------------------------------------------------\n";
			///---------------Equivalent Check------------------------

			if (isEquivalent(result1, result2))
			{
				greenColor();
				cout << "\nEquivalent\n";
			}
			else
			{
				redColor();
				cout << "\nNot Equivalent\n";
			}
			resetColor();
		}
		break;
		case 3:
			system("cls");
			cout << "BYE\n";
			return 0;
		}

		system("pause");
		system("cls");
	}

}