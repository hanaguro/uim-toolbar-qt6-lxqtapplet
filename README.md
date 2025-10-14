# README.md

uim-toolbar-qt6-lxqtapplet
=======================

Download
-----
git clone https://github.com/hanaguro/uim-toolbar-qt6-lxqtapplet.git  
cd uim-toolbar-qt6-lxqtapplet  
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
