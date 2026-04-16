#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Transform.hpp"
#include "Util/Input.hpp"
#include <ctime>

void App::Start() {
    m_BackgroundImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Board.png");
    m_DiceImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Iron_Dice.png");

    m_SummonBtn = std::make_unique<Button>(RESOURCE_DIR "/SpawnDice.png",
                                           glm::vec2(0.0f, -140.0f),
                                           glm::vec2(150.0f, 150.0f));
                                                    
    m_CostText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "10", Util::Color::FromName(Util::Colors::BLACK));                                                
    m_SpBarImage = std::make_shared<Util::Image>(RESOURCE_DIR "/SP.png");
    m_SpTotalText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, "10000", Util::Color::FromName(Util::Colors::BLACK));
    
    // 🌟 啟動時初始化路線
    InitializeWaypoints();

    if (m_BackgroundImage) LOG_DEBUG("成功讀取背景圖片！");
    if (m_DiceImage) LOG_DEBUG("成功讀取骰子圖片！");

    m_CurrentState = State::UPDATE;
}

void App::InitializeWaypoints() {
    m_MonsterWaypoints.clear();
    m_MonsterWaypoints.push_back(glm::vec2(-233.0f, -133.0f)); // 起點：左下黑洞
    m_MonsterWaypoints.push_back(glm::vec2(-233.0f, 180.0f));  // 第一個轉彎：左上
    m_MonsterWaypoints.push_back(glm::vec2(233.0f, 180.0f));   // 第二個轉彎：右上
    m_MonsterWaypoints.push_back(glm::vec2(233.0f, -133.0f));  // 終點：右下黑洞
}

glm::vec2 App::GetTilePosition(int row, int col) {
    float cellSize = 60.7f; 
    float startX = -124.0f; 
    float startY = 95.0f; 
    float x = startX + (col * cellSize);
    float y = startY - (row * cellSize); 
    return glm::vec2(x, y);
}

void App::SpawnMonsters() {
    // 🌟 真正計算時間的地方，這行絕對不能少！
    float deltaTime = Util::Time::GetDeltaTime(); 
    
    m_NormalMonsterTimer += deltaTime;
    m_FastMonsterTimer += deltaTime;
    m_BossMonsterTimer += deltaTime;

    if (m_BossMonsterTimer >= 60.0f) {
        m_BossMonsterTimer -= 60.0f; 
        auto monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/4.png", m_MonsterWaypoints, 0.5f, 500);
        m_Monsters.push_back(monster);
    }

    if (m_FastMonsterTimer >= 10.0f) {
        m_FastMonsterTimer -= 10.0f; 
        auto monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/3.png", m_MonsterWaypoints, 3.0f, 50);
        m_Monsters.push_back(monster);
    }

    if (m_NormalMonsterTimer >= 2.0f) {
        m_NormalMonsterTimer -= 2.0f; 
        auto monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/2.png", m_MonsterWaypoints, 1.5f, 20);
        m_Monsters.push_back(monster);
    }
}

void App::SpawnDice() {
    std::vector<std::pair<int, int>> emptyTiles;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            if (m_BoardData[r][c] == 0) emptyTiles.push_back({r, c});
        }
    }

    if (!emptyTiles.empty()) {
        static bool seeded = false;
        if (!seeded) { srand(time(NULL)); seeded = true; } 
        
        int randomIndex = rand() % emptyTiles.size();
        int row = emptyTiles[randomIndex].first;
        int col = emptyTiles[randomIndex].second;
        m_BoardData[row][col] = 1;
    }
}

void App::Update() {
    glm::mat4 sharedProjection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 2.0f);

    SpawnMonsters(); 
    
    // 使用 Iterator (迭代器) 來巡覽陣列，這樣邊走邊刪才不會當機
    for (auto it = m_Monsters.begin(); it != m_Monsters.end(); ) {
        (*it)->Update(); // 讓怪物移動

        // 檢查這隻怪物是不是走到終點了？
        if ((*it)->IsFinished()) {
            
            LOG_DEBUG("怪物抵達終點！");
            // 💡 未來這裡可以加上：m_PlayerHp -= (*it)->GetHp(); (主堡扣血)

            // erase 會安全地把怪物刪掉，並把指標移到下一個怪物
            it = m_Monsters.erase(it); 
        } else {
            // 如果沒到終點，就正常檢查下一隻
            ++it; 
        }
    }

    if (m_BackgroundImage) {
        Core::Matrices bgData;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)); 
        model = glm::scale(model, glm::vec3(600.0f, 600.0f, 1.0f));
        bgData.m_Model = model;
        bgData.m_Projection = sharedProjection;
        m_BackgroundImage->Draw(bgData);
    }

    if (m_DiceImage) {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 5; c++) {
                if (m_BoardData[r][c] == 1) {
                    Core::Matrices diceData;
                    glm::vec2 pos = GetTilePosition(r, c); 
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.5f));
                    model = glm::scale(model, glm::vec3(56.0f, 56.0f, 1.0f));
                    diceData.m_Model = model;
                    diceData.m_Projection = sharedProjection;
                    m_DiceImage->Draw(diceData);
                }
            }
        }
    }

    for (auto& monster : m_Monsters) {
        monster->Draw(sharedProjection);
    }

    if (m_SummonBtn) m_SummonBtn->Draw(sharedProjection);

    // 🌟 SP 底圖對齊修正
    if (m_SpBarImage) {
        Core::Matrices spBarData;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-125.0f, -160.0f, 0.8f));
        model = glm::scale(model, glm::vec3(180.0f, 180.0f, 1.0f)); 
        spBarData.m_Model = model;
        spBarData.m_Projection = sharedProjection;
        m_SpBarImage->Draw(spBarData);
    }

    // 🌟 SP 文字對齊修正
    if (m_SpTotalText) {
        Core::Matrices spTextData;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, -156.0f, 0.9f));
        glm::vec2 size = m_SpTotalText->GetSize();
        model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f)); 
        spTextData.m_Model = model;
        spTextData.m_Projection = sharedProjection;
        m_SpTotalText->Draw(spTextData);
    }

    // 🌟 按鈕花費文字對齊修正 (往下移動到白框內)
    if (m_CostText) {
        Core::Matrices textData;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -165.0f, 0.9f));
        glm::vec2 size = m_CostText->GetSize();
        model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f)); 
        textData.m_Model = model;
        textData.m_Projection = sharedProjection;
        m_CostText->Draw(textData);
    }

    if (m_SummonBtn && m_SummonBtn->IsClicked()) {
        if (m_CurrentSp >= m_SpCost) { 
            m_CurrentSp -= m_SpCost;   
            SpawnDice();               
            m_SpCost += 10;            
            m_CostText->SetText(std::to_string(m_SpCost));
            m_SpTotalText->SetText(std::to_string(m_CurrentSp));
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {
    LOG_TRACE("End");
}