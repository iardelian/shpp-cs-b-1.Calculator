#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>

using namespace std;

enum types {DELIMITER=1, NUMBER};

class parser {
    char *exp_pointer; //linking to the expression
    char token[30]; //stores present token
    char tokenType; //stores token type

    void isAdditionOrSubtraction(double &result);
    void isMultiplicationOrDivision(double &result);
    void isRaisingToPower(double &result);
    void isUnarySign(double &result);
    void isBrackets(double &result);
    void isNumber(double &result);
    void get_token();
    void syntaxError(int error);
    int isDelimiter(char delim);
public:
    parser();
    double getToken(char *exp);
};

parser::parser(){
    exp_pointer = NULL;
}

double parser::getToken(char *exp){
    double result;

    exp_pointer = exp;

    get_token();
    if(!*token){
        syntaxError(2); //check if expression is not empty
        return 0.0;
    }
    isAdditionOrSubtraction(result);
    if(*token){
        syntaxError(0); // last token must be nullpointer
    }
    return result;

}

// add or subtract two terms
void parser::isAdditionOrSubtraction(double &result){
    char operation;
    double temp;
    isMultiplicationOrDivision(result);
    while((operation = *token) == '+' || operation == '-'){
        get_token();
        isMultiplicationOrDivision(temp);
        switch(operation){
        case '-':
            result = result - temp;
            break;
        case '+':
            result = result + temp;
            break;
        }
    }
}

// multiply or divide two terms
void parser::isMultiplicationOrDivision(double &result){
    char operation;
    double temp;

    isRaisingToPower(result);
    while ((operation = *token) =='*' || operation == '/'){
        get_token();
        isRaisingToPower(temp);
        switch(operation){
        case '*':
            result = result * temp;
            break;
        case '/':
            result = result / temp;
            break;
        }
    }
}

// raising to power
void parser::isRaisingToPower(double &result)
{
    double temp;
    double exponent;
    int coef;

    isUnarySign(result);
    if(*token== '^')
    {
        get_token();
        isUnarySign(temp);
        exponent = result;
        if (temp == 0.0)
        {
            result = 1.0;
            return;
        }
        for ( coef = temp - 1; coef > 0; --coef) {
            result = result * exponent;
        }
    }
}
// determination of unary plus or minus
void parser::isUnarySign(double &result){
    char operation;
    operation = 0;

    if(((tokenType == DELIMITER) && *token=='+') || *token == '-'){
        operation = *token;
        get_token();
    }
    isBrackets(result);
    if (operation == '-'){
        result = -result;
    }
}
// brackets recognition
void parser::isBrackets(double &result){
    if((*token == '(')){
        get_token();
        isAdditionOrSubtraction(result);
        if(*token != ')'){
            syntaxError(1);
        }
        get_token();
    } else {
        isNumber(result);
    }
}

// get number
void parser::isNumber(double &result){
    switch(tokenType){
    case NUMBER:
        result = atof(token);
        get_token();
        return;
    default:
        syntaxError(0);
    }
}
// error message
void parser::syntaxError(int error){

    const char *message[]=
    {
        "Syntax error",
        "Upset the balance of the brackets",
        "Empty expression"
    };

    cout << message[error] << endl;
}
//get next character
void parser::get_token(){
    char *temp;
    tokenType = 0;
    temp = token;
    *temp = '\0';

    if(!*exp_pointer){
        return; //at the end of the expression
    }

    while (isspace(*exp_pointer)) {
        ++exp_pointer; //skipping spaces
    }

    if(strchr(" +-*/^()", *exp_pointer)){
        tokenType = DELIMITER;
        //move to the next character
        *temp++ = *exp_pointer++;
    }  else if (isdigit(*exp_pointer)){
        while (!isDelimiter(*exp_pointer)){
            *temp++ = *exp_pointer++;
        }
        tokenType = NUMBER;
    }
    *temp = '\0';
}

int parser::isDelimiter(char delim){ //if the token is a separator, returns true
    if (strchr(" +-/*^()", delim) || delim == 9 || delim == '\n' || delim == 0){
        return 1;
    }
    return 0;
}

int main()
{
    char expression[50]; // length of the expression by defaults will be 50 characters
    cout << "*-*-*-*-* Calculator *-*-*-*-*\n(to exit the program type 'X')\n\n\n";
    parser ob;
    while(true){
        cout<<"Enter the expression: ";
        cin.getline(expression, 49);
        if (*expression == 'X'){
            break;
        }
        cout << "Answer is: " << ob.getToken(expression) << "\n\n";
    }

    return 0;
}
