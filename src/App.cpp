#include "App.hpp"
#include "Util/Logger.hpp"
#include "Util/Transform.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"
#include <ctime>
#include <numeric> 
#include <cmath> 
#include <random>

void App::Start() {
    // 初始僅載入選單資源
    m_MainMenuImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Main.png");
    m_BackpackImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Backpack/Backpack.png");
    m_GameOverImage = std::make_shared<Util::Image>(RESOURCE_DIR "/game_ending.png");
    m_ChestBgImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Treasure chest/chest.png");
    m_OpenBgImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Treasure chest/open.png");
    m_ChestCardText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, "0/40", Util::Color::FromName(Util::Colors::WHITE));
    m_ChestCountText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "0", Util::Color::FromName(Util::Colors::WHITE));

    m_OrdinaryTagImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Treasure chest/Ordinary_dice.png");
    m_OpenDiceNameText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 18, "Name", Util::Color::FromName(Util::Colors::WHITE));
    m_OpenDiceCountText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 22, "x100", Util::Color::FromName(Util::Colors::WHITE));
    m_OpenGemText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "3", Util::Color::FromName(Util::Colors::WHITE));
    m_OpenGoldText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "2000", Util::Color::FromName(Util::Colors::WHITE));
    m_OpenCardText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, "0 / 40", Util::Color::FromName(Util::Colors::WHITE));

    // ===== 新增：載入底框與骰子圖示 =====
    m_FrameOrdinary = std::make_shared<Util::Image>(RESOURCE_DIR "/Backpack/Ordinary.png");

    // ===== 🌟 載入 1 星骰子本體的圖示 =====
    std::vector<std::string> deckNames = {"Ice", "Poison", "Wind", "Electric", "Iron"};
    for(const auto& name : deckNames) {
        // 使用 _Dice.png 來獲取乾淨的一星骰子圖案
        m_AllDiceIcons.push_back(std::make_shared<Util::Image>(RESOURCE_DIR "/" + name + "/" + name + "_Dice.png"));
    }

    // 🌟 新增這行：將預設的出戰區圖片陣列填滿
    m_ActiveDeckIcons = m_AllDiceIcons;

    // 載入資訊面板背景 (請確保檔名和路徑正確)
    m_InfoPanelImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Backpack/Information.png");

    // 初始化面板上的文字 (字體大小與顏色可後續微調)
    m_InfoNameText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 30, u8"骰子名稱", Util::Color::FromName(Util::Colors::BLACK));
    // 2. 新增稀有度文字 (設定為白色，稍微小一點)
    m_InfoRarityText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, u8"普通", Util::Color::FromName(Util::Colors::WHITE));

    // 3. 新增屬性名稱的「標籤」(黑字)
    m_InfoAtkLabel = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 18, u8"攻擊力", Util::Color::FromName(Util::Colors::BLACK));
    m_InfoSpeedLabel = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 18, u8"攻擊速度", Util::Color::FromName(Util::Colors::BLACK));
    m_InfoAtkText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 18, "20", Util::Color::FromName(Util::Colors::WHITE));
    m_InfoSpeedText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 18, "1.0s", Util::Color::FromName(Util::Colors::WHITE));
    m_InfoUseBtnText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 30, u8"使用", Util::Color::FromName(Util::Colors::WHITE));

    m_CritDmgText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "2000%", Util::Color::FromName(Util::Colors::ORANGE)); 
    m_CritRateText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "5%", Util::Color::FromName(Util::Colors::WHITE));
    m_GameOverWaveText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 55, "WAVE 1", Util::Color::FromName(Util::Colors::WHITE));
    m_GameOverCardText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 25, "+10", Util::Color::FromName(Util::Colors::WHITE));
    
    m_GemText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 30, std::to_string(m_GemCount), Util::Color::FromName(Util::Colors::WHITE));
    m_GoldText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 30, std::to_string(m_GoldCount), Util::Color::FromName(Util::Colors::WHITE));
    
    m_CurrentState = State::MENU;
}

void App::Menu() {
    glm::mat4 sharedProjection = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 2.0f);

    // 繪製主選單
    if (m_MainMenuImage) {
        Core::Matrices menuData;
        menuData.m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(960.0f, 960.0f, 1.0f)); 
        menuData.m_Projection = sharedProjection;
        m_MainMenuImage->Draw(menuData);
    }

    // 偵測點擊跳轉
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        
        // 🛠 加入這行 LOG，如果預設的範圍不夠精準，你可以看終端機的數值來微調
        LOG_DEBUG("目前點擊座標 X: {}, Y: {}", mousePos.x, mousePos.y);
        
        // 練習模式 (左邊橘色按鈕)
        float practiceBtnMinX = -131.0f; 
        float practiceBtnMaxX =  -15.0f; 
        float practiceBtnMinY = -237.0f; 
        float practiceBtnMaxY = -188.0f;

        // 無限模式 (右邊藍色按鈕) - 根據畫面左右對稱抓出的估計值
        float endlessBtnMinX = 3.0f; 
        float endlessBtnMaxX = 130.0f; 
        float endlessBtnMinY = -237.0f; 
        float endlessBtnMaxY = -187.0f;

        if (mousePos.x > practiceBtnMinX && mousePos.x < practiceBtnMaxX && 
            mousePos.y > practiceBtnMinY && mousePos.y < practiceBtnMaxY) {
            InitGame(false); // 進入練習模式
            m_CurrentState = State::UPDATE;
        }
        else if (mousePos.x > endlessBtnMinX && mousePos.x < endlessBtnMaxX && 
                 mousePos.y > endlessBtnMinY && mousePos.y < endlessBtnMaxY) {
            InitGame(true); // 進入無限模式
            m_CurrentState = State::UPDATE;
        }

        float backpackBtnMinX = -96.0f; 
        float backpackBtnMaxX = -37.0f;  
        float backpackBtnMinY = -318.0f; 
        float backpackBtnMaxY = -263.0f; 

        if (mousePos.x > backpackBtnMinX && mousePos.x < backpackBtnMaxX && 
            mousePos.y > backpackBtnMinY && mousePos.y < backpackBtnMaxY) {
            m_CurrentState = State::BACKPACK; // 切換到背包狀態
        }
    
        float chestBtnMinX = 37.0f;  float chestBtnMaxX = 127.0f;
        float chestBtnMinY = 74.0f;  float chestBtnMaxY = 159.0f;
        if (mousePos.x > chestBtnMinX && mousePos.x < chestBtnMaxX && 
            mousePos.y > chestBtnMinY && mousePos.y < chestBtnMaxY) {
            LOG_DEBUG("進入寶箱畫面！");
            m_CurrentState = State::CHEST; 
        }
    }

    // 🌟 繪製寶石與金幣 (在畫面上方)
    if (m_GemText) {
        // 設定寶石數值與位置
        m_GemText->SetText(std::to_string(m_GemCount));
        Core::Matrices gemData;
        glm::vec2 size = m_GemText->GetSize();
        // 座標微調：請根據畫面背景的圖示位置微調 X 和 Y
        gemData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, 340.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        gemData.m_Projection = sharedProjection;
        m_GemText->Draw(gemData);
    }

    if (m_GoldText) {
        // 設定金幣數值與位置
        m_GoldText->SetText(std::to_string(m_GoldCount));
        Core::Matrices goldData;
        glm::vec2 size = m_GoldText->GetSize();
        // 座標微調：請根據畫面背景的圖示位置微調 X 和 Y
        goldData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(135.0f, 340.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        goldData.m_Projection = sharedProjection;
        m_GoldText->Draw(goldData);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::Backpack() {
    glm::mat4 sharedProjection = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 2.0f);

    // 繪製背包畫面
    if (m_BackpackImage) {
        Core::Matrices bgData;
        bgData.m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(960.0f, 960.0f, 1.0f)); 
        bgData.m_Projection = sharedProjection;
        m_BackpackImage->Draw(bgData);
    }

    if (m_CritDmgText) {
        Core::Matrices dmgData;
        glm::vec2 size = m_CritDmgText->GetSize();
        dmgData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 84.0f, 0.9f)), glm::vec3(size.x, size.y, 1.0f));
        dmgData.m_Projection = sharedProjection;
        m_CritDmgText->Draw(dmgData);
    }

    if (m_CritRateText) {
        Core::Matrices rateData;
        glm::vec2 size = m_CritRateText->GetSize();
        rateData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(171.0f, 52.0f, 0.9f)), glm::vec3(size.x, size.y, 1.0f));
        rateData.m_Projection = sharedProjection;
        m_CritRateText->Draw(rateData);
    }

    // ===== 2. 繪製上方：出戰的 5 顆骰子 =====
    float activeStartX = -145.0f; 
    float activeGapX = 74.0f;     
    float activeY = 200.0f;       

    for (int i = 0; i < 5; i++) {
        float currentX = activeStartX + (i * activeGapX);
        
        // 🌟 將這裡的 m_AllDiceIcons 改成 m_ActiveDeckIcons
        if (i < m_ActiveDeckIcons.size() && m_ActiveDeckIcons[i]) {
            Core::Matrices iconData;
            iconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(currentX, activeY, 0.9f)), glm::vec3(60.0f, 60.0f, 1.0f));
            iconData.m_Projection = sharedProjection;
            m_ActiveDeckIcons[i]->Draw(iconData);
        }
    }

    // ===== 3. 繪製下方：備戰列 (一排 4 個，空位完全不顯示) =====
    if (!m_IsWaitingToSwap) {
        float invStartX = -140.0f; 
        float invGapX = 100.0f;     
        float invStartY = -65.0f;  
        float invGapY = -130.0f;   
        int cols = 4; 
        int totalSlots = 16; 

        for (int i = 0; i < totalSlots; i++) {
            int row = i / cols; 
            int col = i % cols; 
            
            float currentX = invStartX + (col * invGapX);
            float currentY = invStartY + (row * invGapY);
            
            // 🌟 關鍵修改：將底框和骰子都包進這個 if 裡面
            // 只有當 i 小於我們實際擁有的骰子數量時，才畫出東西；否則這個格子就是全空的
            if (i < m_AllDiceIcons.size() && m_AllDiceIcons[i]) {
                
                // 畫底框
                if (m_FrameOrdinary) {
                    Core::Matrices frameData;
                    frameData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(currentX, currentY, 0.8f)), glm::vec3(130.0f, 130.0f, 1.0f));
                    frameData.m_Projection = sharedProjection;
                    m_FrameOrdinary->Draw(frameData);
                }
                
                // 畫骰子圖示
                Core::Matrices iconData;
                // 圖示稍微往上移一點 (Y + 10)，對齊框框的視覺中心
                iconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(currentX - 2.5f, currentY + 17.0f, 0.9f)), glm::vec3(60.0f, 60.0f, 1.0f));
                iconData.m_Projection = sharedProjection;
                m_AllDiceIcons[i]->Draw(iconData);
            }
        }
    }

    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();

        // 🌟 把這行加在最前面！這樣你點畫面任何地方，終端機就會印出精準座標
        LOG_DEBUG("滑鼠點擊座標 -> X: {}, Y: {}", mousePos.x, mousePos.y);

        // ==========================================
        // 狀態 2：如果資訊面板是開啟的
        // ==========================================
        if (m_IsInfoPanelOpen) {
            // 更新：橘色「使用」按鈕在放大後的預估判定範圍
            float useBtnMinX = 12.0f;    float useBtnMaxX = 112.0f;
            float useBtnMinY = -163.0f; float useBtnMaxY = -120.0f;

            // 🌟 把你測出來的 X 按鈕範圍填在這裡
            float closeBtnMinX = 102.0f; // 填入測試出的最小值
            float closeBtnMaxX = 123.0f; // 填入測試出的最大值
            float closeBtnMinY = 198.0f; // 填入測試出的最小值
            float closeBtnMaxY = 224.0f; // 填入測試出的最大值

            if (mousePos.x > closeBtnMinX && mousePos.x < closeBtnMaxX &&
                mousePos.y > closeBtnMinY && mousePos.y < closeBtnMaxY) {
                
                LOG_DEBUG("點擊了 X，關閉面板");
                m_IsInfoPanelOpen = false;       // 關閉面板
                m_SelectedInventoryIndex = -1;   // 取消選取
            }

            if (mousePos.x > useBtnMinX && mousePos.x < useBtnMaxX &&
                mousePos.y > useBtnMinY && mousePos.y < useBtnMaxY) {
                // 點擊了「使用」按鈕
                LOG_DEBUG("點擊了使用按鈕！請點擊上方陣列替換");
                m_IsInfoPanelOpen = false;   // 關閉面板
                m_IsWaitingToSwap = true;    // 進入等待替換狀態
            } 
            
        } 
        // ==========================================
        // 狀態 3：按下了使用，正在等待點擊上方出戰區
        // ==========================================
        else if (m_IsWaitingToSwap) {
            float activeStartX = -97.0f; float activeGapX = 49.0f; float activeY = 132.0f;
            bool clickedValidSlot = false; // 用來記錄有沒有點中格子

            for (int i = 0; i < 5; i++) {
                float currentX = activeStartX + (i * activeGapX);
                
                // 🌟 把判定範圍從 30 放大到 45，確保隨便點都能點中格子
                if (mousePos.x > currentX - 20.0f && mousePos.x < currentX + 20.0f &&
                    mousePos.y > activeY - 20.0f && mousePos.y < activeY + 20.0f) {
                    
                    clickedValidSlot = true;
                    DiceType selectedType = m_OwnedDiceTypes[m_SelectedInventoryIndex];
                    
                    // 1. 檢查目前出戰區中，是不是「已經有這顆骰子」了？
                    int existingIndex = -1;
                    for (int j = 0; j < 5; j++) {
                        if (m_CurrentDeck[j] == selectedType) {
                            existingIndex = j;
                            break;
                        }
                    }

                    if (existingIndex != -1) {
                        if (existingIndex != i) {
                            // 🌟 情況 A：已經有這顆骰子 -> 使用 std::swap 完美對調資料與圖片
                            std::swap(m_CurrentDeck[i], m_CurrentDeck[existingIndex]);
                            std::swap(m_ActiveDeckIcons[i], m_ActiveDeckIcons[existingIndex]);
                            LOG_DEBUG("對調成功！第 {} 格 與 第 {} 格對調", existingIndex, i);
                        } else {
                            LOG_DEBUG("點擊了原本的位置，取消動作");
                        }
                    } else {
                        // 🌟 情況 B：出戰區沒有這顆骰子 -> 直接覆寫
                        m_CurrentDeck[i] = selectedType;
                        m_ActiveDeckIcons[i] = m_AllDiceIcons[m_SelectedInventoryIndex];
                        LOG_DEBUG("替換成功！覆寫了出戰區第 {} 格", i);
                    }
                    
                    break; // 處理完就跳出迴圈
                }
            }
            
            // 🌟 不管是成功替換，還是點到了旁邊的空氣 (取消)，都結束懸浮等待狀態
            m_IsWaitingToSwap = false;
            m_SelectedInventoryIndex = -1;
        }
        // ==========================================
        // 狀態 1：一般狀態 (點擊備戰區開啟面板，或點擊返回戰鬥)
        // ==========================================
        else {
            // 返回戰鬥按鈕
            float battleBtnMinX = -37.0f; float battleBtnMaxX = 25.0f;  
            float battleBtnMinY = -318.0f; float battleBtnMaxY = -263.0f; 
            if (mousePos.x > battleBtnMinX && mousePos.x < battleBtnMaxX && 
                mousePos.y > battleBtnMinY && mousePos.y < battleBtnMaxY) {
                m_CurrentState = State::MENU; 
            }

            // 點擊下方備戰區 (16格)
            float invStartX = -95.0f; float invGapX = 69.0f;     
            float invStartY = -42.0f;  float invGapY = -90.0f; 
            for (int i = 0; i < 16; i++) {
                int row = i / 4; int col = i % 4; 
                float currentX = invStartX + (col * invGapX);
                float currentY = invStartY + (row * invGapY);

                if (mousePos.x > currentX - 30.0f && mousePos.x < currentX + 30.0f &&
                    mousePos.y > currentY - 40.0f && mousePos.y < currentY + 40.0f) {
                    
                    if (i < m_OwnedDiceTypes.size()) {
                        m_SelectedInventoryIndex = i; 
                        m_IsInfoPanelOpen = true; // 🌟 開啟資訊面板！
                        LOG_DEBUG("開啟面板：備戰區第 {} 顆骰子", i);
                    }
                    break;
                }
            }
        }
    }
    
    // ===== 繪製資訊面板 (置頂顯示) =====
    if (m_IsInfoPanelOpen && m_InfoPanelImage && m_SelectedInventoryIndex != -1) {
        
        // 1. 畫出背景面板 (放大到跟背包背景一樣大 960x960)
        Core::Matrices panelData;
        panelData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.95f)), glm::vec3(960.0f, 960.0f, 1.0f));
        panelData.m_Projection = sharedProjection;
        m_InfoPanelImage->Draw(panelData);

        // 2. 定義左上角圖示區的基準座標 (請根據實際畫面微調)
        float iconBaseX = -120.0f; 
        float iconBaseY = 180.0f;

        // 3. 先畫出備戰區的「底框」 (Z軸為 0.96f，要在面板之上，骰子之下)
        if (m_FrameOrdinary) {
            Core::Matrices frameData;
            frameData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(iconBaseX, iconBaseY, 0.96f)), glm::vec3(165.0f, 165.0f, 1.0f));
            frameData.m_Projection = sharedProjection;
            m_FrameOrdinary->Draw(frameData);
        }

        // 4. 再畫出該骰子的圖示 (Z軸為 0.97f)
        // 這裡套用之前備戰區的相對位移 (-2.5f, +17.0f)，讓骰子完美置中於框內
        Core::Matrices iconData;
        iconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(iconBaseX - 2.5f, iconBaseY + 25.0f, 0.97f)), glm::vec3(75.0f, 75.0f, 1.0f));
        iconData.m_Projection = sharedProjection;
        m_AllDiceIcons[m_SelectedInventoryIndex]->Draw(iconData);

        // 5. 根據選中的骰子更新文字內容並畫出 (Z軸為 0.98f)
        DiceType type = m_OwnedDiceTypes[m_SelectedInventoryIndex];
        std::string nameStr = "";
        std::string rarityStr = u8"普通"; // 預設為普通
        std::string atkStr = "";
        std::string spdStr = "";

        switch(type) {
            case DiceType::Ice: nameStr = u8"冰骰子"; atkStr = "30"; spdStr = "1.5s"; rarityStr = u8"稀有"; break;
            case DiceType::Poison: nameStr = u8"毒骰子"; atkStr = "20"; spdStr = "1.3s"; rarityStr = u8"普通"; break;
            case DiceType::Wind: nameStr = u8"風骰子"; atkStr = "20"; spdStr = "0.4s"; rarityStr = u8"英雄"; break;
            case DiceType::Electric: nameStr = u8"電骰子"; atkStr = "30"; spdStr = "0.7s"; rarityStr = u8"稀有"; break;
            case DiceType::Iron: nameStr = u8"鐵骰子"; atkStr = "100"; spdStr = "1.0s"; rarityStr = u8"普通"; break;
        }

        // --- 繪製名稱 (面板正上方) ---
        m_InfoNameText->SetText(nameStr);
        Core::Matrices nameData;
        glm::vec2 nameSize = m_InfoNameText->GetSize();
        nameData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 320.0f, 0.98f)), glm::vec3(nameSize.x, nameSize.y, 1.0f));
        nameData.m_Projection = sharedProjection;
        m_InfoNameText->Draw(nameData);

        // --- 🌟 繪製稀有度 (在名稱正下方) ---
        m_InfoRarityText->SetText(rarityStr);
        Core::Matrices rarityData;
        glm::vec2 raritySize = m_InfoRarityText->GetSize();
        // Y 座標設為 285.0f，剛好在名稱 (320.0f) 下方
        rarityData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 290.0f, 0.98f)), glm::vec3(raritySize.x, raritySize.y, 1.0f));
        rarityData.m_Projection = sharedProjection;
        m_InfoRarityText->Draw(rarityData);

        // --- 🌟 繪製左側：攻擊力標籤與數值 ---
        float leftColX = -85.0f; // 左側欄位的 X 基準
        
        // 黑字標籤 (偏上方)
        Core::Matrices atkLabelData;
        glm::vec2 atkLabelSize = m_InfoAtkLabel->GetSize();
        atkLabelData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(leftColX, 57.0f, 0.98f)), glm::vec3(atkLabelSize.x, atkLabelSize.y, 1.0f));
        atkLabelData.m_Projection = sharedProjection;
        m_InfoAtkLabel->Draw(atkLabelData);

        // 白字數值 (在標籤下方)
        m_InfoAtkText->SetText(atkStr);
        Core::Matrices atkData;
        glm::vec2 atkSize = m_InfoAtkText->GetSize();
        atkData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(leftColX-18.0f, 37.0f, 0.98f)), glm::vec3(atkSize.x, atkSize.y, 1.0f));
        atkData.m_Projection = sharedProjection;
        m_InfoAtkText->Draw(atkData);

        // --- 🌟 繪製右側：攻速標籤與數值 ---
        float rightColX = 97.0f; // 右側欄位的 X 基準

        // 黑字標籤 (偏上方)
        Core::Matrices spdLabelData;
        glm::vec2 spdLabelSize = m_InfoSpeedLabel->GetSize();
        spdLabelData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(rightColX, 57.0f, 0.98f)), glm::vec3(spdLabelSize.x, spdLabelSize.y, 1.0f));
        spdLabelData.m_Projection = sharedProjection;
        m_InfoSpeedLabel->Draw(spdLabelData);

        // 白字數值 (在標籤下方)
        m_InfoSpeedText->SetText(spdStr);
        Core::Matrices spdData;
        glm::vec2 spdSize = m_InfoSpeedText->GetSize();
        spdData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(rightColX-18.0f, 37.0f, 0.98f)), glm::vec3(spdSize.x, spdSize.y, 1.0f));
        spdData.m_Projection = sharedProjection;
        m_InfoSpeedText->Draw(spdData);

        // --- 🌟 繪製「使用」按鈕文字 ---
        if (m_InfoUseBtnText) {
            Core::Matrices useBtnTextData;
            glm::vec2 textSize = m_InfoUseBtnText->GetSize();
            
            // 💡 這裡的座標我根據你之前的點擊範圍 (X: 12~112, Y: -163~-120) 抓了大概的中心點
            // 如果執行後覺得偏上、偏下或偏左右，請直接微調 62.0f 和 -141.0f 這兩個數字！
            useBtnTextData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, -213.0f, 0.98f)), glm::vec3(textSize.x, textSize.y, 1.0f));
            useBtnTextData.m_Projection = sharedProjection;
            
            m_InfoUseBtnText->Draw(useBtnTextData);
        }
    }

    // 🌟 新增：繪製「等待替換」狀態的懸浮骰子
    if (m_IsWaitingToSwap && m_SelectedInventoryIndex != -1) {
        // 設定懸浮位置為畫面正中央偏下
        float floatX = 0.0f;     
        float floatY = -150.0f;  
        
        // 1. 畫底框 (Z軸設為 0.98f 確保顯示在最上層)
        if (m_FrameOrdinary) {
            Core::Matrices frameData;
            frameData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(floatX, floatY, 0.98f)), glm::vec3(130.0f, 130.0f, 1.0f));
            frameData.m_Projection = sharedProjection;
            m_FrameOrdinary->Draw(frameData);
        }
        
        // 2. 畫選中骰子的圖示 (Z軸設為 0.99f 疊在框上面)
        if (m_SelectedInventoryIndex < m_AllDiceIcons.size() && m_AllDiceIcons[m_SelectedInventoryIndex]) {
            Core::Matrices iconData;
            // 套用與備戰區相同的相對位移讓它置中於框內
            iconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(floatX - 2.5f, floatY + 17.0f, 0.99f)), glm::vec3(60.0f, 60.0f, 1.0f));
            iconData.m_Projection = sharedProjection;
            m_AllDiceIcons[m_SelectedInventoryIndex]->Draw(iconData);
        }
    }

    // 🌟 繪製寶石與金幣 (在畫面上方)
    if (m_GemText) {
        // 設定寶石數值與位置
        m_GemText->SetText(std::to_string(m_GemCount));
        Core::Matrices gemData;
        glm::vec2 size = m_GemText->GetSize();
        // 座標微調：請根據畫面背景的圖示位置微調 X 和 Y
        gemData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, 340.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        gemData.m_Projection = sharedProjection;
        m_GemText->Draw(gemData);
    }

    if (m_GoldText) {
        // 設定金幣數值與位置
        m_GoldText->SetText(std::to_string(m_GoldCount));
        Core::Matrices goldData;
        glm::vec2 size = m_GoldText->GetSize();
        // 座標微調：請根據畫面背景的圖示位置微調 X 和 Y
        goldData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(135.0f, 340.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        goldData.m_Projection = sharedProjection;
        m_GoldText->Draw(goldData);
    }
    
    // --- 離開與返回邏輯 ---
    // 1. 按下 ESC 鍵，回到主選單
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentState = State::MENU; 
    }
    
    // 2. 點擊視窗右上角的 X，直接結束程式
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;  
    }
}


void App::InitGame(bool isEndlessMode) {
    m_IsEndlessMode = isEndlessMode;

    // 🌟 【重大修復】進入新遊戲前，務必清空上一場殘留的物件與狀態！
    m_Monsters.clear();
    m_Bullets.clear();

    m_PowerUpIcons.clear();
    m_PowerUpCostTexts.clear();
    m_PipCountTexts.clear();
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            m_Board[r][c] = nullptr;
        }
    }
    
    // 重置所有遊戲數值
    m_CurrentWave = 1;
    m_CurrentNormalHp = 100;
    m_PlayerHp = 3;
    m_SmallMonsterSpawnCount = 0;
    m_MonstersSpawnedThisWave = 0;
    m_IsSpawningWave = true;
    m_WaveDelayTimer = 0.0f;
    m_WaveSpawnTimer = 1.0f;
    m_SpCost = 10;
    for (int i = 0; i < 5; i++) {
        m_PowerUpLevels[i] = 1;
        m_PowerUpCosts[i] = 100;
    }
    
    // 無限模式 100 SP 開局，練習模式給 10000 爽玩
    m_CurrentSp = m_IsEndlessMode ? 100 : 10000;
    // 這裡包含原本在 Start 中的所有初始化代碼
    m_BackgroundImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Board.png");
    m_SummonBtn = std::make_unique<Button>(RESOURCE_DIR "/SpawnDice.png", glm::vec2(0.0f, -140.0f), glm::vec2(150.0f, 150.0f));
    m_CostText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 23, "10", Util::Color::FromName(Util::Colors::BLACK));                                                
    m_SpBarImage = std::make_shared<Util::Image>(RESOURCE_DIR "/SP.png");
    m_SpTotalText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, std::to_string(m_CurrentSp), Util::Color::FromName(Util::Colors::BLACK));
    m_SpLevelIcon = std::make_shared<Util::Image>(RESOURCE_DIR "/SP_Levels.png");
    m_SilenceIcon = std::make_shared<Util::Image>(RESOURCE_DIR "/BOSS/Silence/boss_silence.png");

    // 🌟 新增載入血量 UI 圖片
    m_HpImage = std::make_shared<Util::Image>(RESOURCE_DIR "/HP.png");
    m_HpLossImage = std::make_shared<Util::Image>(RESOURCE_DIR "/HP_loss.png");

    // 🌟 新增 WAVE 文字初始化
    // 字體大小設為 30，顏色設定為白色
    m_WaveText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 30, "WAVE 1", Util::Color::FromName(Util::Colors::WHITE));

    InitializeWaypoints();

    
    for(int i = 0; i < 5; i++) {
        std::string diceName = "";
        
        // 根據目前出戰的骰子種類，動態決定要讀取哪個資料夾的圖片
        switch (m_CurrentDeck[i]) {
            case DiceType::Ice:      diceName = "Ice";      break;
            case DiceType::Poison:   diceName = "Poison";   break;
            case DiceType::Wind:     diceName = "Wind";     break;
            case DiceType::Electric: diceName = "Electric"; break;
            case DiceType::Iron:     diceName = "Iron";     break;
        }

        std::string path = RESOURCE_DIR "/" + diceName + "/" + diceName + "_Level_1.png";
        m_PowerUpIcons.push_back(std::make_shared<Util::Image>(path));
        m_PowerUpCostTexts.push_back(std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 20, "100", Util::Color::FromName(Util::Colors::WHITE)));
        m_PipCountTexts.push_back(std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 22, "0", Util::Color::FromName(Util::Colors::BLACK)));
    }
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
        row = r; col = c; 
        return true;
    }
    return false;
}

void App::SpawnMonsters() {
    // 狀態一：正在生成本波怪物
    if (m_IsSpawningWave) {
        float deltaTime = static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;

        // 🌟 波次開場的 2 秒等待時間
        if (m_WaveDelayTimer < 2.0f) {
            m_WaveDelayTimer += deltaTime;
            return; // 時間還沒到 2 秒，直接離開，不推進生怪計時器
        }

        // 2 秒等待結束後，才開始正常推進生怪計時器
        m_WaveSpawnTimer += deltaTime;

        // 計算血量乘數 (打完 BOSS 後，11, 21, 31 關才開始指數增加)
        float hpMultiplier = std::pow(1.1f, (m_CurrentWave - 1) / 10);

        if (m_WaveSpawnTimer >= 1.0f) {
            m_WaveSpawnTimer -= 1.0f; 

            // 🌟 計算基底 SP：每 5 關 +10，上限 50
            int baseSp = 10 + ((m_CurrentWave - 1) / 5) * 10;
            if (baseSp > 50) baseSp = 50;

            // 🌟 獨立機制：每 10 關是 BOSS 關
            if (m_CurrentWave % 10 == 0) {
                int bossHp = static_cast<int>(m_CurrentNormalHp * 10 * hpMultiplier);
                
                int bossEncounter = m_CurrentWave / 10;
                std::string bossImagePath;
                std::shared_ptr<Monster> boss = nullptr;
                
                if (bossEncounter % 3 == 1) {
                    bossImagePath = RESOURCE_DIR "/BOSS/Snake/boss_snake.png";
                    boss = std::make_shared<Monster>(bossImagePath, m_MonsterWaypoints, 0.5f, bossHp, baseSp * 20);
                    boss->SetBoss(BossType::Snake, 2.0f, 10.0f); // 🌟 蛇王：進場2秒放招，冷卻10秒
                } else if (bossEncounter % 3 == 2) {
                    bossImagePath = RESOURCE_DIR "/BOSS/Silence/boss_silence.png";
                    boss = std::make_shared<Monster>(bossImagePath, m_MonsterWaypoints, 0.5f, bossHp, baseSp * 20);
                    boss->SetBoss(BossType::Silence, 2.0f, 7.0f); // 🌟 沉默：進場2秒放招，冷卻7秒
                } else {
                    bossImagePath = RESOURCE_DIR "/BOSS/Knights/boss_knights.png";
                    boss = std::make_shared<Monster>(bossImagePath, m_MonsterWaypoints, 0.5f, bossHp, baseSp * 20);
                    boss->SetBoss(BossType::Knights, 2.0f, 10.0f); // 🌟 騎士：進場2秒放招，冷卻10秒
                }

                // BOSS 給 20 倍 SP
                
                boss->SetSize(glm::vec2(75.0f, 75.0f));
                m_Monsters.push_back(boss);

                m_IsSpawningWave = false; 
                return; // BOSS 關只出一隻，直接中斷
            }

            // ---------- 一般關卡生怪邏輯 ----------
            
            m_MonstersSpawnedThisWave++; 
            int spawnIndex = m_MonstersSpawnedThisWave;

            bool isFastWave = ((m_CurrentWave * 2) % 5 == 0) || ((m_CurrentWave * 2) % 5 == 1);
            bool isMiniBossWave = (m_CurrentWave % 5 == 0);

            std::shared_ptr<Monster> monster = nullptr;

            if (isMiniBossWave && spawnIndex == 1) {
                int miniBossHp = static_cast<int>(m_CurrentNormalHp * 5 * hpMultiplier);
                // 巨型怪給 5 倍 SP
                monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/4.png", m_MonsterWaypoints, 1.5f, miniBossHp, baseSp * 5);
            } 
            else {
                // 處理普通與快速怪物的血量成長 (+10 邏輯)
                if (m_SmallMonsterSpawnCount > 0 && m_SmallMonsterSpawnCount % 2 == 0) {
                    m_CurrentNormalHp += 10;
                }
                m_SmallMonsterSpawnCount++; 

                int scaledHp = static_cast<int>(m_CurrentNormalHp * hpMultiplier);

                if (isFastWave && (spawnIndex == 2 || spawnIndex == 4 || spawnIndex == 6 || spawnIndex == 8 || spawnIndex == 10)) {
                    // 快速怪給 2 倍 SP
                    monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/3.png", m_MonsterWaypoints, 3.0f, scaledHp, baseSp * 2);
                } 
                else {
                    // 普通怪給 1 倍 SP
                    monster = std::make_shared<Monster>(RESOURCE_DIR "/monster/2.png", m_MonsterWaypoints, 1.5f, scaledHp, baseSp);
                }
            }

            m_Monsters.push_back(monster);

            if (m_MonstersSpawnedThisWave >= 20) {
                m_IsSpawningWave = false; 
            }
        }
    } 
    // 狀態二：等待場上怪物清空，進入下一波
    else {
        if (m_Monsters.empty()) {
            // 瞬間切換波次、更新血量、更新 UI
            m_CurrentWave++;
            m_CurrentNormalHp += 50;
            
            // 更新 UI 文字
            if (m_WaveText) {
                m_WaveText->SetText("WAVE " + std::to_string(m_CurrentWave));
            }

            // 重置狀態，準備進入下一波的 2 秒等待
            m_IsSpawningWave = true;
            m_MonstersSpawnedThisWave = 0;
            m_WaveDelayTimer = 0.0f; // 重置 2 秒等待計時器
            m_WaveSpawnTimer = 1.0f; // 設為 1.0f，這樣 2 秒一過就會「立刻」生出第一隻怪
        }
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

        DiceType selectedDice = m_CurrentDeck[rand() % 5];
        switch (selectedDice) {
            case DiceType::Ice: m_Board[targetRow][targetCol] = std::make_shared<IceDice>(pos); break;
            case DiceType::Poison: m_Board[targetRow][targetCol] = std::make_shared<PoisonDice>(pos); break;
            case DiceType::Wind: m_Board[targetRow][targetCol] = std::make_shared<WindDice>(pos); break;
            case DiceType::Electric: m_Board[targetRow][targetCol] = std::make_shared<ElectricDice>(pos); break;
            case DiceType::Iron: m_Board[targetRow][targetCol] = std::make_shared<IronDice>(pos); break;
        }
    }
}

void App::Update() {
    glm::mat4 sharedProjection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, -1.0f, 2.0f);
    SpawnMonsters(); 
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    float upgradeStartX = -177.0f; 
    float upgradeGap = 91.0f;
    float upgradeBtnY = -221.0f; 

    if (!m_IsDragging && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        for(int i = 0; i < 5; i++) {
            float btnX = upgradeStartX + i * upgradeGap;
            
            if (mousePos.x > btnX - 45 && mousePos.x < btnX + 45 &&
                mousePos.y > upgradeBtnY - 45 && mousePos.y < upgradeBtnY + 45) {
                
                if (m_PowerUpLevels[i] < 5 && m_CurrentSp >= m_PowerUpCosts[i]) {
                    m_CurrentSp -= m_PowerUpCosts[i]; 
                    m_PowerUpLevels[i]++; 
                    
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 5; c++) {
                            if (m_Board[r][c] != nullptr && m_Board[r][c]->GetType() == m_CurrentDeck[i]) {
                                m_Board[r][c]->PowerUp();
                            }
                        }
                    }

                    std::string diceName = "";
                    switch (m_CurrentDeck[i]) {
                        case DiceType::Ice:      diceName = "Ice";      break;
                        case DiceType::Poison:   diceName = "Poison";   break;
                        case DiceType::Wind:     diceName = "Wind";     break;
                        case DiceType::Electric: diceName = "Electric"; break;
                        case DiceType::Iron:     diceName = "Iron";     break;
                    }
                    std::string levelStr = (m_PowerUpLevels[i] == 5) ? "max" : std::to_string(m_PowerUpLevels[i]);
                    std::string newPath = RESOURCE_DIR "/" + diceName + "/" + diceName + "_Level_" + levelStr + ".png";
                    m_PowerUpIcons[i] = std::make_shared<Util::Image>(newPath);

                    if (m_PowerUpLevels[i] == 2) m_PowerUpCosts[i] = 200;
                    else if (m_PowerUpLevels[i] == 3) m_PowerUpCosts[i] = 400;
                    else if (m_PowerUpLevels[i] == 4) m_PowerUpCosts[i] = 700;
                    else m_PowerUpCosts[i] = -1; 

                    if (m_PowerUpCosts[i] == -1) {
                        m_PowerUpCostTexts[i]->SetText("0"); 
                    } else {
                        m_PowerUpCostTexts[i]->SetText(std::to_string(m_PowerUpCosts[i]));
                    }
                    m_SpTotalText->SetText(std::to_string(m_CurrentSp));
                }
            }
        }
    }

    if (!m_IsEndlessMode && !m_IsDragging && Util::Input::IsKeyDown(Util::Keycode::MOUSE_RB)) {
        int r, c;
        if (GetGridIndex(mousePos, r, c) && m_Board[r][c] != nullptr) m_Board[r][c] = nullptr; 
    }

    if (!m_IsDragging && (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB))) {
        int r, c;
        if (GetGridIndex(mousePos, r, c) && m_Board[r][c] != nullptr && !m_Board[r][c]->IsSilenced()) {
            m_IsDragging = true;
            m_DragStartRow = r; m_DragStartCol = c;
        }
    }

    if (m_IsDragging && (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) || Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB))) {
        if (m_Board[m_DragStartRow][m_DragStartCol] != nullptr) m_Board[m_DragStartRow][m_DragStartCol]->SetPos(mousePos);
    }

    if (m_IsDragging && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        m_IsDragging = false; 
        int targetR, targetC;
        bool validDrop = GetGridIndex(mousePos, targetR, targetC);
        auto draggedDice = m_Board[m_DragStartRow][m_DragStartCol];

        if (validDrop && draggedDice != nullptr) {
            if (targetR == m_DragStartRow && targetC == m_DragStartCol) {
                draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol)); 
            } else {
                auto targetDice = m_Board[targetR][targetC];
                if (targetDice == nullptr) {
                    draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol));
                } else {
                    if (draggedDice->GetType() == targetDice->GetType() && draggedDice->GetStarLevel() == targetDice->GetStarLevel()) {
                        targetDice->UpgradeStar();
                        m_Board[m_DragStartRow][m_DragStartCol] = nullptr; 
                        targetDice->SetPos(GetTilePosition(targetR, targetC)); 
                    } else {
                        draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol));
                    }
                }
            }
        } else if (draggedDice != nullptr) {
            draggedDice->SetPos(GetTilePosition(m_DragStartRow, m_DragStartCol));
        }
    }

    int pipCounts[5] = {0, 0, 0, 0, 0};
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            if (m_Board[r][c] != nullptr) {
                DiceType type = m_Board[r][c]->GetType();
                int pips = m_Board[r][c]->GetStarLevel();
                for(int i = 0; i < 5; i++) {
                    if (m_CurrentDeck[i] == type) {
                        pipCounts[i] += pips;
                        break;
                    }
                }
            }
        }
    }
    for(int i = 0; i < 5; i++) {
        m_PipCountTexts[i]->SetText(std::to_string(pipCounts[i]));
    }

    std::vector<std::shared_ptr<Monster>> newSummonedMonsters;
    // 🌟 替換成新的怪物生命週期判定
    for (auto it = m_Monsters.begin(); it != m_Monsters.end(); ) {
    (*it)->Update(); 

    // ==========================================
        // 🌟 BOSS 技能發動判定
        // ==========================================
        if ((*it)->PopSkillTrigger()) {
            BossType type = (*it)->GetBossType();
            
            if (type == BossType::Snake) {
                LOG_DEBUG("蛇王發動技能！召喚小兵");
                float hpMult = std::pow(1.1f, (m_CurrentWave - 1) / 10);
                int normalHp = static_cast<int>(m_CurrentNormalHp * hpMult);
                int baseSp = 10 + ((m_CurrentWave - 1) / 5) * 10;
                
                // Lambda 小幫手：生成並設定位置在蛇王腳下
                auto spawnMob = [&](std::string path, float speed, int hp, int sp) {
                    auto mob = std::make_shared<Monster>(path, m_MonsterWaypoints, speed, hp, sp);
                    mob->SetSpawnState((*it)->GetPos(), (*it)->GetWaypointIndex());
                    return mob;
                };

                newSummonedMonsters.push_back(spawnMob(RESOURCE_DIR "/monster/4.png", 1.5f, normalHp * 5, baseSp * 5)); // 小王
                newSummonedMonsters.push_back(spawnMob(RESOURCE_DIR "/monster/3.png", 3.0f, normalHp, baseSp * 2));     // 快怪
                newSummonedMonsters.push_back(spawnMob(RESOURCE_DIR "/monster/2.png", 1.5f, normalHp, baseSp));         // 普怪1
                newSummonedMonsters.push_back(spawnMob(RESOURCE_DIR "/monster/2.png", 1.5f, normalHp, baseSp));         // 普怪2
            }
            else if (type == BossType::Silence) {
                LOG_DEBUG("沉默王發動技能！沉默隨機骰子");
                std::vector<std::pair<int, int>> validTargets;
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 5; c++) {
                        if (m_Board[r][c] != nullptr && !m_Board[r][c]->IsSilenced()) validTargets.push_back({r, c});
                    }
                }
                // 隨機打亂並挑選 2 顆
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(validTargets.begin(), validTargets.end(), g);
                
                int count = 0;
                for (auto& target : validTargets) {
                    m_Board[target.first][target.second]->SetSilenced(true);
                    if (++count >= 2) break;
                }
            }
            else if (type == BossType::Knights) {
                LOG_DEBUG("騎士王發動技能！全盤洗牌");
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 5; c++) {
                        if (m_Board[r][c] != nullptr) {
                            int stars = m_Board[r][c]->GetStarLevel();
                            glm::vec2 pos = m_Board[r][c]->GetPos();
                            
                            DiceType newType = m_CurrentDeck[rand() % 5];
                            std::shared_ptr<Dice> newDice = nullptr;
                            switch (newType) {
                                case DiceType::Ice: newDice = std::make_shared<IceDice>(pos); break;
                                case DiceType::Poison: newDice = std::make_shared<PoisonDice>(pos); break;
                                case DiceType::Wind: newDice = std::make_shared<WindDice>(pos); break;
                                case DiceType::Electric: newDice = std::make_shared<ElectricDice>(pos); break;
                                case DiceType::Iron: newDice = std::make_shared<IronDice>(pos); break;
                            }
                            
                            // 繼承原本的星數
                            for (int i = 1; i < stars; i++) newDice->UpgradeStar();
                            
                            // 繼承該新種類的全局等級
                            int typeIndex = 0;
                            for(int i = 0; i < 5; i++) { if(m_CurrentDeck[i] == newType) { typeIndex = i; break; } }
                            for (int i = 1; i < m_PowerUpLevels[typeIndex]; i++) newDice->PowerUp();
                            
                            m_Board[r][c] = newDice; // 直接覆蓋
                        }
                    }
                }
            }
        }
    
        // ==========================================
        // 🌟 怪物死亡與生命週期判定
        // ==========================================
        if ((*it)->IsFinished() || (*it)->IsDead()) {
            
            if ((*it)->GetBossType() == BossType::Silence) {
                for(int r = 0; r < 3; r++) {
                    for(int c = 0; c < 5; c++) {
                        if (m_Board[r][c]) m_Board[r][c]->SetSilenced(false);
                    }
                }
            }

            if ((*it)->IsFinished()) {
                if (m_PlayerHp > 0) {
                    m_PlayerHp--;
                }
                
                it = m_Monsters.erase(it); 
                
                if (m_IsEndlessMode && m_PlayerHp <= 0) {
                    if (m_GameOverWaveText) {
                        m_GameOverWaveText->SetText(u8"回合 "+ std::to_string(m_CurrentWave));
                    }
                    
                    int cardsEarned = 0;
                    for (int i = 1; i <= m_CurrentWave; i++) {
                        if (i % 10 == 0) {
                            cardsEarned += 6; 
                        } else if (i % 5 == 0) {
                            cardsEarned += 2; 
                        } else {
                            cardsEarned += 1; 
                        }
                    }

                    if (m_GameOverCardText) {
                        m_GameOverCardText->SetText("+" + std::to_string(cardsEarned));
                    }
                    
                    // 🌟 儲存卡片數量
                    m_TotalCards += cardsEarned; 
                    LOG_DEBUG("目前總卡片數量：{}", m_TotalCards);

                    m_CurrentState = State::GAMEOVER;
                }
            } else {
                m_CurrentSp += (*it)->GetSpReward();
                if (m_SpTotalText) m_SpTotalText->SetText(std::to_string(m_CurrentSp));
                it = m_Monsters.erase(it); 
            }
        } else {
            ++it; 
        }
    } 

    // 將蛇王召喚出來的小怪正式加入場上
    m_Monsters.insert(m_Monsters.end(), newSummonedMonsters.begin(), newSummonedMonsters.end());

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 5; c++) {
            if (m_Board[r][c] != nullptr && !(m_IsDragging && r == m_DragStartRow && c == m_DragStartCol)) {
                m_Board[r][c]->Update(m_Monsters, m_Bullets); 
            }
        }
    }

    // 🌟 處理子彈與怪物的碰撞
    for (auto it = m_Bullets.begin(); it != m_Bullets.end(); ) {
        (*it)->Update();
        if ((*it)->IsInvalid()) it = m_Bullets.erase(it); 
        else if ((*it)->IsHit()) {
            auto target = (*it)->GetTarget();
            
            // 造成傷害
            int finalDamage = (*it)->GetDamage();
            
            // 隨機產生 0~99 的數字，如果小於 5，代表 5% 機率觸發爆擊
            if (rand() % 100 < 5) {
                finalDamage *= 20; // 爆擊傷害 2000% (乘以 20 倍)
            }
            
            target->ReceiveDamage(finalDamage);
            
            if ((*it)->IsIce()) target->ApplyIceEffect(0.05f + (m_PowerUpLevels[0] - 1) * 0.02f);
            if ((*it)->IsPoison()) target->ApplyPoisonEffect(50 + (m_PowerUpLevels[1] - 1) * 25);

            // 🌟 處理電骰的彈射邏輯
            if ((*it)->GetBouncesLeft() > 0) {
                // 先記住當前擊中目標的路徑進度，用來判斷誰在它「後面」
                int currentWp = (*it)->GetTarget()->GetWaypointIndex();
                float currentDist = (*it)->GetTarget()->GetDistanceToNextWaypoint();

                (*it)->DecreaseBounce(); 
                int left = (*it)->GetBouncesLeft();
                
                // 計算彈射傷害 (30 + 每級20)
                int bounceBase = 30 + (m_PowerUpLevels[3] - 1) * 20;
                int nextDmg = 0;

                if (left == 1) { // 準備彈向第 2 隻怪
                    nextDmg = static_cast<int>(bounceBase * 0.7f);
                } else if (left == 0) { // 準備彈向第 3 隻怪
                    nextDmg = static_cast<int>(bounceBase * 0.3f);
                }
                
                (*it)->SetDamage(nextDmg);

                // 🌟 新的尋敵邏輯：往後找緊跟著的下一隻
                std::shared_ptr<Monster> nextTarget = nullptr;
                int bestWp = -1;
                float bestDist = 999999.0f;

                for (auto& monster : m_Monsters) {
                    if (!monster->IsDead() && monster != (*it)->GetTarget()) {
                        int mWp = monster->GetWaypointIndex();
                        float mDist = monster->GetDistanceToNextWaypoint();

                        // 判斷條件：必須在當前目標的「後方」
                        // Wp 比較小，或是 Wp 相同但距離下個轉角比較遠 (距離數值較大)
                        bool isBehind = (mWp < currentWp) || (mWp == currentWp && mDist > currentDist);

                        if (isBehind) {
                            // 在所有後方的怪物中，挑選進度「最前面」的一隻
                            if (mWp > bestWp || (mWp == bestWp && mDist < bestDist)) {
                                bestWp = mWp;
                                bestDist = mDist;
                                nextTarget = monster;
                            }
                        }
                    }
                }

                if (nextTarget) { 
                    (*it)->SetTarget(nextTarget); 
                    ++it; 
                } 
                else {
                    // 如果後面已經沒有怪物了，子彈就直接消失
                    it = m_Bullets.erase(it); 
                }
            } else {
                it = m_Bullets.erase(it); 
            }
        } else ++it;
    }

    // --- 繪圖區 ---
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
            // 🌟 加上大括號 {，把畫骰子跟畫沉默圖示包在一起！
            if (m_Board[r][c] != nullptr && !(m_IsDragging && r == m_DragStartRow && c == m_DragStartCol)) {
                
                m_Board[r][c]->Draw(sharedProjection);

                if (m_Board[r][c]->IsSilenced() && m_SilenceIcon) {
                    Core::Matrices overlayData;
                    overlayData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Board[r][c]->GetPos(), 0.96f)), glm::vec3(70.0f, 70.0f, 1.0f));
                    overlayData.m_Projection = sharedProjection;
                    m_SilenceIcon->Draw(overlayData);
                }
                
            }
        }
    }
    if (m_SummonBtn) m_SummonBtn->Draw(sharedProjection);

    for(int i = 0; i < 5; i++) {
        float btnX = upgradeStartX + i * upgradeGap;
        
        Core::Matrices iconData;
        iconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(btnX, upgradeBtnY, 0.9f)), glm::vec3(90.0f, 90.0f, 1.0f));
        iconData.m_Projection = sharedProjection;
        m_PowerUpIcons[i]->Draw(iconData);

        if (m_SpLevelIcon) {
            Core::Matrices spIconData;
            spIconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(btnX - 35.0f, upgradeBtnY - 56.0f, 0.95f)), glm::vec3(50.0f, 50.0f, 1.0f));
            spIconData.m_Projection = sharedProjection;
            m_SpLevelIcon->Draw(spIconData);
        }

        Core::Matrices costData;
        glm::vec2 costSize = m_PowerUpCostTexts[i]->GetSize();
        float textX = btnX + 9.0f; 
        costData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(textX, upgradeBtnY - 55.0f, 0.95f)), glm::vec3(costSize.x, costSize.y, 1.0f));
        costData.m_Projection = sharedProjection;
        m_PowerUpCostTexts[i]->Draw(costData);

        Core::Matrices pipData;
        glm::vec2 pipSize = m_PipCountTexts[i]->GetSize();
        float pipX = btnX + 34.0f;
        float pipY = upgradeBtnY + 33.0f;
        pipData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(pipX, pipY, 0.95f)), glm::vec3(pipSize.x, pipSize.y, 1.0f));
        pipData.m_Projection = sharedProjection;
        m_PipCountTexts[i]->Draw(pipData);
    }

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
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -167.0f, 0.9f)), glm::vec3(size.x, size.y, 1.0f)); 
        textData.m_Projection = sharedProjection;
        m_CostText->Draw(textData);
    }

    if (m_IsDragging && m_Board[m_DragStartRow][m_DragStartCol] != nullptr) m_Board[m_DragStartRow][m_DragStartCol]->Draw(sharedProjection, 0.95f);

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

    // 🌟 新增：繪製玩家血量 UI
    float heartStartX = -168.0f; // 第一顆愛心的 X 座標 (可依畫面微調)
    float heartStartY = -125.0f; // 愛心的 Y 座標 (可依畫面微調)
    float heartGap = 40.0f;      // 愛心之間的間距
    float heartSize = 77.0f;     // 愛心的長寬大小

    for (int i = 0; i < 3; ++i) {
        Core::Matrices hpData;
        float currentX = heartStartX + (i * heartGap);
        
        // 設定愛心的矩陣位置與大小
        hpData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(currentX, heartStartY, 0.9f)), glm::vec3(heartSize, heartSize, 1.0f));
        hpData.m_Projection = sharedProjection;

        // 依據目前血量決定要畫紅心還是灰心
        if (i < m_PlayerHp) {
            if (m_HpImage) m_HpImage->Draw(hpData);
        } else {
            if (m_HpLossImage) m_HpLossImage->Draw(hpData);
        }
    }

    // 🌟 新增繪製 WAVE 文字
    if (m_WaveText) {
        Core::Matrices waveData;
        glm::vec2 textSize = m_WaveText->GetSize();
        
        // 座標微調：X 偏左、Y 偏上。你可以更改 -230.0f 和 250.0f 來對齊你想要的完美位置
        float waveTextX = -190.0f; 
        float waveTextY = 255.0f;  
        
        waveData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(waveTextX, waveTextY, 0.95f)), glm::vec3(textSize.x, textSize.y, 1.0f));
        waveData.m_Projection = sharedProjection;
        m_WaveText->Draw(waveData);
    }

    // 🌟 只有在練習模式 (!m_IsEndlessMode) 才能使用作弊鍵
    if (!m_IsEndlessMode) {
        if (Util::Input::IsKeyUp(Util::Keycode::N)) { m_CurrentSp += 10000; m_SpTotalText->SetText(std::to_string(m_CurrentSp)); }
        if (Util::Input::IsKeyUp(Util::Keycode::M)) {
            for (int r = 0; r < 3; r++) { for (int c = 0; c < 5; c++) m_Board[r][c] = nullptr; }
        }

        if (Util::Input::IsKeyUp(Util::Keycode::Z) || 
            Util::Input::IsKeyUp(Util::Keycode::X) || 
            Util::Input::IsKeyUp(Util::Keycode::C)) {
            
            float hpMultiplier = std::pow(1.1f, (m_CurrentWave - 1) / 10);
            int bossHp = static_cast<int>(m_CurrentNormalHp * 10 * hpMultiplier);
            int baseSp = 10 + ((m_CurrentWave - 1) / 5) * 10;
            if (baseSp > 50) baseSp = 50;

            std::string bossPath;
            std::shared_ptr<Monster> boss = nullptr; // 🌟 補上這行宣告！

            if (Util::Input::IsKeyUp(Util::Keycode::Z)) {
                bossPath = RESOURCE_DIR "/BOSS/Snake/boss_snake.png";
                boss = std::make_shared<Monster>(bossPath, m_MonsterWaypoints, 0.5f, bossHp, baseSp * 20);
                boss->SetBoss(BossType::Snake, 2.0f, 10.0f);
            } else if (Util::Input::IsKeyUp(Util::Keycode::X)) {
                bossPath = RESOURCE_DIR "/BOSS/Silence/boss_silence.png";
                boss = std::make_shared<Monster>(bossPath, m_MonsterWaypoints, 0.5f, bossHp, baseSp * 20);
                boss->SetBoss(BossType::Silence, 2.0f, 7.0f);
            } else if (Util::Input::IsKeyUp(Util::Keycode::C)) {
                bossPath = RESOURCE_DIR "/BOSS/Knights/boss_knights.png";
                boss = std::make_shared<Monster>(bossPath, m_MonsterWaypoints, 0.5f, bossHp, baseSp * 20);
                boss->SetBoss(BossType::Knights, 2.0f, 10.0f);
            }

            if (boss) {
                boss->SetSize(glm::vec2(75.0f, 75.0f));
                m_Monsters.push_back(boss);
            }
        }
    }
    
    // --- 離開與返回邏輯 ---
    // 1. 按下 ESC 鍵，回到主選單
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentState = State::MENU; 
    }
    
    // 2. 點擊視窗右上角的 X，直接結束程式
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;  
    }
}

void App::GameOver() {
    glm::mat4 sharedProjection = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 2.0f);

    if (m_GameOverImage) {
        Core::Matrices bgData;
        bgData.m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(960.0f, 960.0f, 1.0f)); 
        bgData.m_Projection = sharedProjection;
        m_GameOverImage->Draw(bgData);
    }

    // --- 🌟 1. 繪製 WAVE 文字 (發光中心點) ---
    if (m_GameOverWaveText) {
        Core::Matrices waveData;
        glm::vec2 size = m_GameOverWaveText->GetSize();
        // 座標微調：X為0(置中)，Y大約在中心發光處上方一點
        waveData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 55.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        waveData.m_Projection = sharedProjection;
        m_GameOverWaveText->Draw(waveData);
    }

    // --- 🌟 2. 繪製卡片獲得數量 (小卡片圖示旁邊) ---
    if (m_GameOverCardText) {
        Core::Matrices cardData;
        glm::vec2 size = m_GameOverCardText->GetSize();
        // 座標微調：X稍微往右移避開卡片圖示，Y大概在卡片圖示的高度
        cardData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, -10.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        cardData.m_Projection = sharedProjection;
        m_GameOverCardText->Draw(cardData);
    }

    // --- 🌟 3. 繪製出戰的骰子陣容 (深藍色框框內) ---
    float diceStartX = -145.0f; 
    float diceGapX = 74.0f;     
    float diceY = -110.0f; // 💡 這個 Y 座標是我抓深藍色框的預估值，如果有偏上或偏下請微調這個數字！
    
    for (int i = 0; i < 5; i++) {
        float currentX = diceStartX + (i * diceGapX);
        if (i < m_ActiveDeckIcons.size() && m_ActiveDeckIcons[i]) {
            Core::Matrices iconData;
            iconData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(currentX, diceY, 0.95f)), glm::vec3(60.0f, 60.0f, 1.0f));
            iconData.m_Projection = sharedProjection;
            m_ActiveDeckIcons[i]->Draw(iconData);
        }
    }

    // --- 離開與返回邏輯 ---
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentState = State::MENU; 
    }
    // 🌟 新增：點擊滑鼠左鍵判定黃色「確認」按鈕
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        
        // 幫你印出滑鼠座標，如果下面預估的按鈕範圍不夠精準，你可以看終端機微調！
        LOG_DEBUG("結算畫面點擊座標 -> X: {}, Y: {}", mousePos.x, mousePos.y);

        // 💡 預估黃色「確認」按鈕的範圍
        float confirmBtnMinX = -57.0f; 
        float confirmBtnMaxX = 57.0f;  
        float confirmBtnMinY = -235.0f; 
        float confirmBtnMaxY = -190.0f; 

        if (mousePos.x > confirmBtnMinX && mousePos.x < confirmBtnMaxX &&
            mousePos.y > confirmBtnMinY && mousePos.y < confirmBtnMaxY) {
            
            LOG_DEBUG("點擊了確認按鈕，返回主選單！");
            m_CurrentState = State::MENU; 
        }
    }
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;  
    }
}

void App::Chest() {
    glm::mat4 sharedProjection = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 2.0f);

    // 1. 繪製寶箱背景
    if (m_ChestBgImage) {
        Core::Matrices bgData;
        bgData.m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(960.0f, 960.0f, 1.0f)); 
        bgData.m_Projection = sharedProjection;
        m_ChestBgImage->Draw(bgData);
    }

    // 🌟 2. 繪製左側：目前卡片進度 (XX/40)
    if (m_ChestCardText) {
        m_ChestCardText->SetText(std::to_string(m_TotalCards) + "/40");
        Core::Matrices textData;
        glm::vec2 size = m_ChestCardText->GetSize();
        // 💡 預估座標：靠左一點，箭頭的左邊
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-80.0f, 8.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        textData.m_Projection = sharedProjection;
        m_ChestCardText->Draw(textData);
    }

    // 🌟 3. 繪製右側：可開箱數量
    if (m_ChestCountText) {
        m_ChestCountText->SetText(std::to_string(m_TotalCards / 40));
        Core::Matrices textData;
        glm::vec2 size = m_ChestCountText->GetSize();
        // 💡 預估座標：靠右一點，寶箱圖案的旁邊
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(148.0f, 13.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        textData.m_Projection = sharedProjection;
        m_ChestCountText->Draw(textData);
    }

    // --- 🌟 點擊判定邏輯 ---
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        LOG_DEBUG("寶箱畫面點擊 -> X: {}, Y: {}", mousePos.x, mousePos.y);

        // 判定 A：點擊下方黃色「開啟」按鈕
        float openBtnMinX = -50.0f; float openBtnMaxX = 50.0f;
        float openBtnMinY = -185.0f; float openBtnMaxY = -150.0f;
        
        if (mousePos.x > openBtnMinX && mousePos.x < openBtnMaxX &&
            mousePos.y > openBtnMinY && mousePos.y < openBtnMaxY) {
            
            if (m_TotalCards >= 40) {
                m_TotalCards -= 40; // 扣除 40 張卡片
                static bool seeded = false;
                if (!seeded) { srand(static_cast<unsigned int>(time(NULL))); seeded = true; }
                m_OpenedDiceIndex = rand() % 5;
                m_CurrentState = State::OPEN; // 跳轉到開啟畫面
                LOG_DEBUG("開啟寶箱！剩餘卡片: {}", m_TotalCards);
            } else {
                LOG_DEBUG("卡片不足 40 張，無法開啟！");
            }
        }

        // 判定 B：點擊右上角 X 關閉視窗
        float closeBtnMinX = 95.0f; float closeBtnMaxX = 123.0f;
        float closeBtnMinY = 180.0f; float closeBtnMaxY = 206.0f;
        
        if (mousePos.x > closeBtnMinX && mousePos.x < closeBtnMaxX &&
            mousePos.y > closeBtnMinY && mousePos.y < closeBtnMaxY) {
            m_CurrentState = State::MENU; 
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentState = State::MENU; 
    }
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;  
    }
}

// 🌟 新增：OPEN 畫面的基礎骨架
void App::Open() {
    glm::mat4 sharedProjection = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, -1.0f, 2.0f);

    // 1. 繪製開箱背景
    if (m_OpenBgImage) {
        Core::Matrices bgData;
        bgData.m_Model = glm::scale(glm::mat4(1.0f), glm::vec3(960.0f, 960.0f, 1.0f)); 
        bgData.m_Projection = sharedProjection;
        m_OpenBgImage->Draw(bgData);
    }

    // 2. 繪製置中的普通底卡 (Ordinary_dice.png)
    if (m_OrdinaryTagImage) {
        Core::Matrices tagData;
        tagData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 146.0f, 0.95f)), glm::vec3(100.0f, 100.0f, 1.0f));
        tagData.m_Projection = sharedProjection;
        m_OrdinaryTagImage->Draw(tagData);
    }

    // 3. 繪製抽到的骰子圖示 (疊在底卡上)
    if (m_AllDiceIcons[m_OpenedDiceIndex]) {
        Core::Matrices diceData;
        diceData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 90.0f, 0.96f)), glm::vec3(80.0f, 80.0f, 1.0f));
        diceData.m_Projection = sharedProjection;
        m_AllDiceIcons[m_OpenedDiceIndex]->Draw(diceData);
    }

    // 4. 繪製 x100 (骰子右下方) 與 名稱 (骰子正下方)
    if (m_OpenDiceCountText) {
        Core::Matrices countData;
        glm::vec2 size = m_OpenDiceCountText->GetSize();
        countData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 55.0f, 0.97f)), glm::vec3(size.x, size.y, 1.0f));
        countData.m_Projection = sharedProjection;
        m_OpenDiceCountText->Draw(countData);
    }

    std::string nameStr = "";
    switch(m_OpenedDiceIndex) {
        case 0: nameStr = u8"冰骰子"; break;
        case 1: nameStr = u8"毒骰子"; break;
        case 2: nameStr = u8"風骰子"; break;
        case 3: nameStr = u8"電骰子"; break;
        case 4: nameStr = u8"鐵骰子"; break;
    }
    if (m_OpenDiceNameText) {
        m_OpenDiceNameText->SetText(nameStr);
        Core::Matrices nameData;
        glm::vec2 size = m_OpenDiceNameText->GetSize();
        nameData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 30.0f, 0.97f)), glm::vec3(size.x, size.y, 1.0f));
        nameData.m_Projection = sharedProjection;
        m_OpenDiceNameText->Draw(nameData);
    }

    // 5. 繪製寶石 (3) 與 金幣 (2000)
    if (m_OpenGemText) {
        Core::Matrices textData;
        glm::vec2 size = m_OpenGemText->GetSize();
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-39.0f, -26.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        textData.m_Projection = sharedProjection;
        m_OpenGemText->Draw(textData);
    }
    if (m_OpenGoldText) {
        Core::Matrices textData;
        glm::vec2 size = m_OpenGoldText->GetSize();
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(84.0f, -26.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        textData.m_Projection = sharedProjection;
        m_OpenGoldText->Draw(textData);
    }

    // 6. 繪製剩餘卡片進度 (XXX / 40)
    if (m_OpenCardText) {
        m_OpenCardText->SetText(std::to_string(m_TotalCards) + " / 40");
        Core::Matrices textData;
        glm::vec2 size = m_OpenCardText->GetSize();
        // 對齊藍色按鈕上方
        textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(110.0f, -150.0f, 0.95f)), glm::vec3(size.x, size.y, 1.0f));
        textData.m_Projection = sharedProjection;
        m_OpenCardText->Draw(textData);
    }

    // --- 🌟 點擊判定邏輯 ---
    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        LOG_DEBUG("Open Screen Click -> X: " + std::to_string(mousePos.x) + ", Y: " + std::to_string(mousePos.y));

        // 判定 A：黃色「確認」按鈕 (左) -> 回到寶箱畫面
        float confirmBtnMinX = -117.0f; float confirmBtnMaxX = -19.0f;
        float confirmBtnMinY = -150.0f; float confirmBtnMaxY = -115.0f;
        
        if (mousePos.x > confirmBtnMinX && mousePos.x < confirmBtnMaxX &&
            mousePos.y > confirmBtnMinY && mousePos.y < confirmBtnMaxY) {
            m_CurrentState = State::CHEST; 
        }

        // 判定 B：藍色「再開一次」按鈕 (右) -> 直接連抽
        float againBtnMinX = 16.0f;  float againBtnMaxX = 113.0f;
        float againBtnMinY = -150.0f; float againBtnMaxY = -115.0f;
        
        if (mousePos.x > againBtnMinX && mousePos.x < againBtnMaxX &&
            mousePos.y > againBtnMinY && mousePos.y < againBtnMaxY) {
            
            if (m_TotalCards >= 40) {
                m_TotalCards -= 40;
                m_OpenedDiceIndex = rand() % 5; // 再抽一顆新的
                LOG_DEBUG("再開一次！抽到新骰子");
            } else {
                LOG_DEBUG("卡片不足，無法再開！");
            }
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE)) {
        m_CurrentState = State::CHEST; 
    }
    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;  
    }
}



void App::End() { LOG_TRACE("End"); }