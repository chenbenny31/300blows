// Seen in interview: commands run on a value starting at 0.
// "ADD x" / "MUL x" update the value; "FUN name" ... "END" defines a function (body is
// stored, not run); "INV name" runs that body. Bodies may INV other functions.
// API: int run(cmds, result), no globals / members.
// Follow-up: detect infinite recursion (a function that ends up invoking itself).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::stoi;
using std::runtime_error;

// runs cmds on result and returns it; funcs is shared by every level of recursion.
// T: O(L + executed commands), S: O(L + call depth); L = total lines.
int exec(const vector<string>& cmds, int result,
         unordered_map<string, vector<string>>& funcs) {
    int n = cmds.size();
    for (int i = 0; i < n; i++) {
        int sp = cmds[i].find(' ');
        string op = cmds[i].substr(0, sp);
        string arg = cmds[i].substr(sp + 1);
        if (op == "FUN") {
            vector<string> body;
            // copy lines up to the matching END; i ends on END
            for (i++; cmds[i] != "END"; i++) { body.push_back(cmds[i]); }
            funcs[arg] = body;
        } else if (op == "INV") {
            // looked up at call time, so a body may call a function defined later
            if (funcs.count(arg) == 0) { throw runtime_error("undefined " + arg); }
            result = exec(funcs[arg], result, funcs);
        } else if (op == "ADD") {
            result += stoi(arg);
        } else if (op == "MUL") {
            result *= stoi(arg);
        }
    }
    return result;
}

// the required API: the function table lives in this call's frame, not in a global
int run(const vector<string>& cmds, int result) {
    unordered_map<string, vector<string>> funcs;   // name -> body commands in order
    return exec(cmds, result, funcs);
}

// follow-up: active = functions on the current call stack; INV of one never ends
int execSafe(const vector<string>& cmds, int result,
             unordered_map<string, vector<string>>& funcs,
             unordered_set<string>& active) {
    int n = cmds.size();
    for (int i = 0; i < n; i++) {
        int sp = cmds[i].find(' ');
        string op = cmds[i].substr(0, sp);
        string arg = cmds[i].substr(sp + 1);
        if (op == "FUN") {
            vector<string> body;
            for (i++; cmds[i] != "END"; i++) { body.push_back(cmds[i]); }
            funcs[arg] = body;
        } else if (op == "INV") {
            if (funcs.count(arg) == 0) { throw runtime_error("undefined " + arg); }
            if (active.count(arg)) { throw runtime_error("recursion in " + arg); }
            active.insert(arg);
            result = execSafe(funcs[arg], result, funcs, active);
            active.erase(arg);
        } else if (op == "ADD") {
            result += stoi(arg);
        } else if (op == "MUL") {
            result *= stoi(arg);
        }
    }
    return result;
}

int runSafe(const vector<string>& cmds, int result) {
    unordered_map<string, vector<string>> funcs;   // name -> body commands in order
    unordered_set<string> active;                  // functions being executed right now
    return execSafe(cmds, result, funcs, active);
}

void test(const string& label, const vector<string>& cmds, int want) {
    cout << label << (run(cmds, 0) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {"ADD 2", "MUL 3"}, 6);
    test("call twice", {"FUN f", "ADD 1", "MUL 2", "END", "ADD 1", "INV f", "INV f"}, 10);
    test("nested call", {"FUN a", "ADD 1", "END", "FUN b", "INV a", "MUL 3", "END",
                         "INV b"}, 3);
    test("late binding", {"FUN b", "INV a", "END", "FUN a", "ADD 5", "END", "INV b"}, 5);
}
