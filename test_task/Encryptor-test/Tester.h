//this class provides possibility to test the encryptor
#ifndef TESTER_H
#define TESTER_H
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <functional>
#include <filesystem>
#include <regex>


#include "Algorithms.h"
class encryptor;

class unit_test { //constant unit test
public:
	unit_test();
	unit_test(unsigned long, unsigned long, const std::pair<std::function<char(char, char)>, std::string>& a);

	unsigned long get_first_size() const;
	unsigned long get_second_size() const;
	Available_algorithms::Algorithm get_alg() const;
private:
	unsigned long first_file_size;  //first file
	unsigned long second_file_size; //second file
	Available_algorithms::Algorithm _algorithm; //encryption algorithm
};

class Tester
{
public:
	Tester() noexcept;
	void load_tests_from_file(const std::string& file_with_test_cases);

	bool check_one_test(const unit_test& ut) noexcept; //check files for passing one single test
	bool check_all_tests() noexcept; //check all user's test

	void create_files_for_test(const unit_test& u);

	const std::string first_filename{ "First.bin" };
	const std::string second_filename{ "Second.bin" };

	std::unique_ptr<encryptor> enc; //tested encryptor

	std::filesystem::path find_path_of_the_file(const std::string& filename);
private:
	

	std::vector<unit_test> tests; //user's tests

	
};
#endif 

