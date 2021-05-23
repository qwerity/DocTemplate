#pragma once

#include <QMainWindow>
#include <QFileInfoList>
#include <QStandardItemModel>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  void initSignalSlots();
  void initTableModel();

signals:
  void templatesReady();

private:
  Ui::MainWindow *ui = Q_NULLPTR;

  QString templatesFolder;
  QFileInfoList templatesList;

  QStandardItemModel *model = Q_NULLPTR;
};
