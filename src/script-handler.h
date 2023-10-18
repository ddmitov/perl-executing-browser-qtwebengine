/*
 Perl Executing Browser QtWebEngine

 This program is free software;
 you can redistribute it and/or modify it under the terms of the
 GNU Lesser General Public License,
 as published by the Free Software Foundation;
 either version 3 of the License, or (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.
 Dimitar D. Mitov, 2018 - 2019, 2023
 https://github.com/ddmitov/perl-executing-browser-qtwebengine
*/

#ifndef SCRIPT_HANDLER_H
#define SCRIPT_HANDLER_H

#include <QApplication>
#include <QFileDialog>
#include <QProcess>

// ==============================
// SCRIPT HANDLER
// ==============================
class QScriptHandler : public QObject
{
    Q_OBJECT

signals:

    void displayScriptOutputSignal(QString id, QString output);
    void displayScriptErrorsSignal(QString errors);

public slots:

    // Filesystem dialogs:
    QString displayInodeDialog(QString filesystemInput)
    {
        QFileDialog inodesDialog(qApp->activeWindow());

        inodesDialog.setParent(qApp->activeWindow());
        inodesDialog.setOption(QFileDialog::DontUseNativeDialog);
        inodesDialog.setWindowModality(Qt::WindowModal);
        inodesDialog.setViewMode(QFileDialog::Detail);

        if (filesystemInput == "{existing-file}") {
            inodesDialog.setFileMode(QFileDialog::AnyFile);
        }

        if (filesystemInput == "{new-file}") {
            inodesDialog.setAcceptMode(QFileDialog::AcceptSave);
        }

        if (filesystemInput == "{directory}") {
            inodesDialog.setFileMode(QFileDialog::Directory);
        }

        QString selectedInode;

        if (inodesDialog.exec()) {
            QStringList selectedInodes = inodesDialog.selectedFiles();

            if (!selectedInodes.isEmpty()) {
                selectedInode = "";
            }

            if (!selectedInodes.isEmpty()) {
                selectedInode = selectedInodes[0];
            }
        }

        inodesDialog.close();
        inodesDialog.deleteLater();

        return selectedInode;
    }

    // Perl script STDOUT slot:
    void qScriptOutputSlot()
    {
        QString scriptOutput = process.readAllStandardOutput();
        emit displayScriptOutputSignal(this->id, scriptOutput);
    }

    // Perl script STDERR slot:
    void qScriptErrorsSlot()
    {
        QString scriptErrors = process.readAllStandardError();
        emit displayScriptErrorsSignal(scriptErrors);
    }

private:

    QString id;

public:

    QScriptHandler(QString, QJsonObject);

    QProcess process;
};

#endif // SCRIPT_HANDLER_H
