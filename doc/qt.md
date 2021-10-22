# Building Qt for qnob

See docs on [building from Git](https://wiki.qt.io/Building_Qt_6_from_Git).

After checking out Qt source & applying the necessary patches, open MSVC developer command prompt (`vsvarsall.bat`), go to the build dir, and run:

```
..\src\configure -prefix ..\6.2.0_static -debug-and-release -force-debug-info -optimize-size -static -schannel -static-runtime -c++std c++20 -D QT_MESSAGELOGCONTEXT=1 -nomake examples -skip qt3d -skip qt5compat -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtcoap -skip qtconnectivity -skip qtdatavis3d -skip qtfeedback -skip qtgamepad -skip qtgraphicaleffects -skip qtimageformats -skip qtlocation -skip qtlottie -skip qtmacextras -skip qtmqtt -skip qtnetworkauth -skip qtopcua -skip qtpim -skip qtpurchasing -skip qtqa -skip qtquick3d -skip qtquickcontrols -skip qtquickcontrols2 -skip qtquicktimeline -skip qtremoteobjects -skip qtrepotools -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtshadertools -skip qtspeech -skip qtsystems -skip qtvirtualkeyboard -skip qtwayland -skip qtwebchannel -skip qtwebengine -skip qtwebglplugin -skip qtwebsockets -skip qtwebview -skip qtx11extras -skip qtxmlpatterns -skip qtdeclarative -skip qtactiveqt -skip qtsvg -skip qttools -skip qtdoc -skip qttranslations

cmake --build . --parallel
cmake --install .
cmake --install . --config Debug
```

Translations will be needed at some point, so everything starting with `-skip qtdeclarative` will have to be removed from the command line above.

Looks like it might make sense to go through `configure -list-features`.
