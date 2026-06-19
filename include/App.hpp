#ifndef APP_HPP
#define APP_HPP

#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Monster.hpp"
#include "Dice.hpp"
#include "Bullet.hpp"
#include "Button.hpp"
#include <vector>
#include <memory>
#include <string>

class App {
public:
    enum class State { START, MENU, BACKPACK, GAMEOVER, UPDATE, CHEST, OPEN, END }; 
    State GetCurrentState() const { return m_CurrentState; }
    
    void Start();
    void Menu();   
    void Backpack(); 
    void GameOver();
    void Chest();
    void Open();
    void Update();
    void End();

private:
    void InitGame(bool isEndlessMode); // 將原本 Start 的初始化邏輯移至此
    void InitializeWaypoints();
    glm::vec2 GetTilePosition(int row, int col);
    bool GetGridIndex(glm::vec2 pos, int& row, int& col);
    void SpawnMonsters();
    void SpawnDice();

    bool m_IsEndlessMode = false;

    State m_CurrentState = State::START;

    int m_GemCount = 2000;    // 初始寶石
    int m_GoldCount = 100000; // 初始金幣
    std::shared_ptr<Util::Text> m_GemText;
    std::shared_ptr<Util::Text> m_GoldText;
    
    std::shared_ptr<Util::Image> m_MainMenuImage; // 主選單圖片
    std::shared_ptr<Util::Image> m_BackpackImage;
    std::shared_ptr<Util::Image> m_GameOverImage;
    std::shared_ptr<Util::Image> m_FrameOrdinary; // 普通骰子的底框
    std::vector<std::shared_ptr<Util::Image>> m_AllDiceIcons;// 為了在背包顯示，我們先準備一個陣列存放所有種類的 UI 圖片

    // --- 背包系統專用變數 (覆蓋之前的) ---
    int m_SelectedInventoryIndex = -1; 
    std::vector<DiceType> m_OwnedDiceTypes = {
        DiceType::Ice, DiceType::Poison, DiceType::Wind, DiceType::Electric, DiceType::Iron
    };
    std::vector<std::shared_ptr<Util::Image>> m_ActiveDeckIcons;

    // 🌟 新增：資訊面板相關變數
    std::shared_ptr<Util::Image> m_InfoPanelImage;
    bool m_IsInfoPanelOpen = false;      // 判斷面板是否開啟
    bool m_IsWaitingToSwap = false;      // 判斷是否按下了「使用」，正在等待點選上方骰子

    // 🌟 新增：資訊面板上的動態文字
    std::shared_ptr<Util::Text> m_InfoNameText;  // 骰子名稱
    std::shared_ptr<Util::Text> m_InfoAtkText;   // 攻擊力
    std::shared_ptr<Util::Text> m_InfoSpeedText; // 攻擊速度

    // 🌟 新增這三個：稀有度與屬性標籤
    std::shared_ptr<Util::Text> m_InfoRarityText; // 顯示 "普通"、"稀有" 等
    std::shared_ptr<Util::Text> m_InfoAtkLabel;   // 顯示黑字的 "攻擊力"
    std::shared_ptr<Util::Text> m_InfoSpeedLabel; // 顯示黑字的 "攻擊速度"
    std::shared_ptr<Util::Text> m_InfoUseBtnText;

    std::shared_ptr<Util::Text> m_CritDmgText;
    std::shared_ptr<Util::Text> m_CritRateText;
    std::shared_ptr<Util::Image> m_BackgroundImage;
    std::shared_ptr<Util::Image> m_SpBarImage;
    std::shared_ptr<Util::Text> m_SpTotalText;
    std::shared_ptr<Util::Text> m_CostText;
    std::unique_ptr<Button> m_SummonBtn;
    std::shared_ptr<Util::Image> m_SpLevelIcon;

    std::shared_ptr<Dice> m_Board[3][5] = {nullptr};
    std::vector<std::shared_ptr<Monster>> m_Monsters;
    std::vector<std::shared_ptr<Bullet>> m_Bullets;
    std::vector<glm::vec2> m_MonsterWaypoints;

    std::shared_ptr<Util::Text> m_GameOverWaveText;
    std::shared_ptr<Util::Text> m_GameOverCardText;
    std::shared_ptr<Util::Image> m_SilenceIcon;

    std::shared_ptr<Util::Image> m_ChestBgImage;
    std::shared_ptr<Util::Image> m_OpenBgImage; 
    int m_TotalCards = 400;
    std::shared_ptr<Util::Text> m_ChestCardText;  // 顯示 "目前數量/40"
    std::shared_ptr<Util::Text> m_ChestCountText; // 顯示 "X 數量"

    std::shared_ptr<Util::Image> m_OrdinaryTagImage; // 普通骰子的底卡
    std::shared_ptr<Util::Text> m_OpenDiceNameText;  // 骰子名稱
    std::shared_ptr<Util::Text> m_OpenDiceCountText; // x100
    std::shared_ptr<Util::Text> m_OpenGemText;       // 寶石數量
    std::shared_ptr<Util::Text> m_OpenGoldText;      // 金幣數量
    std::shared_ptr<Util::Text> m_OpenCardText;      // 剩餘卡片
    int m_OpenedDiceIndex = 0;
    
    int m_CurrentSp = 10000;
    int m_SpCost = 10;
    
    bool m_IsDragging = false;
    int m_DragStartRow = -1;
    int m_DragStartCol = -1;

    int m_CurrentWave = 1; // 記錄當前波數
    std::shared_ptr<Util::Text> m_WaveText; // 用來顯示 WAVE 字樣的文字物件

    // 🌟 新增以下控制波次狀態的變數
    int m_MonstersSpawnedThisWave = 0; // 記錄這波已經生了幾隻怪
    float m_WaveSpawnTimer = 0.0f;     // 生怪間隔計時器
    bool m_IsSpawningWave = true;      // 狀態：目前是否正在生怪中
    int m_CurrentNormalHp = 100;       // 怪物的基礎血量
    int m_SmallMonsterSpawnCount = 0;
    float m_WaveDelayTimer = 0.0f;

    int m_PlayerHp = 3; // 玩家初始血量，最多 3 顆心
    std::shared_ptr<Util::Image> m_HpImage;
    std::shared_ptr<Util::Image> m_HpLossImage;

    std::vector<DiceType> m_CurrentDeck = {
        DiceType::Ice, DiceType::Poison, DiceType::Wind, DiceType::Electric, DiceType::Iron
    };

    int m_PowerUpLevels[5] = {1, 1, 1, 1, 1}; 
    int m_PowerUpCosts[5] = {100, 100, 100, 100, 100}; 
    std::vector<std::shared_ptr<Util::Image>> m_PowerUpIcons;
    std::vector<std::shared_ptr<Util::Text>> m_PowerUpCostTexts;
    std::vector<std::shared_ptr<Util::Text>> m_PipCountTexts;

    int GetNextPowerUpCost(int currentLevel) {
        switch(currentLevel) {
            case 1: return 200;
            case 2: return 400;
            case 3: return 700;
            default: return -1; 
        }
    }
};

#endif