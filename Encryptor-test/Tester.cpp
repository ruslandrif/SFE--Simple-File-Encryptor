#include "Tester.h"

unit_test::unit_test() {
	first_file_size = 0;
	second_file_size = 0;
	_algorithm = ALGOS::XOR;
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
	std::regex string_format(
		"([\\d]+)"
		"(;)"
		"([\\d]+)"
		"(;)"
		"([\\w]+[\\s\\S]*)"
	);
	std::cmatch result;


	std::fstream f;
	f.open(file_with_test_cases, std::fstream::in);
	if (!f.is_open()) throw std::invalid_argument("incorrect filename");

	std::string current_string;

	while (!f.eof()) {
		f >> current_string;

		std::regex_match(current_string.data(), result, string_format);

		unsigned long first = std::atoi(result[1].str().data());
		unsigned long second = std::atoi(result[3].str().data());

		std::pair<std::function<char(char, char)>, std::string> alg = ALGOS::string_to_alg[result[5].str()];

		tests.push_back(unit_test(first, second, alg));
	}


	f.close();
}

bool Tester::check_one_test(const unit_test& ut) {
	std::fstream first;
	std::fstream second;
	std::fstream result;

	unsigned long max_size = std::max(std::filesystem::file_size("First_file.bin"), std::filesystem::file_size("Second_file.bin"));
	unsigned long min_size = std::min(std::filesystem::file_size("First_file.bin"), std::filesystem::file_size("Second_file.bin"));
	first.open("First_file.bin", std::fstream::in);
	second.open("Second_file.bin", std::fstream::in);
	result.open("Result.bin", std::fstream::in);

	char f;
	char s;
	char r;
	int count_read = 0;

	while (count_read < max_size) {
		f = first.get();
		s = second.get();
		result.read(&r,1);

		if (f == -1) f = 0;
		if (s == -1) s = 0;

		if (ut.get_alg().first(f, s) != r)
			return false;
		count_read++;
	}
	bool ff = (first.eof());
	s = second.get();

	first.close();
	second.close();
	result.close();
	return true;
}

void Tester::create_files_for_test(const unit_test& u) noexcept{
	enc->generate_file_(u.get_first_size(), "First_file.bin");
	enc->generate_file_(u.get_second_size(), "Second_file.bin");
	enc->set_first_file(std::filesystem::path("First_file.bin"));
	enc->set_second_file(std::filesystem::path("Second_file.bin"));
	enc->set_encrypt_alg(u.get_alg());
	enc->start_encrypt();
}

bool Tester::check_all_tests() {
	for (auto test : tests) {

		create_files_for_test(test);
		
		if (!check_one_test(test))
			return false;
	}
	return true;
}