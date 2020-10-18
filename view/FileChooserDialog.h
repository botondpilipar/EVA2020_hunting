#ifndef FILECHOOSERDIALOG_H
#define FILECHOOSERDIALOG_H

#include <QObject>
#include <QDialog>
#include <QFileSystemModel>
#include "ui_FileChooserDialog.h"

class FileChooserDialog : public QDialog
{
    Q_OBJECT
public:
    FileChooserDialog();

    void setDisplayedDirectory(const QString& directory);
    QStringList getDisplayedFiles() const;

private:
    std::unique_ptr<Ui::Dialog> ui;
    QDir mCurrentDirectory;
    QFileSystemModel mFileSystemModel;

signals:

};

#endif // FILECHOOSERDIALOG_H
