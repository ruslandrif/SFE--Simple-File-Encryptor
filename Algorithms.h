#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <functional>
#include <map>
namespace ALGOS {
	static std::pair<std::function<char(char, char)>, std::string> XOR = { [](char f, char s) {
		//return ((f ^ s) > 33 && (f ^ s) <= 126) ? (f ^ s) : '1'; //to avoid special characters
		return f ^ s;
	} ,"XOR" };

	static std::pair<std::function<char(char, char)>, std::string> OR = { [](char f, char s) {
		return f | s;
	} ,"OR" };

	static std::pair<std::function<char(char, char)>, std::string> AND = { [](char f, char s) {
		return f & s;
	} ,"AND" };

	static std::pair<std::function<char(char, char)>, std::string> CONSTANT_ONE = { [](char f, char s) {
		return '1';
	} ,"CONSTANT_ONE" };

	static std::pair<std::function<char(char, char)>, std::string> CONSTANT_ZERO = { [](char f, char s) {
		return '0';
	} ,"CONSTANT_ZERO" };

	static std::map<std::string, std::pair<std::function<char(char, char)>, std::string>> string_to_alg = {
		{"XOR",XOR},
		{"OR",OR},
		{"AND",AND},
		{"CONSTANT_ONE",CONSTANT_ONE},
		{"CONSTANT_ZERO",CONSTANT_ZERO}
	};
}
#endif
