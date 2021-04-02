#include "Main_interface.h"
#include "encryptor.h"
#include "Algorithms.h"
#include "Config.h"
Config Main_interface::configuration = Config(); //default config

Main_interface::Main_interface(QWidget* parent) noexcept : QWidget(parent) {

	main_encryptor = std::make_unique<encryptor>();

	try {
		configuration.read_from_file("config.toml");  //read configuration from input file
	}
	catch (const std::runtime_error& e) {
		configuration.write_config_to_file();
	}
	
	last_first_size_str = std::to_string(configuration.get_first_size()).data();
	last_second_size_str = std::to_string(configuration.get_second_size()).data();
	
	start_btn = std::make_unique<QPushButton>(this);
	start_btn->setIcon(QIcon("icons\\start.png"));   //start button
	start_btn->setText("Start");

	exit_btn = std::make_unique<QPushButton>(this); //exit button
	exit_btn->setText("Exit");
	exit_btn->setIcon(QIcon("icons\\exit.png"));

	settings_btn = std::make_unique<QPushButton>(this);   //settings button
	settings_btn->setText("Settings");
	settings_btn->setIcon(QIcon("icons//settings.png"));

	generate_files_btn = std::make_unique<QPushButton>(this);  //button to generate files
	generate_files_btn->setText("Generate big files");
	generate_files_btn->hide();

	set_first_size_btn = std::make_unique<QPushButton>(this);
	set_first_size_btn->setText("Save size");
	set_first_size_btn->hide();

	set_second_size_btn = std::make_unique<QPushButton>(this);
	set_second_size_btn->setText("Save size");
	set_second_size_btn->hide();

	processing_lbl = std::make_unique<QLabel>(this);    //label to show that files is encrypting now
	loading_movie = std::make_unique<QMovie>("icons\\loading.gif");
	processing_lbl->setMovie(loading_movie.get());
	processing_lbl->setAlignment(Qt::AlignCenter);
	processing_lbl->hide();

	encryption_processing_progress = std::make_unique<QProgressBar>(this);
	encryption_processing_progress->setRange(0, 100);
	encryption_processing_progress->setValue(0);
	encryption_processing_progress->hide();

	processing_text_lbl = std::make_unique<QLabel>(this);
	processing_text_lbl->setText("Encryption in process. Do not close the program!");

	processing_text_lbl->setAlignment(Qt::AlignCenter);
	processing_text_lbl->hide();

	open_file_error_lbl = std::make_unique<QLabel>("Error opening file. Reselect one of the file.");
	open_file_error_lbl->setAlignment(Qt::AlignCenter);
	open_file_error_lbl->hide();

	incorrect_file_size_error_lbl = std::make_unique<QLabel>("You entered invalid characters! Only positive numbers are allowed.");
	incorrect_file_size_error_lbl->setAlignment(Qt::AlignCenter);
	incorrect_file_size_error_lbl->setGeometry(this->height() / 2, this->width() / 2, 400, 200);
	incorrect_file_size_error_lbl->hide();

	first_file_lbl = std::make_unique<QLabel>("Choose first file size in kylobytes: ", this);
	first_file_lbl->setAlignment(Qt::AlignCenter);
	first_file_lbl->hide();

	second_file_lbl = std::make_unique<QLabel>("Choose second file size in kylobytes: ", this);
	second_file_lbl->setAlignment(Qt::AlignCenter);                        //some labels for settings
	second_file_lbl->hide();
	
	algorithm_lbl = std::make_unique<QLabel>("Choose algorithm: ", this);
	algorithm_lbl->setAlignment(Qt::AlignCenter);
	algorithm_lbl->hide();

	result_file_path_lbl = std::make_unique<QLabel>(this);
	result_file_path_lbl->hide();

	encryption_done_lbl = std::make_unique<QLabel>(this);
	encryption_done_lbl->setText("Encryption was done succesfully");
	encryption_done_lbl->setAlignment(Qt::AlignCenter);
	encryption_done_lbl->hide();

	encryption_duration_lbl = std::make_unique<QLabel>(this);
	encryption_duration_lbl->hide();

	select_first_file_btn = std::make_unique<QPushButton>(this);
	select_first_file_btn->setText("Select first file");
	select_first_file_btn->hide();

	select_second_file_btn = std::make_unique<QPushButton>(this);
	select_second_file_btn->setText("Select second file");
	select_second_file_btn->hide();


	to_main_menu_btn = std::make_unique<QPushButton>(this);
	to_main_menu_btn->setText("Return to main menu");
	to_main_menu_btn->setIcon(QIcon("icons\\menu.png"));
	to_main_menu_btn->hide();

	first_file_size_edit = std::make_unique<QLineEdit>(this);
	first_file_size_edit->setValidator(new QIntValidator(1,Config::max_file_size,this));
	first_file_size_edit->hide();

	second_file_size_edit = std::make_unique<QLineEdit>(this);
	second_file_size_edit->setValidator(new QIntValidator(1, Config::max_file_size, this));
	second_file_size_edit->hide();

	algorithm_choose_box = std::make_unique<QComboBox>(this);
	list_of_algorithms = QStringList({ "XOR","OR","AND","CONSTANT_ONE","CONSTANT_ZERO" });
	algorithm_choose_box->addItems(list_of_algorithms);
	algorithm_choose_box->hide();

	menu_layout = std::make_unique<QVBoxLayout>(this);  //main layout
	menu_layout->setSpacing(10);

	first_file_layout = std::make_unique<QHBoxLayout>();
	second_file_layout = std::make_unique<QHBoxLayout>();
	algorithm_layout = std::make_unique<QHBoxLayout>();

	settings_temp_layout = std::make_unique<QHBoxLayout>();
	settings_temp_layout->setSpacing(10);

	first_choosen_file_path = second_choosen_file_path = std::filesystem::current_path();

	setFixedSize(1280, 720);

	setLayout(menu_layout.get());

	setWindowTitle("File encryptor");

	files_paths_lbl = std::make_unique<QLabel>(this);
	files_paths_lbl->setText("First file: None\nSecond file: None.");  //labels with info about generated files
	files_paths_lbl->setAlignment(Qt::AlignCenter);
	files_paths_lbl->hide();

	start_encrypt_btn = std::make_unique<QPushButton>(this);
	start_encrypt_btn->setText("Start encrypt choosen files");
	start_encrypt_btn->hide();
	
	

	
	//if text in the file size field was changed
	connect(first_file_size_edit.get(), &QLineEdit::textChanged, this, [this](const QString& s) {
		set_first_size_btn->setEnabled(s != last_first_size_str);
	});

	connect(second_file_size_edit.get(), &QLineEdit::textChanged, this, [this](const QString& s) {
		set_second_size_btn->setEnabled(s != last_second_size_str);
	});

	//if files size was changed
	connect(set_first_size_btn.get(), &QPushButton::clicked, this, [this]() {
		change_file_size(FIRST_FILE, first_file_size_edit, set_first_size_btn);
		last_first_size_str = first_file_size_edit->text();
	});

	connect(set_second_size_btn.get(), &QPushButton::clicked, this, [this]() {
		change_file_size(SECOND_FILE, second_file_size_edit, set_second_size_btn);
		last_second_size_str = second_file_size_edit->text();
	});

	//if algorithm was changed
	connect(algorithm_choose_box.get(), static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated), this, [&](const QString& s) {
		Main_interface::configuration.set_alg(Available_algorithms::string_to_alg[s.toStdString()]);

		Main_interface::configuration.write_config_to_file();
	});

	connect(to_main_menu_btn.get(), &QPushButton::clicked, this, &Main_interface::menu);

	connect(start_btn.get(), &QPushButton::clicked, this, [this]() {
		if (!(is_both_files_choosen())) { //if files are not choosen
			this->start_program();
		}
		else 
			ready_to_encrypt_screen(); //if files is already choosen
	});

	connect(settings_btn.get(), &QPushButton::clicked, this, &Main_interface::show_settings);

	connect(exit_btn.get(), &QPushButton::clicked, qApp, &QApplication::quit);

	//choose files from explorer
	connect(select_first_file_btn.get(), &QPushButton::clicked, this, [this]() {
		search_for_file(FIRST_FILE);
	});

	connect(select_second_file_btn.get(), &QPushButton::clicked, this, [this]() {
		search_for_file(SECOND_FILE);
	});

	//generate files
	connect(generate_files_btn.get(), &QPushButton::clicked, this, [this]() {
		std::thread t1([this]() {this->generate_file("First_file.bin", FIRST_FILE); });
		std::thread t2([this]() {this->generate_file("Second_file.bin", SECOND_FILE); });  //generate two files
		t1.join();
		t2.join();

		generate_files_btn->setText("Regenerate files");
		try {
			ready_to_encrypt_screen();
		}
		catch (const std::string& e) { //from ready_to_encrypt
			open_file_error_lbl->show();
		}
	});

	connect(start_encrypt_btn.get(), &QPushButton::clicked, this, [this]() {
		
		menu_layout->removeWidget(files_paths_lbl.get());
		menu_layout->removeWidget(start_encrypt_btn.get());
		menu_layout->removeWidget(select_first_file_btn.get());
		menu_layout->removeWidget(select_second_file_btn.get());
		menu_layout->removeWidget(generate_files_btn.get());

		generate_files_btn->hide();
		files_paths_lbl->hide();
		start_encrypt_btn->hide();
		select_first_file_btn->hide();
		select_second_file_btn->hide();
		exit_btn->hide();
		to_main_menu_btn->hide();

		menu_layout->insertWidget(0, processing_lbl.get());
		menu_layout->insertWidget(0, encryption_processing_progress.get());
		menu_layout->insertWidget(0, processing_text_lbl.get());

		encryption_processing_progress->setValue(0);
		encryption_processing_progress->show();
		processing_text_lbl->show();
		loading_movie->start();
		processing_lbl->show();

		std::thread encrypt_thread([this]() {main_encryptor->start_encrypt(); });
		encrypt_thread.detach();
	});

	connect(main_encryptor.get(), &encryptor::update_bar, encryption_processing_progress.get(), &QProgressBar::setValue);

	connect(main_encryptor.get(), &encryptor::encryption_done_signal, this, &Main_interface::encryptor_finished);

	menu();
}

inline const std::filesystem::path& Main_interface::get_first_file_path() const noexcept{ return first_choosen_file_path; }
inline const std::filesystem::path& Main_interface::get_second_file_path() const noexcept{ return second_choosen_file_path; }


void Main_interface::menu() noexcept {

	menu_layout->addWidget(start_btn.get());
	menu_layout->addWidget(settings_btn.get());
	menu_layout->addWidget(exit_btn.get());

	settings_btn->show();
	start_btn->show();
	exit_btn->show();

	menu_layout->removeWidget(generate_files_btn.get());
	menu_layout->removeWidget(select_first_file_btn.get());
	menu_layout->removeWidget(select_second_file_btn.get());
	menu_layout->removeWidget(to_main_menu_btn.get());
	menu_layout->removeWidget(files_paths_lbl.get());
	menu_layout->removeWidget(start_encrypt_btn.get());
	menu_layout->removeWidget(result_file_path_lbl.get());
	menu_layout->removeWidget(encryption_done_lbl.get());
	menu_layout->removeWidget(encryption_duration_lbl.get());
	menu_layout->removeItem(first_file_layout.get());
	menu_layout->removeItem(second_file_layout.get());
	menu_layout->removeItem(algorithm_layout.get());

	algorithm_layout->removeWidget(algorithm_choose_box.get());
	algorithm_layout->removeWidget(algorithm_lbl.get());


	first_file_layout->removeWidget(first_file_size_edit.get());
	first_file_layout->removeWidget(first_file_lbl.get());
	first_file_layout->removeWidget(set_first_size_btn.get());

	second_file_layout->removeWidget(second_file_size_edit.get());
	second_file_layout->removeWidget(second_file_lbl.get());
	second_file_layout->removeWidget(set_second_size_btn.get());

	first_file_size_edit->hide();
	first_file_lbl->hide();
	set_first_size_btn->hide();
	algorithm_choose_box->hide();
	algorithm_lbl->hide();
	second_file_size_edit->hide();
	second_file_lbl->hide();
	set_second_size_btn->hide();
	encryption_duration_lbl->hide();
	encryption_done_lbl->hide();
	result_file_path_lbl->hide();
	start_encrypt_btn->hide();
	files_paths_lbl->hide();
	generate_files_btn->hide();
	select_first_file_btn->hide();
	select_second_file_btn->hide();
	to_main_menu_btn->hide();
	
	
}

void Main_interface::start_program() noexcept {
	start_btn->hide();
	settings_btn->hide();

	menu_layout->removeWidget(start_btn.get());
	menu_layout->removeWidget(settings_btn.get());


	menu_layout->insertWidget(0, generate_files_btn.get());
	menu_layout->insertWidget(0, to_main_menu_btn.get());
	menu_layout->insertWidget(0, select_second_file_btn.get());
	menu_layout->insertWidget(0, select_first_file_btn.get());
	menu_layout->insertWidget(3, files_paths_lbl.get());
	menu_layout->insertWidget(2, files_paths_lbl.get());

	generate_files_btn->show();
	to_main_menu_btn->show();
	select_second_file_btn->show();
	select_first_file_btn->show();
	files_paths_lbl->show();

}




void Main_interface::search_for_file(unsigned Which) noexcept { //function to choose files from root directory

	const auto root_path_ = std::filesystem::current_path().root_directory();
	const bool is_first_file_choosen = (Which == FIRST_FILE);

	//choose file from explorer
	std::wstring fname{ QFileDialog::getOpenFileName(this, "Choose file", root_path_.string().data(), "*.*").toStdWString() };
	
	if (is_first_file_choosen) {
		first_choosen_file_path = std::filesystem::path(fname);
		select_first_file_btn->setText("Reselect first file");
		main_encryptor->set_first_file(first_choosen_file_path);
	}
	else {
		second_choosen_file_path = std::filesystem::path(fname);
		select_second_file_btn->setText("Reselect second file");
		main_encryptor->set_second_file(second_choosen_file_path);
	}

	set_text_of_choosen_files_path();
	
	//if both files are choosen
	if (is_both_files_choosen()) {
		try {
			ready_to_encrypt_screen();
		}
		catch (const std::string& s) { //if there is problem with opening files
			open_file_error_lbl->show();
			if (s == first_choosen_file_path.string())
				open_file_error_lbl->setText(std::string(first_choosen_file_path.string() + " is not opened").data());
			else if(s == second_choosen_file_path.string())
				open_file_error_lbl->setText(std::string(second_choosen_file_path.string() + " is not opened").data());
			else 
				open_file_error_lbl->setText(std::string(s + " files are not opened").data());
		}
	}
}

void Main_interface::ready_to_encrypt_screen() {

	try {
		is_filenames_correct(first_choosen_file_path.string(), second_choosen_file_path.string());
	}
	catch (const std::string& s) {
		throw s;
	}

	menu_layout->removeWidget(select_first_file_btn.get());
	menu_layout->removeWidget(select_second_file_btn.get());
	menu_layout->removeWidget(start_btn.get());
	menu_layout->removeWidget(settings_btn.get());

	start_btn->hide();
	settings_btn->hide();

	menu_layout->insertWidget(0, start_encrypt_btn.get());
	menu_layout->insertWidget(1, generate_files_btn.get());
	menu_layout->insertWidget(1, files_paths_lbl.get());
	menu_layout->insertWidget(2, to_main_menu_btn.get());
	menu_layout->insertWidget(3, select_second_file_btn.get());
	menu_layout->insertWidget(3, select_first_file_btn.get());

	generate_files_btn->show();
	start_encrypt_btn->show();
	files_paths_lbl->show();
	select_first_file_btn->show();
	select_second_file_btn->show();
	to_main_menu_btn->show();

	select_first_file_btn->setText("Reselect first file");
	select_second_file_btn->setText("Reselect second file");
	
	set_text_of_choosen_files_path();

	main_encryptor->set_encrypt_alg(configuration.get_alg());

}

void Main_interface::final_screen() noexcept{
	menu_layout->insertWidget(0, encryption_done_lbl.get());
	menu_layout->insertWidget(1, result_file_path_lbl.get());
	menu_layout->insertWidget(2, encryption_duration_lbl.get());

	encryption_done_lbl->setText("Encryption was ended succesfully!");
	encryption_done_lbl->setAlignment(Qt::AlignCenter);
	encryption_done_lbl->show();

	std::filesystem::path result_file_path{ find_path_of_the_file("Result.bin")};
	
	result_file_path_lbl->setText("Path to result file: " + QString(result_file_path.string().data()));
	result_file_path_lbl->setAlignment(Qt::AlignCenter);
	result_file_path_lbl->show();

	encryption_duration_lbl->setText("Last encryption time: " + QString(std::to_string(main_encryptor->get_last_encryption_time().count()).data()) + " seconds");
	encryption_duration_lbl->setAlignment(Qt::AlignCenter);
	encryption_duration_lbl->show();
}

void Main_interface::encryptor_finished() {

	menu_layout->removeWidget(processing_lbl.get());
	menu_layout->removeWidget(processing_text_lbl.get());
	menu_layout->removeWidget(encryption_processing_progress.get());

	processing_lbl->hide();
	encryption_processing_progress->hide();
	processing_text_lbl->hide();
	
	to_main_menu_btn->show();
	exit_btn->show();
	final_screen();
}

void Main_interface::generate_file(const std::string& filename,std::size_t which)  {
	main_generator.seed(2000);

	constexpr unsigned up_border{ 122 };
	constexpr unsigned down_border{ 5 };
	constexpr unsigned kylobyte_size_in_bytes{ 1024 };

	//we will use c++ 11 posibilities to generate random numbers
	const std::uniform_int_distribution<> first_distribution(down_border, up_border);  //generate one ascii character number. The big string will be construct like this:
	const std::uniform_int_distribution<> second_distribution(down_border + 5, up_border - 5);							      //for example, number is 35, so the string will be {36,37,38,...,up_border,up_border-1,...}

	const bool is_first_file_choosen = (which == FIRST_FILE);

	const unsigned size_in_kylobytes = (is_first_file_choosen ? configuration.get_first_size() : configuration.get_second_size()); //size of the generated file
	const unsigned start_symbol = (is_first_file_choosen ? first_distribution : second_distribution)(main_generator);

	std::fstream newfile(filename, std::fstream::out | std::fstream::binary);
	
	std::string big_string; //defining out of scope, to avoid reinitialization
	for (unsigned curr_kylobyte = 0; curr_kylobyte < size_in_kylobytes; ++curr_kylobyte) {
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

	//set filenames and sizes of generated files

	if (is_first_file_choosen) {
		first_choosen_file_path = find_path_of_the_file(filename);
		main_encryptor->set_first_file(std::filesystem::path(filename));
	}
	else {
		second_choosen_file_path = find_path_of_the_file(filename);
		main_encryptor->set_second_file(std::filesystem::path(filename));
	}
			
}

void Main_interface::show_settings() noexcept {
	menu_layout->removeWidget(start_btn.get());
	menu_layout->removeWidget(settings_btn.get());
	menu_layout->removeWidget(exit_btn.get());

	start_btn->hide();
	settings_btn->hide();
	exit_btn->hide();

	first_file_size_edit->setText(std::to_string(Main_interface::configuration.get_first_size()).data());
	second_file_size_edit->setText(std::to_string(Main_interface::configuration.get_second_size()).data());

	first_file_layout->addWidget(first_file_lbl.get());
	first_file_layout->addWidget(first_file_size_edit.get());
	first_file_layout->addWidget(set_first_size_btn.get());
	
	second_file_layout->addWidget(second_file_lbl.get());
	second_file_layout->addWidget(second_file_size_edit.get());
	second_file_layout->addWidget(set_second_size_btn.get());

	algorithm_layout->addWidget(algorithm_lbl.get());
	algorithm_layout->addWidget(algorithm_choose_box.get());

	auto data = toml::parse("config.toml");

	settings_temp_layout->addWidget(to_main_menu_btn.get());
	settings_temp_layout->addWidget(exit_btn.get());
	
	algorithm_choose_box->setCurrentIndex(list_of_algorithms.indexOf(toml::find<std::string>(data, "algorithm").data()));

	menu_layout->insertLayout(0,first_file_layout.get());
	menu_layout->insertLayout(1,second_file_layout.get());
	menu_layout->insertLayout(0,algorithm_layout.get());
	menu_layout->insertLayout(3,settings_temp_layout.get());

	set_first_size_btn->show();
	set_second_size_btn->show();
	first_file_lbl->show();
	first_file_size_edit->show();
	second_file_lbl->show();
	second_file_size_edit->show();
	algorithm_lbl->show();
	algorithm_choose_box->show();
	to_main_menu_btn->show();
	exit_btn->show();

	set_first_size_btn->setEnabled(false);
	set_second_size_btn->setEnabled(false);

	setLayout(menu_layout.get());

}

//set paths of the choosen files
void Main_interface::set_text_of_choosen_files_path() noexcept {
	std::string label_text{ "First file:" };
	label_text += (first_choosen_file_path == std::filesystem::current_path()) ? "None" : ((first_choosen_file_path.string() +
		+ ". Size: " + std::to_string(std::filesystem::file_size(first_choosen_file_path)) + " bytes"));

	label_text += "\nSecond file: ";
	label_text += (second_choosen_file_path == std::filesystem::current_path()) ? "None" : ((second_choosen_file_path.string() +
		+ ". Size: " + std::to_string(std::filesystem::file_size(second_choosen_file_path)) + " bytes"));

	files_paths_lbl->setText(QString(label_text.data()));
}

//return path of the file with the given filename
std::filesystem::path Main_interface::find_path_of_the_file(const std::string& _filename) noexcept {
	
	using namespace std::filesystem;

	for (const auto& entry : directory_iterator(current_path())) {
		if (entry.path().filename() == _filename) {
			return entry.path();
		}
	}

	return current_path();
}

//if "Save size was pressed"
void Main_interface::change_file_size(unsigned which,std::unique_ptr<QLineEdit>& line_edit_with_size, std::unique_ptr<QPushButton>& button) noexcept{

	const bool is_first_file_choosen = (which == FIRST_FILE);

	button->setEnabled(false);

	if (std::cmatch result; !std::regex_match(first_file_size_edit->text().toStdString().data(), result, std::regex("([\\d])+"))) {
		incorrect_file_size_error_lbl->show(); //if incorrect value was entered
		line_edit_with_size->setText(std::to_string(Main_interface::configuration.get_first_size()).data());
	}
	else {
		line_edit_with_size->setText(std::atol(line_edit_with_size->text().toStdString().data()) <= Config::max_file_size ? line_edit_with_size->text().toStdString().data() : std::to_string(Config::max_file_size).data());
		line_edit_with_size->setText(std::atol(line_edit_with_size->text().toStdString().data()) != 0 ? line_edit_with_size->text().toStdString().data() : "1");

		std::string txt = line_edit_with_size->text().toStdString();
		auto nulls_removed_label_text = txt.erase(0, txt.find_first_not_of('0')).data(); //delete nulls from the beginning
		line_edit_with_size->setText(nulls_removed_label_text);

		if(unsigned size_ = std::atol(line_edit_with_size->text().toStdString().data()); is_first_file_choosen)
			Main_interface::configuration.set_first_size((size_ != 0 ? size_ : 1));
		else 
			Main_interface::configuration.set_second_size((size_ != 0 ? size_ : 1));

		Main_interface::configuration.write_config_to_file();

	}
}

//check if we can open choosen files
void Main_interface::is_filenames_correct(const std::string& first, const std::string& second) {
	std::fstream f(first, std::fstream::in | std::fstream::binary);
	std::fstream s(second, std::fstream::in | std::fstream::binary);

	if (!f.is_open() || !s.is_open())
		throw std::string(first + second);

	if (!f.is_open()) {
		s.close();
		throw first;
	}

	if (!s.is_open()) {
		f.close();
		throw second;
	}

	f.close();
	s.close();
}
inline bool Main_interface::is_both_files_choosen() const noexcept {
	return (first_choosen_file_path != std::filesystem::current_path() && second_choosen_file_path != std::filesystem::current_path());
}

