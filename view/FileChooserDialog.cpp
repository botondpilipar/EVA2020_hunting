#include "FileChooserDialog.h"
#include <QTreeView>

FileChooserDialog::FileChooserDialog()
    :
    ui(new Ui::Dialog),
    mCurrentDirectory(QDir::currentPath())
{
    ui->setupUi(this);
    this->setWindowTitle("Choose file");
    auto modelIndex = mFileSystemModel.setRootPath(mCurrentDirectory.path());

    ui->fileListView->setModel(&mFileSystemModel);
    ui->fileListView->setRootIndex(modelIndex);
}

void FileChooserDialog::setDisplayedDirectory(const QString& directory)
{
    mCurrentDirectory.setCurrent(directory);
    ui->fileListView->setRootIndex(mFileSystemModel.index(directory));

}
