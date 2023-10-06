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

#include <QJsonObject>

#include "script-handler.h"

// ==============================
// SCRIPT HANDLER CONSTRUCTOR:
// ==============================
QScriptHandler::QScriptHandler(QJsonObject scriptJsonObject)
    : QObject(0)
{
    id = scriptJsonObject["id"].toString();

    scriptFullFilePath =
            qApp->property("appDir").toString() + "/" +
            scriptJsonObject["scriptRelativePath"].toString();

    // Signals and slots for local Perl scripts:
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

//    QObject::connect(&scriptProcess,
//                     SIGNAL(finished(int, QProcess::ExitStatus)),
//                     this,
//                     SLOT(qScriptFinishedSlot())
//                     );

    process.setWorkingDirectory(qApp->property("appDir").toString());

    process.start((qApp->property("perlInterpreter").toString()),
                        QStringList() << scriptFullFilePath,
                        QProcess::Unbuffered | QProcess::ReadWrite);
}
