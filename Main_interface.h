#ifndef MAIN_INT
#define MAIN_INT
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <filesystem>
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem> // C++17 standard header file name
#include <thread>
#include <mutex>
#include <random>

#include "encryptor.h"
#define FIRST_FILE 1
#define SECOND_FILE 2
class Main_interface : public QWidget 
{
public:
	Main_interface(QWidget* parent = nullptr);

	void set_current_directpry_path(const std::filesystem::path& newPath);

	const std::filesystem::path& get_current_dir_path() const { return current_directory_to_choose; }

	void generate_file(const std::string& filename,std::size_t which);

	const std::filesystem::path& get_first_file_path() const { return first_choosen_file; }
	const std::filesystem::path& get_second_file_path() const { return second_choosen_file; }

	void bring_files_to_encryptor(const std::filesystem::path& _first, const std::filesystem::path& _second);

public slots:
	void menu() noexcept;
	void start_program();
	void search_for_files();
	void show_list_of_files();
	void ready_to_encrypt_screen();
private slots:
	
private:
	std::unique_ptr<QPushButton> start;
	std::unique_ptr<QPushButton> exit;

	std::unique_ptr<QPushButton> generate_files;
	std::unique_ptr<QPushButton> choose_from_existing;

	std::unique_ptr<QPushButton> start_encrypt;

	std::unique_ptr<QPushButton> to_root_path;

	std::unique_ptr<QPushButton> to_main_menu;

	std::unique_ptr<QVBoxLayout> menu_layout;
	
	std::vector<std::unique_ptr<QPushButton>> files_to_choose;

	std::filesystem::path current_directory_to_choose;

	std::filesystem::path first_choosen_file;
	std::filesystem::path second_choosen_file;

	encryptor main_encryptor;

	std::unique_ptr<QLabel> test_lbl;

	std::unique_ptr<QLabel> acces_denied;

	std::mt19937 gen1{ time(0) };
	
};

#endif

