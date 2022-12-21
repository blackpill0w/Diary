#include "./main_window.hpp"

#include <QUiLoader>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
   resize(500, 500);
   // Load frame
   QUiLoader loader;

   QFile f("../UI/initialFrame.ui");
   f.open(QIODevice::ReadOnly);
   initial_frame = (QFrame *) loader.load(&f, this);
   f.close();

   QFile f2("../UI/newDiaryFrame.ui");
   f2.open(QIODevice::ReadOnly);
   new_diary_frame = (QFrame *) loader.load(&f2, this);
   new_diary_frame->hide();
   f2.close();

   new_diary_button  = initial_frame->findChild<QPushButton*>("newDiaryButton");
   open_diary_button = initial_frame->findChild<QPushButton*>("openDiaryButton");
   QObject::connect(new_diary_button, &QPushButton::clicked, [&](){ switch_frame(new_diary_frame); });
}

void MainWindow::switch_frame(QFrame *frame_to_show) {
   initial_frame->hide();
   new_diary_frame->hide();
   frame_to_show->show();
}
