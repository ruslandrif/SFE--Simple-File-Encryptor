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

	
	void read_from_file(const std::string&);
	void write_config_to_file() const;

	unsigned long get_first_size() const;
	unsigned long get_second_size() const;

	std::function<char(char, char)> get_alg() const;

	void set_first_size(unsigned int fs);
	void set_second_size(unsigned int ss);

	void set_alg(const std::pair<std::function<char(char, char)>, std::string>& a);

private:
	std::pair<std::function<char(char, char)>,std::string> alg;
	unsigned long first_size;
	unsigned long second_size;

	bool compare_std_functions(const std::function<char(char, char)>& first, const std::function<char(char, char)>& second) const;
};
#endif 

