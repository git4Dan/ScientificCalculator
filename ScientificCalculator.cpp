#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <stack>
#include <queue>

using namespace std;

void computationAlgorithm(string temp);
void stackCompute();
unordered_map<string, double> ScienceCalc;//map for variables
unordered_map<char, int> operPrec;//map for operator precedence
queue<string> operands;//queue for operands
stack<char> operators;//stack for operators
stack<double> compute;//stack to compute everything
bool quitBool = false;//bool if quit is entered
bool divByZero = false;//bool for dividing by zero error
bool undeclaredBool = false;//bool for finding an undeclared variable
bool varBool = false;//bool for declaring a variable in compute function
bool declVar = false;//bool for declaring a variable in algorithm function

int main ()
{
	//constants
	ScienceCalc["Pi"] = 3.14169;
	ScienceCalc["e"] = 2.718;

	//initialize all precedence of values to be used
	operPrec['#'] = 5;//char for sin
	operPrec['&'] = 5;// char for cos
	operPrec['!'] = 5;//char for log
	operPrec['('] = 5;
	operPrec['^'] = 4;
	operPrec['*'] = 3;
	operPrec['/'] = 3;
	operPrec['+'] = 2;
	operPrec['-'] = 2;
	operPrec[')'] = 1;

	string input;

	//input != quit  in while loop
	//eventually make it end when front of queue equals quit
	while(quitBool == false){//while quit is not found

		divByZero = false;//set all these to false again before next iteration
		undeclaredBool = false;
		varBool = false;
		declVar = false;

		getline(cin, input);//use getline to get each inputed string
		computationAlgorithm(input);
		stackCompute();

	}

  return 0;
}//end of main

//3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3
void computationAlgorithm(string temp){//this function determines order of precedence and correctly converst infix to postfix form
	string oper;
	string oper2;
	string var;
	string s;
	string oper3;
	string num2;
	bool closeParen = false;
	int opPrec;
	bool letPresent = false;

	for(unsigned int i = 0; i < temp.length(); i++){
		if(temp[i] == ' '){//used to ignore whitespace
			continue;
		}
		else if(temp[i] == '='){//if an '=' is found, push it onto queue
			s.push_back(temp[i]);
			operands.push(s);
			s = "";
			continue;
		}
		else if(isalpha(temp[i]) && temp[i] != ' '){//if an alphabetical char is found
			while(isalpha(temp[i]) && temp[i] != ' '){
				var.push_back(temp[i]);
				if(i+1 <= temp.length()){//CHECK THIS!!!!
					if(!isalpha(temp[i+1])){
						if(var == "let" && operands.empty()){
							operands.push(var);
							letPresent = true;
						}
						else if(var == "sin"){//FIX THIS IF NECESSARY!!!
							operators.push('#');
						}
						else if(var == "cos"){//if any of these functions are used, push a special character as this is a char stack
							operators.push('&');
						}
						else if(var == "log"){
							operators.push('!');
						}
						else if(letPresent == true){//if a let has been found, then something is being declared
							letPresent = false;
							operands.push(var);
							declVar = true;
						}
						else{
							operands.push(var);
						}
						var = "";
						break;
					}
				}
				i++;
			}

		}
		else if((isdigit(temp[i]) || temp[i] == '.' || temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '^' || temp[i] == ')' || temp[i] == '(') && temp[i] != ' ' && !isalpha(temp[i])){
				if(isdigit(temp[i])){
					while((i < temp.length()) && (isdigit(temp[i]) || temp[i] == '.')){//to deal with numbers and decimals
						oper2.push_back(temp[i]);
						i++;
					}
					operands.push(oper2);
					i--;
					oper2 = "";
					continue;
				}
				else if(temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '^' || temp[i] == '(' || temp[i] == ')'){//if an operator is found
					opPrec = operPrec[temp[i]];
					if(operators.empty() || temp[i] == '('){
						operators.push(temp[i]);
					}
					else if((temp[i] == '*' || temp[i] == '/') && operPrec[operators.top()] >= opPrec && operators.top() != '(' && operators.top() != '#' && operators.top() != '&' && operators.top() != '!'){
						while(!operators.empty() && operPrec[operators.top()] >= opPrec && operators.top() != '('){
							oper2.push_back(operators.top());
							operators.pop();
							operands.push(oper2);
							oper2 = "";
						}
						operators.push(temp[i]);
					}
					else if((temp[i] == '+' || temp[i] == '-') && operPrec[operators.top()] >= opPrec && operators.top() != '(' && operators.top() != '#' && operators.top() != '&' && operators.top() != '!'){
						while(!operators.empty() && operPrec[operators.top()] >= opPrec && operators.top() != '('){
							oper2.push_back(operators.top());
							operators.pop();
							operands.push(oper2);
							oper2 = "";
						}
						operators.push(temp[i]);
					}
					else if(temp[i] == ')'){
							while(closeParen == false && !operators.empty() && operPrec[operators.top()] >= opPrec){//pop everything until open parenthesis is found
								if(operators.top() != '('){
									oper.push_back(operators.top());
									operators.pop();
									operands.push(oper);
									oper = "";
								}
								else{
									operators.pop();
									closeParen = true;
									if(!operators.empty() && (operators.top() == '#' || operators.top() == '&' || operators.top() == '!')){//if a function is the top, then push onto queue
										oper.push_back(operators.top());
										operators.pop();
										operands.push(oper);
										oper = "";
									}
									break;
								}
							}
							closeParen = false;
						}
					else{
						operators.push(temp[i]);
					}
				}
			}
		}

	string stackStuff;
	while(!operators.empty()){//clear stack and push whats left onto queue
		stackStuff.push_back(operators.top());
		operands.push(stackStuff);
		stackStuff = "";
		operators.pop();
	}
}//end of computationAlgorithm function

void stackCompute(){//this function computes the everything using another stack
	string queueString;
	string calcVar;
	double value;
	double value2;
	bool defined = false;
	int i = 0;
	double mapValue;
	string front;

	while(!operands.empty()){//keep running until queue is empty

		front = operands.front();

		if(isalpha(front[i]) || front == "="){//REVIEW WHETHER TO EVEN LEAVE THE '=' INSIDE QUEUE
			if(front == "quit"){//if quit is found, end program
				quitBool = true;
				operands.pop();
			}
			else if(front == "let" || front == "="){//can also do the unreference variable part here if no let
				operands.pop();//immediately pop any let || =
			}
			else if(!operands.empty()){
				queueString = operands.front();
				if(isalpha(queueString[i]) && !operands.empty()){//CHECK IF SHOULD USE MAP COUNT!!!
					if(declVar == true){//this would be true if a variable is declared
						calcVar = queueString;
						operands.pop();
						queueString = "";
						declVar = false;
						defined = true;
					}
					else if(ScienceCalc.count(queueString) > 0){//if the variable has already been declared, then push onto stack
						mapValue = ScienceCalc[queueString];
						compute.push(mapValue);
						operands.pop();
					}
					else if(ScienceCalc.count(queueString) == 0){//if this variable is not declared, then there is an undeclared variable
						cout<<"Undeclared-Variable\n";
						undeclaredBool = true;
						break;
				}

				}
			}

		}
		else if(isdigit(front[i])){//if a digit
			value = atof(front.c_str());
			operands.pop();
			compute.push(value);
		}
		else if(front == "#"){//sin
			operands.pop();
			value = compute.top();
			compute.pop();
			value = sin(value);
			compute.push(value);
		}
		else if(front == "&"){//cos
			operands.pop();
			value = compute.top();
			compute.pop();
			value = cos(value);
			compute.push(value);
		}
		else if(front == "!"){//log
			operands.pop();
			value = compute.top();
			compute.pop();
			value = log(value);
			compute.push(value);
		}
		else if(front == "^"){//if an exponent is found
			operands.pop();
			value = compute.top();
			compute.pop();
			value2 = compute.top();
			compute.pop();
			value = pow(value2, value);
			compute.push(value);
		}
		else if(front == "*"){//for multiplying
			operands.pop();
			value = compute.top();
			compute.pop();
			value2 = compute.top();
			compute.pop();
			value = value2 * value;
			compute.push(value);
		}
		else if(front == "/"){//for dividing
			operands.pop();
			value = compute.top();
			compute.pop();
			value2 = compute.top();
			compute.pop();
			if(value == 0){//checks if dividing by a zero
				cout<<"Division-By-Zero\n";
				divByZero = true;
				break;
			}
			value = value2 / value;
			compute.push(value);
		}
		else if(front == "+"){//for addition
			operands.pop();
			value = compute.top();
			compute.pop();
			value2 = compute.top();
			compute.pop();
			value = value2 + value;
			compute.push(value);
		}
		else if(front == "-"){//for subtraction
			operands.pop();
			value = compute.top();
			compute.pop();
			value2 = compute.top();
			compute.pop();
			value = value2 - value;
			compute.push(value);
		}
	}
		if(defined == true && undeclaredBool == false){//store a new variable in hash map
			ScienceCalc[calcVar] = compute.top();
			varBool = true;
		}

		if(!compute.empty() && (quitBool != true && divByZero != true && undeclaredBool != true && varBool != true)){//output the top of the stack
			cout<<compute.top()<<"\n";
		}
		while(!operands.empty()){
			operands.pop();//clear queue again just to be safe
			}

		while(!compute.empty()){//clears the stack just to be safe
			compute.pop();
		}
	}//end of compute function

