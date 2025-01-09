#include <iostream>
#include <string.h>
using namespace std;

class String {
public:
    /*---tool function---*/
    void PrintString();
        // print string
    /*---object function---*/
    String(const char *init, int m);
        // constructor using input string init of length m
    bool operator == (String t);
        // equality test
    bool operator!();
        // empty test, true or false
    int Length();
        // get the number of characters of *this
    String Concat(String t);  
        // concatenation with another string t
    String Substr(int i, int j);
        // generate a substring i~i+j-1
    int Find_KMP(String pat);
        // Return an index i such that pat matches the substring
        // of the object begins at position i.  Return -1 if pat
        // is empty or not a substring of the object
    void _BuildFailureFunction();
        // failure function for KMP
    int Find_Boyre_Moore(String pat);
        // Return an index i such that pat matches the substring
        // of the object begins at position i.  Return -1 if pat
        // is empty or not a substring of the object
    void _BuildLastOccFuntion();
        // last occurrence function for Boyer-Moore
    String Delete(int start, int len);
        // remove length characters beginning at start
    String CharDelete(char c);
        // returns the string with all occurrence of c removed.
    
    int Compare(String y);
        // compare two strings of letters of alphabet.
    
private:
    const char *str;
    int length;
    int *F;
    int *L;
};

int main() {
    const char *A = "We have data structure on Fridays.";
    const char *B = "data structure";
    // const char *A = "abacaabaccabacabaabb";
    // const char *B = "abacab";
    const char *C = "";

    String str1(A, strlen(A));
    String str2(B, strlen(B));
    String str3(C, strlen(C));

    int x, y;
    char c;

    cout << "---Demo 2---" << endl;
    cout << "String 1 = ";
    str1.PrintString(); 
    cout << "String 2 = ";
    str2.PrintString();

    cout << "**String Comparison**" << endl;
    cout << "Is String 1 equals to String 2? " << str1.operator==(str3) << endl;
    cout << endl;

    cout << "**To test is empty**" << endl;
    cout << "(Set String 3 a empty string)" << endl;
    cout << "Is String 1 empty? " << str1.operator!() << endl;
    cout << "Is String 2 empty? " << str2.operator!() << endl;
    cout << endl;

    cout << "**String length**" << endl;
    cout << "Length of String 1: " << str1.Length() << endl;
    cout << "Length of String 2: " << str2.Length() << endl;
    cout << endl;

    cout << "**String concat**" << endl;
    str3 = str1.Concat(str2);
    cout << "String 1 concat String 2 is: ";
    str3.PrintString();
    str3 = str2.Concat(str1);
    cout << "String 2 concat String 1 is: ";
    str3.PrintString();
    cout << endl;

    cout << "**Substring**" << endl;
    cout << "Enter (start, length): ";
    cin >> x >> y;
    str3 = str1.Substr(x, y);
    str3.PrintString();
    cout << endl;

    cout << "**KMP search**" << endl;
    cout << "The position of string 2 in string 1 is at "
        << str1.Find_KMP(str2) << endl;
    cout << endl;

    cout << "**Boyer-Moore search**" << endl;
    cout << "The position of string 2 in string 1 is at "
        << str1.Find_Boyre_Moore(str2) << endl;

    cout << "**String delete**" << endl;
    cout << "Enter (start, length): ";
    cin >> x >> y;
    str3 = str1.Delete(5, 5);
    str3.PrintString();
    cout << endl;

    cout << "**Delete a char**" << endl;
    cout << "Enter the char you want to delete: ";
    cin >> c;
    str3 = str1.CharDelete(c);
    cout << "String 1 is ";
    str3.PrintString();
    str3 = str2.CharDelete(c);
    cout << "String 2 is ";
    str3.PrintString();
    cout << endl;

    cout << "**ASCII code compare**" << endl;
    cout << str1.Compare(str2) << endl;
    cout << endl;

    return 0;
}

// print string
void String::PrintString() {
    cout << str << endl;
}

// constructor using input string init of length m
String::String(const char *init, int m) {
    str  = init;
    length = m;
}

//equality test
bool String::operator == (String t) {
    return (strcmp(str, t.str) == 0); // strcmp == 0 if str1 equals to str2
}

// empty test, true or false
bool String::operator!() {
    return (length == 0);
}

// get the number of characters of *this
// excluding space
int String::Length() {
    int i = 0;
    int char_count = 0;

    while (str[i] != '\0') {
        if (str[i] != ' ')
           char_count++; 
        i++;
    }

    return char_count;
}

// concatenation with another string t
String String::Concat(String t) {
    char *temp = new char[length + t.length + 1];
    strcpy(temp, str);
    strcat(temp, t.str);
    String C(temp, length + t.length);

    return C;
}

// generate a substring i~i+j-1
String String::Substr(int i, int j) {
    if ((i + j) > length)
        throw "out of bound";

    char *temp = new char[j + 1];
    strncpy(temp, str + i, j);
    temp[j] = '\0';
    String C(temp, j);

    return C;
}

// Return an index i such that pat matches the substring
// of the object begins at position i.  Return -1 if pat
// is empty or not a substring of the object
int String::Find_KMP(String pat) {
    int posP = 0, posS = 0;
    int lengthP = pat.Length(), lengthS = Length();

    pat._BuildFailureFunction();

    while((posP < lengthP) && (posS < lengthS)) {
        if (pat.str[posP] == str[posS]) { 
            posP++; 
            posS++; 
        } else {
            if (posP == 0) 
                posS++;
            else 
                posP = pat.F[posP -1] + 1;
        }
    }
    if (posP < lengthP) 
        return -1;
    else // Textbook stops pattern matching once a match is found
        return posS - lengthP;
}

void String::_BuildFailureFunction()
{
    F = new int[length];
    F[0] = -1;
    int i, j;
    for (j = 1; j < length; j++)
    {
        i = F[j - 1];
        while ((str[j] != str[i + 1]) && (i >= 0))
            // when accumulating but fail, find "its" LPS
            i = F[i];
        if (str[j] == str[i + 1])
            F[j] = i + 1;
        else
            F[j] = -1;
    }
}

// Return an index i such that pat matches the substring
// of the object begins at position i.  Return -1 if pat
// is empty or not a substring of the object
int String::Find_Boyre_Moore(String pat) {
    int i, j, l;
    int lengthP = pat.Length(), lengthS = Length();
    int m = lengthP;
    int n = lengthS;

    pat._BuildLastOccFuntion();

    i = m - 1;
    j = n - 1;
    while (i <= n) {
        if (pat.str[j] == str[i]) {
            if (j == 0)
                return i;
            else {
                i--;
                j--;
            }
        } else {
            l = pat.L[(int)str[i]];
            i = i + lengthP - min(j, 1 + l);
            j = m - 1;
        }
    }
    return -1;
}

// last occurrence function for Boyer-Moore
void String::_BuildLastOccFuntion()
{
    int i;

    L = new int[128]();
    for (i = 0; i < length; i++) {
        L[(int)str[i]] = i;
    }
}

//remove length characters beginning at start
String String::Delete(int start, int len) {
    char *temp = new char[length - len + 1];
    int t = 0; // temp's index
    int i;

    for (i = 0; i < start; i++, t++)
        temp[t] = str[i];

    for (i = start + len; i < length; i++, t++)
        temp[t] = str[i];

    temp[t] = '\0';

    String C(temp, t);

    return C;
}

// returns the string with all occurrence of c removed.
String String::CharDelete(char c) {
    int len = length;
    char *temp = new char[len + 1];
    int i = 0;
    int j = 0;

    for (i = 0; i < length; i++) {
        if (str[i] == c)
            len--;
    }
    for (i = 0; i < length; i++) {
        if (str[i] != c)
            temp[j++] = str[i];
    }
    temp[j] = '\0';

    String C(temp, len);
    return C;
}

// compare two strings of letters of alphabet.
int String::Compare(String y) {
    string s1, s2;
    for (int i = 0; i < length; i++)
        s1 += str[i];
    for (int i = 0; i < y.length; i++)
        s2 += y.str[i];
    if (s1 > s2)
        return 1;
    else if (s1 < s2)
        return -1;
    else
        return 0;
}