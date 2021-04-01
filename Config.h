//this class store user's settings and interract with config.file
#ifndef CONFIG_H
#define CONFIG_H
#include <toml11-master/toml.hpp>
#include <string>
#include <functional>
#include <fstream>
#include <string>
#include "Algorithms.h"
class Config
{
public:
	Config();

	static const std::uintmax_t max_file_size; //maximum size of the generated files
	
	void read_from_file(const std::string&); //read and write current configuration
	void write_config_to_file() const;

	std::uintmax_t get_first_size() const;
	std::uintmax_t get_second_size() const;
	const Available_algorithms::Algorithm& get_alg() const;

	void set_first_size(std::uintmax_t fs);
	void set_second_size(std::uintmax_t ss);
	void set_alg(const Available_algorithms::Algorithm& a);

private:
	Available_algorithms::Algorithm alg;
	std::uintmax_t first_size;
	std::uintmax_t second_size;

	
};
#endif 

