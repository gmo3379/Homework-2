#include <iostream>
#include <fstream>
#include <sstream>
#include<string>
#include<stack>


using namespace std;

int evaluateValue(string line);
double evaluate(string expression);
int prec(char c);
string replaceValue (string line, string variable, int value);

int main(int argc, const char * argv[]) {
    
    string line;
    ifstream input;
    string variableNames[10];
    int variableValues[10];
    int arrayIndex = 0;
    bool assignmentLine = false;
    input.open ("/Users/guillermo/Desktop/Homework 2/Homework 2/text.py");
    
    if (!input) {
        cerr << "Unable to open file datafile.txt";
        exit(1);
    }
    
    while (getline(input, line)){
        string temp = "";
        for (int i=0 ; i < line.size(); i++){
            if (line[i] == '='){
                variableNames[arrayIndex] = temp;
                temp = "";
                assignmentLine = true;
            }
            if (line[i] != '='){
            temp= temp + line[i];
            }
            if (i == (line.size() - 1) && assignmentLine){
                for (int x=0 ; x<arrayIndex ; x++){
                    size_t found = temp.find(variableNames[x]);
                    if (found != string::npos) {
                        temp.replace(found, sizeof(to_string(variableValues[x]))-1, to_string(variableValues[x]));
                    }
                }
                variableValues[arrayIndex] = evaluate(temp);
                arrayIndex++;
            }
        }
    }
    
    cout << variableNames[0] << variableValues[0] << endl;
    cout << variableNames[1] << variableValues[1] << endl;
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
                } else if (xxx[i] == ')')
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
            return evaluate(tok.substr(0, i)) + evaluate(tok.substr(i+1, tok.length()-i-1));
        } else if (tok[i] == '-')
        {
            //cout << tok.substr(0, i) + " - " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) - eval(tok.substr(i+1, tok.length()-i-1)) << endl;
            return evaluate(tok.substr(0, i)) - evaluate(tok.substr(i+1, tok.length()-i-1));
        }
    }
    
    for (int i = 0; i < tok.length(); i++)
    {
        if (tok[i] == '*')
        {
            //cout << tok.substr(0, i) + " * " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) * eval(tok.substr(i+1, tok.length()-i-1)) << endl;
            return evaluate(tok.substr(0, i)) * evaluate(tok.substr(i+1, tok.length()-i-1));
        } else if (tok[i] == '/')
        {
            //cout << tok.substr(0, i) + " / " +  tok.substr(i+1, tok.length()-i-1) << " == " << eval(tok.substr(0, i)) / eval(tok.substr(i+1, tok.length()-i-1)) << endl;
            return evaluate(tok.substr(0, i)) / evaluate(tok.substr(i+1, tok.length()-i-1));
        }
    }
    
    //cout << stod(tok.c_str()) << endl;
    return stod(tok.c_str()); // Return the value...
}
