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

#include <QWebEngineSettings>

#include "webengine-page.h"

// ==============================
// PAGE CLASS CONSTRUCTOR
// ==============================
QPage::QPage()
    : QWebEnginePage()
{
    QWebEngineSettings::globalSettings()->
            setDefaultTextEncoding(QString("utf-8"));

    QWebEngineSettings::globalSettings()->
            setAttribute(
                QWebEngineSettings::JavascriptCanOpenWindows, false
                );

    QWebEngineSettings::globalSettings()->
            setAttribute(
                QWebEngineSettings::LocalContentCanAccessRemoteUrls, false
                );

    QWebEngineSettings::globalSettings()->
            setAttribute(
                QWebEngineSettings::JavascriptEnabled, true
                );

    QWebEngineSettings::globalSettings()->
            setAttribute(
                QWebEngineSettings::XSSAuditingEnabled, true
                );
}
