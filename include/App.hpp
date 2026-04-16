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

class App {
public:
    enum class State { START, UPDATE, END };
    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

private:
    State m_CurrentState = State::START;
    
    std::shared_ptr<Util::Image> m_BackgroundImage; 
    std::shared_ptr<Util::Image> m_DiceImage;
    std::unique_ptr<Button> m_SummonBtn;
    
    std::shared_ptr<Util::Text> m_CostText; 
    std::shared_ptr<Util::Image> m_SpBarImage; 
    std::shared_ptr<Util::Text> m_SpTotalText; 

    int m_SpCost = 10;       
    int m_CurrentSp = 10000; 
    int m_BoardData[3][5] = {0}; 
    
    std::vector<std::shared_ptr<Monster>> m_Monsters;
    std::vector<glm::vec2> m_MonsterWaypoints; 
    float m_NormalMonsterTimer = 0.0f;
    float m_FastMonsterTimer = 0.0f;
    float m_BossMonsterTimer = 0.0f;

    void InitializeWaypoints();
    void SpawnMonsters();
    glm::vec2 GetTilePosition(int row, int col);
    void SpawnDice();
};

#endif