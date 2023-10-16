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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "file-reader.h"
#include "webengine-view.h"

// ==============================
// MAIN WINDOW CLASS DEFINITION:
// (QTWEBENGINE VERSION)
// ==============================
class QMainBrowserWindow : public QMainWindow
{
    Q_OBJECT

signals:

    void startMainWindowClosingSignal();

public slots:

    void qDisplayErrorSlot(QString errorMessage)
    {
        QFileReader *resourceReader = new QFileReader(QString(":/error.html"));

        QString htmlErrorContents = resourceReader->fileContents;
        htmlErrorContents.replace("ERROR_MESSAGE", errorMessage);

        webViewWidget->setHtml(htmlErrorContents);
    }

    void setMainWindowTitleSlot(QString title)
    {
        setWindowTitle(title);
    }

public:

    QWebEngineView *webViewWidget;

    explicit QMainBrowserWindow(QWidget *parent = 0);
};

#endif // MAIN_WINDOW_H
