# README.md

uim-toolbar-qt6-systray
=======================

This is a minimal Qt6 system tray application intended as a starting point for a
`uim-toolbar-qt6-systray` implementation. It intentionally keeps the actual
uim integration open-ended so you can wire in the correct ipc/embedding method.

Build
-----

mkdir build && cd build
cmake ..
cmake --build .

Run
---

./uim-toolbar-qt6-systray

Integration pointers
--------------------
 - If upstream uim provides a `uim-toolbar-qt6` binary you can `QProcess::startDetached` it.
 - If uim exposes a widget or library, instantiate and add it to `ToolbarPopup` layout.
 - If uim's toolbar runs as a separate process and uses XEmbed, you can obtain its X11 window id
   and reparent it into a Qt widget (platform-specific).


*/
