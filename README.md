
一个棋类游戏，与孢子分裂有关，本为**cpp重构版**，游戏核心主要玩法由**cold1840**制定的，可以看他的[python版本](https://github.com/cold1840/SporeBG)，这个版本旨在用**cpp**完全重写这个游戏，并加入许多功能，同时**cold1840**也在一定程度上参与项目的开发与维护  

## 构建指南
确保电脑安装了`cmake`和`ninja`，使用`gcc`与`g++`：
```bash
git clone https://github.com/zhenyan121/SporeBG-Conid.git
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
ninja
```
## 项目结构
```bash
SporeBG-Conid/
├── src/
│   ├── core/                 # 核心应用与窗口管理
│   ├── game/                 # 游戏逻辑核心：棋盘、棋子、规则等
│   ├── graphics/             # 图形渲染相关
│   ├── input/                # 输入处理系统（键盘、鼠标）
│   ├── scenes/               # 场景管理系统（菜单、对局等）
│   ├── utils/                # 工具类与配置
│   └── main.cpp              # 程序入口（SDL3 回调）
├── assets/                   # 资源文件：图片、字体、音效
│   ├── images/
│   ├── fonts/
│   └── sounds/
├── config/                   # 配置文件（如 game.ini）
└── build/                    # 编译输出目录（建议 .gitignore 掉）
```

## 一些想说的
这是我第一次用cpp写的项目，想写个比较完整的出来，但是遇到了一堆问题，只能说**路漫漫其修远兮，吾将上下而求索**，如果你有好的建议也是可以提出来的