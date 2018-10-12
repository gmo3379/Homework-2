#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>



using namespace std;

int evaluateValue(string line);
double evaluate(string expression);
int prec(char c);
string replaceValue(string line, string variable, int value);

int main(int argc, const char * argv[]) {

	string line;
	ifstream input;
	string variableNames[10];
	int functionVariableValues[10];
    string functionVariableNames[10];
    int variableValues[10];
	int arrayIndex = 0;
    int functionArrayIndex = 0;
	bool assignmentLine = false;
    bool inFunction = false;
    bool inStringPrint = false;

	input.open("/Users/guillermo/Desktop/Homework 2/Homework 2/text.py");


	
	if (!input) {
		cerr << "Unable to open file datafile.txt";
		exit(1);
	}

	while (getline(input, line)) {
		string temp = "";
        
        size_t defFound = line.find("def");
        size_t returnFound = line.find("return");
        size_t printFound = line.find ("print");
        
        if (defFound != string::npos){
            inFunction = true;
        }
        else if (returnFound != string::npos) {
            inFunction = false;
            temp = "";
            
            for (int x=static_cast<int>(returnFound)+7; x< line.size();x++){
                temp = temp + line[x];
            }
            
            for (int x = 0; x<functionArrayIndex; x++) {
                if (temp == functionVariableNames[x]) {
                    variableNames[arrayIndex] = "f()";
                    variableValues[arrayIndex] = functionVariableValues[x];
                    arrayIndex++;
                }
            }
        }
        if (printFound != string::npos){
            size_t openParenthesisIndex = line.find('(');
            for (int x=static_cast<int>(openParenthesisIndex)+1; x< line.size();x++){
                if(line[x] == ',' && !inStringPrint){
                    temp = "";
                }
                else if (line[x] == '"' && !inStringPrint){
                    inStringPrint = true;
                }
                else if (line[x] == '"' && inStringPrint){
                    inStringPrint = false;
                    cout << temp;
                }
                else if (line[x] != '"' && line[x] != ')'){
                    temp=temp+line[x];
                }
                else if (!inStringPrint){
                    if (inFunction){
                        for (int x = 0; x<functionArrayIndex; x++) {
                            size_t found = temp.find(functionVariableNames[x]);
                            if (found != string::npos) {
                                cout << functionVariableValues[x];
                            }
                        }
                    }
                    else {
                        for (int x = 0; x<arrayIndex; x++) {
                            size_t found = temp.find(variableNames[x]);
                            if (found != string::npos) {
                                cout << variableValues[x];
                            }
                        }
                    }
                }
            }
        }
        else if (returnFound == string::npos) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '=') {
                    if (inFunction){
                        functionVariableNames[functionArrayIndex] = temp;
                    }
                    else {
                        variableNames[arrayIndex] = temp;
                    }
                    temp = "";
                    assignmentLine = true;
                }
                if (iswspace(line[i])) {

                }
                if (line[i] != '=' && !iswspace(line[i])) {
                    temp = temp + line[i];
                }
                
                if (i == (line.size() - 1) && assignmentLine) {
                    
                    if (inFunction){
                        for (int x = 0; x<functionArrayIndex; x++) {
                            size_t found = temp.find(functionVariableNames[x]);
                            if (found != string::npos) {
                                temp.replace(found, functionVariableNames[x].size(), to_string(functionVariableValues[x]));
                            }
                        }
                        
                    }
                    else {
                        for (int x = 0; x<arrayIndex; x++) {
                            size_t found = temp.find(variableNames[x]);
                            if (found != string::npos) {
                                temp.replace(found, variableNames[x].size(), to_string(variableValues[x]));
                            }
                        }
                    }
                    
                    if (inFunction){
                        functionVariableValues[functionArrayIndex] = evaluate(temp);
                        functionArrayIndex++;
                    }
                    else {
                        variableValues[arrayIndex] = evaluate(temp);
                        arrayIndex++;
                    }
                }
            }
        }
	}
	/*for (int i = 0; i < 3; i++) {
		cout << functionVariableNames[i] << "= " << functionVariableValues[i] << endl;
	}
    for (int i = 0; i < 3; i++) {
        cout << variableNames[i] << variableValues[i] << endl;
    }*/
	return 0;
}

double evaluate(string expr)
{
	string xxx; // Get Rid of Spaces
	for (int i = 0; i < expr.length(); i++)
	{
		if (expr[i] != ' ')
		{
			xxx += expr[i];
		}
	}

	string tok = ""; // Do parantheses first

	for (int i = 0; i < xxx.length(); i++)
	{
		if (xxx[i] == '(')
		{
			int iter = 1;
			string token;
			i++;
			while (true)
			{
				if (xxx[i] == '(')
				{
					iter++;
				}
				else if (xxx[i] == ')')
				{
					iter--;
					if (iter == 0)
					{
						i++;
						break;
					}
				}
				token += xxx[i];
				i++;
			}
			//cout << "(" << token << ")" << " == " << to_string(eval(token)) <<  endl;
			tok += to_string(evaluate(token));
		}
		tok += xxx[i];
	}

	for (int i = 0; i < tok.length(); i++)
	{
		if (tok[i] == '+')
		{
			//cout << tok.substr(0, i) + " + " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) + eval(tok.substr(i+1, tok.length()-i-1)) << endl;
			return evaluate(tok.substr(0, i)) + evaluate(tok.substr(i + 1, tok.length() - i - 1));
		}
		else if (tok[i] == '-')
		{
			//cout << tok.substr(0, i) + " - " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) - eval(tok.substr(i+1, tok.length()-i-1)) << endl;
			return evaluate(tok.substr(0, i)) - evaluate(tok.substr(i + 1, tok.length() - i - 1));
		}
	}

	for (int i = 0; i < tok.length(); i++)
	{
		if (tok[i] == '*')
		{
			//cout << tok.substr(0, i) + " * " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) * eval(tok.substr(i+1, tok.length()-i-1)) << endl;
			return evaluate(tok.substr(0, i)) * evaluate(tok.substr(i + 1, tok.length() - i - 1));
		}
		else if (tok[i] == '/')
		{
			//cout << tok.substr(0, i) + " / " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) / eval(tok.substr(i+1, tok.length()-i-1)) << endl;
			return evaluate(tok	.substr(0, i)) / evaluate(tok.substr(i + 1, tok.length() - i - 1));
		}
	}

	//cout << stod(tok.c_str()) << endl;
	return stod(tok.c_str()); // Return the value...
}
