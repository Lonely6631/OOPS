#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Time.hpp" 
#include <memory>
#include <string>
#include <vector>
#include <algorithm> 

class Monster {
public:
    Monster(const std::string& imagePath, std::vector<glm::vec2> waypoints, float speed, int hp) 
        : m_Waypoints(waypoints), m_OriginalSpeed(speed), m_CurrentSpeed(speed), m_Hp(hp) {
        m_Image = std::make_shared<Util::Image>(imagePath);
        m_HpText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 15, std::to_string(m_Hp), Util::Color::FromName(Util::Colors::WHITE));
        
        m_IceSnowImage = std::make_shared<Util::Image>(RESOURCE_DIR "/Ice/Ice_snow.png");
        m_PoisonUI = std::make_shared<Util::Image>(RESOURCE_DIR "/Poison/Poison_UI.png");

        if (!m_Waypoints.empty()) {
            m_Pos = m_Waypoints[0];
            m_CurrentWaypointIndex = 1; 
        } else {
            m_Pos = glm::vec2(0.0f);
            m_CurrentWaypointIndex = 0;
        }
    }

    void Update() {
        if (m_Waypoints.empty() || m_CurrentWaypointIndex >= m_Waypoints.size() || IsDead()) return;

        float deltaTime = static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
        
        if (m_IsPoisoned) {
            m_PoisonTickTimer -= deltaTime;
            if (m_PoisonTickTimer <= 0.0f) {
                ReceiveDamage(50); 
                m_PoisonTickTimer += 1.0f; 
            }
        }

        if (m_IceEffectTimer > 0) {
            m_IceEffectTimer -= deltaTime;
            if (m_IceEffectTimer <= 0) {
                m_IceStack = 0; 
                UpdateSpeed();  
            }
        }

        glm::vec2 targetPos = m_Waypoints[m_CurrentWaypointIndex];
        glm::vec2 direction = targetPos - m_Pos;
        float distance = glm::length(direction);

        if (distance <= m_CurrentSpeed) {
            m_Pos = targetPos;
            m_CurrentWaypointIndex++; 
        } else {
            m_Pos += glm::normalize(direction) * m_CurrentSpeed;
        }
    }

    void Draw(const glm::mat4& projection) {
        if (!m_Image || IsDead()) return; 

        // 1. 畫怪物本體 (Z=0.6)
        Core::Matrices data;
        data.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0.6f)), glm::vec3(100.0f, 100.0f, 1.0f)); 
        data.m_Projection = projection;
        m_Image->Draw(data);

        // 2. 畫冰凍特效 (Z=0.75)
        if (m_IceEffectTimer > 0 && m_IceSnowImage) {
            Core::Matrices snowData;
            glm::vec2 snowPos = m_Pos + glm::vec2(0.0f, 15.5f); 
            snowData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(snowPos, 0.75f)), glm::vec3(37.0f, 50.0f, 1.0f));
            snowData.m_Projection = projection;
            m_IceSnowImage->Draw(snowData);
        }

        if (m_HpText) {
            m_HpText->SetText(std::to_string(m_Hp)); 
            glm::vec2 textSize = m_HpText->GetSize();
            // 血量位置稍微往右偏移一點點比較美觀
            glm::vec2 hpPos = m_Pos + glm::vec2(7.5f, 0.0f);

            // 3. 🌟 畫中毒狀態 UI (先畫圖示當底，座標與血量完全重疊，Z=0.65 介於怪物與文字之間)
            if (m_IsPoisoned && m_PoisonUI) {
                Core::Matrices poisonData;
                // 大小設為 40x40 以便包覆文字
                poisonData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(hpPos, 0.65f)), glm::vec3(40.0f, 40.0f, 1.0f));
                poisonData.m_Projection = projection;
                m_PoisonUI->Draw(poisonData);
            }

            // 4. 畫血量文字 (最後畫，確保在最上面，Z=0.7)
            Core::Matrices textData;
            textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(hpPos, 0.7f)), glm::vec3(textSize, 1.0f));
            textData.m_Projection = projection;
            m_HpText->Draw(textData);
        }
    }

    void UpdateSpeed() {
        float speedMultiplier = 1.0f;
        if (m_IceEffectTimer > 0) {
            speedMultiplier -= (m_IceStack * 0.05f);
        }
        speedMultiplier = std::max(0.5f, speedMultiplier);
        m_CurrentSpeed = m_OriginalSpeed * speedMultiplier;
    }

    void ApplyIceEffect() {
        if (m_IceStack < 3) m_IceStack++;
        m_IceEffectTimer = 2.0f; 
        UpdateSpeed(); 
    }

    void ApplyPoisonEffect() {
        if (!m_IsPoisoned) {
            m_IsPoisoned = true;
            m_PoisonTickTimer = 1.0f; 
        }
    }

    void ReceiveDamage(int damage) { m_Hp -= damage; }
    bool IsDead() const { return m_Hp <= 0; }
    bool IsFinished() const { return !m_Waypoints.empty() && m_CurrentWaypointIndex >= m_Waypoints.size(); }

    glm::vec2 GetPos() const { return m_Pos; }
    int GetHp() const { return m_Hp; }
    size_t GetWaypointIndex() const { return m_CurrentWaypointIndex; }
    float GetDistanceToNextWaypoint() const {
        if (m_Waypoints.empty() || m_CurrentWaypointIndex >= m_Waypoints.size()) return 0.0f;
        return glm::length(m_Waypoints[m_CurrentWaypointIndex] - m_Pos);
    }
    
    bool IsPoisoned() const { return m_IsPoisoned; }

private:
    std::shared_ptr<Util::Image> m_Image;
    std::shared_ptr<Util::Image> m_IceSnowImage; 
    std::shared_ptr<Util::Image> m_PoisonUI; 
    std::shared_ptr<Util::Text> m_HpText;
    std::vector<glm::vec2> m_Waypoints;
    glm::vec2 m_Pos;
    float m_OriginalSpeed; 
    float m_CurrentSpeed;  
    int m_Hp;
    size_t m_CurrentWaypointIndex;
    
    float m_IceEffectTimer = 0.0f; 
    int m_IceStack = 0; 

    bool m_IsPoisoned = false;
    float m_PoisonTickTimer = 0.0f;
};

#endif