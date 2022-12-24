#pragma once

#include <string>
#include <vector>
#include <memory>

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

class DiaryApp {
public: // static vars
   inline static constexpr int winW = 600;
   inline static constexpr int winH = 600;
   inline static const QString app_directory_name = ".diary";
public:
   const QString app_directory_full_path;
private:
   // Main window
   QMainWindow  *mainWin                = nullptr;
   // Frames
   QFrame       *initial_frame          = nullptr;
   QFrame       *edit_diary_frame       = nullptr;
   QFrame       *load_diary_frame       = nullptr;
   // Inside initial_frame
   QPushButton  *new_diary_button       = nullptr;
   QPushButton  *open_diary_button      = nullptr;
   // Inside edit_diary_frame
   QPushButton  *EDF_go_back_button     = nullptr;
   QPushButton  *save_button            = nullptr;
   QLineEdit    *diary_title_entry      = nullptr;
   QTextEdit    *diary_body_entry       = nullptr;
   // Inside load_diary_frame
   QPushButton  *LDF_go_back_button     = nullptr;
   QWidget      *diaries_buttons_widget = nullptr;
   std::vector< std::shared_ptr<QPushButton> > diaries{};

public:
   explicit DiaryApp();
   void start();
private:
   void switch_frame(QFrame *frame_to_show);
   void save_diary();
   void load_diary(const QString& file_path);
   void load_diaries();
};