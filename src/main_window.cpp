#include "./main_window.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>

#include <QUiLoader>
#include <QFile>
#include <QDir>
#include <QLayout>
#include <QDate>

namespace fs = std::filesystem;

DiaryApp::DiaryApp()
: app_directory_full_path{QDir::homePath().toStdString() + fs::path::preferred_separator + app_directory_name}
{
   // Initialize variables
   diaries.reserve(16);
   // Load frame
   QUiLoader loader;

   // Load the window
   QFile f1("../UI/mainWin.ui");
   f1.open(QIODevice::ReadOnly);
   mainWin = (QMainWindow *) loader.load(&f1);
   mainWin->resize(winW, winH);
   // Initial frame
   initial_frame = mainWin->findChild<QFrame*>("initialFrame");
   // Buttons to make a new diary and edit an existing one
   new_diary_button  = initial_frame->findChild<QPushButton*>("newDiaryButton");
   open_diary_button = initial_frame->findChild<QPushButton*>("openDiaryButton");
   // Add events
   QObject::connect(open_diary_button, &QPushButton::clicked, [&](){ switch_frame(load_diary_frame); load_diaries(); });
   QObject::connect(new_diary_button, &QPushButton::clicked, [&]() {
      diary_title_entry->clear();
      diary_body_entry->clear();
      switch_frame(edit_diary_frame);
   });

   // New diary (the editor) frame
   QFile f2("../UI/newDiaryFrame.ui");
   f2.open(QIODevice::ReadOnly);
   edit_diary_frame = (QFrame *) loader.load(&f2, mainWin);
   edit_diary_frame->hide();
   // Diary title line edit
   diary_title_entry = edit_diary_frame->findChild<QLineEdit*>("diaryTitleLineEdit");
   // Diary body text edit
   diary_body_entry = edit_diary_frame->findChild<QTextEdit*>("diaryBodyTextEdit");
   // Save button
   save_button = edit_diary_frame->findChild<QPushButton*>("saveButton");
   QObject::connect(save_button, &QPushButton::clicked, [&](){ save_diary(); });
   // Button to go back to the initial frame
   EDF_go_back_button = edit_diary_frame->findChild<QPushButton*>("goBackButton");
   // Add icon
   EDF_go_back_button->setIcon(QIcon("../img/left_arrow.png"));
   EDF_go_back_button->setIconSize(QSize(42, 25));
   // Add event
   QObject::connect(EDF_go_back_button, &QPushButton::clicked, [&](){ switch_frame(initial_frame); });

   // Load diary frame
   QFile f3("../UI/loadDiaryFrame.ui");
   f3.open(QIODevice::ReadOnly);
   load_diary_frame = (QFrame *) loader.load(&f3, mainWin);
   load_diary_frame->hide();
   diaries_buttons_widget = load_diary_frame->findChild<QWidget*>("diariesButtonsWidget");
   // Button to go back to the initial frame
   LDF_go_back_button = load_diary_frame->findChild<QPushButton*>("goBackButton");
   // Add icon
   LDF_go_back_button->setIcon(QIcon("../img/left_arrow.png"));
   LDF_go_back_button->setIconSize(QSize(42, 25));
   // Add event
   QObject::connect(LDF_go_back_button, &QPushButton::clicked, [&](){ switch_frame(initial_frame); });
}

void DiaryApp::start() {
   mainWin->show();
}

void DiaryApp::switch_frame(QFrame *frame_to_show) {
   // Switch to a certain frame
   initial_frame->hide();
   edit_diary_frame->hide();
   load_diary_frame->hide();
   frame_to_show->show();
}

void DiaryApp::save_diary() {
   assert(!edit_diary_frame.isHidden());
   const std::string file_path {
      app_directory_full_path
      + fs::path::preferred_separator
      + QDate::currentDate().toString("dd-MM-yyyy").toStdString()
      + ".txt"
   };
   std::ofstream diary_file{ file_path };
   diary_file << diary_title_entry->text().toStdString()
              << '\n'
              << diary_body_entry->toPlainText().toStdString();
}

void DiaryApp::load_diary(QString file_path) {
   std::cout << file_path.toStdString() << std::endl;
   switch_frame(edit_diary_frame);
   std::ifstream diary_file{ file_path.toStdString() };
   std::string tmp{};
   getline(diary_file, tmp);
   diary_title_entry->insert(tmp.c_str());
   while (getline(diary_file, tmp)) {
      diary_body_entry->append(tmp.c_str());
   }
}

void DiaryApp::load_diaries() {
   assert(!load_diary_frame.isHidden());
   // Add a button for each diary file
   // Each button opens its correspinding file to edit
   for (const auto& diary_file: fs::directory_iterator(app_directory_full_path)) {
      diaries.emplace_back( std::make_shared<QPushButton>() );
      std::shared_ptr<QPushButton> b = diaries.back();

      const QString file_path = std::string(diary_file.path()).c_str();
      // /full/path/to/file/dd-MM-yyyy.txt -> dd-MM-yyyy
      b->setText(file_path.sliced(file_path.length() - 14, 10));
      diaries_buttons_widget->layout()->addWidget(b.get());
      QObject::connect(
         //b.get(),
         b.get(),
         &QPushButton::clicked,
         [=](){ load_diary(file_path); }
      );
      b->show();
   }
   // Push everything to the top
   diaries_buttons_widget->layout()->setAlignment(Qt::AlignTop);
}
