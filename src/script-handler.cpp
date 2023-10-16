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

#include <QJsonObject>

#include "script-handler.h"

// ==============================
// SCRIPT HANDLER CONSTRUCTOR:
// ==============================
QScriptHandler::QScriptHandler(QString scriptId, QJsonObject scriptJsonObject)
    : QObject(0)
{
    // Script ID:
    id = scriptId;

    // Set Perl script full path:
    QString scriptFullFilePath =
            qApp->property("appDir").toString() + "/" +
            scriptJsonObject["scriptRelativePath"].toString();

    // Signals and slots for Perl script STDOUT and STDERR:
    QObject::connect(&process,
                     SIGNAL(readyReadStandardOutput()),
                     this,
                     SLOT(qScriptOutputSlot())
                     );

    QObject::connect(&process,
                     SIGNAL(readyReadStandardError()),
                     this,
                     SLOT(qScriptErrorsSlot())
                     );

    // Set Perl script working directory:
    process.setWorkingDirectory(qApp->property("appDir").toString());

    // Get Perl interpreter:
    QString perlInterpreterSetting =
            scriptJsonObject["perlInterpreter"].toString();

    QString perlInterpreter;

    if (perlInterpreterSetting.length() > 0) {
        perlInterpreter =
                qApp->property("appDir").toString() + '/' +
                scriptJsonObject["perlInterpreter"].toString();
    }

    if (perlInterpreterSetting.length() == 0) {
        perlInterpreter = "perl";
    }

    // Start Perl script:
    process.start(perlInterpreter,
                  QStringList() << scriptFullFilePath,
                  QProcess::Unbuffered | QProcess::ReadWrite);
}
