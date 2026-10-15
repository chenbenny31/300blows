// Apple DV (hack2hire): three SystemVerilog constraint questions.
// (1) a is one-hot without $onehot; (2) every window of 5 consecutive elements of arr5 is
// pairwise distinct; (3) incArr is strictly increasing with adjacent gap >= 2.
class Constraints;
    rand bit [7:0] a;
    rand int unsigned arr5[];     // sized by the caller, e.g. arr5 = new[8]
    rand int unsigned incArr[];   // sized by the caller

    // power of two has a single set bit; a != 0 because 0 & (0 - 1) == 0 too
    constraint one_hot_c {
        a != 0;
        (a & (a - 1)) == 0;
    }

    // the guard skips incomplete windows, so arrays shorter than 5 get no constraint
    constraint unique5_c {
        foreach (arr5[i]) {
            if (i + 4 < arr5.size()) {
                unique {arr5[i], arr5[i + 1], arr5[i + 2], arr5[i + 3], arr5[i + 4]};
            }
        }
    }

    // compare before subtracting so the unsigned difference cannot wrap around
    constraint inc_gap_c {
        foreach (incArr[i]) {
            if (i + 1 < incArr.size()) {
                incArr[i + 1] > incArr[i];
                incArr[i + 1] - incArr[i] >= 2;
            }
        }
    }
endclass
