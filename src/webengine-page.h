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
 Dimitar D. Mitov, 2018 - 2019
 https://github.com/ddmitov/perl-executing-browser-qtwebengine
*/

#ifndef PAGE_H
#define PAGE_H

#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegExp>
#include <QWebEnginePage>

#include "script-handler.h"

// ==============================
// WEB PAGE CLASS DEFINITION:
// ==============================
class QPage : public QWebEnginePage
{
    Q_OBJECT

public slots:

    // ==============================
    // Filesystem dialogs:
    // ==============================
    QString displayInodeDialog(QString filesystemInput)
    {
        QFileDialog inodesDialog(qApp->activeWindow());

        inodesDialog.setParent(qApp->activeWindow());
        inodesDialog.setOption(QFileDialog::DontUseNativeDialog);
        inodesDialog.setWindowModality(Qt::WindowModal);
        inodesDialog.setViewMode(QFileDialog::Detail);

        if (filesystemInput == "single-file") {
            inodesDialog.setFileMode(QFileDialog::AnyFile);
        }

        if (filesystemInput == "new-file-name") {
            inodesDialog.setAcceptMode(QFileDialog::AcceptSave);
        }

        if (filesystemInput == "multiple-files") {
            inodesDialog.setFileMode(QFileDialog::ExistingFiles);
        }

        if (filesystemInput == "directory") {
            inodesDialog.setFileMode(QFileDialog::Directory);
        }

        QStringList selectedInodes;

        if (inodesDialog.exec()) {
            selectedInodes = inodesDialog.selectedFiles();
        }

        inodesDialog.close();
        inodesDialog.deleteLater();

        QString inodesFormatted = "";

        if (!selectedInodes.isEmpty()) {
            foreach (QString userSelectedInode, selectedInodes) {
                inodesFormatted.append(userSelectedInode);
                inodesFormatted.append(";");
            }

            // Remove the final ";" from filesystem dialog output:
            inodesFormatted.replace(QRegExp(";$"), "");
        }

        return inodesFormatted;
    }

    // ==============================
    // Perl scripts:
    // ==============================

    void qStartScript(QString scriptObjectName)
    {
        QPage::runJavaScript("JSON.stringify(" + scriptObjectName + ")",
                             0,
                             [scriptObjectName, this](QVariant scriptSettings)
        {
            QJsonDocument scriptJsonDocument =
                    QJsonDocument::fromJson(scriptSettings.toString().toUtf8());

            if (!scriptJsonDocument.isEmpty()) {
                QJsonObject scriptJsonObject = scriptJsonDocument.object();

                QScriptHandler *scriptHandler =
                        new QScriptHandler(scriptObjectName, scriptJsonObject);

                QObject::connect(scriptHandler,
                                 SIGNAL(displayScriptOutputSignal(QString,
                                                                  QString)),
                                 this,
                                 SLOT(qDisplayScriptOutputSlot(QString,
                                                               QString))
                                 );

                QObject::connect(scriptHandler,
                                 SIGNAL(displayScriptErrorsSignal(QString)),
                                 this,
                                 SLOT(qDisplayScriptErrorsSlot(QString))
                                 );

                // Get script input, if any:
                QString scriptInput =
                        scriptJsonObject["scriptInput"].toString();

                QString filesystemInput =
                        scriptJsonObject["filesystemInput"].toString();

                if (filesystemInput.length() > 0) {
                    scriptInput = displayInodeDialog(filesystemInput);
                }

                if (scriptInput.length() > 0) {
                    if (scriptHandler->process.isOpen()) {
                        scriptHandler->process.write(scriptInput.toUtf8());
                        scriptHandler->process.write(QString("\n").toLatin1());
                    }
                }
            }
        }
        );
    }

    void qDisplayScriptOutputSlot(QString id, QString output)
    {
        QPage::runJavaScript(id + ".stdoutFunction('" + output + "')", 0);
    }

    void qDisplayScriptErrorsSlot(QString errors)
    {
        if (errors.length() > 0) {
            errors.replace("\"", "\\\"");
            errors.replace("\'", "\\'");
            errors.replace("\n", "\\n");
            errors.replace("\r", "");

            QPage::runJavaScript("console.error('" + errors + "')", 0);
        }
    }

protected:

    bool acceptNavigationRequest(const QUrl &url,
                                 QWebEnginePage::NavigationType navType,
                                 bool isMainFrame
                                 ) override;

    // ==============================
    // JavaScript Alert:
    // ==============================
    virtual void javaScriptAlert(const QUrl &url, const QString &msg)
    override
    {
        Q_UNUSED(url);

        QMessageBox alert (qApp->activeWindow());

        alert.setParent(qApp->activeWindow());
        alert.setWindowModality(Qt::WindowModal);
        alert.setWindowTitle(title());
        alert.setText(msg);
        alert.setButtonText(QMessageBox::Ok, "OK");
        alert.setDefaultButton(QMessageBox::Ok);

        alert.exec();
    }

    // ==============================
    // JavaScript Confirm:
    // ==============================
    virtual bool javaScriptConfirm(const QUrl &url, const QString &msg)
    override
    {
        Q_UNUSED(url);

        QMessageBox messageBox (qApp->activeWindow());

        messageBox.setParent(qApp->activeWindow());
        messageBox.setWindowModality(Qt::WindowModal);
        messageBox.setWindowTitle(title());
        messageBox.setText(msg);
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setButtonText(QMessageBox::Yes, "Yes");
        messageBox.setButtonText(QMessageBox::No, "No");

        return QMessageBox::Yes == messageBox.exec();
    }

    // ==============================
    // JavaScript Prompt:
    // ==============================
    virtual bool javaScriptPrompt(const QUrl &url, const QString &msg,
                                  const QString &defaultValue, QString *result)
    override
    {
        Q_UNUSED(url);

        bool ok = false;

        QInputDialog prompt;

        prompt.setParent(qApp->activeWindow());
        prompt.setModal(true);
        prompt.setWindowTitle(title());
        prompt.setLabelText(msg);
        prompt.setInputMode(QInputDialog::TextInput);
        prompt.setTextValue(defaultValue);
        prompt.setOkButtonText("OK");
        prompt.setCancelButtonText("Cancel");

        if (prompt.exec() == QDialog::Accepted) {
            *result = prompt.textValue();
            ok = true;

            return ok;
        }

        return ok;
    }

public:

    QPage();

};

#endif // PAGE_H
