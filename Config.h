//this class store user's settings and interract with config.file
#ifndef CONFIG_H
#define CONFIG_H
#include <toml11-master/toml.hpp>
#include <string>
#include <functional>
#include <fstream>
#include <string>
class Config
{
public:
	Config();

	static const std::uintmax_t max_file_size;
	
	void read_from_file(const std::string&);
	void write_config_to_file() const;

	std::uintmax_t get_first_size() const;
	std::uintmax_t get_second_size() const;

	const std::pair<std::function<char(char, char)>, std::string>& get_alg() const;

	void set_first_size(std::uintmax_t fs);
	void set_second_size(std::uintmax_t ss);

	void set_alg(const std::pair<std::function<char(char, char)>, std::string>& a);

private:
	std::pair<std::function<char(char, char)>,std::string> alg;
	std::uintmax_t first_size;
	std::uintmax_t second_size;

	
};
#endif 

