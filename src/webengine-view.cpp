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

#include <webengine-view.h>

// ==============================
// VIEW CLASS CONSTRUCTOR:
// ==============================
QViewWidget::QViewWidget()
    : QWebEngineView(0)
{
    mainPage = new QPage();

    // Page Loaded signal and slot:
    QObject::connect(mainPage,
                     SIGNAL(pageLoadedSignal()),
                     this,
                     SLOT(qPageLoadedSlot())
                     );

    setPage(mainPage);
}
