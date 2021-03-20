#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <functional>

namespace ALGOS {
	static std::pair<std::function<char(char, char)>, std::string> XOR = { [](char f, char s) {
		return f ^ s;
	} ,"XOR" };

	static std::pair<std::function<char(char, char)>, std::string> OR = { [](char f, char s) {
		return f ^ s;
	} ,"OR" };

	static std::pair<std::function<char(char, char)>, std::string> AND = { [](char f, char s) {
		return f ^ s;
	} ,"AND" };

	static std::pair<std::function<char(char, char)>, std::string> CONSTANT_ONE = { [](char f, char s) {
		return f ^ s;
	} ,"CONSTANT ONE" };

	static std::pair<std::function<char(char, char)>, std::string> CONSTANT_ZERO = { [](char f, char s) {
		return f ^ s;
	} ,"CONSTANT ZERO" };
}
#endif
