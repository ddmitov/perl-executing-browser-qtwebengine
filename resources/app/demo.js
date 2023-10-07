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

// Settings objects for Perl scripts:
const perlInfo = {}
perlInfo.scriptRelativePath = 'perl_scripts/perl_info.pl'

perlInfo.stdoutFunction = function (stdout) {
  const newElement = document.createElement('pre')
  newElement.innerHTML = stdout
  document.getElementById('perl-info-output').appendChild(newElement)
  document.getElementById('perl-info-button').style.display = 'none'
}

const openFile = {}
openFile.scriptRelativePath = 'perl_scripts/open_file.pl'
openFile.stdoutFunction = function (stdout) {
  displayTestResult('open-file', stdout)
}

const newFile = {}
newFile.scriptRelativePath = 'perl_scripts/new_file.pl'
newFile.stdoutFunction = function (stdout) {
  displayTestResult('new-file', stdout)
}

const openFiles = {}
openFiles.scriptRelativePath = 'perl_scripts/open_files.pl'
openFiles.stdoutFunction = function (stdout) {
  displayTestResult('open-files', stdout)
}

const openDirectory = {}
openDirectory.scriptRelativePath = 'perl_scripts/open_directory.pl'
openDirectory.stdoutFunction = function (stdout) {
  displayTestResult('open-directory', stdout)
}

// Settings objects for filesystem dialogs:
const selectFile = {}
selectFile.type = 'single-file'
selectFile.receiverFunction = function (fileName) {
  openFile.inputData = fileName

  clearTestData()

  const form = document.createElement('form')
  form.setAttribute('action', 'openFile.script')
  document.body.appendChild(form)
  form.submit()
}

const newFilename = {}
newFilename.type = 'new-file-name'
newFilename.receiverFunction = function (fileName) {
  newFile.inputData = fileName

  clearTestData()

  const form = document.createElement('form')
  form.setAttribute('action', 'newFile.script')
  document.body.appendChild(form)
  form.submit()
}

const selectFiles = {}
selectFiles.type = 'multiple-files'
selectFiles.receiverFunction = function (fileNames) {
  openFiles.inputData = fileNames

  clearTestData()

  const form = document.createElement('form')
  form.setAttribute('action', 'openFiles.script')
  document.body.appendChild(form)
  form.submit()
}

const selectDirectory = {}
selectDirectory.type = 'directory'
selectDirectory.receiverFunction = function (directoryName) {
  openDirectory.inputData = directoryName

  clearTestData()

  const form = document.createElement('form')
  form.setAttribute('action', 'openDirectory.script')
  document.body.appendChild(form)
  form.submit()
}

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
