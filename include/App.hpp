#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Transform.hpp"
#include "Util/Text.hpp"   
#include "Util/Color.hpp" 
#include "Util/Time.hpp" 
#include <memory>
#include <vector>
#include "Button.hpp"
#include "Monster.hpp"   
#include "Dice.hpp" 
#include "Bullet.hpp" 

class App {
public:
    enum class State { START, UPDATE, END };
    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    void InitializeWaypoints();
    void SpawnMonsters();
    void SpawnDice();
    glm::vec2 GetTilePosition(int row, int col);
    
    // 🌟 新增：反推座標是哪一個格子
    bool GetGridIndex(glm::vec2 pos, int& row, int& col);

    State m_CurrentState = State::START;
    
    std::shared_ptr<Util::Image> m_BackgroundImage; 
    std::unique_ptr<Button> m_SummonBtn;
    std::shared_ptr<Util::Text> m_CostText; 
    std::shared_ptr<Util::Image> m_SpBarImage; 
    std::shared_ptr<Util::Text> m_SpTotalText; 

    int m_SpCost = 10;       
    int m_CurrentSp = 10000; 
    std::shared_ptr<Dice> m_Board[3][5] = {nullptr}; 

    std::vector<glm::vec2> m_MonsterWaypoints;
    std::vector<std::shared_ptr<Monster>> m_Monsters;
    std::vector<std::shared_ptr<Bullet>> m_Bullets; 

    // --- 🌟 拖曳合成相關變數 ---
    bool m_IsDragging = false;
    int m_DragStartRow = -1;
    int m_DragStartCol = -1;

    float m_NormalMonsterTimer = 0.0f;
    float m_FastMonsterTimer = 0.0f;
    float m_BossMonsterTimer = 0.0f;

    int m_NormalMonsterCount = 0;      
    int m_CurrentNormalHp = 100;       
    std::vector<int> m_HpHistory;      
};

#endif