#include "Main_interface.h"
#include "encryptor.h"
#include "Algorithms.h"
#include "Config.h"
Config Main_interface::configuration = Config(); //default config

Main_interface::Main_interface(QWidget* parent) : QWidget(parent) {

	main_encryptor = std::make_unique<encryptor>();

	configuration.read_from_file("config.toml");  //read configuration from input file

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

	processing = std::make_shared<QLabel>(this);    //label to show that files is encrypting now
	processing->setText("Processing...");
	processing->setGeometry(this->width() / 2, this->height() / 2, 400, 200);
	processing->hide();

	second_file_lbl = std::make_unique<QLabel>("Choose second file size in kylobytes: ", this);
	first_file_lbl = std::make_unique<QLabel>("Choose first file size in kylobytes: ", this);
	algorithm_lbl = std::make_unique<QLabel>("Choose algorithm: ", this);
	second_file_lbl->setAlignment(Qt::AlignCenter);                        //some labels for settings
	first_file_lbl->setAlignment(Qt::AlignCenter);
	algorithm_lbl->setAlignment(Qt::AlignCenter);
	second_file_lbl->hide();
	first_file_lbl->hide();
	algorithm_lbl->hide();

	result_file_path_lbl = std::make_unique<QLabel>(this);
	result_file_path_lbl->hide();

	encryption_done = std::make_unique<QLabel>(this);
	encryption_done->hide();

	encryption_duration = std::make_unique<QLabel>(this);
	encryption_duration->hide();

	choose_from_existing = std::make_unique<QPushButton>(this);
	choose_from_existing->setText("Choose files to encrypt from existing ones (Should be bigger then 1 MB.))");
	choose_from_existing->hide();

	to_root_path = std::make_unique<QPushButton>(this);
	to_root_path->setText("Root directory");
	to_root_path->setIcon(QIcon("icons\\root.png"));
	to_root_path->hide();

	to_main_menu = std::make_unique<QPushButton>(this);
	to_main_menu->setText("Return to main menu");
	to_main_menu->setIcon(QIcon("icons\\menu.png"));
	to_main_menu->hide();

	first_file_size = std::make_unique<QLineEdit>(this);
	first_file_size->setValidator(std::make_unique<QIntValidator>(0, 10000, this).get());
	first_file_size->hide();

	second_file_size = std::make_unique<QLineEdit>(this);
	second_file_size->setValidator(std::make_unique<QIntValidator>(0, 10000, this).get());
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
	files_paths->setAlignment(Qt::AlignCenter);

	files_paths->hide();

	current_directory_to_choose = std::filesystem::current_path().root_path();

	start_encrypt = std::make_unique<QPushButton>(this);
	start_encrypt->setText("Start encrypt choosen files");
	start_encrypt->hide();

	acces_denied = std::make_unique<QLabel>(this);
	acces_denied->hide();

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
	menu_layout->removeWidget(choose_from_existing.get());
	menu_layout->removeWidget(to_main_menu.get());
	menu_layout->removeWidget(files_paths.get());
	menu_layout->removeWidget(start_encrypt.get());
	menu_layout->removeWidget(result_file_path_lbl.get());
	menu_layout->removeWidget(encryption_done.get());
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
	first_file_size->hide();
	first_file_lbl->hide();

	second_file_layout->removeWidget(second_file_size.get());
	second_file_layout->removeWidget(second_file_lbl.get());
	second_file_size->hide();
	second_file_lbl->hide();

	encryption_duration->hide();
	encryption_done->hide();
	result_file_path_lbl->hide();
	start_encrypt->hide();
	files_paths->hide();
	generate_files->hide();
	choose_from_existing->hide();
	to_main_menu->hide();
	start->show();
	exit->show();

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
}

void Main_interface::start_program() {
	start->hide();
	menu_layout->removeWidget(start.get());

	settings->hide();
	menu_layout->removeWidget(settings.get());

	menu_layout->insertWidget(0, to_main_menu.get());
	to_main_menu->show();

	menu_layout->insertWidget(0, choose_from_existing.get());
	choose_from_existing->show();

	menu_layout->insertWidget(0, generate_files.get());
	generate_files->show();



	menu_layout->insertWidget(2, files_paths.get());
	files_paths->show();

	connect(choose_from_existing.get(), &QPushButton::clicked, this, [this]() {this->search_for_files(); });

	connect(generate_files.get(), &QPushButton::clicked, this, [this]() {
		try {
			std::thread t1([this]() {this->generate_file("First_file.bin", FIRST_FILE); });
			std::thread t2([this]() {this->generate_file("Second_file.bin", SECOND_FILE); });  //generate two files

			t1.join();
			t2.join();
			//menu_layout->removeWidget(tmp_lbl.get());
	/*		tmp_lbl->hide();*/
			ready_to_encrypt_screen();
		}
		catch (const std::invalid_argument& e) {

		}
		});

	connect(to_main_menu.get(), &QPushButton::clicked, this, &Main_interface::menu);
}

void Main_interface::search_for_files() { //function to choose files from root directory


	menu_layout->removeWidget(generate_files.get());
	menu_layout->removeWidget(choose_from_existing.get());

	choose_from_existing->hide();
	generate_files->hide();

	menu_layout->insertWidget(0, to_root_path.get());

	show_list_of_files();

}

void Main_interface::show_list_of_files() {
	settings->hide();
	menu_layout->removeWidget(settings.get());

	for (auto& button : files_to_choose) {
		if (!menu_layout->isEmpty())
			menu_layout->removeWidget(button.get());

		button->hide();
	}
	files_to_choose.clear();

	menu_layout->removeWidget(acces_denied.get());
	acces_denied->hide();

	menu_layout->insertWidget(0, to_root_path.get());
	to_root_path->setEnabled(!(current_directory_to_choose == std::filesystem::current_path().root_path()));
	to_root_path->show();


	if (std::filesystem::is_directory(current_directory_to_choose)) { //show the list of files in the current directory (works bad with huge directories)
		for (const auto& entry : std::filesystem::directory_iterator(current_directory_to_choose)) {
			std::filesystem::file_status info = std::filesystem::status(entry.path());
			//if (std::filesystem::is_regular_file(entry.path()) || std::filesystem::is_directory(entry.path())) {
			std::unique_ptr<QPushButton> newbutton = std::make_unique<QPushButton>(this);
			std::string current_format_path = entry.path().string();


			newbutton->setText(current_format_path.data());

			if (!std::filesystem::is_directory(entry.path())) {
				newbutton->setIcon(QIcon("icons\\file.png"));
			}
			else {
				newbutton->setIcon(QIcon("icons\\folder.png"));
			}
			menu_layout->insertWidget(0, newbutton.get());
			connect(newbutton.get(), &QPushButton::clicked, this, [this, entry]() {
				this->set_current_directpry_path(entry.path());
				if (std::filesystem::exists(entry.path())) {

					show_list_of_files();
				}
				else {
					this->set_current_directpry_path(std::filesystem::current_path().root_path());
					show_list_of_files();
				}
				});
			files_to_choose.push_back(std::move(newbutton));


		}
	}
	else {

		std::fstream f;
		f.open(current_directory_to_choose.string());

		if (f.is_open()) {

			if (first_choosen_file == std::filesystem::current_path()) first_choosen_file = current_directory_to_choose;
			else if (second_choosen_file == std::filesystem::current_path())
				second_choosen_file = current_directory_to_choose;
			f.close();



		}
		else {
			for (auto& button : files_to_choose) {
				if (!menu_layout->isEmpty())
					menu_layout->removeWidget(button.get());

				button->hide();
			}
			files_to_choose.clear();

			menu_layout->insertWidget(0, acces_denied.get());
			acces_denied->setText("Access to file " + QString(current_directory_to_choose.string().data()) + " was denied."); //can not open choosen file
			acces_denied->show();

		}
		std::string label_text = "First file:";

		label_text += (first_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(first_choosen_file.string().data())
			+ ". Size: " + std::to_string(std::filesystem::file_size(first_choosen_file)) + " bytes"));
		label_text += "\nSecond file: ";
		label_text += (second_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(second_choosen_file.string().data())
			+ ". Size: " + std::to_string(std::filesystem::file_size(second_choosen_file)) + " bytes"));

		files_paths->setText(QString(label_text.data()));
		files_paths->setAlignment(Qt::AlignCenter);


		files_paths->show();
		/*this->set_current_directpry_path(std::filesystem::current_path().root_path());
		show_list_of_files();*/
	}

	files_to_choose.shrink_to_fit();

	if (first_choosen_file != std::filesystem::current_path() && second_choosen_file != std::filesystem::current_path())
		ready_to_encrypt_screen();

	connect(to_root_path.get(), &QPushButton::clicked, this, [this]() {
		this->set_current_directpry_path(std::filesystem::current_path().root_path());
		show_list_of_files();
		});

}

void Main_interface::ready_to_encrypt_screen() {
	for (auto& button : files_to_choose) {
		if (!menu_layout->isEmpty())
			menu_layout->removeWidget(button.get());

		button->hide();
	}
	files_to_choose.clear();
	menu_layout->insertWidget(0, start_encrypt.get());
	menu_layout->removeWidget(to_root_path.get());

	menu_layout->removeWidget(choose_from_existing.get());
	menu_layout->removeWidget(start.get());
	start->hide();

	settings->hide();
	menu_layout->removeWidget(settings.get());
	choose_from_existing->hide();
	generate_files->setText("Regenerate files");
	menu_layout->insertWidget(1, generate_files.get());
	generate_files->show();
	to_root_path->hide();
	start_encrypt->show();

	menu_layout->insertWidget(1, files_paths.get());
	files_paths->show();

	menu_layout->insertWidget(2, to_main_menu.get());
	to_main_menu->show();

	//print ifo about choosen files
	std::string label_text = "First file:";
	label_text += (first_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(first_choosen_file.string().data())
		+ ". Size: " + std::to_string(std::filesystem::file_size(first_choosen_file)) + " bytes"));
	label_text += "\nSecond file: ";
	label_text += (second_choosen_file == std::filesystem::current_path()) ? std::string("None") : ((std::string(second_choosen_file.string().data())
		+ ". Size: " + std::to_string(std::filesystem::file_size(second_choosen_file)) + " bytes"));

	files_paths->setText(QString(label_text.data()));


	main_encryptor->set_first_file(first_choosen_file);
	main_encryptor->set_second_file(second_choosen_file);
	main_encryptor->set_encrypt_alg(Main_interface::configuration.get_alg());
	connect(start_encrypt.get(), &QPushButton::clicked, this, [this]() {main_encryptor->start_encrypt(); final_screen(); });
}


void Main_interface::generate_file(const std::string& filename, std::size_t which) {
	std::string big_string;
	big_string.reserve(1000);  //string size will be 1 kB

	const int up_border = 126;
	const int down_border = 33;

	//we will use c++ 11 posibilities to generate random numbers
	std::uniform_int_distribution<> uid1(down_border, up_border);  //generate one ascii character number. The big string will be construct like this:
	std::uniform_int_distribution<> uid2(down_border + 5, up_border - 5);							      //for example, number is 35, so the string will be {36,37,38,...,up_border,up_border-1,...}

	std::fstream newfile;
	newfile.open(filename, std::fstream::out);

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

	//set filenames and sizes of generated files
	for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (std::filesystem::path(entry.path()).filename() == filename) {
			((which == FIRST_FILE) ? first_choosen_file : second_choosen_file) = entry.path();
			break;
		}
	}


}

void Main_interface::final_screen() {
	settings->hide();
	menu_layout->removeWidget(settings.get());
	menu_layout->removeWidget(files_paths.get());
	menu_layout->removeWidget(start_encrypt.get());
	menu_layout->removeWidget(to_root_path.get());
	files_paths->hide();
	start_encrypt->hide();
	to_root_path->hide();



	encryption_done->setText("Encryption was ended succesfully!");
	encryption_done->setAlignment(Qt::AlignCenter);
	menu_layout->insertWidget(0, encryption_done.get());
	encryption_done->show();

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

	encryption_duration->setText("Last encryption time in seconds: " + QString(std::to_string(main_encryptor->get_last_encryption_time().count()).data()));
	encryption_duration->setAlignment(Qt::AlignCenter);
	menu_layout->insertWidget(2, encryption_duration.get());
	encryption_duration->show();
}

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


	second_file_layout->addWidget(second_file_lbl.get());
	second_file_layout->addWidget(second_file_size.get());

	algorithm_layout->addWidget(algorithm_lbl.get());
	algorithm_layout->addWidget(algorithm.get());

	auto data = toml::parse("config.toml");
	settings_temp_layout->addWidget(to_main_menu.get());

	settings_temp_layout->addWidget(exit.get());

	algorithm->setCurrentIndex(list_of_algorithms.indexOf(toml::find<std::string>(data, "algorithm").data()));
	//algorithm->set

	menu_layout->insertLayout(0, first_file_layout.get());
	menu_layout->insertLayout(1, second_file_layout.get());
	menu_layout->insertLayout(0, algorithm_layout.get());

	menu_layout->insertLayout(3, settings_temp_layout.get());



	first_file_lbl->show();
	first_file_size->show();

	second_file_lbl->show();
	second_file_size->show();

	algorithm_lbl->show();
	algorithm->show();

	to_main_menu->show();
	exit->show();

	setLayout(menu_layout.get());

	connect(first_file_size.get(), &QLineEdit::textChanged, this, [this](const QString& s) {
		Main_interface::configuration.set_first_size(std::atoi(s.toStdString().data()) > 1 ? std::atoi(s.toStdString().data()) : 1);
		Main_interface::configuration.write_config_to_file();
		});
	connect(second_file_size.get(), &QLineEdit::textChanged, this, [this](const QString& s) {
		Main_interface::configuration.set_second_size(std::atoi(s.toStdString().data()) > 1 ? std::atoi(s.toStdString().data()) : 1);
		Main_interface::configuration.write_config_to_file();
		});
	connect(algorithm.get(), static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::textActivated), this, [&](const QString& s) {
		Main_interface::configuration.set_alg(ALGOS::string_to_alg[s.toStdString()]);

		Main_interface::configuration.write_config_to_file();
		});

	connect(to_main_menu.get(), &QPushButton::clicked, this, &Main_interface::menu);
}
