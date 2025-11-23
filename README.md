
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
## 项目结构
```bash
ChessGame/
├── src/
│   ├── core/                 # 核心游戏逻辑
│   │   ├── Game.h/cpp        # 游戏核心管理
│   │   ├── Board.h/cpp       # 棋盘逻辑和规则实现
│   │   ├── Piece.h/cpp       # 棋子基类和具体棋子实现
│   │   └── Rules.h/cpp       # 游戏规则验证
│   ├── ai/                   # 人工智能模块
│   │   ├── AIBase.h/cpp      
│   │   ├── MinimaxAI.h/cpp   
│   │   └── Heuristics.h/cpp  
│   ├── network/              # 网络模块
│   │   ├── NetworkManager.h/cpp 
│   │   ├── Protocol.h/cpp    
│   │   └── Server.h/cpp      
│   ├── ui/                   # 用户界面
│   │   ├── Renderer.h/cpp    # 渲染器(SDL3)
│   │   ├── UIComponents.h/cpp 
│   │   └── MenuSystem.h/cpp  
│   ├── input/                # ← 输入处理模块（键盘、鼠标等）
│   │   ├── InputManager.h/cpp
│   ├── utils/                # 工具类
│   │   ├── Logger.h/cpp      
│   │   ├── Config.h/cpp      
│   │   └── Utils.h/cpp       
│   └── main.cpp              # 程序入口（使用 SDL3 回调）
├── assets/                   # 资源文件
│   ├── images/              
│   ├── fonts/               
│   └── sounds/              
├── config/                  # 配置文件（如 game.ini）
└── build/                   # 构建输出目录（.gitignore 掉）
```

## 一些想说的
这是我第一次用cpp写的项目，想写个比较完整的出来，但是遇到了一堆问题，只能说**路漫漫其修远兮，吾将上下而求索**，如果你有好的建议也是可以提出来的