#include "scenes/base/Scene.h"
#include "utils/Config.h"
#include <fstream>

class BadAppleScene : public Scene {
public:
    BadAppleScene();
    ~BadAppleScene();

    void onEnter(CoreData* coreData) override;
    void handleClick(int logicalX, int logicalYY) override;
    void update() override;
    void renderWorld() override; // 逻辑世界
    void renderUI() override;    // 屏幕 UI
private:
    int m_Width;
    int m_Height;
    int m_cellSize;
    BoardArea m_area;

    std::ifstream m_file;

    float m_currentTime = 0.0f;

    SDL_Renderer* m_renderer;
    

    std::vector<std::string> m_frame;
    
    void updateFrame();

    bool readFrame();
};