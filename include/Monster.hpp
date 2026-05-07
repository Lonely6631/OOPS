#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include <memory>
#include <string>
#include <vector>

class Monster {
public:
    Monster(const std::string& imagePath, std::vector<glm::vec2> waypoints, float speed, int hp) 
        : m_Waypoints(waypoints), m_Speed(speed), m_Hp(hp) {
        m_Image = std::make_shared<Util::Image>(imagePath);
        m_HpText = std::make_shared<Util::Text>(RESOURCE_DIR "/fonts/jf-openhuninn-2.1.ttf", 15, std::to_string(m_Hp), Util::Color::FromName(Util::Colors::WHITE));
        
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

        glm::vec2 targetPos = m_Waypoints[m_CurrentWaypointIndex];
        glm::vec2 direction = targetPos - m_Pos;
        float distance = glm::length(direction);

        if (distance <= m_Speed) {
            m_Pos = targetPos;
            m_CurrentWaypointIndex++; 
        } else {
            m_Pos += glm::normalize(direction) * m_Speed;
        }
    }

    void Draw(const glm::mat4& projection) {
        if (!m_Image || IsDead()) return; // 死了就不畫

        Core::Matrices data;
        data.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0.6f)), glm::vec3(100.0f, 100.0f, 1.0f)); 
        data.m_Projection = projection;
        m_Image->Draw(data);

        if (m_HpText) {
            m_HpText->SetText(std::to_string(m_Hp)); 
            glm::vec2 textSize = m_HpText->GetSize();
            glm::vec2 offset(7.5f, 0.0f); 
            glm::vec2 finalPos = m_Pos + offset;

            Core::Matrices textData;
            textData.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(finalPos, 0.7f)), glm::vec3(textSize, 1.0f));
            textData.m_Projection = projection;
            m_HpText->Draw(textData);
        }
    }

    // 🌟 新增受傷與死亡判定
    void ReceiveDamage(int damage) { m_Hp -= damage; }
    bool IsDead() const { return m_Hp <= 0; }
    bool IsFinished() const { return !m_Waypoints.empty() && m_CurrentWaypointIndex >= m_Waypoints.size(); }

    glm::vec2 GetPos() const { return m_Pos; }
    int GetHp() const { return m_Hp; }

private:
    std::shared_ptr<Util::Image> m_Image;
    std::shared_ptr<Util::Text> m_HpText;
    std::vector<glm::vec2> m_Waypoints;
    glm::vec2 m_Pos;
    float m_Speed;
    int m_Hp;
    size_t m_CurrentWaypointIndex;
};

#endif