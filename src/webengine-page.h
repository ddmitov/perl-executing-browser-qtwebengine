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
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QWebEnginePage>

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

public slots:

    void qPageLoadedSlot(bool ok)
    {
        if (ok) {
            // Send signal that page is loaded:
            emit pageLoadedSignal();

            QString pageSettingsJavaScript =
                    "if (typeof pebSettings !== 'undefined') {"
                        "JSON.stringify(pebSettings)"
                    "}";

            // Get page settings:
            QPage::runJavaScript(pageSettingsJavaScript,
                                 0,
                                 [&](QVariant pageSettingsJsResult)
            {
                qGetPageSettings(pageSettingsJsResult);
            }
            );
        }
    }

    // ==============================
    // Page settings:
    // ==============================
    void qGetPageSettings(QVariant pageSettingsJsResult) {
        QJsonDocument pageSettingsJsonDoc =
                QJsonDocument::fromJson(
                    pageSettingsJsResult.toString().toUtf8()
                    );

        if (!pageSettingsJsonDoc.isEmpty()) {
            QJsonObject pageSettings = pageSettingsJsonDoc.object();

            if (pageSettings["okLabel"].toString().length() > 0) {
                okLabel = pageSettings["okLabel"].toString();
            }

            if (pageSettings["cancelLabel"].toString().length() > 0) {
                cancelLabel = pageSettings["cancelLabel"].toString();
            }

            if (pageSettings["yesLabel"].toString().length() > 0) {
                yesLabel = pageSettings["yesLabel"].toString();
            }

            if (pageSettings["noLabel"].toString().length() > 0) {
                noLabel = pageSettings["noLabel"].toString();
            }

            if (pageSettings["cutLabel"].toString().length() > 0) {
                qApp->setProperty(
                            "cutLabel",
                            pageSettings["cutLabel"].toString()
                        );
            }

            if (pageSettings["copyLabel"].toString().length() > 0) {
                qApp->setProperty(
                            "copyLabel",
                            pageSettings["copyLabel"].toString()
                        );
            }

            if (pageSettings["pasteLabel"].toString().length() > 0) {
                qApp->setProperty(
                            "pasteLabel",
                            pageSettings["pasteLabel"].toString()
                        );
            }

            if (pageSettings["selectAllLabel"].toString().length() > 0) {
                qApp->setProperty(
                            "selectAllLabel",
                            pageSettings["selectAllLabel"].toString());
            }
        }
    }

    // ==============================
    // Filesystem dialogs:
    // ==============================
    QString qDisplayDialog(QJsonObject dialogJsonObject)
    {
        QString type = dialogJsonObject["type"].toString();

        QFileDialog inodesDialog (qApp->activeWindow());

        inodesDialog.setParent(qApp->activeWindow());
        inodesDialog.setWindowModality(Qt::WindowModal);
        inodesDialog.setViewMode(QFileDialog::Detail);

        if (type == "single-file") {
            inodesDialog.setFileMode(QFileDialog::AnyFile);
        }

        if (type == "new-file-name") {
            inodesDialog.setAcceptMode(QFileDialog::AcceptSave);
        }

        if (type == "multiple-files") {
            inodesDialog.setFileMode(QFileDialog::ExistingFiles);
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

        QString inodesFormatted = "";

        if (!selectedInodes.isEmpty()) {
            foreach (QString userSelectedInode, selectedInodes) {
                inodesFormatted.append(userSelectedInode);
                inodesFormatted.append(";");
            }

            // Remove the final ";" from the output:
            inodesFormatted.replace(-1, 1, "");
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

                // Get script input, if any:
                QString scriptInput =
                        scriptJsonObject["scriptInput"].toString();

                if (scriptInput == "dialog") {
                    QJsonObject dialog = scriptJsonObject["dialog"].toObject();

                    scriptInput = qDisplayDialog(dialog);
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
