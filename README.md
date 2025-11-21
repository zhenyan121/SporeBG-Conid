一个棋类游戏，与孢子分裂有关，本为cpp重构版，游戏核心主要玩法由cold1840制定的，可以看他的python版本，这个版本旨在用cpp完全重写这个游戏，并加入许多功能，同时cold1840也在一定程度上参与项目的开发与维护  

构建指南
确保电脑安装了cmake和ninja,使用gcc与g++
克隆仓库以及子模块
git clone --recursive https://github.com/zhenyan121/SporeBG-Conid.git
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja

