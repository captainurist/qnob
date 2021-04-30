# Building Qt for qnob

````
..\Src\configure -prefix C:\Qt\6.0.1\msvc2019_64_static -release -force-debug-info -optimize-size -static -schannel -static-runtime -skip qtdeclarative -skip qtquick3d -skip qtquicktimeline -skip qtshadertools -skip qtquickcontrols2 -skip qt5compat -skip qtwayland -skip qtsvg
cmake --build . --parallel
cmake --install .
```

Looks like it might make sense to go through `configure -list-features`.
