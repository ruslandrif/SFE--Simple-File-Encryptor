//file for unit tests
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Tester.h"
#include "Algorithms.h"
#include <iostream>

unsigned int Factorial(unsigned int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
    Tester t;
    t.load_tests_from_file("test_cases.csv");
    REQUIRE(t.check_all_tests());

    std::mt19937 gen1;
    std::uniform_int_distribution<> first(1, 1000); //first file's size generator
    std::uniform_int_distribution<> second(1, 1000); //second file's size generator
    std::uniform_int_distribution<> indx(0, 4);  //algorithm generator
    std::vector< std::pair<std::function<char(char, char)>, std::string>> algorithms_set = { ALGOS::XOR,ALGOS::OR, ALGOS::AND, ALGOS::CONSTANT_ONE, ALGOS::CONSTANT_ZERO, };


    for (int i = 0; i < 10; ++i) {
        unit_test random_test(first(gen1), second(gen1), algorithms_set[indx(gen1)]);
        t.create_files_for_test(random_test);
        std::cout << "Random test. First file size: " << random_test.get_first_size() << ". Second file size: " << random_test.get_second_size() << ". ALgorithm = " << random_test.get_alg().second << std::endl;
        REQUIRE(t.check_one_test(random_test));
    }
}


