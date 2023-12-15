
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QtWidgets/QMainWindow>
#include "ui_EventHandler.h"
#include <QTreeView>
#include <QFileSystemModel>
#include"FileIO.h"
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>
#include<qDebug>


class EventHandler : public QMainWindow
{
    Q_OBJECT

public:
    EventHandler(QWidget* parent = nullptr);

 
private:
    QMenuBar* createMenuBar();
    QTreeView* createFileTreeView();
    QFileSystemModel* fileSystemModel;
    QString selectedFilePath;

    void copyFile(const QString& sourcePath, const QString& destinationPath);
    void pasteFile(const QString& sourcePath, const QString& destinationPath);
    void deleteFile(const QString& filePath);

private slots:
    void onCopyActionTriggered();
    void onPasteActionTriggered();
    void onDeleteActionTriggered();
    void compressClicked();
    void printCode();
    void decompressClicked();
    void onTreeViewClicked(const QModelIndex& index);
};
#endif