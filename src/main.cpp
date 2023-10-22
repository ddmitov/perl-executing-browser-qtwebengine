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

#include <QApplication>
#include <QTextCodec>
#include <QtGlobal>

#include "main-window.h"

// ==============================
// APPLICATION DEFINITION
// ==============================
int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    // Application version:
    application.setApplicationVersion("1.1.0");

    // UTF-8 encoding application-wide:
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    // Application directory:
    QString browserDirectory = application.applicationDirPath().toLatin1();
    QString applicationDirName = browserDirectory + "/app";

    application.setProperty("appDir", applicationDirName);

    // Application icon:
    QString iconPathName = applicationDirName + "/app.png";

    QPixmap icon(32, 32);
    QFile iconFile(iconPathName);

    if (iconFile.exists()) {
        icon.load(iconPathName);
        QApplication::setWindowIcon(icon);
    } else {
        // Set the embedded default icon
        // in case no external icon file is found:
        icon.load(":/camel.png");
        QApplication::setWindowIcon(icon);
    }

    // Main window initialization:
    QMainBrowserWindow mainWindow;

    mainWindow.setWindowIcon(icon);
    mainWindow.setCentralWidget(mainWindow.mainViewWidget);
    mainWindow.showMaximized();

    // Start page:
    QString startPageFilePath = applicationDirName + "/index.html";
    QFile startPageFile(startPageFilePath);

    if (startPageFile.exists()) {
        mainWindow.mainViewWidget->setUrl(
                    QUrl::fromLocalFile(startPageFilePath));
    }

    if (!startPageFile.exists()) {
        mainWindow.qDisplayErrorSlot(QString("No start page is found."));
    }

    return application.exec();
}
