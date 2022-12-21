#include "./main_window.hpp"

#include <QUiLoader>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
   // Load frame
   QFile f("../UI/newDiaryFrame.ui");
   f.open(QIODevice::ReadOnly);
   QUiLoader loader;
   newDiaryFrame = (QFrame *) loader.load(&f, this);
   newDiaryFrame->hide();
}
