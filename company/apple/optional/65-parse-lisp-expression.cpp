// LC 736: evaluate a Lisp-like expression: integers, variables, (add e1 e2),
// (mult e1 e2), (let v1 e1 v2 e2 ... expr). let assigns in order and opens a scope;
// a variable resolves to its innermost binding.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::stoi;

// read up to the next space or ')'
string parseToken(const string& s, int& i) {
    int start = i;
    while (i < (int)s.size() && s[i] != ' ' && s[i] != ')') { i++; }
    return s.substr(start, i - start);
}

// innermost scope first
int lookup(const vector<unordered_map<string, int>>& scopes, const string& name) {
    for (int d = scopes.size() - 1; d >= 0; d--) {
        auto it = scopes[d].find(name);
        if (it != scopes[d].end()) { return it->second; }
    }
    return 0;
}

// recursive descent; i moves past the expression that starts at s[i].
// T: O(n^2) (lookups walk the scope depth), S: O(n).
int eval(const string& s, int& i, vector<unordered_map<string, int>>& scopes) {
    if (s[i] != '(') {
        string tok = parseToken(s, i);
        if (tok[0] == '-' || (tok[0] >= '0' && tok[0] <= '9')) { return stoi(tok); }
        return lookup(scopes, tok);
    }
    i++;   // '('
    string op = parseToken(s, i);
    i++;   // ' '
    int result = 0;
    if (op == "let") {
        scopes.push_back({});
        while (true) {
            if (s[i] == '(') {
                result = eval(s, i, scopes);
                break;
            }
            int start = i;
            string name = parseToken(s, i);
            // a token right before ')' is the final expression, not a variable to bind
            if (s[i] == ')') {
                i = start;
                result = eval(s, i, scopes);
                break;
            }
            i++;   // ' '
            scopes.back()[name] = eval(s, i, scopes);
            i++;   // ' '
        }
        scopes.pop_back();
    } else {
        int a = eval(s, i, scopes);
        i++;   // ' '
        int b = eval(s, i, scopes);
        result = op == "add" ? a + b : a * b;
    }
    i++;   // ')'
    return result;
}

int evaluate(const string& expression) {
    int i = 0;
    vector<unordered_map<string, int>> scopes;   // scopes.back() = innermost let
    return eval(expression, i, scopes);
}

void test(const string& label, const string& expression, int want) {
    cout << label << (evaluate(expression) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("nested scope", "(let x 2 (mult x (let x 3 y 4 (add x y))))", 14);
    test("reassign", "(let x 1 y 2 x (add x y) (add x y))", 5);
    test("var result", "(let x 3 x 2 x)", 2);
    test("negative", "(add 1 -2)", -1);
}
