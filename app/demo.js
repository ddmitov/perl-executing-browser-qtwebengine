// Perl Executing Browser QtWebEngine Demo

// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License,
// as published by the Free Software Foundation;
// either version 3 of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.
// Dimitar D. Mitov, 2018 - 2019, 2023
// https://github.com/ddmitov/perl-executing-browser-qtwebengine

// Settings object for perl_info.pl:
const perlInfo = {}

perlInfo.scriptRelativePath = 'perl_scripts/perl_info.pl'

perlInfo.stdoutFunction = function (stdout) {
  const newElement = document.createElement('pre')
  newElement.innerHTML = stdout
  document.getElementById('perl-info-output').appendChild(newElement)
  document.getElementById('perl-info-button').style.display = 'none'
}

// Settings object for open_file.pl:
const openFile = {}

openFile.scriptRelativePath = 'perl_scripts/open_file.pl'

openFile.scriptInput = 'dialog'

openFile.dialog = {}
openFile.dialog.type = 'single-file'
// openFile.dialog.title = 'Select Single File'

openFile.stdoutFunction = function (stdout) {
  displayTestResult('open-file', stdout)
}

// Settings object for new_file.pl:
const newFile = {}

newFile.scriptRelativePath = 'perl_scripts/new_file.pl'

newFile.scriptInput = 'dialog'

newFile.dialog = {}
newFile.dialog.type = 'new-file-name'
// newFile.dialog.title = 'New File'

newFile.stdoutFunction = function (stdout) {
  displayTestResult('new-file', stdout)
}

// Settings object for open_files.pl:
const openFiles = {}

openFiles.scriptRelativePath = 'perl_scripts/open_files.pl'

openFiles.scriptInput = 'dialog'

openFiles.dialog = {}
openFiles.dialog.type = 'multiple-files'
// openFiles.dialog.title = 'Select Multiple Files'

openFiles.stdoutFunction = function (stdout) {
  displayTestResult('open-files', stdout)
}

// Settings object for open_directory.pl:
const openDirectory = {}

openDirectory.scriptRelativePath = 'perl_scripts/open_directory.pl'

openDirectory.scriptInput = 'dialog'

openDirectory.dialog = {}
openDirectory.dialog.type = 'directory'
// openDirectory.dialog.title = 'Select Directory'

openDirectory.stdoutFunction = function (stdout) {
  displayTestResult('open-directory', stdout)
}

// Helper functions:
function clearTestData () {
  const container = document.getElementById('filesystem-tests')

  while (container.firstChild) {
    container.removeChild(container.firstChild)
  }
}

function displayTestResult (id, stdout) {
  const existingElement = document.getElementById(id)

  if (existingElement === null) {
    clearTestData()

    const newElement = document.createElement('pre')
    newElement.id = id
    newElement.innerHTML = stdout
    document.getElementById('filesystem-tests').appendChild(newElement)
  } else {
    existingElement.innerHTML = existingElement.innerHTML + stdout
  }
}
