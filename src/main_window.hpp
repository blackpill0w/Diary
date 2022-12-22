#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QPushButton>

class DiaryApp {
public:
   QMainWindow *mainWin;
   QFrame *initial_frame;
   QFrame *new_diary_frame;
   QPushButton *new_diary_button;
   QPushButton *open_diary_button;
   // Inside new diary frame
   QPushButton *go_back_button;
public:
   explicit DiaryApp();
   void switch_frame(QFrame *frame_to_show);
   void start();
};