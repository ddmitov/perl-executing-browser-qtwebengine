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

#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QUrl>
#include <QWebEnginePage>

#include "file-reader.h"
#include "script-handler.h"

// ==============================
// WEB PAGE CLASS DEFINITION:
// (QTWEBENGINE VERSION)
// ==============================
class QPage : public QWebEnginePage
{
    Q_OBJECT

signals:

    void pageLoadedSignal();
    void closeWindowSignal();

public slots:

    void qPageLoadedSlot(bool ok)
    {
        if (ok) {
            if (QPage::url().scheme() == "file") {
                // Inject all browser-specific Javascript:
                QFileReader *resourceReader =
                        new QFileReader(QString(":/peb.js"));

                QString pebJavaScript = resourceReader->fileContents;

                QPage::runJavaScript(pebJavaScript);

                QPage::runJavaScript(QString("peb.getPageSettings()"),
                                     [&](QVariant result){
                    qGetPageSettings(result);
                });

                // Send signal to the html-viewing class that a page is loaded:
                emit pageLoadedSignal();
            }
        }
    }

    // ==============================
    // Page settings:
    // ==============================
    void qGetPageSettings(QVariant settingsJsResult) {
        QJsonDocument settingsJsonDocument =
                QJsonDocument::fromJson(settingsJsResult.toString().toUtf8());

        if (!settingsJsonDocument.isEmpty()) {
            QJsonObject settingsJsonObject = settingsJsonDocument.object();

            // Get Perl interpreter:
            QString perlInterpreter;
            QString perlInterpreterSetting =
                    settingsJsonObject["perlInterpreter"].toString();

            if (perlInterpreterSetting.length() > 0) {
                perlInterpreter =
                        qApp->property("appDir").toString() + '/' +
                        settingsJsonObject["perlInterpreter"].toString();
            }

            if (perlInterpreterSetting.length() == 0) {
                perlInterpreter = "perl";
            }

            qApp->setProperty("perlInterpreter", perlInterpreter);

            // Get dialog and context menu labels:
            if (settingsJsonObject["okLabel"].toString().length() > 0) {
                okLabel = settingsJsonObject["okLabel"].toString();
            }

            if (settingsJsonObject["cancelLabel"].toString().length() > 0) {
                cancelLabel = settingsJsonObject["cancelLabel"].toString();
            }

            if (settingsJsonObject["yesLabel"].toString().length() > 0) {
                yesLabel = settingsJsonObject["yesLabel"].toString();
            }

            if (settingsJsonObject["noLabel"].toString().length() > 0) {
                noLabel = settingsJsonObject["noLabel"].toString();
            }

            if (settingsJsonObject["cutLabel"].toString().length() > 0) {
                qApp->setProperty("cutLabel",
                                  settingsJsonObject["cutLabel"].toString());
            }

            if (settingsJsonObject["copyLabel"].toString().length() > 0) {
                qApp->setProperty("copyLabel",
                                  settingsJsonObject["copyLabel"].toString());
            }

            if (settingsJsonObject["pasteLabel"].toString().length() > 0) {
                qApp->setProperty("pasteLabel",
                                  settingsJsonObject["pasteLabel"].toString());
            }

            if (settingsJsonObject["selectAllLabel"].toString().length() > 0) {
                qApp->setProperty(
                            "selectAllLabel",
                            settingsJsonObject["selectAllLabel"].toString());
            }
        }
    }

    // ==============================
    // Filesystem dialogs:
    // ==============================
    void qHandleDialogs(QString dialogObjectName)
    {
        QString dialogSettingsJavaScript =
                "peb.getDialogSettings(" + dialogObjectName + ")";

        QPage::runJavaScript(dialogSettingsJavaScript,
                             [dialogObjectName, this](QVariant dialogSettings)
        {
            QJsonDocument dialogJsonDocument =
                    QJsonDocument::fromJson(
                        dialogSettings.toString().toUtf8());

            if (!dialogJsonDocument.isEmpty()) {
                QJsonObject dialogJsonObject = dialogJsonDocument.object();
                dialogJsonObject["id"] = dialogObjectName;

                qReadDialogSettings(dialogJsonObject);
            }
        });
    }

    void qReadDialogSettings(QJsonObject dialogJsonObject)
    {
        QString id = dialogJsonObject["id"].toString();
        QString type = dialogJsonObject["type"].toString();

        QFileDialog inodesDialog (qApp->activeWindow());
        inodesDialog.setWindowModality(Qt::WindowModal);
        inodesDialog.setViewMode(QFileDialog::Detail);

        if (type == "single-file") {
            inodesDialog.setFileMode(QFileDialog::AnyFile);
        }

        if (type == "multiple-files") {
            inodesDialog.setFileMode(QFileDialog::ExistingFiles);
        }

        if (type == "new-file-name") {
            inodesDialog.setAcceptMode(QFileDialog::AcceptSave);
        }

        if (type == "directory") {
            inodesDialog.setFileMode(QFileDialog::Directory);
        }

        QStringList selectedInodes;

        if (inodesDialog.exec()) {
            selectedInodes = inodesDialog.selectedFiles();
        }

        inodesDialog.close();
        inodesDialog.deleteLater();

        if (!selectedInodes.isEmpty()) {
            QString inodesFormatted;

            foreach (QString userSelectedInode, selectedInodes) {
                inodesFormatted.append(userSelectedInode);
                inodesFormatted.append(";");
            }

            inodesFormatted.replace(QRegularExpression(";$"), "");

            QString outputInsertionJavaScript =
                    id + ".receiverFunction('" + inodesFormatted + "'); null";

            QPage::runJavaScript(outputInsertionJavaScript);
        }
    }

    // ==============================
    // Perl scripts:
    // ==============================

    void qStartScript(QString scriptObjectName)
    {
        if (QPage::url().scheme() == "file") {
            QString scriptSettingsJavaScript =
                    "peb.getScriptSettings(" + scriptObjectName + ")";

            QPage::runJavaScript(scriptSettingsJavaScript,
                                 [scriptObjectName, this]
                                 (QVariant scriptSettings)
            {
                QJsonDocument scriptJsonDocument =
                        QJsonDocument::fromJson(scriptSettings
                                                .toString().toUtf8());

                if (!scriptJsonDocument.isEmpty()) {
                    QJsonObject scriptJsonObject = scriptJsonDocument.object();

                    QScriptHandler *scriptHandler =
                            new QScriptHandler(scriptJsonObject);

                    scriptHandler->id = scriptObjectName;

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

                    QString scriptInput =
                            scriptJsonObject["scriptInput"].toString();

                    if (scriptInput.length() > 0) {
                        if (scriptHandler->process.isOpen()) {
                            scriptHandler->process
                                    .write(scriptInput.toUtf8());
                            scriptHandler->process
                                    .write(QString("\n").toLatin1());
                        }
                    }
                }
            });
        }
    }

    void qDisplayScriptOutputSlot(QString id, QString output)
    {
        if (QPage::url().scheme() == "file") {
            QString outputInsertionJavaScript =
                        id + ".stdoutFunction('" + output + "'); null";

            QPage::runJavaScript(outputInsertionJavaScript);
        }
    }

    void qDisplayScriptErrorsSlot(QString errors)
    {
        if (QPage::url().scheme() == "file") {
            if (errors.length() > 0) {
                errors.replace("\"", "\\\"");
                errors.replace("\'", "\\'");
                errors.replace("\n", "\\n");
                errors.replace("\r", "");

                QString perlScriptErrorMessage =
                        "console.error('" + errors + "'); null";

                QPage::runJavaScript(perlScriptErrorMessage);
            }
        }
    }

    // ==============================
    // Page-closing routine:
    // ==============================
    void qStartWindowClosingSlot()
    {
        if (QPage::url().scheme() == "file") {
            QPage::runJavaScript(QString("peb.checkUserInputBeforeClose()"),
                                 [&](QVariant jsResult){

                if (jsResult.toByteArray().length() > 0) {
                    if (jsResult.toBool() == true) {
                        emit closeWindowSignal();
                    }
                }
            });
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
        alert.setButtonText(QMessageBox::Ok, okLabel);
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
        messageBox.setButtonText(QMessageBox::Yes, yesLabel);
        messageBox.setButtonText(QMessageBox::No, noLabel);

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
        prompt.setOkButtonText(okLabel);
        prompt.setCancelButtonText(cancelLabel);

        if (prompt.exec() == QDialog::Accepted) {
            *result = prompt.textValue();
            ok = true;

            return ok;
        }

        return ok;
    }

private:

    QString okLabel;
    QString cancelLabel;
    QString yesLabel;
    QString noLabel;

public:

    QPage();

};

#endif // PAGE_H
