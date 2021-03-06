#include "Config.h"


const std::uintmax_t Config::max_file_size = 100000;

Config::Config()  {
	first_size = 100;
	second_size = 100;
	alg = Available_algorithms::XOR;
}



std::uintmax_t Config::get_first_size() const { return first_size; }

std::uintmax_t Config::get_second_size() const { return second_size; }

const Available_algorithms::Algorithm& Config::get_alg() const { return alg; }

void Config::set_first_size(std::uintmax_t fs) {
	first_size = (fs <= max_file_size) ? fs : max_file_size;
}

void Config::set_second_size(std::uintmax_t ss) {
	second_size = (ss <= max_file_size) ? ss : max_file_size;
}

void Config::set_alg(const Available_algorithms::Algorithm& a) {
	alg = a; //can be public member
}

void Config::read_from_file(const std::string& filename) { //parse toml file and read configuration properties
	if (!std::filesystem::exists(filename)) throw std::runtime_error("Config file does not exist");

	auto file_data = toml::parse(filename);

	alg = Available_algorithms::string_to_alg[toml::find<std::string>(file_data, "algorithm")];
	first_size = toml::find<std::uintmax_t>(file_data, "FIRST_SIZE");
	second_size = toml::find<std::uintmax_t>(file_data, "SECOND_SIZE");
}



void Config::write_config_to_file() const { //change config file if settings was changed
	std::fstream config_stream("config.toml", std::fstream::out);
	
    config_stream << "algorithm = \"" << alg.second << "\"\n";
	config_stream << "FIRST_SIZE = " << first_size << std::endl;
	config_stream << "SECOND_SIZE = " << second_size << std::endl;

	config_stream.close();
}