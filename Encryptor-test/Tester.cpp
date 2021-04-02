#include "Tester.h"
#include "Algorithms.h"
#include "encryptor.h"
unit_test::unit_test() {
	first_file_size = 0;
	second_file_size = 0;
	_algorithm = Available_algorithms::XOR;
}

unit_test::unit_test(unsigned long f, unsigned long s, const std::pair<std::function<char(char, char)>, std::string>& a) {
	first_file_size = (f <= 1000) ? f : 1000;
	second_file_size = (s <= 1000) ? s : 1000;

	_algorithm = a;
}

unsigned long unit_test::get_first_size() const {
	return first_file_size;
}
unsigned long unit_test::get_second_size() const {
	return second_file_size;
}
std::pair <std::function<char(char, char)>, std::string > unit_test::get_alg() const {
	return _algorithm;
}

Tester::Tester() noexcept {
	enc = std::make_unique<encryptor>();
}

void Tester::load_tests_from_file(const std::string& file_with_test_cases) {
	const std::regex string_format(
		"([\\d]+)"
		"(;)"
		"([\\d]+)"
		"(;)"
		"([\\w]+[\\s\\S]*)" //use regex to parse string with test case
	);
	std::cmatch result;


	std::fstream f(file_with_test_cases, std::fstream::in | std::fstream::binary);
	if (!f.is_open()) throw std::invalid_argument("incorrect filename");

	std::string current_string;

	while (!f.eof()) {
		f >> current_string;

		std::regex_match(current_string.data(), result, string_format); //parse string 

		unsigned long first = std::atoi(result[1].str().data());
		unsigned long second = std::atoi(result[3].str().data());
		                                                           //get info about current unit test
	    Available_algorithms::Algorithm alg = Available_algorithms::string_to_alg[result[5].str()];

		tests.push_back(unit_test(first, second, alg));
	}


	f.close();
}

bool Tester::check_one_test(const unit_test& ut) noexcept{
	std::fstream first(enc->get_first_file_path().string(), std::fstream::in | std::fstream::binary);
	std::fstream second(enc->get_second_file_path().string(), std::fstream::in | std::fstream::binary);
	std::fstream result("Result.bin", std::fstream::in | std::fstream::binary);

	const unsigned long max_size = std::max(std::filesystem::file_size(first_filename), std::filesystem::file_size(second_filename));

	char f{ '\0' };
	char s{ '\0' };
	char r{ '\0' };
	int count_read = 0;

	while (count_read < max_size) {
		first.read(&f, 1);
		second.read(&s, 1);
		result.read(&r,1);

		if (first.eof()) f = 0;
		if (second.eof()) s = 0;

		if (ut.get_alg().first(f, s) != r)   //read both generated files, result file, and check, if the symbol from result is correct
			return false;
		count_read++;
	}
	first.close();
	second.close();
	result.close();
	return true;
}

void Tester::create_files_for_test(const unit_test& u) {
	try {
		enc->generate_file_(u.get_first_size(), first_filename);
		enc->generate_file_(u.get_second_size(), second_filename);
	}
	catch (const std::runtime_error& e) {
		throw std::runtime_error("");
	}
	enc->set_first_file(find_path_of_the_file(first_filename));
	enc->set_second_file(find_path_of_the_file(second_filename));
	enc->set_encrypt_alg(u.get_alg());
	enc->start_encrypt();
}

bool Tester::check_all_tests() noexcept{
	for (auto test : tests) {

		create_files_for_test(test);
		
		if (!check_one_test(test))
			return false;
	}
	return true;
}

std::filesystem::path Tester::find_path_of_the_file(const std::string& filename) {
	using namespace std::filesystem;

	for (const auto& entry : directory_iterator(current_path())) {
		if (entry.path().filename() == filename) {
			return entry.path();
		}
	}

	return current_path();
}