//encryption algorithms stored there
#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <functional>
#include <map>
namespace Available_algorithms {

	using Algorithm = std::pair<std::function<char(char, char)>, std::string>; //custom type to avoid long names

	static const Algorithm XOR = { [](char f, char s) {
		return (f ^ s); 
	} ,"XOR" };

	static const Algorithm OR = { [](char f, char s) {
		return (f | s);  
	} ,"OR" };

	static const Algorithm AND = { [](char f, char s) {
		return (f & s);  
	} ,"AND" };

	static const Algorithm CONSTANT_ONE = { [](char f, char s) {
		return '1';
	} ,"CONSTANT_ONE" };

	static const Algorithm CONSTANT_ZERO = { [](char f, char s) {
		return '0';
	} ,"CONSTANT_ZERO" };

	static  std::map<std::string, const Algorithm> string_to_alg = {
		{"XOR",XOR},
		{"OR",OR},
		{"AND",AND},
		{"CONSTANT_ONE",CONSTANT_ONE},
		{"CONSTANT_ZERO",CONSTANT_ZERO}
	};
}
#endif
