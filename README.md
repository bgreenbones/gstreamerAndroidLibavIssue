# To reproduce issue:

Linking of `libav` is attempted in `src/CMakeLists.txt`. Search `# linking libav` to find specific steps.

Place GStreamer binaries for Android in `third-party/` and name the folder `gst-android` 

```
cd example
flutter build apk
```


