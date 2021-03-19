#include "encryptor.h"

std::size_t encryptor::written_characters = 0;

encryptor::encryptor() : first(std::filesystem::current_path()),second(std::filesystem::current_path()){
	
}

encryptor::encryptor(const std::filesystem::path& first, const std::filesystem::path& second) noexcept : first(first), second(second) {
	first_read = false;
	second_read = false;
}

void encryptor::start_encrypt() {

	maximum_size = std::max(std::filesystem::file_size(first), std::filesystem::file_size(second));
	
	auto start = std::chrono::high_resolution_clock::now();

	std::thread t1([this]() {
		
		_read_file(first);
		
	});
	std::thread t2([this]() {
		
		_read_file(second);
		
	});


	
	std::thread write_thread([this]() {
		
		std::fstream result;
		result.open("Result.bin", std::fstream::out);
		encryptor::written_characters = 0;
	/*	writing_process->setValue(written);
		writing_process->setRange(0, maximum_size);*/
	
		/*writing_process->show();*/
		while (encryptor::written_characters != maximum_size) {
			std::unique_lock<std::mutex> first_lock(first_m);
			std::unique_lock<std::mutex> second_lock(second_m);

			first_cv.wait(first_lock, [this]() {return !(first_file.empty()); });
			second_cv.wait(second_lock, [this]() {return !(second_file.empty()); });

			result.put(first_file.front() ^ second_file.front());

			first_file.pop();
			second_file.pop();
			++encryptor::written_characters;
			//writing_process->setValue(written_characters);
		}
		/*writing_process->hide();*/
		result.close();
	});

	t1.join();
	t2.join();
	write_thread.join();


	auto end = std::chrono::high_resolution_clock::now();

	encryption_time = end - start;
}

void encryptor::_read_file(const std::filesystem::path& f_path) {
	std::fstream f;
	f.open(f_path.string(),std::fstream::in);
	
	int read = 0;


	while (read < maximum_size) {
		std::lock_guard<std::mutex> lg(((f_path == first) ? first_m : second_m));
		((f_path == first) ? first_file : second_file).push(0);
		((f_path == first) ? first_cv : second_cv).notify_one();
		++read;
	}

	f.close();
}

void encryptor::set_first_file(const std::filesystem::path& f) {
	first = (std::filesystem::exists(f)) ? f : std::filesystem::current_path();
}

void encryptor::set_second_file(const std::filesystem::path& s) {
	second = (std::filesystem::exists(s)) ? s : std::filesystem::current_path();
}