#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <thread>
#include <functional>
#include <mutex>
class encryptor
{
public:
	encryptor();
	encryptor(const std::filesystem::path&, const std::filesystem::path&) noexcept;

	void start_encrypt();

	void _read_file(const std::filesystem::path& f_path) noexcept;
	


	void set_first_file(const std::filesystem::path& f);
	void set_second_file(const std::filesystem::path& s);

	const std::filesystem::path& get_first_file() const noexcept { return first; }
	const std::filesystem::path& get_second_file() const noexcept { return second; }

	


private:
	std::filesystem::path first;
	std::filesystem::path second;

	std::vector<int> my_bitset;
	std::mutex m;



	bool first_read;
	bool second_read;
};
#endif

