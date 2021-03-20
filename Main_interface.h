#ifndef MAIN_INT
#define MAIN_INT
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStringList>
#include <QLineEdit>
#include <toml11-master/toml.hpp>
#include <filesystem>
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem> // C++17 standard header file name
#include <thread>
#include <mutex>
#include <random>
#include "Config.h"
class encryptor;


#define FIRST_FILE 1


#define SECOND_FILE 2

class Main_interface : public QWidget 
{
public:

	static Config configuration;

	Main_interface(QWidget* parent = nullptr);

	void set_current_directpry_path(const std::filesystem::path& newPath);

	const std::filesystem::path& get_current_dir_path() const;

	void generate_file(const std::string& filename,std::size_t which);

	const std::filesystem::path& get_first_file_path() const;
	const std::filesystem::path& get_second_file_path() const;

	

public slots:
	void menu() noexcept;
	void show_settings() noexcept;
	void start_program();
	void search_for_files();
	void show_list_of_files();
	void ready_to_encrypt_screen();
	void final_screen();
private slots:
	
private:
	std::unique_ptr<QPushButton> start;
	std::unique_ptr<QPushButton> exit;
	std::unique_ptr<QPushButton> settings;

	std::unique_ptr<QLineEdit> first_file_size;
	std::unique_ptr<QLineEdit> second_file_size;

	std::unique_ptr<QComboBox> algorithm;

	QStringList list_of_algorithms;

	std::unique_ptr<QPushButton> generate_files;
	std::unique_ptr<QPushButton> choose_from_existing;

	std::unique_ptr<QPushButton> start_encrypt;

	std::unique_ptr<QPushButton> to_root_path;
	std::unique_ptr<QPushButton> to_main_menu;

	std::unique_ptr<QVBoxLayout> menu_layout;

	std::unique_ptr<QHBoxLayout> settings_temp_layout;
	std::unique_ptr<QHBoxLayout> first_file_layout;
	std::unique_ptr<QHBoxLayout> second_file_layout;
	std::unique_ptr<QHBoxLayout> algorithm_layout;
	
	std::vector<std::unique_ptr<QPushButton>> files_to_choose;

	std::filesystem::path current_directory_to_choose;
	std::filesystem::path first_choosen_file;
	std::filesystem::path second_choosen_file;

	std::shared_ptr<QLabel> processing;

	std::unique_ptr<encryptor> main_encryptor;

	std::unique_ptr<QLabel> files_paths;
	std::unique_ptr<QLabel> result_file_path_lbl;
	std::unique_ptr<QLabel> encryption_done;
	std::unique_ptr<QLabel> encryption_duration;
	std::unique_ptr<QLabel> acces_denied;

	std::unique_ptr<QLabel> second_file_lbl;
	std::unique_ptr<QLabel> first_file_lbl;
	std::unique_ptr<QLabel> algorithm_lbl;

	std::mt19937 gen1{ time(0) };
	
};

#endif

