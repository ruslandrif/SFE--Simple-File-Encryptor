#include "Config.h"
#include "Algorithms.h"
Config::Config()  {
	first_size = 0;
	second_size = 0;
	alg = ALGOS::XOR;
}

unsigned long Config::get_first_size() const { return first_size; }

unsigned long Config::get_second_size() const { return second_size; }

std::function<char(char, char)> Config::get_alg() const { return alg.first; }

void Config::set_first_size(unsigned int fs) {
	first_size = (fs <= 10000) ? fs : 10000;
}

void Config::set_second_size(unsigned int ss) {
	second_size = (ss <= 10000) ? ss : 10000;
}

void Config::set_alg(const std::pair<std::function<char(char, char)>,std::string>& a) {
	alg = a; //can be public member
}

void Config::read_from_file(const std::string& filename) {
	auto file_data = toml::parse(filename);

	std::string algorithm = toml::find<std::string>(file_data, "algorithm");

	if (algorithm == "XOR") this->alg = ALGOS::XOR;
	else if (algorithm == "OR") this->alg = ALGOS::OR;
	else if (algorithm == "AND") this->alg = ALGOS::AND;
	else if (algorithm == "CONSTANT ONE") this->alg = ALGOS::CONSTANT_ONE;
	else if (algorithm == "CONSTANT ZERO") this->alg = ALGOS::CONSTANT_ZERO;
	else /*unknown*/ this->alg = ALGOS::XOR;

	first_size = toml::find<unsigned long>(file_data, "FIRST_SIZE");
	second_size = toml::find<unsigned long>(file_data, "SECOND_SIZE");
}

bool Config::compare_std_functions(const std::function<char(char, char)>& first, const std::function<char(char, char)>& second) const {
	return first('a', 'b') == second('a', 'b');
}

void Config::write_config_to_file() const {
	std::fstream config_stream;
	config_stream.open("config.toml", std::fstream::out);
    config_stream << "algorithm = \"" << alg.second << "\"\n";


	config_stream << "FIRST_SIZE = " << first_size << std::endl;
	config_stream << "SECOND_SIZE = " << second_size << std::endl;

	config_stream.close();
}