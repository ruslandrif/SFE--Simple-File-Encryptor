#include "encryptor.h"




encryptor::encryptor() noexcept : path_to_first_file(std::filesystem::current_path()), path_to_second_file(std::filesystem::current_path()) {
	
}

encryptor::encryptor(const std::filesystem::path& first, const std::filesystem::path& second) noexcept : path_to_first_file(first), path_to_second_file(second) {

}



void encryptor::start_encrypt() noexcept {

	const bool same_files{ path_to_first_file == path_to_second_file };

	std::uintmax_t written_characters{ 0 };

	auto start = std::chrono::high_resolution_clock::now(); //fix start of the time

	maximum_size = std::max(std::filesystem::file_size(path_to_first_file), std::filesystem::file_size(path_to_second_file)); //choose bigger file size

	const int one_percent_of_encryption = maximum_size / 100;

	std::thread read_thread_1([this]() { _read_file(path_to_first_file); });
	read_thread_1.detach();

	if (!same_files) {
		std::thread read_thread_2([this]() { _read_file(path_to_second_file); });
		read_thread_2.detach();
	}

	char current_encrypted_symbol{ '\0' };

	std::fstream result_file("Result.bin", std::fstream::out | std::fstream::binary);

	
	std::thread write_thread([&]() {

		while (written_characters < maximum_size) {

			std::unique_lock<std::mutex> first_lock(first_mutex); //lock first mutexe
			std::unique_lock<std::mutex> second_lock(second_mutex); //lock second mutex

			first_cv.wait(first_lock, [this]() {return !(first_file_queue.empty()); });
			second_cv.wait(second_lock, [this]() {return !(second_file_queue.empty()); }); //wait while queues is not empty

			current_encrypted_symbol = this->encryption_alg.first(first_file_queue.front(), second_file_queue.front()); //encrypt

			first_file_queue.pop();
			second_file_queue.pop();

			result_file.write(&current_encrypted_symbol, 1);

			++written_characters;

			if (written_characters % one_percent_of_encryption == 0)
				emit update_bar(written_characters / one_percent_of_encryption);

		}
	});

	write_thread.join();

	result_file.close();

	auto end = std::chrono::high_resolution_clock::now(); //fix end of the time

	encryption_time = end - start; //fix time of the encryption

	emit encryption_done_signal();
}

void encryptor::_read_file(const std::filesystem::path& f_path) noexcept {
	const bool same_files = (path_to_first_file == path_to_second_file);

	std::fstream file_to_read(f_path.wstring(), std::fstream::in | std::fstream::binary);

	int read_characters{ 0 }; 
	char current_read_symbol{ 0 };

	while (read_characters < maximum_size) {
		file_to_read.read(&current_read_symbol, 1);

		if (file_to_read.eof())
			current_read_symbol = 0;

		std::lock_guard<std::mutex> lg(((f_path == path_to_first_file) ? first_mutex : second_mutex));

		((f_path == path_to_first_file) ? first_file_queue : second_file_queue).push(current_read_symbol);
		((f_path == path_to_first_file) ? first_cv : second_cv).notify_one();

		if (same_files) { //if files are same, this function will only push symbols to first queue, so need to add same symbol to the second queue
			second_file_queue.push(current_read_symbol);
			second_cv.notify_one();
		}

		++read_characters;
	}

	file_to_read.close();
}

void encryptor::generate_file_(unsigned size, const std::string& fname) {
	std::mt19937 gen1;
	gen1.seed(2000);

	constexpr unsigned up_border{ 122 };
	constexpr unsigned down_border{ 5 };
	constexpr unsigned kylobyte_size_in_bytes{ 1024 };

	//we will use c++ 11 posibilities to generate random numbers
	const std::uniform_int_distribution<> first_distribution(down_border, up_border);  //generate one ascii character number. The big string will be construct like this:
	const std::uniform_int_distribution<> second_distribution(down_border + 5, up_border - 5);							      //for example, number is 35, so the string will be {36,37,38,...,up_border,up_border-1,...}

	

	//const unsigned size_in_kylobytes = (is_first_file_choosen ? configuration.get_first_size() : configuration.get_second_size()); //size of the generated file
	
	unsigned start_symbol = first_distribution(gen1);


	std::fstream newfile(fname, std::fstream::out | std::fstream::binary);

	std::string big_string; //defining out of scope, to avoid reinitialization
	for (unsigned curr_kylobyte = 0; curr_kylobyte < size; ++curr_kylobyte) {
		big_string.clear();
		big_string.reserve(kylobyte_size_in_bytes);

		//fill one kylobyte string
		for (unsigned current_symbol = start_symbol / 2 + 5, additional_value = 1, curr_byte = 0; curr_byte < kylobyte_size_in_bytes; ++curr_byte) {

			if (current_symbol == up_border || current_symbol == down_border)
				additional_value *= -1;  //1 or -1

			big_string.push_back(current_symbol);

			current_symbol += additional_value;
		}
		newfile.write(big_string.data(), big_string.size());
	}
	newfile.close();

}

void encryptor::set_first_file(const std::filesystem::path& f) noexcept {
	path_to_first_file = (std::filesystem::exists(f)) ? f : std::filesystem::current_path();
}

void encryptor::set_second_file(const std::filesystem::path& s) noexcept {
	path_to_second_file = (std::filesystem::exists(s)) ? s : std::filesystem::current_path();
}

void encryptor::set_encrypt_alg(const Available_algorithms::Algorithm& _alg) noexcept {
	encryption_alg = _alg;
}

const std::filesystem::path& encryptor::get_first_file_path() const noexcept { return path_to_first_file; }

const std::filesystem::path& encryptor::get_second_file_path() const noexcept { return path_to_second_file; }

std::chrono::duration<float> encryptor::get_last_encryption_time() const noexcept { return this->encryption_time; }


encryptor::~encryptor() {}