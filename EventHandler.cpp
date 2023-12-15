
#include"EventHandler.h"
#include<QLabel>
#include <qinputdialog.h>
#include <QPlainTextEdit>

EventHandler::EventHandler(QWidget* parent): QMainWindow(parent)
{
    resize(1200, 900);
    // 设置主窗口标题
    setWindowTitle(u8"文件压缩工具");

    // 创建菜单栏
    setMenuBar(createMenuBar());

    // 创建文件树视图
    setCentralWidget(createFileTreeView());
}

QMenuBar* EventHandler::createMenuBar()//创建菜单栏
{
    QMenuBar* menuBar = new QMenuBar(this);

    // 创建一个菜单项
    QMenu* fileMenu = menuBar->addMenu(QString(u8"文件"));
    QMenu* toolMenu = menuBar->addMenu(QString(u8"哈夫曼功能"));

    // 添加退出动作
    QAction* exitAction = new QAction(u8"退出", this);
    connect(exitAction, &QAction::triggered, this, &EventHandler::close);
    fileMenu->addAction(exitAction);

    // 添加复制、粘贴和删除动作
    QAction* copyAction = new QAction(u8"复制", this);
    connect(copyAction, &QAction::triggered, this, &EventHandler::onCopyActionTriggered);
    fileMenu->addAction(copyAction);

    QAction* pasteAction = new QAction(u8"粘贴", this);
    connect(pasteAction, &QAction::triggered, this, &EventHandler::onPasteActionTriggered);
    fileMenu->addAction(pasteAction);

    QAction* deleteAction = new QAction(u8"删除", this);
    connect(deleteAction, &QAction::triggered, this, &EventHandler::onDeleteActionTriggered);
    fileMenu->addAction(deleteAction);

    QAction* compressAction = new QAction(u8"压缩", this);
    connect(compressAction, &QAction::triggered, this, &EventHandler::compressClicked);
    toolMenu->addAction(compressAction);

    QAction* decompressAction = new QAction(u8"解压缩", this);
    connect(decompressAction, &QAction::triggered, this, &EventHandler::decompressClicked);
    toolMenu->addAction(decompressAction);

    QAction* printTreeAction = new QAction(u8"显示编码图", this);
    toolMenu->addAction(printTreeAction);

    QAction* printCodeAction = new QAction(u8"显示编码表", this);
    connect(printCodeAction, &QAction::triggered, this, &EventHandler::printCode);
    toolMenu->addAction(printCodeAction);


    return menuBar;
}

QTreeView* EventHandler::createFileTreeView()//创建文件树视图
{
    // 获取系统上的所有驱动器
    QFileInfoList drives = QDir::drives();

    // 创建文件系统模型
    fileSystemModel = new QFileSystemModel(this);

    // 设置根路径为计算机
    fileSystemModel->setRootPath("Computer");

    // 创建文件树视图
    QTreeView* treeView = new QTreeView(this);
    treeView->setModel(fileSystemModel);

    // 设置树视图的相关属性
    treeView->setHeaderHidden(true); // 隐藏表头
    treeView->setRootIndex(fileSystemModel->index("Computer")); // 设置根目录索引
    connect(treeView, &QTreeView::clicked, this, &EventHandler::onTreeViewClicked);

    return treeView;
}

void EventHandler::copyFile(const QString& sourcePath, const QString& destinationPath)
{
    QFile sourceFile(sourcePath);
    QFile destinationFile(destinationPath);

    if (sourceFile.copy(destinationPath)) {
        qDebug() << "File copied successfully.";
    }
    else {
        qDebug() << "Error copying file:" << sourceFile.errorString();
    }
}

void EventHandler::pasteFile(const QString& sourcePath, const QString& destinationPath)
{
    copyFile(sourcePath, destinationPath);
    centralWidget()->findChild<QTreeView*>()->reset(); // 重置树视图; // 刷新文件系统模型，更新树视图
}

void EventHandler::deleteFile(const QString& filePath)
{
    QFile file(filePath);

    if (file.remove()) {
        qDebug() << "File deleted successfully.";
    }
    else {
        qDebug() << "Error deleting file:" << file.errorString();
    }
}

void EventHandler::onCopyActionTriggered()
{
    QModelIndex currentIndex = centralWidget()->findChild<QTreeView*>()->currentIndex();
    QString sourcePath = fileSystemModel->filePath(currentIndex);
    QApplication::clipboard()->setText(sourcePath);
    qDebug() << "File copied to clipboard:" << sourcePath;
}

void EventHandler::onPasteActionTriggered()
{
    QTreeView* treeView = centralWidget()->findChild<QTreeView*>();
    if (treeView) {
        QModelIndex currentIndex = treeView->currentIndex();
        QString destinationPath = fileSystemModel->filePath(currentIndex);
        QString sourcePath = QApplication::clipboard()->text();

        if (!sourcePath.isEmpty() && !destinationPath.isEmpty()) {
            pasteFile(sourcePath, QDir(destinationPath).filePath(QFileInfo(sourcePath).fileName()));
        }
        else {
            qDebug() << "Invalid source or destination path.";
        }
    }
    else {
        qDebug() << "?" << endl;
    }
 
}

void EventHandler::onDeleteActionTriggered()
{
    QModelIndex currentIndex = centralWidget()->findChild<QTreeView*>()->currentIndex();
    QString filePath = fileSystemModel->filePath(currentIndex);

    if (QMessageBox::question(this, "Delete File", "Are you sure you want to delete this file?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        deleteFile(filePath);
        centralWidget()->findChild<QTreeView*>()->reset(); // 重置树视图 
    }
}
void EventHandler::printCode()//打印编码表
{
    if (selectedFilePath == NULL)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(u8"提醒");
        msgBox.setText(u8"未选中文件！");
        msgBox.exec();
    }
    else {
        FileIO tool(selectedFilePath);
        QMap<QChar, QString> temp = tool.printCodeTable();
        QString mapText;
 
        for (auto it = temp.constBegin(); it != temp.constEnd(); ++it) {
            mapText += QString("%1: %2\n").arg(it.key()).arg(it.value());
        }

        // 创建只读的 QPlainTextEdit 控件
        QPlainTextEdit* readOnlyTextEdit = new QPlainTextEdit;
        readOnlyTextEdit->setReadOnly(true);
        readOnlyTextEdit->setPlainText(mapText);

        // 创建一个 QDialog 作为对话框
        QDialog dialog;
        dialog.resize(500, 300);
        dialog.setWindowTitle(u8"映射表内容");

        // 使用垂直布局
        QVBoxLayout layout(&dialog);

        layout.addWidget(new QLabel(u8"映射表内容如下："));
        layout.addWidget(readOnlyTextEdit);

        // 显示对话框
        dialog.exec();
    

    }
}
void EventHandler::decompressClicked()//解压缩
{
    if (selectedFilePath == NULL)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(u8"提醒");
        msgBox.setText(u8"未选中文件！");
        msgBox.exec();
    }
    else {
        FileIO tool(selectedFilePath);
        if (tool.decompression())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(u8"恭喜");
            msgBox.setText(u8"解压缩成功！");
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(u8"提醒");
            msgBox.setText(u8"解压缩失败！");
            msgBox.exec();
        }

    }

}
void EventHandler::compressClicked()//压缩
{
    if (selectedFilePath == NULL)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(u8"提醒");
        msgBox.setText(u8"未选中文件！");
        msgBox.exec();
    }
    else {
        FileIO tool(selectedFilePath);
        if (tool.compress())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(u8"恭喜");
            msgBox.setText(u8"压缩成功！");
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(u8"提醒");
            msgBox.setText(u8"压缩失败！");
            msgBox.exec();
        }

    }
 
}

void EventHandler::onTreeViewClicked(const QModelIndex& index)//获取文件路径
{
    // 获取文件路径
    selectedFilePath = fileSystemModel->filePath(index);
    qDebug() << u8"被选择的文件路径为：" << selectedFilePath;

    // 在这里使用 filePath 进行文件相关的操作
}


