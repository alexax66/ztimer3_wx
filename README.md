# zTimer3 (wxWidgets version)
ztimer3_wx is [ztimer](https://github.com/peter15914/ztimer) rewritten to C++ language with wxWidgets used as GUI library.

This project is in pre-alpha state. Most features are implemented, but application has some unresolved issues (with GUI on some desktop environments).

## Warning
This application may potentially trigger seizures for people with photosensitive epilepsy. User discretion is advised.

## Screenshots
Main window.

## Platforms
ztimer3_wx was tested on these platforms:
*   Linux
*   Windows

### Building on Linux
To build ztimer3_wx on your computer you will need these packages:
*   wxWidgets 3.0 or later
*   The build-essential package 
*   CMake
*   Git

Git clone the sources:

    git clone https://github.com/peter15914/ztimer3_wx.git

Run CMake and build ztimer3_wx:

  ```bash
  cd ztimer3_wx
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  make
  ```

![alt text][screen001]

[screen001]: https://github.com/peter15914/ztimer3_wx/blob/master/img/ztimer3_wx_001.png "ztimer3_wx - Main window"
