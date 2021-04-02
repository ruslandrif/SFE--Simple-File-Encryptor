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
#include <QObject>

#include "Algorithms.h"
class encryptor : public QObject
{
	Q_OBJECT

public:
	
	explicit encryptor() noexcept;
	encryptor(const std::filesystem::path&, const std::filesystem::path&) noexcept;
	~encryptor();

	void start_encrypt() noexcept;
	void _read_file(const std::filesystem::path& f_path) noexcept;

	void set_first_file(const std::filesystem::path& f) noexcept;
	void set_second_file(const std::filesystem::path& s) noexcept;
	void set_encrypt_alg(const Available_algorithms::Algorithm&) noexcept;

	const std::filesystem::path& get_first_file_path() const noexcept;
	const std::filesystem::path& get_second_file_path() const noexcept;
	std::chrono::duration<float> get_last_encryption_time() const noexcept;

	void generate_file_(unsigned size, const std::string& fname);

signals:
	void encryption_done_signal();
	void update_bar(int);
private:
	std::filesystem::path path_to_first_file;
	std::filesystem::path path_to_second_file;

	std::queue<char> first_file_queue;
	std::queue<char> second_file_queue;

	std::condition_variable first_cv;
	std::condition_variable second_cv;

	std::mutex first_mutex;
	std::mutex second_mutex;

	std::uintmax_t maximum_size{ 0 };
	
	std::chrono::duration<float> encryption_time{ 0 };

	Available_algorithms::Algorithm encryption_alg;

};


#endif

