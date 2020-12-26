#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Tests/tests.h"
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
//    fefu::hash_map<int, int> a;
//    for(int i = 1; i < 5000; i++) {
//        a.insert({i * 3, i * 4});
//    }
//    std::cout << a.contains(3) << endl;
//    std::cout << a.contains(12000) << endl;
//    for(int i = 0; i < 5000; i++) {
//        a.erase(i*3);
//    }
//    std::cout << a.contains(3) << endl;
//    std::cout << a.contains(12000) << endl;
//    std::cout << a.size() << endl;

    return 0;
}
