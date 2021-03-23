#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H
#include <string>
#include <random>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <thread>
#include <functional>
#include <mutex>
#include <queue>
#include <chrono>

class encryptor
{
public:
	static std::size_t written_characters;

	explicit encryptor();
	~encryptor();
	encryptor(const std::filesystem::path&, const std::filesystem::path&) noexcept;

	void start_encrypt();

	void _read_file(const std::filesystem::path& f_path);

	std::size_t get_maximum_file_size() const noexcept;

	void set_first_file(const std::filesystem::path& f);
	void set_second_file(const std::filesystem::path& s);

	void set_encrypt_alg(const std::pair<std::function<char(char, char)>, std::string>&);

	const std::filesystem::path& get_first_file() const noexcept;
	const std::filesystem::path& get_second_file() const noexcept;

	void generate_file_(unsigned long kylobytes_size, const std::string& name);

	std::chrono::duration<float> get_last_encryption_time() const noexcept;

private:
	std::filesystem::path first;
	std::filesystem::path second;

	std::queue<char> first_file;
	std::queue<char> second_file;
	std::mutex first_m;
	std::mutex second_m;

	std::size_t maximum_size{ 0 };

	std::chrono::duration<float> encryption_time{ 0 };


	std::condition_variable first_cv;
	std::condition_variable second_cv;

	std::pair<std::function<char(char, char)>, std::string> encryption_alg;
};
#endif

