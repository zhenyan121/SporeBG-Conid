#include "SceneManager.h"

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
    while (!m_scenes.empty()) {
        m_scenes.pop();
    }
}