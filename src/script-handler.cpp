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
#include <QJsonObject>

#include "script-handler.h"

// ==============================
// SCRIPT HANDLER CONSTRUCTOR
// ==============================
QScriptHandler::QScriptHandler(QString scriptId, QJsonObject scriptJsonObject)
    : QObject(0)
{
    // Script ID:
    id = scriptId;

    // Script full path:
    QString scriptFullFilePath =
            qApp->property("appDir").toString() + "/" +
            scriptJsonObject["scriptRelativePath"].toString();

    // Signals and slots for script STDOUT and STDERR:
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

    // Script working directory:
    process.setWorkingDirectory(qApp->property("appDir").toString());

    // Perl interpreter:
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

    // Start script:
    process.start(perlInterpreter,
                  QStringList() << scriptFullFilePath,
                  QProcess::Unbuffered | QProcess::ReadWrite);

    // Get script input, if any:
    QString scriptInput = scriptJsonObject["scriptInput"].toString();

    if (scriptInput.length() > 0) {
        QStringList tagList;

        tagList.append("{existing-file}");
        tagList.append("{new-file}");
        tagList.append("{directory}");

        foreach (QString tag, tagList) {
            while (scriptInput.contains(tag)) {
                QString replacement = displayInodeDialog(tag);

                scriptInput.replace(scriptInput.indexOf(tag),
                                    tag.size(),
                                    replacement
                                    );
            }
        }

        if (process.isOpen()) {
            process.write(scriptInput.toUtf8());
            process.write(QString("\n").toLatin1());
        }
    }
}
