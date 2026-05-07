#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Transform.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"
#include <ctime>
#include <numeric> 
#include <cmath> 

void App::Start() {
    m_BackgroundImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Board.png");
    
    m_SummonBtn = std::make_unique<Button>(RESOURCE_DIR "/SpawnDice.png", glm::vec2(0.0f, -140.0f), glm::vec2(150.0f, 150.0f));
    m_CostText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "10", Util::Color::FromName(Util::Colors::BLACK));                                                
    m_SpBarImage = std::make_shared<Util::Image>(RESOURCE_DIR "/SP.png");
    m_SpTotalText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, "10000", Util::Color::FromName(Util::Colors::BLACK));
    
    InitializeWaypoints();
    m_CurrentState = State::UPDATE;
}

void App::InitializeWaypoints() {
    m_MonsterWaypoints = {
        {-232.0f, -133.0f}, {-232.0f, 180.0f}, {231.0f, 180.0f}, {231.0f, -133.0f}
    };
}

glm::vec2 App::GetTilePosition(int row, int col) {
    float cellSize = 60.7f; 
    float startX = -124.0f; 
    float startY = 95.0f; 
    return glm::vec2(startX + (col * cellSize), startY - (row * cellSize));
}

bool App::GetGridIndex(glm::vec2 pos, int& row, int& col) {
    float cellSize = 60.7f; 
    float startX = -124.0f; 
    float startY = 95.0f; 
    
    int c = std::round((pos.x - startX) / cellSize);
    int r = std::round((startY - pos.y) / cellSize); 
    
    if (r >= 0 && r < 3 && c >= 0 && c < 5) {
        row = r; 
        col = c; 
        return true;
    }
    return false;
}

void App::SpawnMonsters() {
    float deltaTime = static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f; 
    m_NormalMonsterTimer += deltaTime;
    m_FastMonsterTimer += deltaTime;
    m_BossMonsterTimer += deltaTime;

    if (m_NormalMonsterTimer >= 2.0f) {
        m_NormalMonsterTimer = 0.0f; 
        auto monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/2.png", m_MonsterWaypoints, 1.5f, m_CurrentNormalHp);
        m_Monsters.push_back(monster);
        m_HpHistory.push_back(m_CurrentNormalHp);
        if (m_HpHistory.size() > 10) m_HpHistory.erase(m_HpHistory.begin());
        m_NormalMonsterCount++;
        if (m_NormalMonsterCount % 2 == 0) m_CurrentNormalHp += 10;
    }

    if (m_FastMonsterTimer >= 10.0f) {
        m_FastMonsterTimer = 0.0f; 
        auto monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/3.png", m_MonsterWaypoints, 3.0f, m_CurrentNormalHp);
        m_Monsters.push_back(monster);
    }

    if (m_BossMonsterTimer >= 60.0f) {
        m_BossMonsterTimer = 0.0f; 
        int bossHp = std::accumulate(m_HpHistory.begin(), m_HpHistory.end(), 0);
        if (m_HpHistory.size() < 10) bossHp = m_CurrentNormalHp * 10;
        auto monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/4.png", m_MonsterWaypoints, 0.5f, bossHp);
        m_Monsters.push_back(monster);
    }
}

void App::SpawnDice() {
    std::vector<std::pair<int, int>> emptyTiles;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            if (m_Board[r][c] == nullptr) emptyTiles.push_back({r, c});
        }
    }
    if (!emptyTiles.empty()) {
        static bool seeded = false;
        if (!seeded) { srand(static_cast<unsigned int>(time(NULL))); seeded = true; } 
        
        int randomIndex = rand() % emptyTiles.size();
        int targetRow = emptyTiles[randomIndex].first;
        int targetCol = emptyTiles[randomIndex].second;
        glm::vec2 pos = GetTilePosition(targetRow, targetCol);

        int diceType = rand() % 5;
        switch (diceType) {
            case 0: m_Board[targetRow][targetCol] = std::make_shared<IceDice>(pos); break;
            case 1: m_Board[targetRow][targetCol] = std::make_shared<PoisonDice>(pos); break;
            case 2: m_Board[targetRow][targetCol] = std::make_shared<WindDice>(pos); break;
            case 3: m_Board[targetRow][targetCol] = std::make_shared<ElectricDice>(pos); break;
            case 4: m_Board[targetRow][targetCol] = std::make_shared<IronDice>(pos); break;
        }
    }
}

void App::Update() {
    glm::mat4 sharedProjection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 2.0f);
    SpawnMonsters(); 
    
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    if (!m_IsDragging && (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB))) {
        int r, c;
        if (GetGridIndex(mousePos, r, c) && m_Board[r][c] != nullptr) {
            m_IsDragging = true;
            m_DragStartRow = r;
            m_DragStartCol = c;
        }
    }

    if (m_IsDragging && (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB))) {
        if (m_Board[m_DragStartRow][m_DragStartCol] != nullptr) {
            m_Board[m_DragStartRow][m_DragStartCol]->SetPos(mousePos);
        }
    }

    if (m_IsDragging && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_IsDragging = false; 
        int targetR, targetC;
        bool validDrop = GetGridIndex(mousePos, targetR, targetC);
        
        auto draggedDice = m_Board[m_DragStartRow][m_DragStartCol];

        if (validDrop && draggedDice != nullptr) {
            auto targetDice = m_Board[targetR][targetC];

            if (targetR == m_DragStartRow && targetC == m_DragStartCol) {
                draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol));
            }
            else if (targetDice == nullptr) {
                m_Board[targetR][targetC] = draggedDice;
                m_Board[m_DragStartRow][m_DragStartCol] = nullptr;
                draggedDice->SetPos(GetTilePosition(targetR, targetC));
            }
            else {
                if (draggedDice->GetType() == targetDice->GetType() && draggedDice->GetLevel() == targetDice->GetLevel()) {
                    targetDice->Upgrade();
                    m_Board[m_DragStartRow][m_DragStartCol] = nullptr; 
                    targetDice->SetPos(GetTilePosition(targetR, targetC)); 
                } else {
                    draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol));
                }
            }
        } else if (draggedDice != nullptr) {
            draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol));
        }
    }

    for (auto it = m_Monsters.begin(); it != m_Monsters.end(); ) {
        (*it)->Update(); 
        if ((*it)->IsFinished() || (*it)->IsDead()) it = m_Monsters.erase(it); 
        else ++it; 
    }

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            if (m_Board[r][c] != nullptr && !(m_IsDragging && r == m_DragStartRow && c == m_DragStartCol)) {
                m_Board[r][c]->Update(m_Monsters, m_Bullets); 
            }
        }
    }

    // 🌟 最關鍵的子彈彈跳判定
    for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ) {
        (*it)->Update();
        if ((*it)->IsInvalid()) {
            it = m_Bullets.erase(it); 
        } else if ((*it)->IsHit()) {
            // 1. 打中怪物，扣血
            (*it)->GetTarget()->ReceiveDamage((*it)->GetDamage()); 

            // 2. 判斷是否有剩餘彈跳次數
            if ((*it)->GetBouncesLeft() > 0) {
                (*it)->DecreaseBounce(); // 扣除一次額度
                
                std::shared_ptr<Monster> nextTarget = nullptr;
                float minDistance = 999999.0f;

                // 找尋離「被擊中怪物」最近的其他怪物
                for (auto& monster : m_Monsters) {
                    if (!monster->IsDead() && monster != (*it)->GetTarget()) {
                        float dist = glm::length(monster->GetPos() - (*it)->GetTarget()->GetPos());
                        if (dist < minDistance) {
                            minDistance = dist;
                            nextTarget = monster;
                        }
                    }
                }

                if (nextTarget) {
                    // 有找到下一個受害者，切換目標！子彈不消失，繼續飛！
                    (*it)->SetTarget(nextTarget);
                    ++it; 
                } else {
                    // 場上沒別的怪了，子彈只好消失
                    it = m_Bullets.erase(it); 
                }
            } else {
                // 彈跳次數用完，子彈消失
                it = m_Bullets.erase(it); 
            }
        } else {
            ++it;
        }
    }

    if (m_BackgroundImage) {
        Core::Matrices bgData;
        bgData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)), glm::vec3(600.0f, 600.0f, 1.0f));
        bgData.m_Projection = sharedProjection;
        m_BackgroundImage->Draw(bgData);
    }

    for (auto& monster : m_Monsters) monster->Draw(sharedProjection);
    for (auto& bullet : m_Bullets) bullet->Draw(sharedProjection); 

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            if (m_Board[r][c] != nullptr && !(m_IsDragging && r == m_DragStartRow && c == m_DragStartCol)) {
                m_Board[r][c]->Draw(sharedProjection);
            }
        }
    }

    if (m_SummonBtn) m_SummonBtn->Draw(sharedProjection);

    if (m_SpBarImage) {
        Core::Matrices spBarData;
        spBarData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-125.0f, -160.0f, 0.8f)), glm::vec3(180.0f, 180.0f, 1.0f)); 
        spBarData.m_Projection = sharedProjection;
        m_SpBarImage->Draw(spBarData);
    }
    if (m_SpTotalText) {
        Core::Matrices spTextData;
        glm::vec2 size = m_SpTotalText->GetSize();
        spTextData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, -156.0f, 0.9f)), glm::vec3(size.x, size.y, 1.0f)); 
        spTextData.m_Projection = sharedProjection;
        m_SpTotalText->Draw(spTextData);
    }
    if (m_CostText) {
        Core::Matrices textData;
        glm::vec2 size = m_CostText->GetSize();
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -165.0f, 0.9f)), glm::vec3(size.x, size.y, 1.0f)); 
        textData.m_Projection = sharedProjection;
        m_CostText->Draw(textData);
    }

    if (m_IsDragging && m_Board[m_DragStartRow][m_DragStartCol] != nullptr) {
        m_Board[m_DragStartRow][m_DragStartCol]->Draw(sharedProjection, 0.95f);
    }

    if (m_SummonBtn && m_SummonBtn->IsClicked()) {
        bool hasSpace = false;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 5; c++) if (m_Board[r][c] == nullptr) hasSpace = true;
        }
        if (hasSpace && m_CurrentSp >= m_SpCost) {
            m_CurrentSp -= m_SpCost;
            SpawnDice();
            m_SpCost += 10;
            m_CostText->SetText(std::to_string(m_SpCost));
            m_SpTotalText->SetText(std::to_string(m_CurrentSp));
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::N)) {
        m_CurrentSp += 10000;
        m_SpTotalText->SetText(std::to_string(m_CurrentSp));
    }

    if (Util::Input::IsKeyUp(Util::Keycode::M)) {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 5; c++) m_Board[r][c] = nullptr;
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) m_CurrentState = State::END;
}

void App::End() { LOG_TRACE("End"); }