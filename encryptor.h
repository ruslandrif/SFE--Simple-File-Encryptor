#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <thread>
#include <functional>
#include <mutex>
#include <queue>
#include <QProgressBar>
#include <QLabel>
#include <chrono>
class encryptor
{
public:
	static std::size_t written_characters;

	explicit encryptor();
	encryptor(const std::filesystem::path&, const std::filesystem::path&) noexcept;

	void start_encrypt();

	void _read_file(const std::filesystem::path& f_path);
	
	std::size_t get_maximum_file_size() const noexcept { return maximum_size; }

	void set_first_file(const std::filesystem::path& f);
	void set_second_file(const std::filesystem::path& s);

	const std::filesystem::path& get_first_file() const noexcept { return first; }
	const std::filesystem::path& get_second_file() const noexcept { return second; }

	
	std::chrono::duration<float> get_last_encryption_time() { return encryption_time; }
	
private:
	std::filesystem::path first;
	std::filesystem::path second;

	std::queue<char> first_file;
	std::queue<char> second_file;
	std::mutex first_m;
	std::mutex second_m;

	std::size_t maximum_size{0};

	std::chrono::duration<float> encryption_time{0};
	

	std::condition_variable first_cv;
	std::condition_variable second_cv;

	bool first_read;
	bool second_read;
};
#endif

