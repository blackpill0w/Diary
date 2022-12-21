#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QPushButton>

class MainWindow : public QMainWindow {
public:
   QFrame *initial_frame;
   QFrame *new_diary_frame;
   QPushButton *new_diary_button;
   QPushButton *open_diary_button;
public:
   explicit MainWindow(QWidget *parent = nullptr);
   void switch_frame(QFrame *frame_to_show);
};