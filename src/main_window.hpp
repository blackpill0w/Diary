#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QFrame>

class MainWindow : public QMainWindow {
public:
   QFrame *newDiaryFrame;
public:
   explicit MainWindow(QWidget *parent = nullptr);
};