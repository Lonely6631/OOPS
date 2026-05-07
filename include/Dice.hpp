#ifndef DICE_HPP
#define DICE_HPP

#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/Time.hpp"
#include "Monster.hpp"
#include "Bullet.hpp"
#include <memory>
#include <string>
#include <vector>
#include <algorithm> 

enum class DiceType { Ice, Poison, Wind, Electric, Iron };

class Dice {
public:
    Dice(DiceType type, const std::string& imagePath, glm::vec2 pos) 
        : m_Type(type), m_Pos(pos), m_Level(1) {
        m_Image = std::make_shared<Util::Image>(imagePath);
    }
    virtual ~Dice() = default; 

    virtual void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) {
        m_CooldownTimer -= static_cast<float>(Util::Time::GetDeltaTimeMs()) / 1000.0f;
    }

    void Draw(const glm::mat4& projection, float zIndex = 0.5f) {
        if (!m_Image) return;
        Core::Matrices data;
        data.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, zIndex)), glm::vec3(56.0f, 56.0f, 1.0f));
        data.m_Projection = projection;
        m_Image->Draw(data);
    }

    glm::vec2 GetPos() const { return m_Pos; }
    void SetPos(glm::vec2 pos) { m_Pos = pos; } 

    DiceType GetType() const { return m_Type; }
    int GetLevel() const { return m_Level; }

    virtual void Upgrade() {
        m_Level++;
    }

protected:
    std::shared_ptr<Util::Image> m_Image;
    glm::vec2 m_Pos;
    float m_CooldownTimer = 0.0f; 
    DiceType m_Type; 
    int m_Level;     

    std::shared_ptr<Monster> FindFrontMostMonster(const std::vector<std::shared_ptr<Monster>>& monsters) {
        std::shared_ptr<Monster> target = nullptr;
        int maxWaypoint = -1;
        float minDistance = 999999.0f;

        for (auto& monster : monsters) {
            if (!monster->IsDead()) {
                int wpIdx = static_cast<int>(monster->GetWaypointIndex());
                float dist = monster->GetDistanceToNextWaypoint();
                
                if (wpIdx > maxWaypoint || (wpIdx == maxWaypoint && dist < minDistance)) {
                    maxWaypoint = wpIdx;
                    minDistance = dist;
                    target = monster;
                }
            }
        }
        return target;
    }

    // 🌟 新增：尋找「未中毒」的最前方怪物 (毒骰專用)
    std::shared_ptr<Monster> FindUnpoisonedFrontMostMonster(const std::vector<std::shared_ptr<Monster>>& monsters) {
        std::shared_ptr<Monster> target = nullptr;
        std::shared_ptr<Monster> fallback = nullptr;
        int maxWp = -1; float minDist = 999999.0f;
        int fbMaxWp = -1; float fbMinDist = 999999.0f;

        for (auto& monster : monsters) {
            if (!monster->IsDead()) {
                int wpIdx = static_cast<int>(monster->GetWaypointIndex());
                float dist = monster->GetDistanceToNextWaypoint();

                // 備用方案：記錄最前面的怪 (防止全場都中毒時骰子發呆)
                if (wpIdx > fbMaxWp || (wpIdx == fbMaxWp && dist < fbMinDist)) {
                    fbMaxWp = wpIdx; fbMinDist = dist;
                    fallback = monster;
                }

                // 優先目標：尋找「沒有中毒」且最前面的怪
                if (!monster->IsPoisoned()) {
                    if (wpIdx > maxWp || (wpIdx == maxWp && dist < minDist)) {
                        maxWp = wpIdx; minDist = dist;
                        target = monster;
                    }
                }
            }
        }
        return target ? target : fallback;
    }
};

class IronDice : public Dice {
public:
    IronDice(glm::vec2 pos) : Dice(DiceType::Iron, RESOURCE_DIR "/Iron/Iron_Dice.png", pos) {}
    
    void Upgrade() override {
        Dice::Upgrade(); 
        if (m_Level == 2) m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Iron/Iron_Dice_2.png");
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = nullptr;
            int maxHp = -1;
            for (auto& monster : monsters) {
                if (!monster->IsDead() && monster->GetHp() > maxHp) {
                    maxHp = monster->GetHp();
                    target = monster;
                }
            }
            if (target) {
                bullets.push_back(std::make_shared<Bullet>(RESOURCE_DIR "/Iron/Iron_Bullet.png", m_Pos, target, 20.0f, 100, 40.0f));
                m_CooldownTimer = std::max(0.01f, 1.0f / m_Level); 
            }
        }
    }
};

class ElectricDice : public Dice {
public:
    ElectricDice(glm::vec2 pos) : Dice(DiceType::Electric, RESOURCE_DIR "/Electric/Electric_Dice.png", pos) {}

    void Upgrade() override {
        Dice::Upgrade(); 
        if (m_Level == 2) m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Electric/Electric_Dice_2.png");
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindFrontMostMonster(monsters);
            if (target) {
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Electric/Electric_Bullet.png", 
                    m_Pos, target, 20.0f, 40, 40.0f, 2 
                ));
                m_CooldownTimer = std::max(0.01f, 0.7f / m_Level); 
            }
        }
    }
};

class IceDice : public Dice {
public:
    IceDice(glm::vec2 pos) : Dice(DiceType::Ice, RESOURCE_DIR "/Ice/Ice_Dice.png", pos) {}

    void Upgrade() override {
        Dice::Upgrade(); 
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindFrontMostMonster(monsters);
            if (target) {
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Ice/Ice_Bullet.png", m_Pos, target, 20.0f, 30, 40.0f, 0, true 
                ));
                m_CooldownTimer = std::max(0.01f, 1.5f / m_Level); 
            }
        }
    }
};

// 🌟 實作：毒骰 (PoisonDice)
class PoisonDice : public Dice {
public:
    PoisonDice(glm::vec2 pos) : Dice(DiceType::Poison, RESOURCE_DIR "/Poison/Poison_Dice.png", pos) {}

    void Upgrade() override {
        Dice::Upgrade(); 
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            // 使用專屬的索敵邏輯：優先找沒中毒的怪
            std::shared_ptr<Monster> target = FindUnpoisonedFrontMostMonster(monsters);

            if (target) {
                // 發射毒子彈 (倒數第二個 false 是 isIce，最後一個 true 是 isPoison)
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Poison/Poison_Bullet.png", m_Pos, target, 20.0f, 20, 40.0f, 0, false, true 
                ));
                m_CooldownTimer = std::max(0.01f, 1.3f / m_Level); 
            }
        }
    }
};

// 🌟 實作：風骰 (WindDice)
class WindDice : public Dice {
public:
    WindDice(glm::vec2 pos) : Dice(DiceType::Wind, RESOURCE_DIR "/Wind/Wind_Dice.png", pos) {}

    void Upgrade() override {
        Dice::Upgrade(); 
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindFrontMostMonster(monsters);

            if (target) {
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Wind/Wind_Bullet.png", m_Pos, target, 20.0f, 40, 40.0f, 0 
                ));
                // 攻速 0.45s，並自帶 10% 攻速增加特性 (乘上 0.9 縮短冷卻)
                m_CooldownTimer = std::max(0.01f, (0.45f * 0.9f) / m_Level); 
            }
        }
    }
};

#endif