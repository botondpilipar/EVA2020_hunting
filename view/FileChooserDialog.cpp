#include "FileChooserDialog.h"
#include <QTreeView>

FileChooserDialog::FileChooserDialog()
    :
    ui(new Ui::Dialog),
    mCurrentDirectory(QDir::currentPath())
{
    ui->setupUi(this);
    this->setWindowTitle("Choose file");
    QFileSystemModel* model = new QFileSystemModel;
    auto modelIndex = model->setRootPath(mCurrentDirectory.path());

    ui->fileListView->setModel(model);
    ui->fileListView->setRootIndex(modelIndex);
}

void FileChooserDialog::setDisplayedDirectory(const QString& directory)
{
    mCurrentDirectory.setCurrent(directory);
}
