
一个棋类游戏，与孢子分裂有关，本为**cpp重构版**，游戏核心主要玩法由**cold1840**制定的，可以看他的[python版本](https://github.com/cold1840/SporeBG)，这个版本旨在用**cpp**完全重写这个游戏，并加入许多功能，同时**cold1840**也在一定程度上参与项目的开发与维护  

## 构建指南
确保电脑安装了`cmake`和`ninja`，使用`gcc`与`g++`：
```bash
git clone --recursive https://github.com/zhenyan121/SporeBG-Conid.git
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja
```
