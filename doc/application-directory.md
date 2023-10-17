# Perl Executing Browser QtWebEngine - Application Directory

PEB is created to work from any directory and all file paths used by PEB are relative to the directory of the PEB executable.  

PEB Application Directory looks like this:

```bash
.
├── {PEB_executable}
└── app
    ├── index.html
    └── app.png
```

The PEB Application Directory must be named ``app`` and it must be located in the directory of the PEB executable.

All Perl scripts started by PEB must be located within the PEB Application Directory and its subdirectories. The working directory of all PEB Perl scripts is the PEB Application Directory.

When started, PEB is trying to find the following files in its Application Directory:

* **Start Page:**  
  Start page pathname must be: ``{PEB Application Directory}/app/index.html``  

  If start page is missing, an error message is displayed.  

* **Icon:**
  Icon pathname must be: ``{PEB Application Directory}/app/app.png``  

  Icon file is optional.  
  If icon file is found on application startup, it is used as application icon.  
  If icon file is not found, the default icon embedded in the resources of the PEB binary is used.
