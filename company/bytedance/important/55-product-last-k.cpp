// ByteDance / LC 1352: ProductOfNumbers with add(num) (0 <= num <= 100) and getProduct(k)
// = product of the last k added numbers (k never exceeds the count added).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

class ProductOfNumbers {
    // prefix[i] = product of the first i numbers since the last 0
    vector<int> prefix{1};

public:
    // a 0 zeroes every window containing it, so restart the prefixes. T: O(1).
    void add(int num) {
        if (num == 0) {
            prefix = {1};
            return;
        }
        prefix.push_back(prefix.back() * num);
    }

    // prefix ratio. T: O(1).
    int getProduct(int k) {
        int n = prefix.size();
        // fewer than k numbers since the last 0: the window covers that 0
        if (k >= n) { return 0; }
        return prefix.back() / prefix[n - 1 - k];
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ProductOfNumbers p;
    p.add(3);
    p.add(0);
    p.add(2);
    p.add(5);
    p.add(4);
    check(p.getProduct(2) == 20 && p.getProduct(3) == 40, "after zero");
    check(p.getProduct(4) == 0, "covers zero");
    p.add(8);
    check(p.getProduct(2) == 32, "add after query");
}
