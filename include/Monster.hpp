#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include <memory>
#include <string>
#include <vector>

class Monster {
public:
    Monster(const std::string& imagePath, std::vector<glm::vec2> waypoints, float speed, int hp = 10) 
        : m_Waypoints(waypoints), m_Speed(speed), m_Hp(hp) {
        m_Image = std::make_shared<Util::Image>(imagePath);
        
        if (!m_Waypoints.empty()) {
            m_Pos = m_Waypoints[0];
            m_CurrentWaypointIndex = 1; 
        } else {
            m_Pos = glm::vec2(0.0f);
            m_CurrentWaypointIndex = 0;
        }
    }

    void Update() {
        if (m_Waypoints.empty() || m_CurrentWaypointIndex >= m_Waypoints.size()) return;

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
        if (!m_Image) return;

        Core::Matrices data;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0.6f)); 
        model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f)); 
        
        data.m_Model = model;
        data.m_Projection = projection;
        m_Image->Draw(data);
    }

    glm::vec2 GetPos() const { return m_Pos; }
    int GetHp() const { return m_Hp; }

    // 判斷是否已經走完所有路徑點
    bool IsFinished() const {
        // 如果路徑點有東西，而且目前的索引已經超出陣列大小，代表走完了
        return !m_Waypoints.empty() && m_CurrentWaypointIndex >= m_Waypoints.size();
    }

private:
    std::shared_ptr<Util::Image> m_Image;
    glm::vec2 m_Pos;
    float m_Speed; 
    int m_Hp;      
    
    std::vector<glm::vec2> m_Waypoints;
    size_t m_CurrentWaypointIndex;     
};

#endif