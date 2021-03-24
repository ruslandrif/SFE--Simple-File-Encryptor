#include "encryptor.h"


std::size_t encryptor::written_characters = 0;

encryptor::encryptor() : first(std::filesystem::current_path()), second(std::filesystem::current_path()) {

}

encryptor::encryptor(const std::filesystem::path& first, const std::filesystem::path& second) noexcept : first(first), second(second) {

}

void encryptor::start_encrypt() {


	maximum_size = std::max(std::filesystem::file_size(first), std::filesystem::file_size(second));

	auto start = std::chrono::high_resolution_clock::now();

	std::thread t1([this]() { //launch two threads to read both files

		_read_file(first);

		});
	std::thread t2([this]() {

		_read_file(second);

		});



	std::thread write_thread([&]() {

		std::fstream result;
		result.open("Result.bin", std::fstream::out | std::fstream::binary);
		encryptor::written_characters = 0;
		/*	writing_process->setValue(written);
			writing_process->setRange(0, maximum_size);*/

			/*writing_process->show();*/
		while (encryptor::written_characters < maximum_size) {
			std::unique_lock<std::mutex> first_lock(first_m); //lock both mutexes
			std::unique_lock<std::mutex> second_lock(second_m);

			first_cv.wait(first_lock, [this]() {return !(first_file.empty()); });
			second_cv.wait(second_lock, [this]() {return !(second_file.empty()); });  //wait while queues is not empty (read threads will notify about that)


			char c = this->encryption_alg.first(first_file.front(), second_file.front()); //encrypt
			                                                                   
			result.write(&c, 1);  //write                               //this implementation make queues fully threadsafe

			first_file.pop();
			second_file.pop();  //pop value from both queues
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

	encryption_time = end - start; //fix time of the encryption
}

void encryptor::_read_file(const std::filesystem::path& f_path) {
	std::fstream f;
	f.open(f_path.string(), std::fstream::in | std::fstream::binary);

	int read = 0;

	while (!f.eof()) {
		std::lock_guard<std::mutex> lg(((f_path == first) ? first_m : second_m)); //lock file's mutex
		char curr = '\0';
		f.read(&curr, 1);
		if (!f.eof()) {
			((f_path == first) ? first_file : second_file).push(curr); //push value to queue and notify that queue is not empty now
			((f_path == first) ? first_cv : second_cv).notify_one();
			++read;
		}
	}


	while (read < maximum_size) { //if this file is less size then second
		std::lock_guard<std::mutex> lg(((f_path == first) ? first_m : second_m));
		((f_path == first) ? first_file : second_file).push(0);
		((f_path == first) ? first_cv : second_cv).notify_one();
		++read;
	}

	f.close();
}

void encryptor::generate_file_(unsigned long kylobytes_size, const std::string& name) {
	std::string big_string;
	big_string.reserve(1024);  //string size will be 1 kB

	const int up_border = 122;
	const int down_border = 38;

	std::mt19937 gen1;
	gen1.seed(rand() % 2000);
	
	std::uniform_int_distribution<> uid1(down_border, up_border);  

	std::fstream newfile;
	newfile.open(name, std::fstream::out);

	if (!newfile.is_open()) throw std::invalid_argument("filename is incorrect");



	for (int i = 0; i < kylobytes_size; ++i) {
		big_string.clear();
		big_string.reserve(1024);

		int number = uid1(gen1);
		int additional = 1;
		for (int j = 0; j < 1024; ++j) {
			if (number == up_border)
				additional = -1;
			else if (number == down_border)
				additional = 1;
			big_string.push_back(number);
			number += additional;
		}
		newfile.write(big_string.data(), big_string.size());
	}
	newfile.close();
}

void encryptor::set_first_file(const std::filesystem::path& f) {
	first = (std::filesystem::exists(f)) ? f : std::filesystem::current_path();
}

void encryptor::set_second_file(const std::filesystem::path& s) {
	second = (std::filesystem::exists(s)) ? s : std::filesystem::current_path();
}

const std::filesystem::path& encryptor::get_first_file() const noexcept { return first; }
const std::filesystem::path& encryptor::get_second_file() const noexcept { return second; }

std::chrono::duration<float> encryptor::get_last_encryption_time() const noexcept { return this->encryption_time; }

std::size_t encryptor::get_maximum_file_size() const noexcept { return maximum_size; }

void encryptor::set_encrypt_alg(const std::pair<std::function<char(char, char)>, std::string>& _alg) {
	encryption_alg = _alg;
}

encryptor::~encryptor() {

}