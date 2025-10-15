# README.md

uim-toolbar-qt6-lxqtwidget
=======================

Download
-----
git clone https://github.com/hanaguro/uim-toolbar-qt6-lxqtwidget.git  
cd uim-toolbar-qt6-lxqtwidget  
git submodule init  
git submodule update  

Build
-----
mkdir build && cd build  
cmake -DCMAKE_BUILD_TYPE=Release ..  
make -j$(nproc)  

Install
-----
sudo make install
