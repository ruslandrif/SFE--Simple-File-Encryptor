#ifndef MAIN_INT
#define MAIN_INT
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include <QComboBox>
#include <QStringList>
#include <QLineEdit>
#include <QFileDialog>
#include <QProgressBar>
#include <QMovie>
#include <toml11-master/toml.hpp>
#include <filesystem>
#include <memory>
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem> // C++17 standard header file name
#include <thread>
#include <mutex>
#include <random>

class Config;
class encryptor;

class Main_interface : public QWidget 
{

public:

	const unsigned FIRST_FILE{1};
	const unsigned SECOND_FILE{2};

	static Config configuration;

	Main_interface(QWidget* parent = nullptr) noexcept;

	void generate_file(const std::string& filename,std::size_t which);


	inline const std::filesystem::path& get_first_file_path() const noexcept;
	inline const std::filesystem::path& get_second_file_path() const noexcept;
	
public slots:
	void menu() noexcept;
	void show_settings() noexcept;
	void start_program() noexcept;
	void search_for_file(unsigned WHICH) noexcept;
	void ready_to_encrypt_screen();
	void final_screen() noexcept;

private slots:
	void encryptor_finished();
private:

	std::unique_ptr<encryptor> main_encryptor;

	std::unique_ptr<QPushButton> start_btn;
	std::unique_ptr<QPushButton> exit_btn;
	std::unique_ptr<QPushButton> settings_btn; //start, exit and settings buttons

	std::unique_ptr<QPushButton> generate_files_btn; //buttons for generate and choose files
	std::unique_ptr<QPushButton> select_first_file_btn;
	std::unique_ptr<QPushButton> select_second_file_btn;

	std::unique_ptr<QPushButton> start_encrypt_btn;
	std::unique_ptr<QPushButton> to_main_menu_btn;
	std::unique_ptr<QPushButton> set_first_size_btn; //settings buttons to change files size
	std::unique_ptr<QPushButton> set_second_size_btn;


	std::unique_ptr<QLabel> processing_lbl;
	std::unique_ptr<QLabel> processing_text_lbl; //labels for showing process of the encryption

	std::unique_ptr<QLabel> files_paths_lbl; //path to choosen files, and result file path 
	std::unique_ptr<QLabel> result_file_path_lbl;

	std::unique_ptr<QLabel> encryption_done_lbl;
	std::unique_ptr<QLabel> encryption_duration_lbl; //labels to show the final screen

	 //error labels
	std::unique_ptr<QLabel> open_file_error_lbl;
	std::unique_ptr<QLabel> incorrect_file_size_error_lbl;

	std::unique_ptr<QLabel> second_file_lbl; //settings labels to say user some message
	std::unique_ptr<QLabel> first_file_lbl;
	std::unique_ptr<QLabel> algorithm_lbl;

	std::unique_ptr<QLineEdit> first_file_size_edit; //line edits to enter files size
	std::unique_ptr<QLineEdit> second_file_size_edit;

	std::unique_ptr<QComboBox> algorithm_choose_box;

	QStringList list_of_algorithms; //list of available algorithms

	std::unique_ptr<QVBoxLayout> menu_layout; //main layout of the interface
	std::unique_ptr<QHBoxLayout> settings_temp_layout;
	std::unique_ptr<QHBoxLayout> first_file_layout;
	std::unique_ptr<QHBoxLayout> second_file_layout;     //layouts for settings
	std::unique_ptr<QHBoxLayout> algorithm_layout;
	
	std::filesystem::path first_choosen_file_path; //paths to choosen files
	std::filesystem::path second_choosen_file_path;

	std::unique_ptr<QProgressBar> encryption_processing_progress; //progress bar to show how many percent of the files is already encrypted

	std::unique_ptr<QMovie> loading_movie;
	
	std::mt19937 main_generator{ time(0) }; //random generator for generating random files

	QString last_first_size_str;
	QString last_second_size_str;

	void set_text_of_choosen_files_path() noexcept; //if one of the file was choosen this function update the path's to the files on the screen
	void change_file_size(unsigned which,std::unique_ptr<QLineEdit>& line_edit_with_size,std::unique_ptr<QPushButton>& button) noexcept;//change file size, if "Save size" button was pressed
	void is_filenames_correct(const std::string& first, const std::string& second); //check if the filenames is correct
	
	std::filesystem::path find_path_of_the_file(const std::string& filename) noexcept; //returns the path to the file with the given filename

	inline bool is_both_files_choosen() const noexcept;
};

#endif

