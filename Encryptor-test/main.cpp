//file for unit tests
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Tester.h"
#include "Algorithms.h"
#include <iostream>
#include "encryptor.h"

TEST_CASE("User tests", "[tests]") {
    Tester t;
    t.load_tests_from_file("test_cases.csv");
    REQUIRE(t.check_all_tests());

    std::mt19937 gen1;
    std::uniform_int_distribution<> first(1, 1000); //first file's size generator
    std::uniform_int_distribution<> second(1, 1000); //second file's size generator
    std::uniform_int_distribution<> indx(0, 4);  //algorithm generator
    gen1.seed(std::mt19937::default_seed);

    std::vector<Available_algorithms::Algorithm> algorithms_set = { 
        Available_algorithms::XOR,
        Available_algorithms::OR, 
        Available_algorithms::AND, 
        Available_algorithms::CONSTANT_ONE, 
        Available_algorithms::CONSTANT_ZERO, 
    };


    for (int i = 0; i < 10; ++i) { //random tests
        unit_test random_test(first(gen1), second(gen1), algorithms_set[indx(gen1)]);
        try {
            t.create_files_for_test(random_test);
        }
        catch (const std::runtime_error& e) {
            continue;
        }
        std::cout << "Random test. First file size: " << random_test.get_first_size() << ". Second file size: " << random_test.get_second_size() << ". Algorithm = " << random_test.get_alg().second << std::endl;
        REQUIRE(t.check_one_test(random_test));
    }

    t.enc->generate_file_(1000, "Same_file"); //test case for same files
    t.enc->set_first_file(t.find_path_of_the_file("Same_file"));
    t.enc->set_second_file(t.find_path_of_the_file("Same_file"));
    
    unit_test same_files(1000, 1000, Available_algorithms::OR);
    t.enc->set_encrypt_alg(same_files.get_alg());
    t.enc->start_encrypt();
   
    
    REQUIRE(t.check_one_test(same_files));
}


