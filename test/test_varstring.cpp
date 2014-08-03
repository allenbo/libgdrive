#include "gdrive/util.hpp"
#include <iostream>
#include <vector>
#include <cassert>

using namespace GDRIVE;

int main()  {
    std::string left = "abc";
    std::string right = "def";
    std::string comb  = left + "-" + right;
    std::vector<std::string> rst = VarString::split(comb, "-");
    assert(rst[0] == left);
    assert(rst[1] == right);
}
