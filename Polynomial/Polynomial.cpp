#include <iostream>
using namespace std;

class Polynomial;

class Term {
public:
	friend class Polynomial;
    friend ostream &operator << (ostream &out, const Polynomial &poly);
        // overloading <<
    friend istream &operator >> (istream &in, Polynomial &poly);
        // overloading >>

private:
    float coef;
    int exp;
};

class Polynomial {
    // p(x) = a0 x^e0 + … + an x^en 
    // where ai is nonzero float and ei is non-negative int

public:
/*---tool function---*/
    void NewTerm(float input_coef, int input_exp);
        // add a new term on the tail
    void sortArray();
        // sorting termArray ascending by they're exp

/*---object function---*/ 
    Polynomial();
        // construct the polynomial p(x) = 0
    Polynomial Add(Polynomial poly);
        // return the sum of *this and poly
    Polynomial Mult(Polynomial poly);
        // return the product of *this and poly
    float Eval(float f);
        // Evaluate the polynomial *this at f and return the results
    int operator!();
        // if *this is the zero polynomial, return 1; else return 0;
    float Coef(int e);
        // return the coefficient of e in *this
    int LeadExp();
        // return the largest exponent in *this
    friend ostream &operator << (ostream &out, const Polynomial &poly);
        // overloading <<
    friend istream &operator >> (istream &in, Polynomial &poly);
        // overloading >>

private:
	Term* termArray;    // the polynomial array
	int	capacity;       // size of termArray
	int	terms;          // number of nonzero terms
};

int main(int argc, char* argv[]) {
    try {
        Polynomial poly1, poly2;
        float y;

        cout << "--- Demo cin/cout ---" << endl;
        cout << "(Ex: 5x^3 + 4x^7 + 8x^0 -> input: 5 3 4 7 8 0)" << endl;
        cout << "Type: <int>" << endl;
        #ifdef USE_DEFAULT
            poly1.NewTerm(5, 3);
            poly1.NewTerm(4, 7);
            poly1.NewTerm(8, 0);
        #else
            cout << "Enter Polynomial 1: ";
            cin >> poly1;
            cout << "Enter Polynomial 2: ";
            cin >> poly2;
            cout << endl;
        #endif

        cout << "Perform sorting..." << endl;
        poly1.sortArray();
        poly2.sortArray();

        cout << "Polynomial 1 is: " << poly1;
        cout << "Polynomial 2 is: " << poly2;

        cout << "--- Demo Add() ---" << endl;
        Polynomial poly3 = poly1.Add(poly2);
        cout << "Polynomail 1 + Polynomial 2 = " << poly3;

        cout << "--- Demo Eval() ---" << endl;
        #ifdef USE_DEFAULT  
            cout << "Polynomial 1 = " << poly1.Eval(2) << " at y = 2" << endl;
            cout << "Polynomial 2 = " << poly2.Eval(2) << " at y = 2" << endl;
        #else
            cout << "Enter float y: ";
            cin >> y;
            cout << "Polynomial 1 = " << poly1.Eval(y) << " at y = " << y << endl;
            cout << "Polynomial 2 = " << poly2.Eval(y) << " at y = " << y << endl;
        #endif
    } catch(exception const &e) {
        cout << e.what() << endl;
    }

    return 0;
}