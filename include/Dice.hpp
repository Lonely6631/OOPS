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
    static constexpr float MIN_COOLDOWN = 0.01f;

    Dice(DiceType type, const std::string& imagePath, glm::vec2 pos) 
        : m_Type(type), m_Pos(pos), m_StarLevel(1), m_PowerUpLevel(1) {
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
    int GetStarLevel() const { return m_StarLevel; }
    int GetPowerUpLevel() const { return m_PowerUpLevel; }

    virtual void UpgradeStar() { m_StarLevel++; }
    virtual void PowerUp() { m_PowerUpLevel++; }

    void SetSilenced(bool s) { m_IsSilenced = s; }
    bool IsSilenced() const { return m_IsSilenced; }

protected:
    std::shared_ptr<Util::Image> m_Image;
    glm::vec2 m_Pos;
    float m_CooldownTimer = 0.0f; 
    DiceType m_Type; 
    int m_StarLevel;     
    int m_PowerUpLevel;
    bool m_IsSilenced = false;  

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

    std::shared_ptr<Monster> FindUnpoisonedFrontMostMonster(const std::vector<std::shared_ptr<Monster>>& monsters) {
        std::shared_ptr<Monster> target = nullptr;
        std::shared_ptr<Monster> fallback = nullptr;
        int maxWp = -1; float minDist = 999999.0f;
        int fbMaxWp = -1; float fbMinDist = 999999.0f;

        for (auto& monster : monsters) {
            if (!monster->IsDead()) {
                int wpIdx = static_cast<int>(monster->GetWaypointIndex());
                float dist = monster->GetDistanceToNextWaypoint();
                if (wpIdx > fbMaxWp || (wpIdx == fbMaxWp && dist < fbMinDist)) {
                    fbMaxWp = wpIdx; fbMinDist = dist;
                    fallback = monster;
                }
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
    
    void UpgradeStar() override {
        Dice::UpgradeStar(); 
        if (m_StarLevel <= 7) {
            std::string suffix = (m_StarLevel == 1) ? "" : "_" + std::to_string(m_StarLevel);
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Iron/Iron_Dice" + suffix + ".png");
        }
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets);
        if (m_IsSilenced) return; 
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
                int damage = 100 + (m_PowerUpLevel - 1) * 100;
                bullets.push_back(std::make_shared<Bullet>(RESOURCE_DIR "/Iron/Iron_Bullet.png", m_Pos, target, 10.0f, damage, 40.0f));
                m_CooldownTimer = std::max(MIN_COOLDOWN, 1.0f / m_StarLevel); 
            }
        }
    }
};

class ElectricDice : public Dice {
public:
    ElectricDice(glm::vec2 pos) : Dice(DiceType::Electric, RESOURCE_DIR "/Electric/Electric_Dice.png", pos) {}

    void UpgradeStar() override {
        Dice::UpgradeStar(); 
        if (m_StarLevel <= 7) {
            std::string suffix = (m_StarLevel == 1) ? "" : "_" + std::to_string(m_StarLevel);
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Electric/Electric_Dice" + suffix + ".png");
        }
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        if (m_IsSilenced) return;
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindFrontMostMonster(monsters);
            if (target) {
                // 🌟 電骰：第一發 = 100% 基礎(30) + 100% 彈射(30)
                int baseDmg = 30 + (m_PowerUpLevel - 1) * 10;
                int bounceBase = 30 + (m_PowerUpLevel - 1) * 20;
                int initialTotalDmg = baseDmg + bounceBase;

                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Electric/Electric_Bullet.png", 
                    m_Pos, target, 10.0f, initialTotalDmg, 40.0f, 2 
                ));
                m_CooldownTimer = std::max(MIN_COOLDOWN, 0.7f / m_StarLevel); 
            }
        }
    }
};

class IceDice : public Dice {
public:
    IceDice(glm::vec2 pos) : Dice(DiceType::Ice, RESOURCE_DIR "/Ice/Ice_Dice.png", pos) {}

    void UpgradeStar() override {
        Dice::UpgradeStar(); 
        if (m_StarLevel <= 7) {
            std::string suffix = (m_StarLevel == 1) ? "" : "_" + std::to_string(m_StarLevel);
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Ice/Ice_Dice" + suffix + ".png");
        }
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        if (m_IsSilenced) return;
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindFrontMostMonster(monsters);
            if (target) {
                int damage = 30 + (m_PowerUpLevel - 1) * 30;
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Ice/Ice_Bullet.png", m_Pos, target, 10.0f, damage, 40.0f, 0, true 
                ));
                m_CooldownTimer = std::max(MIN_COOLDOWN, 1.5f / m_StarLevel); 
            }
        }
    }
};

class PoisonDice : public Dice {
public:
    PoisonDice(glm::vec2 pos) : Dice(DiceType::Poison, RESOURCE_DIR "/Poison/Poison_Dice.png", pos) {}

    void UpgradeStar() override {
        Dice::UpgradeStar(); 
        if (m_StarLevel <= 7) {
            std::string suffix = (m_StarLevel == 1) ? "" : "_" + std::to_string(m_StarLevel);
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Poison/Poison_Dice" + suffix + ".png");
        }
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        if (m_IsSilenced) return;
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindUnpoisonedFrontMostMonster(monsters);
            if (target) {
                int damage = 20 + (m_PowerUpLevel - 1) * 10;
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Poison/Poison_Bullet.png", m_Pos, target, 10.0f, damage, 40.0f, 0, false, true 
                ));
                m_CooldownTimer = std::max(MIN_COOLDOWN, 1.3f / m_StarLevel); 
            }
        }
    }
};

class WindDice : public Dice {
public:
    WindDice(glm::vec2 pos) : Dice(DiceType::Wind, RESOURCE_DIR "/Wind/Wind_Dice.png", pos) {}

    void UpgradeStar() override {
        Dice::UpgradeStar(); 
        if (m_StarLevel <= 7) {
            std::string suffix = (m_StarLevel == 1) ? "" : "_" + std::to_string(m_StarLevel);
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Wind/Wind_Dice" + suffix + ".png");
        }
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        if (m_IsSilenced) return;
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = FindFrontMostMonster(monsters);
            if (target) {
                int damage = 20 + (m_PowerUpLevel - 1) * 15;
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Wind/Wind_Bullet.png", m_Pos, target, 10.0f, damage, 40.0f, 0 
                ));
                
                float speedMultiplier = 1.0f + (m_PowerUpLevel - 1) * 0.10f;
                float baseCooldown = 0.405f / speedMultiplier; 
                m_CooldownTimer = std::max(MIN_COOLDOWN, baseCooldown / m_StarLevel); 
            }
        }
    }
};

#endif