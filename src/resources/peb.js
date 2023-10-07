// Perl Executing Browser

// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU Lesser General Public License,
// as published by the Free Software Foundation;
// either version 3 of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.
// Dimitar D. Mitov, 2013 - 2020, 2023
// https://github.com/ddmitov/perl-executing-browser-qtwebengine

// PEB embedded JavaScript code:
const peb = {}

peb.getPageSettings = function () {
  if (window.pebSettings !== null) {
    return JSON.stringify(window.pebSettings)
  }
}

peb.getDialogSettings = function (dialogSettings) {
  if (window[dialogSettings] !== null) {
    return JSON.stringify(dialogSettings)
  }
}

peb.getScriptSettings = function (scriptSettings) {
  if (window[scriptSettings] !== null) {
    if (typeof scriptSettings.inputData === 'function') {
      scriptSettings.scriptInput = scriptSettings.inputData()
    } else {
      scriptSettings.scriptInput = scriptSettings.inputData
    }

    return JSON.stringify(scriptSettings)
  }
}

peb.checkUserInputBeforeClose = function () {
  let textEntered = false
  let close = true

  const textFields = document.getElementsByTagName('textarea')

  for (let index = 0; index < textFields.length; index++) {
    if (textFields[index].value.length > 0) {
      textEntered = true
    }
  }

  const inputBoxes = document.querySelectorAll('input[type=text]')

  for (let index = 0; index < inputBoxes.length; index++) {
    if (inputBoxes[index].value.length > 0) {
      textEntered = true
    }
  }

  if (textEntered === true && window.pebSettings.closeConfirmation !== null) {
    if (typeof window.pebSettings.closeConfirmation === 'function') {
      close = confirm(window.pebSettings.closeConfirmation)
    }
  }

  return close
}
