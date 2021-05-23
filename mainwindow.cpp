#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <duckx.hpp>

#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  initSignalSlots();
  initTableModel();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::initSignalSlots() {
  connect(ui->setDocumentsFolder, &QPushButton::clicked, [this](){
    templatesFolder = QFileDialog::getExistingDirectory(this, tr("Set Templates Directory"));
    templatesList = QDir(templatesFolder).entryInfoList(QStringList() << "*.doc" << "*.docx", QDir::Files);
    if (! templatesList.isEmpty()) {
      Q_EMIT templatesReady();
    }
    else {
      QMessageBox::information(this, qApp->applicationName(), tr("There is no templates(*.doc, *.docx) in selected directory"));
    }
  });
  connect(ui->actionSetDocumentsFolder, &QAction::triggered, ui->setDocumentsFolder, &QPushButton::clicked);

  connect(this, &MainWindow::templatesReady, [this](){
    for(auto &filename : templatesList) {
      duckx::Document doc(filename.absoluteFilePath().toStdString());
      doc.open();
      const QRegExp rx(R"(\{\{(.*)\}\})");
      for (auto p : doc.paragraphs()) {
        for (auto r : p.runs()) {
          auto txt = QString::fromStdString(r.get_text());
          if (-1 != rx.indexIn(txt)) {
            auto item = new QStandardItem(rx.cap(1));
            item->setEditable(false);
            model->appendRow(item);
          }
        }
      }
      doc.save();
    }
  });
}

void MainWindow::initTableModel() {
  model = new QStandardItemModel(0, 2, this);
  model->setHeaderData(0, Qt::Horizontal, "Name", Qt::DisplayRole);
  model->setHeaderData(1, Qt::Horizontal, "Value");
  ui->tableView->setModel(model);
}
