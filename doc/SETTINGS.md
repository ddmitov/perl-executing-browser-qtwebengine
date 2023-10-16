# Perl Executing Browser QtWebEngine - Settings

Every Perl script run by PEB must have a JavaScript settings object with an arbitrary name and fixed object properties. The name of the JavaScript settings object with a ``.script`` extension forms a pseudo link used to start the corresponding Perl script.  

There are two methods to start a local Perl script:  

* **Click a pseudo link:**  
  ```html
  <a href="example.script">Start Perl script</a>
  ```

* **Submit a form to a pseudo link:**  

  ```html
  <form action="example.script">
    <input type="submit" value="Start Perl script">
  </form>
  ```

A minimal example of a JavaScript settings object for a Perl script run by PEB:  

```javascript
const example = {}

example.scriptRelativePath = 'relative/path/to/script.pl'

example.stdoutFunction = function (stdout) {
  const container = document.getElementById('DOM-element-id')
  container.innerText = stdout
}
```

A JavaScript settings object for a Perl script run by PEB must have the following basic properties:

* **scriptRelativePath**  
  ``String`` for the relative path of a Perl script run by PEB  

  The relative path of a script is converted to a full path using the ``{PEB_executable_directory}/app`` as a root folder. PEB does not check filename extensions or shebang lines of Perl scripts. Scripts without filename extensions can also be used.  

* **stdoutFunction**  
  ``function`` executed every time data is available on Pel script STDOUT  

  The only parameter passed to the ``stdoutFunction`` is the STDOUT ``String``.  

  An example of an immediate STDOUT data display without accumulation:

  ```javascript
  example.stdoutFunction = function (stdout) {
    document.getElementById('DOM-element-id').textContent = stdout
  }
  ```

  Please note that many Perl scripts do not give their STDOUT data in a single shot.  
  If several chunks of output have to be combined, this must be done at JavaScript level:  

  ```javascript
  let accumulatedOutput

  example.stdoutFunction = function (stdout) {
    accumulatedOutput = accumulatedOutput + stdout
    document.getElementById('DOM-element-id').textContent = accumulatedOutput
  };
  ```

A JavaScript settings object for a Perl script run by PEB may also have the following additional properties:

* **perlInterpreter**  
  ``String`` for the relative path of a relocatable Perl interpreter used by PEB  

  The relative path of a relocatable Perl interpreter is converted to a full path using the ``{PEB_executable_directory}/app`` path as a root folder. If a relocatable Perl interpreter is not configured, PEB will use the first Perl interpreter on PATH.  

* **scriptInput**  
  ``String``  

  ``scriptInput`` is written on Perl script STDIN.  

  ``scriptInput = 'dialog'`` means that a file or directory selection dialog defined by the ``dialog.type`` property will be displayed before starting a Perl script.

* **dialog.type**  
  ``String`` containing one of the following:

  * ``single-file``  
  The actual opening of an existing file is performed by a Perl script and not by PEB.  

  * ``multiple-files``  
  When multiple files are selected, different filenames are separated by a semicolon ``;``  

  * ``new-file-name``  
  The actual creation of a new file is performed by a Perl script and not by PEB.  

  * ``directory``  
  When ``directory`` type of dialog is used, an existing or a new directory may be selected.  
  Any new directory will be immediately created by PEB.
