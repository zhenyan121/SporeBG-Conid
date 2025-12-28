#pragma once
#include "GameUIManager.h"
#include "network/NetData.h"
class OnlineGameUIManager : public GameUIManager {
public:
    using OnlineTypeEvent = std::function<void(NetType )>;
    OnlineGameUIManager(SceneEventCallback eventCallback);
    ~OnlineGameUIManager();
    void init() override;
    void setOnlineTypeCallback(OnlineTypeEvent type);

    void hideOnlineButtons();

private:
    OnlineTypeEvent m_onlineTypeEvent;
};