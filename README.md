# imageglue
ImageGlue is small program to crop and glue several images together.

You can either drag and drop image files to main window or click "Add" button to open them one by one. Select a file and click "Remove" to unload images.
Each image can be cropped and moved relative to glued image's top-left corner. 
Blank areas not covered by images will be filled with white. Negative cropping will fill areas outside the original image with black. 
Clicking "Preview" will create, or update, preview and allow you to click "Save" to save the glued image.

## Building

### Windows

ImageGlue should build without changes under Windows 11 with Microsoft
Visual C++ 2019-2022, vcpkg, CMake 3.16 or later, and Qt 6.10 or later. If CMake can't find zlib headers, rename
`CMakeUserPresets.json.sample` to `CMakeMakeUserPresets.json` and update `VCPKG_ROOT` in
the file to reflect installation path of standalone version of vcpkg. vcpkg can be
downloaded from [GitHub](https://github.com/microsoft/vcpkg/releases). Pass the vcpkg
toolchain file to CMake when configuring the build.

For example:
```
md build
cd build
cmake -D CMAKE_TOOLCHAIN_FILE=c:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```

### Linux

It should also build with minimal changes under most Linux distributions with Qt6 development packages installed.
