#include "encryptor.h"

encryptor::encryptor() : first(std::filesystem::current_path()),second(std::filesystem::current_path()){
	
}

encryptor::encryptor(const std::filesystem::path& first, const std::filesystem::path& second) noexcept : first(first), second(second) {
	first_read = false;
	second_read = false;
}

void encryptor::start_encrypt() {

	std::fstream first_s;
	std::fstream second_s;

	first_s.open(first.string());
	second_s.open(second.string());
	
	my_bitset = std::vector<int>(std::filesystem::file_size(first) + 1, -1);

	std::thread t1([&]() {
		try {
			_read_file(first);
		}
		catch (...) {

		}
	});
	std::thread t2([&]() {
		try {
			_read_file(second);
		}
		catch (...) {

		}
	});

	t1.join();
	t2.join();

	std::fstream result;
	result.open("Result.bin", std::fstream::out);

	for (auto i : my_bitset) {
		result.put(i);
	}

	result.close();
	my_bitset.clear();
}

void encryptor::_read_file(const std::filesystem::path& f_path) noexcept {
	std::fstream f;
	f.open(f_path.string(),std::fstream::in);
	if (!f.is_open()) throw std::runtime_error("");
	int count = 0;
	while (!f.eof()) {
		std::lock_guard<std::mutex> lg(m);
		if (my_bitset[count] == -1)
			my_bitset[count] = f.get();
		else my_bitset[count] = my_bitset[count] ^ f.get();
		++count;
	
	}


	f.close();
}

void encryptor::set_first_file(const std::filesystem::path& f) {
	first = (std::filesystem::exists(f)) ? f : std::filesystem::current_path();
}

void encryptor::set_second_file(const std::filesystem::path& s) {
	second = (std::filesystem::exists(s)) ? s : std::filesystem::current_path();
}