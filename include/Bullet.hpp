#ifndef BULLET_HPP
#define BULLET_HPP

#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Monster.hpp"
#include <memory>
#include <string>

class Bullet {
public:
    Bullet(const std::string& imagePath, glm::vec2 pos, std::shared_ptr<Monster> target, float speed, int damage, float size = 20.0f, int bounces = 0)
        : m_Pos(pos), m_Target(target), m_Speed(speed), m_BaseDamage(damage), m_Damage(damage), m_Size(size), m_MaxBounces(bounces), m_BouncesLeft(bounces) {
        m_Image = std::make_shared<Util::Image>(imagePath);
    }

    void Update() {
        if (IsInvalid()) return; 
        
        glm::vec2 targetPos = m_Target->GetPos();
        glm::vec2 direction = targetPos - m_Pos;
        float distance = glm::length(direction);

        if (distance > 0) {
            m_Pos += glm::normalize(direction) * m_Speed;
        }
    }

    void Draw(const glm::mat4& projection) {
        if (!m_Image) return;
        Core::Matrices data;
        data.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0.65f)), glm::vec3(m_Size, m_Size, 1.0f));
        data.m_Projection = projection;
        m_Image->Draw(data);
    }

    bool IsHit() const {
        if (IsInvalid()) return false;
        return glm::length(m_Target->GetPos() - m_Pos) <= (m_Speed + m_Size / 4.0f);
    }

    bool IsInvalid() const {
        return !m_Target || m_Target->IsDead();
    }

    std::shared_ptr<Monster> GetTarget() const { return m_Target; }
    int GetDamage() const { return m_Damage; }

    int GetBouncesLeft() const { return m_BouncesLeft; }
    
    // 🌟 修正：彈跳時根據次數衰減傷害！
    void DecreaseBounce() { 
        m_BouncesLeft--; 
        
        // 針對電骰子設定的 2 次彈跳 (共打 3 隻) 進行 100% -> 70% -> 30% 衰減
        if (m_MaxBounces == 2) {
            if (m_BouncesLeft == 1) {
                m_Damage = static_cast<int>(m_BaseDamage * 0.7f); // 第 2 下：70%
            } else if (m_BouncesLeft == 0) {
                m_Damage = static_cast<int>(m_BaseDamage * 0.3f); // 第 3 下：30%
            }
        }
    }
    
    void SetTarget(std::shared_ptr<Monster> newTarget) { m_Target = newTarget; }

private:
    std::shared_ptr<Util::Image> m_Image;
    glm::vec2 m_Pos;
    std::shared_ptr<Monster> m_Target;
    float m_Speed;
    
    // 🌟 新增原始傷害變數來幫助計算倍率
    int m_BaseDamage; 
    int m_Damage;     
    float m_Size; 
    int m_MaxBounces; 
    int m_BouncesLeft; 
};

#endif