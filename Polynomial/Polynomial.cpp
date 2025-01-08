#include <iostream>
using namespace std;

class Polynomial;

class Term {
public:
    Term() {coef = 0; exp = 0;}
    ~Term() {}
	friend class Polynomial;

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
    void ChangeSize1D(const int newSize);
        // change the array size to newSize
    void SortArray();
        // sorting termArray ascending by they're exp

/*---object function---*/ 
    Polynomial();
        // construct the polynomial p(x) = 0
    ~Polynomial() {delete [] termArray;}
        // destructor
    Polynomial* Add(Polynomial *poly);
        // return the sum of *this and poly
    float Eval(float f);
        // Evaluate the polynomial *this at f and return the results
    friend ostream &operator<< (ostream &out, const Polynomial &poly);
        // overloading <<
    friend istream &operator>> (istream &in, Polynomial &poly);
        // overloading >>

private:
    float GetCoef(int e) const {return termArray[e].coef;}
        // return the coefficient of e
    int GetEXP(int e) const {return termArray[e].exp;};
        // return the exponent of e
	Term* termArray;    // the polynomial array
	int	capacity;       // size of termArray
	int	terms;          // number of nonzero terms
};

int main() {
    #ifdef USE_DEFAULT
        cout << "USE_DEFAULT is defined. Use default as input" << endl;
    #endif

    try {
        Polynomial poly1, poly2;
        float y;

        cout << "--- Demo cin/cout ---" << endl;
        cout << "Ex: 5x^3 + 4x^7 + 8x^0 -> input: 5 3 4 7 8 0" << endl;
        cout << "Coef<non-zeno, float>, Exp<non-negative, int>" << endl;
        #ifdef USE_DEFAULT
            cout << "Poly1 Default input: 5 3 4 7 8 0" << endl;
            poly1.NewTerm(5, 3);
            poly1.NewTerm(4, 7);
            poly1.NewTerm(8, 0);
            cout << "Poly2 Default input: 3 3 3 1 -3 7" << endl;
            poly2.NewTerm(3, 3);
            poly2.NewTerm(3, 1);
            poly2.NewTerm(-3, 7);
        #else
            cout << "Enter Polynomial 1: ";
            cin >> poly1;
            cout << "Enter Polynomial 2: ";
            cin >> poly2;
            cout << endl;
        #endif

        cout << "Perform sorting..." << endl;
        poly1.SortArray();
        poly2.SortArray();

        cout << "Polynomial 1 is: " << poly1;
        cout << "Polynomial 2 is: " << poly2;

        cout << "--- Demo Add() ---" << endl;
        Polynomial *poly3;
        poly3 = poly1.Add(&poly2);
        cout << "Polynomail 1 + Polynomial 2 = " << *poly3;

        cout << "--- Demo Eval() ---" << endl;
        #ifdef USE_DEFAULT  
            cout << "Default y = 2" << endl;
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

// construct the polynomial p(x) = 0
Polynomial::Polynomial() {
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
}

// add a new term to the end of termArray
void Polynomial::NewTerm(float input_coef, int input_exp) {
    if (input_coef == 0 || input_exp < 0)
        throw runtime_error("ai non-zero, ei non-negative");

    if (terms == capacity) {
        ChangeSize1D(2 * capacity);
    }
    termArray[terms].coef = input_coef;     
    termArray[terms].exp = input_exp;
    terms++;
}

void Polynomial::ChangeSize1D(const int newSize) {
    if (newSize < terms)
        throw runtime_error("New size must be greater than number of terms");

    Term *temp = new Term[newSize];
    copy(termArray, termArray + terms, temp);
    delete [] termArray;
    termArray = temp;
    capacity = newSize;
}

// sorting termArray ascending by they're exp
// choosing insertion sort because it has better best case 
void Polynomial::SortArray() {
    int i, j;
    float temp_coef;
    int temp_exp;

    for (i = 1; i < terms; i++) {
        temp_exp = termArray[i].exp;
        temp_coef = termArray[i].coef;
        for (j = i - 1; ((j >= 0) && (temp_exp > termArray[j].exp)); j--) {
            termArray[j + 1].exp = termArray[j].exp;
            termArray[j + 1].coef = termArray[j].coef;
        } 
        termArray[j + 1].exp = temp_exp;
        termArray[j + 1].coef = temp_coef;
    }
}

// return the sum of *this and poly
// **need to sort first**
Polynomial* Polynomial::Add(Polynomial *poly) {
    // c = a.Add(b)
    // a: this termArray
    // b: poly(input)
    // c: output
    Polynomial *c = new Polynomial();
    int aPos = 0, bPos = 0;
    float temp_coef;
    
    while ((aPos < terms) && (bPos < poly->terms)) {
        if (termArray[aPos].exp == poly->termArray[bPos].exp) {
            temp_coef = termArray[aPos].coef + poly->termArray[bPos].coef;
            if (temp_coef != 0) // coef should be non-zero
                c->NewTerm(temp_coef, termArray[aPos].exp);
            aPos++;
            bPos++;
        } else if (termArray[aPos].exp < poly->termArray[bPos].exp) {
            c->NewTerm(poly->termArray[bPos].coef, poly->termArray[bPos].exp);
            bPos++;
        } else {
            c->NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }

    // put remaining terms form a(or b) to c
    for (; aPos < terms; aPos++)
        c->NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < poly->terms; bPos++)
        c->NewTerm(poly->termArray[bPos].coef, poly->termArray[bPos].exp);
    return c;
}

// Evaluate the polynomial *this at f and return the results
float Polynomial::Eval(float f) {
    int i, j;
    float sum = 0;

    for (i = 0; i < terms; i++) {
        float termValue = termArray[i].coef;
        for (j = 0; j < termArray[i].exp; j++)
            termValue *= f;
        sum += termValue; 
    }
    
    return sum;
}

// overloading <<
ostream &operator<< (ostream &out, const Polynomial &poly) {
    int i;    

    for (i = 0; i < poly.terms; i++) {
        float a = poly.GetCoef(i);
        out << poly.GetCoef(i) << "x^" << poly.GetEXP(i);
        if (i != poly.terms - 1)
            out << " + ";         // last term doesn't need "+"
    }
    cout << endl;

    return out;
}

// overloading >>
istream &operator>> (istream &in, Polynomial &poly) {
    float a; // coef
    int e;   // exp
    int ch;  

    do {
        in >> a;                       
        in >> e;
        poly.NewTerm(a, e);             
    } while ((ch = getchar()) != '\n'); // check whether is last input

    return in;
}