#include <QApplication>

#include "./main_window.hpp"

int main(int argc, char** argv) {
   QApplication app(argc, argv);
   DiaryApp d{};
   d.start();
   app.exec();
}