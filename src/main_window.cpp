#include "./main_window.hpp"

#include <iostream>
#include <cassert>

#include <QUiLoader>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QLayout>
#include <QDate>

DiaryApp::DiaryApp()
: app_directory_full_path{QDir::homePath() + QDir::separator() + app_directory_name}
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
      switch_frame(edit_diary_frame);
   });

   // New diary (the editor) frame
   QFile f2("../UI/editDiaryFrame.ui");
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
   // Clear text edit
   diary_title_entry->clear();
   diary_body_entry->clear();
   // Switch to a certain frame
   initial_frame->hide();
   edit_diary_frame->hide();
   load_diary_frame->hide();
   frame_to_show->show();
}

void DiaryApp::save_diary() {
   assert(!edit_diary_frame.isHidden());
   const QString file_path {
      app_directory_full_path
      + QDir::separator()
      + QDate::currentDate().toString("dd-MM-yyyy")
      + ".txt"
   };

   QFile diary_file{ file_path };
   diary_file.open(QIODevice::ReadWrite);
   QTextStream stream{ &diary_file };
   stream << diary_title_entry->text()
          << '\n'
          << diary_body_entry->toPlainText();
}

void DiaryApp::load_diary(const QString& file_path) {
   switch_frame(edit_diary_frame);
   QFile diary_file{ file_path };
   diary_file.open(QIODevice::ReadOnly);
   QTextStream stream{ &diary_file };

   QString tmp{ stream.readLine() };
   diary_title_entry->insert(tmp);

   while (true) {
      tmp = stream.readLine();
      if (tmp.isNull()) break;
      diary_body_entry->append(tmp);
   }
}

void DiaryApp::load_diaries() {
   assert(!load_diary_frame.isHidden());
   while (diaries_buttons_widget->layout()->count() != 0) {
      diaries_buttons_widget->layout()->removeItem(diaries_buttons_widget->layout()->itemAt(0));
   }
   // Add a button for each diary file
   // Each button opens its correspinding file to edit
   QDir diary_dir{ app_directory_full_path };
   QStringList files{ diary_dir.entryList(QStringList() << "*.txt", QDir::Files | QDir::NoDot) };
   for (int i = 0; i < files.size(); ++i) {
      diaries.emplace_back( std::make_shared<QPushButton>() );
      std::shared_ptr<QPushButton> b = diaries.back();
      b->setText(files[i]);

      diaries_buttons_widget->layout()->addWidget(b.get());
      QObject::connect(
         b.get(),
         &QPushButton::clicked,
         [=](){ load_diary(app_directory_full_path + QDir::separator() + files[i]); }
      );
      b->show();
   }
   // Push everything to the top
   diaries_buttons_widget->layout()->setAlignment(Qt::AlignTop);
}