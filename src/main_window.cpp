#include "./main_window.hpp"

#include <iostream>
#include <QUiLoader>
#include <QFile>

DiaryApp::DiaryApp() {
   // Load frame
   QUiLoader loader;

   // Load the window
   QFile f1("../UI/mainWin.ui");
   f1.open(QIODevice::ReadOnly);
   mainWin = (QMainWindow *) loader.load(&f1);
   mainWin->resize(600, 600);
   f1.close();
   // Initial frame
   initial_frame = mainWin->findChild<QFrame*>("initialFrame");
   new_diary_button  = initial_frame->findChild<QPushButton*>("newDiaryButton");
   open_diary_button = initial_frame->findChild<QPushButton*>("openDiaryButton");
   QObject::connect(new_diary_button, &QPushButton::clicked, [&](){ switch_frame(new_diary_frame); });
   QObject::connect(open_diary_button, &QPushButton::clicked, [&](){ switch_frame(new_diary_frame); });
   // New diary (the editor) frame
   QFile f2("../UI/newDiaryFrame.ui");
   f2.open(QIODevice::ReadOnly);
   new_diary_frame = (QFrame *) loader.load(&f2, mainWin);
   new_diary_frame->hide();
   f2.close();
   // Button to go back to the initial frame
   go_back_button  = new_diary_frame->findChild<QPushButton*>("goBackButton");
   go_back_button->setIcon(QIcon("../img/left_arrow.png"));
   go_back_button->setIconSize(QSize(42, 25));
   QObject::connect(go_back_button, &QPushButton::clicked, [&](){ switch_frame(initial_frame); });
}

void DiaryApp::start() {
   mainWin->show();
}

void DiaryApp::switch_frame(QFrame *frame_to_show) {
   initial_frame->hide();
   new_diary_frame->hide();
   frame_to_show->show();
}
