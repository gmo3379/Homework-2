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
string addWhiteSpace(string &temp);
std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to);

//added code
bool evalBoolExp(string boolExp);
void replaceValues(string &temp, string variableNames[], int variableValues[], int arrayIndex);
bool isOperator(char temp);
bool evalBoolExp(string Expression);
int firstChar(string line);
int lineNumber = 1;

int main(int argc, const char * argv[]) {
    
    string line;
    ifstream input;
    string variableNames[50];
    int functionVariableValues[50];
    string functionVariableNames[50];
    int variableValues[50];
    string printFuncitonNames[50];
    string printFuncitonValues[50];
    int arrayIndex = 0;
    int functionArrayIndex = 0;
    int printFunctionIndex = 0;
    bool assignmentLine = false;
    bool inFunction = false;
    bool inStringPrint = false;
    bool elseSeen = false;
    string funcitonPrint = "";
    string currentFunctionName = "";
    
    int ifIndex = 0;
    bool ifStatement = true;
    
    
    bool unassignedVar = false;
    input.open("/Users/guillermo/Desktop/Homework 2/Homework 2/text.py");
    //input.open("myFile.txt");
    
    
    if (!input) {
        cerr << "Unable to open file datafile.txt";
        exit(1);
    }
    
    while (getline(input, line)) {
        
        
        string temp = "";
        
        size_t defFound = line.find("def");
        size_t returnFound = line.find("return");
        size_t printFound = line.find("print");
        size_t ifFound = line.find("if");
        size_t elseFound = line.find("else");
        string boolExp = "";
        bool functionFound=false;
        
        
        for (int x=0 ; x < printFunctionIndex ; x++){
            size_t defNameFound = line.find(printFuncitonNames[x]);
            if (defNameFound != string::npos && defNameFound < 2){
                cout << printFuncitonValues[x];
                functionFound=true;
            }
        }
        
        if (firstChar(line) == 0 && inFunction){
            inFunction = false;
        }
        if (line.empty() || firstChar(line) == line.size() || line[0] == '#'){
            //do nothing
        }
        else if (unassignedVar == true){
            
        }
        else if (defFound != string::npos) {
            currentFunctionName = "";
            inFunction = true;
            for (int x=4 ; x < line.size()-1 ; x++){
                currentFunctionName = currentFunctionName + line[x];
            }
            printFuncitonNames[printFunctionIndex] = currentFunctionName;
        }
        else if (returnFound != string::npos) {
            inFunction = false;
            temp = "";
            
            for (int x = static_cast<int>(returnFound) + 7; x< line.size(); x++) {
                temp = temp + line[x];
            }
            bool variableNameFound = false;
            for (int x = 0; x<functionArrayIndex; x++) {
                if (temp == functionVariableNames[x]) {
                    variableNames[arrayIndex] = currentFunctionName;
                    variableValues[arrayIndex] = functionVariableValues[x];
                    arrayIndex++;
                    variableNameFound = true;
                }
            }
            if (variableNameFound == false){
                variableNames[arrayIndex] = currentFunctionName;
                variableValues[arrayIndex] = stoi(temp);
                arrayIndex++;
            }
        }
        else if (ifFound != string::npos){
            size_t openParenthesisIndex = line.find('(');
            size_t closeParenthesisIndex = line.find(')');
            ifIndex = static_cast<int>(ifFound);
            
            //bad but lazy
            if (openParenthesisIndex != string::npos && closeParenthesisIndex != string::npos) {
                for (int i = static_cast<int>(openParenthesisIndex) + 1; i < closeParenthesisIndex; i++) {
                    boolExp += line[i];
                }
            }
            else {
                for (int i = static_cast<int>(ifFound) + 3; i < line.size(); i++) {
                    if (line[i] != ':') {
                        boolExp += line[i];
                    }
                }
            }
            if (inFunction){
                replaceValues(boolExp,functionVariableNames,functionVariableValues,functionArrayIndex);
            }
            else {
                replaceValues(boolExp, variableNames, variableValues, arrayIndex);
            }
            
            ifStatement = evalBoolExp(boolExp);
            
        }
        else if (elseFound != string::npos) {
            elseSeen = true;
        }
        else if((ifStatement == false) && (ifIndex + 3 == firstChar(line)) && !elseSeen){

        }
        else if((ifStatement == true) && (ifIndex + 3 == firstChar(line)) && elseSeen){
            
        }
        else if (printFound != string::npos) {
            size_t openParenthesisIndex = line.find('(');
            for (int x = static_cast<int>(openParenthesisIndex) + 1; x< line.size(); x++) {
                if (line[x] == ',' && !inStringPrint) {
                    temp = "";
                }
                else if (line[x] == '"' && !inStringPrint) {
                    inStringPrint = true;
                }
                else if (line[x] == '"' && inStringPrint) {
                    inStringPrint = false;
                    if (inFunction){
                        printFuncitonValues[printFunctionIndex] = printFuncitonValues[printFunctionIndex] + temp;
                    }
                    else {
                        cout << temp;
                    }
                    temp = "";
                }
                else if (line[x] != '"' && line[x] != ')') {
                    temp = temp + line[x];
                }
                else if (!inStringPrint) {
                    if (inFunction) {
                        for (int x = 0; x<functionArrayIndex; x++) {
                            size_t found = temp.find(functionVariableNames[x]);
                            if (found != string::npos) {
                                printFuncitonValues[printFunctionIndex] = printFuncitonValues[printFunctionIndex] + to_string(functionVariableValues[x]);
                            }
                        }
                    }
                    else {
                        for (int x = 0; x<arrayIndex; x++) {
                            size_t found = temp.find(variableNames[x]);
                            if (found != string::npos) {
                                cout << variableValues[x] << endl;
                            }
                        }
                    }
                }
                if (line [x] == ')'){
                    if (inFunction){
                        printFuncitonValues[printFunctionIndex] = printFuncitonValues[printFunctionIndex] + '\n';
                        printFunctionIndex++;
                    }
                }
            }
        }
        else if (!functionFound){
            string currentVar;
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '=') {
                    if (inFunction) {
                        functionVariableNames[functionArrayIndex] = temp;
                        currentVar = temp;
                    }
                    else {
                        variableNames[arrayIndex] = temp;
                        currentVar = temp;
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
                    if (inFunction) {
                        addWhiteSpace(temp);
                        for (int x = 0; x<functionArrayIndex; x++) {
                            string toFind = " " + functionVariableNames[x] + " ";
                            size_t found = temp.find(toFind);
                            if (found != string::npos) {
                                ReplaceAll(temp, toFind, to_string(functionVariableValues[x]));
                            }
                        }
                        
                    }
                    else {
                        addWhiteSpace(temp);
                        for (int x = 0; x<arrayIndex; x++) {
                            string toFind = " " + variableNames[x] + " ";
                            size_t found = temp.find(toFind);
                            if (found != string::npos) {
                                ReplaceAll(temp, toFind, to_string(variableValues[x]));
                            }
                        }
                    }
                    for (int i=0;i<temp.size();i++){
                        if(!isdigit(temp[i]) && !isOperator(temp[i]) && !iswspace(temp[i])){
                            unassignedVar = true;
                            cout << "Eror: Unassigned Variable." << endl;
                        }
                    }
                    if (unassignedVar == false){
                        if (inFunction) {
                            functionVariableValues[functionArrayIndex] = evaluate(temp);
                            for (int x=0; x< functionArrayIndex; x++){
                                if (functionVariableNames[x] == currentVar){
                                    functionVariableNames[x] = " ";
                                }
                            }
                            functionArrayIndex++;
                        }
                        else {
                            variableValues[arrayIndex] = evaluate(temp);
                            for (int x=0; x< arrayIndex; x++){
                                if (variableNames[x] == currentVar){
                                    variableNames[x] = " ";
                                }
                            }
                            arrayIndex++;
                        }
                    }
                }
            }
        }
        lineNumber++;
    }
     //for (int i = 0; i < 3; i++) {
     //cout << functionVariableNames[i] << "= " << functionVariableValues[i] << endl;
     //}
     //for (int i = 0; i < 3; i++) {
     //cout << variableNames[i] << " = " << variableValues[i] << endl;
     //}
    //for (int i = 0; i < 3; i++) {
    //    cout << printFuncitonNames[i] << " = " << printFuncitonValues[i] << endl;
    //}
    return 0;
}

int firstChar(string line){
    int i =0;
    while(iswspace(line[i])){
        i++;
    }
    return i;
}

bool evalBoolExp(string Expression) {
    bool varaiblesAssigned = true;
    
    
    if (varaiblesAssigned) {
        string leftSide = "";
        string rightSide = "";
        string Operator = "";
        
        bool leftDone = false;
        
        for (int i = 0; i < Expression.size(); i++) {
            if (!iswspace(Expression[i]) && !isOperator(Expression[i]) && leftDone == false) {
                leftSide += Expression[i];
            }
            else if (isOperator(Expression[i])) {
                Operator += Expression[i];
                leftDone = true;
            }
            else if (leftDone == true && !iswspace(Expression[i])) {
                rightSide += Expression[i];
            }
        }
        //cout << leftSide << rightSide << endl;
        int intLeftSide = stoi(leftSide);
        int intRightSide = stoi(rightSide);
        
        //cout << intLeftSide << Operator << intRightSide << endl;
        if (Operator == "<") {
            return intLeftSide < intRightSide;
        }
        else if (Operator == "<=") {
            return intLeftSide <= intRightSide;
        }
        else if (Operator == ">") {
            return intLeftSide > intRightSide;
        }
        else if (Operator == ">=") {
            return intLeftSide >= intRightSide;
        }
        else if (Operator == "==") {
            return intLeftSide == intRightSide;
        }
        else {
            return intLeftSide != intRightSide;
        }
    }
    else {
        return false;
    }
}

void replaceValues(string &temp, string variableNames[], int variableValues[], int arrayIndex) {
    addWhiteSpace(temp);
    for (int x = 0; x < temp.size(); x++) {
        string toFind = " " + variableNames[x] + " ";
        size_t found = temp.find(toFind);
        if (found != string::npos) {
            ReplaceAll(temp, toFind, to_string(variableValues[x]));
        }
    }
}

std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

bool isOperator(char temp) {
    string operators = "+-*/=<>";
    bool itIs = false;
    for (int i = 0; i < operators.size(); i++) {
        if (temp == operators[i]) {
            itIs = true;
        }
    }
    return itIs;
}

string addWhiteSpace(string &temp) {
    string white = " ";
    int k = 0;
    //bool functionFound = false;
    while (k < temp.size()) {
        if (k == 0) {
            //cout << "enter if" << "k=" << k<<  endl;
            temp.insert(0, white);
            k++;
            //cout << "value of temp after if:" << temp << endl;
        }
        else if (isOperator(temp[k]) && isalnum(temp[k - 1])) {
            
            //cout << "enter else if k=" << k<< " operator found and previous item is not white" << endl;
            temp.insert(k, white);
            k++;
            //cout << "value of temp after elseif1:" << temp << endl;
        }
        else if (isalnum(temp[k]) && isOperator(temp[k - 1])) {
            //cout << "enter else if k=" << k<<" operator has no whitespace after it" << endl;
            temp.insert(k, white);
            k++;
            //cout << "value of temp after elseif2:" << temp << endl;
            
        }
        
        else if (temp[k - 1] == ')') {
            temp.insert(k, white);
            k++;
        }
        else if (k == temp.size() - 1) {
            const char *p;
            p = " ";
            temp.append(p);
            //cout << "value of temp: " << temp << "test" << endl;
            k += 2;
        }
        else {
            //cout << "else k=" << k << endl;;
            k++;
        }
    }
    return temp;
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
            return evaluate(tok.substr(0, i)) / evaluate(tok.substr(i + 1, tok.length() - i - 1));
        }
    }
    
    //cout << stod(tok.c_str()) << endl;
    
    return stod(tok.c_str()); // Return the value...
}
