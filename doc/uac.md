# The Problem

Hotkeys don't work when UAC-elevated app (read: pretty much any installer there is) grabs focus. Even tray wheel events stop working.


# Solution

It's possible to run qnob elevated, but this will trigger an UAC dialog every time. Not the best solution for startup apps. This is the command:
```
powershell -Command "Start-Process qnob.exe -Verb runAs"
```

Another option is to use [scheduled tasks](https://www.digitalcitizen.life/use-task-scheduler-launch-programs-without-uac-prompts/). It's possible to do this from C++ btw, probably worth researching.
