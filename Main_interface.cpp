#include "Main_interface.h"
#include "encryptor.h"
#include "Algorithms.h"
#include "Config.h"
Config Main_interface::configuration = Config(); //default config

Main_interface::Main_interface(QWidget* parent) : QWidget(parent) {

	main_encryptor = std::make_unique<encryptor>();

	try {
		configuration.read_from_file("config.toml");  //read configuration from input file
	}
	catch (const std::runtime_error& e) {
		configuration.write_config_to_file();
	}
	last_first_size = std::to_string(configuration.get_first_size()).data();
	last_second_size = std::to_string(configuration.get_second_size()).data();
	start = std::make_unique<QPushButton>(this);
	start->setIcon(QIcon("icons\\start.png"));   //start button
	start->setText("Start");

	exit = std::make_unique<QPushButton>(this); //exit button
	exit->setText("Exit");
	exit->setIcon(QIcon("icons\\exit.png"));

	settings = std::make_unique<QPushButton>(this);   //settings button
	settings->setText("Settings");
	settings->setIcon(QIcon("icons//settings.png"));

	generate_files = std::make_unique<QPushButton>(this);  //button to generate files
	generate_files->setText("Generate big files");
	generate_files->hide();

	set_first_size = std::make_unique<QPushButton>(this);
	set_first_size->setText("Save size");
	set_first_size->hide();

	set_second_size = std::make_unique<QPushButton>(this);
	set_second_size->setText("Save size");
	set_second_size->hide();

	processing = std::make_unique<QLabel>(this);    //label to show that files is encrypting now
	loading_gif = std::make_unique<QMovie>("icons\\loading.gif");
	processing->setMovie(loading_gif.get());
	processing->setAlignment(Qt::AlignCenter);
	processing->hide();


	second_file_lbl = std::make_unique<QLabel>("Choose second file size in kylobytes: ",this);

	processing_progress = std::make_unique<QProgressBar>(this);
	processing_progress->setRange(0, 100);
	processing_progress->setValue(0);
	processing_progress->hide();

	processing_text = std::make_unique<QLabel>(this);
	processing_text->setText("Encryption in process. Do not close the program!");

	processing_text->setAlignment(Qt::AlignCenter);
	processing_text->hide();

	error_open_file = std::make_unique<QLabel>("Error opening file. Reselect one of the file.");
	error_open_file->setAlignment(Qt::AlignCenter);
	error_open_file->hide();

	same_files = std::make_unique<QLabel>("Same files was selected! Reselect one of the files");
	same_files->setAlignment(Qt::AlignCenter);
	same_files->setGeometry(this->height() / 2, this->width() / 2, 400, 200);
	same_files->hide();

	incorrect_file_size = std::make_unique<QLabel>("You entered invalid characters! Only positive numbers are allowed.");
	incorrect_file_size->setAlignment(Qt::AlignCenter);
	incorrect_file_size->setGeometry(this->height() / 2, this->width() / 2, 400, 200);
	incorrect_file_size->hide();

	second_file_lbl = std::make_unique<QLabel>("Choose second file size in kylobytes: ", this);

	first_file_lbl = std::make_unique<QLabel>("Choose first file size in kylobytes: ", this);
	algorithm_lbl = std::make_unique<QLabel>("Choose algorithm: ",this);
	second_file_lbl->setAlignment(Qt::AlignCenter);                        //some labels for settings
	first_file_lbl->setAlignment(Qt::AlignCenter);
	algorithm_lbl->setAlignment(Qt::AlignCenter);
	second_file_lbl->hide();
	first_file_lbl->hide();
	algorithm_lbl->hide();

	result_file_path_lbl = std::make_unique<QLabel>(this);
	result_file_path_lbl->hide();

	encryption_done_lbl = std::make_unique<QLabel>(this);
	encryption_done_lbl->setText("Encryption was done succesfully");
	encryption_done_lbl->setAlignment(Qt::AlignCenter);
	encryption_done_lbl->hide();

	encryption_duration = std::make_unique<QLabel>(this);
	encryption_duration->hide();

	select_first = std::make_unique<QPushButton>(this);
	select_first->setText("Select first file");
	select_first->hide();

	select_second = std::make_unique<QPushButton>(this);
	select_second->setText("Select second file");
	select_second->hide();

	to_root_path = std::make_unique<QPushButton>(this);
	to_root_path->setText("Root directory");
	to_root_path->setIcon(QIcon("icons\\root.png"));
	to_root_path->hide();

	to_main_menu = std::make_unique<QPushButton>(this);
	to_main_menu->setText("Return to main menu");
	to_main_menu->setIcon(QIcon("icons\\menu.png"));
	to_main_menu->hide();

	first_file_size = std::make_unique<QLineEdit>(this);
	first_file_size->setValidator(new QIntValidator(1,Config::max_file_size,this));
	first_file_size->hide();

	second_file_size = std::make_unique<QLineEdit>(this);
	second_file_size->setValidator(new QIntValidator(1, Config::max_file_size, this));
	second_file_size->hide();

	algorithm = std::make_unique<QComboBox>(this);

	list_of_algorithms = QStringList({ "XOR","OR","AND","CONSTANT_ONE","CONSTANT_ZERO" });
	algorithm->addItems(list_of_algorithms);
	algorithm->hide();

	menu_layout = std::make_unique<QVBoxLayout>(this);  //main layout
	menu_layout.get()->setSpacing(10);

	first_file_layout = std::make_unique<QHBoxLayout>();
	second_file_layout = std::make_unique<QHBoxLayout>();
	algorithm_layout = std::make_unique<QHBoxLayout>();

	settings_temp_layout = std::make_unique<QHBoxLayout>();
	settings_temp_layout->setSpacing(10);

	first_choosen_file = second_choosen_file = std::filesystem::current_path();

	setFixedSize(1280, 720);

	setLayout(menu_layout.get());

	setWindowTitle("File encryptor");

	files_paths = std::make_unique<QLabel>(this);
	files_paths->setText("First file: None\nSecond file: None.");  //labels with info about generated files
	//files_paths->setTextFormat(Qt::TextFormat::MarkdownText);
	files_paths->setAlignment(Qt::AlignCenter);
	
	files_paths->hide();
	
	current_directory_to_choose = std::filesystem::current_path().root_path();

	start_encrypt = std::make_unique<QPushButton>(this);
	start_encrypt->setText("Start encrypt choosen files");
	start_encrypt->hide();
	
	acces_denied = std::make_unique<QLabel>(this);
	acces_denied->hide();

	
	
	connect(first_file_size.get(), &QLineEdit::textChanged, this, [this](const QString& s) {
		if (s != last_first_size)
			set_first_size->setEnabled(true);
		else
			set_first_size->setEnabled(false);
		/*Main_interface::configuration.set_first_size(std::atoi(s.toStdString().data()) > 1 ? std::atol(s.toStdString().data()) : 1);
		Main_interface::configuration.write_config_to_file();*/
	});

	connect(second_file_size.get(), &QLineEdit::textChanged, this, [this](const QString& s) {
		if(s != last_second_size)
			set_second_size->setEnabled(true);
		else 
			set_second_size->setEnabled(false);
		/*Main_interface::configuration.set_second_size(std::atoi(s.toStdString().data()) > 1 ? std::atol(s.toStdString().data()) : 1);
		Main_interface::configuration.write_config_to_file();*/
	});

	connect(set_first_size.get(), &QPushButton::clicked, this, [this]() {
		set_first_size->setEnabled(false);
		std::cmatch result;
		if (!std::regex_match(first_file_size->text().toStdString().data(), result, std::regex("([\\d])+"))) {
			incorrect_file_size->show();
			first_file_size->setText(std::to_string(Main_interface::configuration.get_first_size()).data());
		}
		else {
			first_file_size->setText(std::atol(first_file_size->text().toStdString().data()) <= Config::max_file_size ? first_file_size->text().toStdString().data() : std::to_string(Config::max_file_size).data());
			first_file_size->setText(std::atol(first_file_size->text().toStdString().data()) != 0 ? first_file_size->text().toStdString().data() : "1");
			
			last_first_size = first_file_size->text();
			Main_interface::configuration.set_first_size((std::atol(first_file_size->text().toStdString().data()) != 0 ? std::atol(first_file_size->text().toStdString().data()) : 1));
			Main_interface::configuration.write_config_to_file();

			std::string txt = first_file_size->text().toStdString();
			first_file_size->setText((txt.erase(0, txt.find_first_not_of('0'))).data()); //delete nulls from the beginning
		}
	});
	connect(set_second_size.get(), &QPushButton::clicked, this, [this]() {
		set_second_size->setEnabled(false);
		std::cmatch result;
		if (!std::regex_match(second_file_size->text().toStdString().data(), result, std::regex("([\\d])+"))) {
			incorrect_file_size->show();
			second_file_size->setText(std::to_string(Main_interface::configuration.get_first_size()).data());
		}
		else {
			second_file_size->setText(std::atol(second_file_size->text().toStdString().data()) <= Config::max_file_size ? second_file_size->text().toStdString().data() : std::to_string(Config::max_file_size).data());
			second_file_size->setText((std::atol(second_file_size->text().toStdString().data()) != 0 ? second_file_size->text().toStdString().data() : "1"));
			last_second_size = second_file_size->text();
			Main_interface::configuration.set_second_size((std::atol(second_file_size->text().toStdString().data()) != 0 ? std::atol(second_file_size->text().toStdString().data()) : 1));
			Main_interface::configuration.write_config_to_file();

			std::string txt = second_file_size->text().toStdString();
			second_file_size->setText((txt.erase(0, txt.find_first_not_of('0'))).data()); //delete nulls from the beginning
		}
	});

	connect(algorithm.get(), static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated), this, [&](const QString& s) {
		Main_interface::configuration.set_alg(ALGOS::string_to_alg[s.toStdString()]);

		Main_interface::configuration.write_config_to_file();
	});

	connect(to_main_menu.get(), &QPushButton::clicked, this, &Main_interface::menu);

	connect(start.get(), &QPushButton::clicked, this, [this]() {
		if (!(first_choosen_file != std::filesystem::current_path() && second_choosen_file != std::filesystem::current_path())) { //if files are not choosen
			this->start_program();
		}
		else ready_to_encrypt_screen(); //if files is already choosen
	});

	connect(settings.get(), &QPushButton::clicked, this, [this]() {
		show_settings();
	});

	connect(exit.get(), &QPushButton::clicked, qApp, &QApplication::quit);

	connect(select_first.get(), &QPushButton::clicked, this, [this]() {
		search_for_file(FIRST_FILE);
	});

	connect(select_second.get(), &QPushButton::clicked, this, [this]() {
		search_for_file(SECOND_FILE);
	});

	connect(generate_files.get(), &QPushButton::clicked, this, [this]() {
		try {
			std::thread t1([this]() {this->generate_file("First_file.bin", FIRST_FILE); });
			std::thread t2([this]() {this->generate_file("Second_file.bin", SECOND_FILE); });  //generate two files

			t1.join();
			t2.join();
			generate_files->setText("Regenerate files");
			ready_to_encrypt_screen();
		}
		catch (const std::invalid_argument& e) {

		}
	});

	connect(start_encrypt.get(), &QPushButton::clicked, this, [this]() {
		/*if (first_choosen_file != second_choosen_file) {*/
			menu_layout->removeWidget(files_paths.get());
			menu_layout->removeWidget(start_encrypt.get());
			menu_layout->removeWidget(select_first.get());
			menu_layout->removeWidget(select_second.get());
			menu_layout->removeWidget(generate_files.get());

			generate_files->hide();
			files_paths->hide();
			start_encrypt->hide();
			select_first->hide();
			select_second->hide();

			exit->hide();
			to_main_menu->hide();

			menu_layout->insertWidget(0, processing.get());
			menu_layout->insertWidget(0, processing_progress.get());
			menu_layout->insertWidget(0, processing_text.get());
			processing_progress->setValue(0);
			processing_progress->show();
			processing_text->show();
			loading_gif->start();
			processing->show();
			std::thread encrypt_thread([this]() {main_encryptor->start_encrypt(); });
			encrypt_thread.detach();
		//}
		/*else {
			same_files->show();
		}*/
	});

	connect(main_encryptor.get(), &encryptor::update_bar, this, [this](int val) {
		processing_progress->setValue(val);
	});
	connect(main_encryptor.get(), &encryptor::encryption_done_signal, this, &Main_interface::encryptor_finished);

	menu();
}

const std::filesystem::path& Main_interface::get_current_dir_path() const { return current_directory_to_choose; } //for searching files

const std::filesystem::path& Main_interface::get_first_file_path() const { return first_choosen_file; }
const std::filesystem::path& Main_interface::get_second_file_path() const { return second_choosen_file; }

void Main_interface::set_current_directpry_path(const std::filesystem::path& newPath) {
	//if (std::filesystem::exists(newPath))
		current_directory_to_choose = newPath;
	//current_directory_to_choose = std::filesystem::current_path().root_path();
}

void Main_interface::menu() noexcept {
	menu_layout->addWidget(start.get());
	menu_layout->addWidget(settings.get());
	menu_layout->addWidget(exit.get());

	settings->show();

	menu_layout->removeWidget(generate_files.get());
	menu_layout->removeWidget(select_first.get());
	menu_layout->removeWidget(select_second.get());
	menu_layout->removeWidget(to_main_menu.get());
	menu_layout->removeWidget(files_paths.get());
	menu_layout->removeWidget(start_encrypt.get());
	menu_layout->removeWidget(result_file_path_lbl.get());
	menu_layout->removeWidget(encryption_done_lbl.get());
	menu_layout->removeWidget(encryption_duration.get());
	menu_layout->removeItem(first_file_layout.get());
	menu_layout->removeItem(second_file_layout.get());
	menu_layout->removeItem(algorithm_layout.get());

	algorithm_layout->removeWidget(algorithm.get());
	algorithm_layout->removeWidget(algorithm_lbl.get());
	algorithm->hide();
	algorithm_lbl->hide();

	first_file_layout->removeWidget(first_file_size.get());
	first_file_layout->removeWidget(first_file_lbl.get());
	first_file_layout->removeWidget(set_first_size.get());
	first_file_size->hide();
	first_file_lbl->hide();
	set_first_size->hide();

	second_file_layout->removeWidget(second_file_size.get());
	second_file_layout->removeWidget(second_file_lbl.get());
	second_file_layout->removeWidget(set_second_size.get());
	second_file_size->hide();
	second_file_lbl->hide();
	set_second_size->hide();

	encryption_duration->hide();
	encryption_done_lbl->hide();
	result_file_path_lbl->hide();
	start_encrypt->hide();
	files_paths->hide();
	generate_files->hide();
	select_first->hide();
	select_second->hide();
	to_main_menu->hide();
	start->show();
	exit->show();

	
}

void Main_interface::start_program()  {
	start->hide();
	menu_layout->removeWidget(start.get());
	
	settings->hide();
	menu_layout->removeWidget(settings.get());


	menu_layout->insertWidget(0, generate_files.get());
	generate_files->show();


	menu_layout->insertWidget(0, to_main_menu.get());
	to_main_menu->show();

	menu_layout->insertWidget(0, select_second.get());
	select_second->show();
	menu_layout->insertWidget(0, select_first.get());
	select_first->show();

	menu_layout->insertWidget(3,files_paths.get());
	


	
	menu_layout->insertWidget(2, files_paths.get());
	files_paths->show();



	
}




void Main_interface::search_for_file(unsigned Which) { //function to choose files from root directory

	auto root_path_ = std::filesystem::current_path().root_directory();

	std::wstring fname = QFileDialog::getOpenFileName(this, "Choose file", root_path_.string().data(), "*.*").toStdWString();
	std::filesystem::path p(fname);
	((Which == FIRST_FILE) ? first_choosen_file : second_choosen_file) = p;
	qDebug() << ((Which == FIRST_FILE) ? first_choosen_file.string().data() : second_choosen_file.string().data());
	std::string label_text = "First file:";
	label_text += (first_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(first_choosen_file.string().data())
		+ ". Size: " + std::to_string(std::filesystem::file_size(first_choosen_file)) + " bytes"));
	label_text += "\nSecond file: ";
	label_text += (second_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(second_choosen_file.string().data())
		+ ". Size: " + std::to_string(std::filesystem::file_size(second_choosen_file)) + " bytes"));

	files_paths->setText(QString(label_text.data()));

	((Which == FIRST_FILE) ? select_first : select_second)->setText("Reselect " + QString(((Which == FIRST_FILE) ? "first" : "second")) + " file");


	if(Which == FIRST_FILE)
		main_encryptor->set_first_file(first_choosen_file);
	else 
		main_encryptor->set_second_file(second_choosen_file);
	
	if (first_choosen_file != std::filesystem::current_path() && second_choosen_file != std::filesystem::current_path()) {
		try {
			ready_to_encrypt_screen();
		}

	

		catch (const std::invalid_argument& e) {
			error_open_file->show();
		}
	}
}




void Main_interface::ready_to_encrypt_screen() {

	std::fstream f;
	std::fstream s;
	f.open(first_choosen_file.wstring(), std::fstream::in | std::fstream::binary);
	s.open(second_choosen_file.wstring(), std::fstream::in | std::fstream::binary);

	if (!f.is_open()) {
		throw std::invalid_argument(first_choosen_file.string());
		if (s.is_open())
			s.close();
	}
	if (!s.is_open()) {
		throw std::invalid_argument(second_choosen_file.string());
		if (f.is_open())
			f.close();
	}


	menu_layout->insertWidget(0,start_encrypt.get());
	menu_layout->removeWidget(to_root_path.get());
	
	
	menu_layout->removeWidget(start.get());
	start->hide();

	generate_files->setText("Regenerate files");

	menu_layout->insertWidget(0, start_encrypt.get());
	menu_layout->removeWidget(to_root_path.get());

	menu_layout->removeWidget(select_first.get());
	menu_layout->removeWidget(select_second.get());
	menu_layout->removeWidget(start.get());
	start->hide();

	settings->hide();
	menu_layout->removeWidget(settings.get());

	

	menu_layout->insertWidget(1, generate_files.get());
	generate_files->show();
	to_root_path->hide();
	start_encrypt->show();

	menu_layout->insertWidget(1, files_paths.get());
	files_paths->show();

	menu_layout->insertWidget(2, to_main_menu.get());
	to_main_menu->show();

	select_first->setText("Reselect first file");
	select_second->setText("Reselect second file");
	
	menu_layout->insertWidget(3, select_second.get());
	menu_layout->insertWidget(3, select_first.get());
	select_first->show();
	select_second->show();
	//print ifo about choosen files
	std::string label_text = "First file:";
	label_text += (first_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(first_choosen_file.string().data())
		+ ". Size: " + std::to_string(std::filesystem::file_size(first_choosen_file)) + " bytes"));
	label_text += "\nSecond file: ";
	label_text += (second_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(second_choosen_file.string().data())
		+ ". Size: " + std::to_string(std::filesystem::file_size(second_choosen_file)) + " bytes"));

	files_paths->setText(QString(label_text.data()));


	
	main_encryptor->set_encrypt_alg(Main_interface::configuration.get_alg());
	

	
}

void Main_interface::final_screen() {
	


	encryption_done_lbl->setText("Encryption was ended succesfully!");
	encryption_done_lbl->setAlignment(Qt::AlignCenter);
	menu_layout->insertWidget(0, encryption_done_lbl.get());
	encryption_done_lbl->show();

	std::filesystem::path result_file_path;
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (std::filesystem::path(entry.path()).filename() == "Result.bin") {
			result_file_path = entry.path();
			break;
		}
	}

	menu_layout->insertWidget(1, result_file_path_lbl.get());
	result_file_path_lbl->setText("Path to result file: " + QString(result_file_path.string().data()));
	result_file_path_lbl->setAlignment(Qt::AlignCenter);
	result_file_path_lbl->show();

	encryption_duration->setText("Last encryption time: " + QString(std::to_string(main_encryptor->get_last_encryption_time().count()).data()) + " seconds");
	encryption_duration->setAlignment(Qt::AlignCenter);
	menu_layout->insertWidget(2, encryption_duration.get());
	encryption_duration->show();
}

void Main_interface::encryptor_finished() {
	menu_layout->removeWidget(processing.get());
	menu_layout->removeWidget(processing_text.get());
	menu_layout->removeWidget(processing_progress.get());
	processing->hide();
	processing_progress->hide();
	processing_text->hide();
	to_main_menu->show();
	exit->show();
	final_screen();
}

void Main_interface::generate_file(const std::string& filename,std::size_t which)  {
	std::string big_string;
	big_string.reserve(1000);  //string size will be 1 kB

	const int up_border = 126;
	const int down_border = 33;

	  //we will use c++ 11 posibilities to generate random numbers
	std::uniform_int_distribution<> uid1(down_border, up_border);  //generate one ascii character number. The big string will be construct like this:
	std::uniform_int_distribution<> uid2(down_border + 5, up_border - 5);							      //for example, number is 35, so the string will be {36,37,38,...,up_border,up_border-1,...}
	
	std::fstream newfile;
	newfile.open(filename,std::fstream::out);

	if (!newfile.is_open()) throw std::invalid_argument("filename is incorrect");

	unsigned long kylobytes_size = (which == FIRST_FILE) ? Main_interface::configuration.get_first_size() : Main_interface::configuration.get_second_size();

	for (int i = 0; i < kylobytes_size; ++i) {
		big_string.clear();
		big_string.reserve(1000);

		int number = (which == FIRST_FILE ? uid1 : uid2)(gen1);
		int additional = 1;
		for (int j = 0; j < 1024; ++j) { //fill one kylobyte string
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
	//set filenames and sizes of generated files
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (std::filesystem::path(entry.path()).filename() == filename) {
			((which == FIRST_FILE) ? first_choosen_file : second_choosen_file) = entry.path();
			break;
		}
	}
	
	if (which == FIRST_FILE) {
		main_encryptor->set_first_file(std::filesystem::path(filename));
	}
	else {
		main_encryptor->set_second_file(std::filesystem::path(filename));
	}
	
}

//void Main_interface::final_screen() {
//	menu_layout->removeWidget(files_paths.get());
//	menu_layout->removeWidget(start_encrypt.get());
//	menu_layout->removeWidget(to_root_path.get());
//	files_paths->hide();
//	start_encrypt->hide();
//	to_root_path->hide();
//
//	
//
//	
//	menu_layout->insertWidget(0, encryption_done_lbl.get());
//	encryption_done_lbl->show();
//
//	std::filesystem::path result_file_path;
//	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
//		if (std::filesystem::path(entry.path()).filename() == "Result.bin") {
//			result_file_path = entry.path();
//			break;
//		}
//	}
//
//	menu_layout->insertWidget(1, result_file_path_lbl.get());
//	result_file_path_lbl->setText("Path to result file: " + QString(result_file_path.string().data()));
//	result_file_path_lbl->setAlignment(Qt::AlignCenter);
//	result_file_path_lbl->show();
//
//	
//
//}




void Main_interface::show_settings() noexcept {
	menu_layout->removeWidget(start.get());
	menu_layout->removeWidget(settings.get());
	start->hide();
	settings->hide();

	menu_layout->removeWidget(exit.get());
	exit->hide();

	

	first_file_size->setText(std::to_string(Main_interface::configuration.get_first_size()).data());
	second_file_size->setText(std::to_string(Main_interface::configuration.get_second_size()).data());

	first_file_layout->addWidget(first_file_lbl.get());
	first_file_layout->addWidget(first_file_size.get());
	first_file_layout->addWidget(set_first_size.get());
	
	

	

	

	second_file_layout->addWidget(second_file_lbl.get());
	second_file_layout->addWidget(second_file_size.get());
	second_file_layout->addWidget(set_second_size.get());


	algorithm_layout->addWidget(algorithm_lbl.get());
	algorithm_layout->addWidget(algorithm.get());

	auto data = toml::parse("config.toml");
	settings_temp_layout->addWidget(to_main_menu.get());

	settings_temp_layout->addWidget(exit.get());
	
	algorithm->setCurrentIndex(list_of_algorithms.indexOf(toml::find<std::string>(data, "algorithm").data()));
	//algorithm->set

	menu_layout->insertLayout(0,first_file_layout.get());
	menu_layout->insertLayout(1,second_file_layout.get());
	menu_layout->insertLayout(0,algorithm_layout.get());
	
	menu_layout->insertLayout(3,settings_temp_layout.get());


	set_first_size->show();
	set_first_size->setEnabled(false);

	set_second_size->show();
	set_second_size->setEnabled(false);


	first_file_lbl->show();
	first_file_size->show();
	

	second_file_lbl->show();
	second_file_size->show();
	

	algorithm_lbl->show();
	algorithm->show();

	to_main_menu->show();
	exit->show();



	setLayout(menu_layout.get());

}
