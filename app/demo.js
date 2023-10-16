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

openFile.filesystemInput = 'single-file'

openFile.stdoutFunction = function (stdout) {
  displayTestResult(stdout)
}

// Settings object for new_file.pl:
const newFile = {}

newFile.scriptRelativePath = 'perl_scripts/new_file.pl'

newFile.filesystemInput = 'new-file-name'

newFile.stdoutFunction = function (stdout) {
  displayTestResult(stdout)
}

// Settings object for open_files.pl:
const openFiles = {}

openFiles.scriptRelativePath = 'perl_scripts/open_files.pl'

openFiles.filesystemInput = 'multiple-files'

openFiles.stdoutFunction = function (stdout) {
  displayTestResult(stdout)
}

// Settings object for open_directory.pl:
const openDirectory = {}

openDirectory.scriptRelativePath = 'perl_scripts/open_directory.pl'

openDirectory.filesystemInput = 'directory'

openDirectory.stdoutFunction = function (stdout) {
  displayTestResult(stdout)
}

// Helper functions:
function displayTestResult (stdout) {
  clearTestData()

  const newElement = document.createElement('pre')
  newElement.innerHTML = stdout
  document.getElementById('filesystem-tests').appendChild(newElement)
}

function clearTestData () {
  const container = document.getElementById('filesystem-tests')

  while (container.firstChild) {
    container.removeChild(container.firstChild)
  }
}
