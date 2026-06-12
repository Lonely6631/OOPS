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
    Monster(const std::string& imagePath, std::vector<glm::vec2> waypoints, float speed, int hp, int spReward = 0) 
        : m_Waypoints(waypoints), m_OriginalSpeed(speed), m_CurrentSpeed(speed), m_Hp(hp), m_SpReward(spReward) {
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

    void SetSize(glm::vec2 size) { m_Size = size; }

    void Update() {
        if (m_Waypoints.empty() || m_CurrentWaypointIndex >= m_Waypoints.size() || IsDead()) return;

        float deltaTime = static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
        
        // 🌟 使用動態的中毒傷害
        if (m_IsPoisoned) {
            m_PoisonTickTimer -= deltaTime;
            if (m_PoisonTickTimer <= 0.0f) {
                ReceiveDamage(m_PoisonDamage); 
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

        Core::Matrices data;
        data.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0.6f)), glm::vec3(m_Size, 1.0f)); 
        data.m_Projection = projection;
        m_Image->Draw(data);

        if (m_IceEffectTimer > 0 && m_IceSnowImage) {
            Core::Matrices snowData;
            glm::vec2 snowPos = m_Pos + glm::vec2(0.0f, 15.5f); 
            snowData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(snowPos, 0.75f)), glm::vec3(37.0f, 60.0f, 1.0f));
            snowData.m_Projection = projection;
            m_IceSnowImage->Draw(snowData);
        }

        if (m_HpText) {
            m_HpText->SetText(std::to_string(m_Hp)); 
            glm::vec2 textSize = m_HpText->GetSize();
            glm::vec2 hpPos = m_Pos + glm::vec2(7.5f, 0.0f);

            if (m_IsPoisoned && m_PoisonUI) {
                Core::Matrices poisonData;
                poisonData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(hpPos - glm::vec2(7.5f, 0.0f), 0.65f)), glm::vec3(70.0f, 70.0f, 1.0f));
                poisonData.m_Projection = projection;
                m_PoisonUI->Draw(poisonData);
            }

            Core::Matrices textData;
            textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(hpPos, 0.7f)), glm::vec3(textSize, 1.0f));
            textData.m_Projection = projection;
            m_HpText->Draw(textData);
        }
    }

    void UpdateSpeed() {
        float speedMultiplier = 1.0f;
        if (m_IceEffectTimer > 0) {
            // 🌟 根據傳入的減速比例計算
            speedMultiplier -= (m_IceStack * m_CurrentSlowRate);
        }
        speedMultiplier = std::max(0.2f, speedMultiplier); // 避免完全定格，最低保底 20% 速度
        m_CurrentSpeed = m_OriginalSpeed * speedMultiplier;
    }

    // 🌟 接收外部傳入的減速比例
    void ApplyIceEffect(float slowRate) {
        if (m_IceStack < 3) m_IceStack++;
        m_IceEffectTimer = 2.0f; 
        m_CurrentSlowRate = slowRate; 
        UpdateSpeed(); 
    }

    // 🌟 接收外部傳入的中毒傷害
    void ApplyPoisonEffect(int poisonDamage) {
        if (!m_IsPoisoned) {
            m_IsPoisoned = true;
            m_PoisonTickTimer = 1.0f; 
        }
        m_PoisonDamage = poisonDamage; 
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

    int GetSpReward() const { return m_SpReward; }

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
    int m_SpReward;
    glm::vec2 m_Size = glm::vec2(100.0f, 100.0f);
    
    float m_IceEffectTimer = 0.0f; 
    int m_IceStack = 0; 
    float m_CurrentSlowRate = 0.05f; // 預設每層 5%

    bool m_IsPoisoned = false;
    float m_PoisonTickTimer = 0.0f;
    int m_PoisonDamage = 50; // 預設中毒傷害
};

#endif