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
#include <algorithm> // 🌟 引入 algorithm 以使用 std::max

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
};

class IronDice : public Dice {
public:
    IronDice(glm::vec2 pos) : Dice(DiceType::Iron, RESOURCE_DIR "/Iron/Iron_Dice.png", pos) {}
    
    void Upgrade() override {
        Dice::Upgrade(); 
        if (m_Level == 2) {
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Iron/Iron_Dice_2.png");
        }
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
                bullets.push_back(std::make_shared<Bullet>(RESOURCE_DIR "/Iron/Iron_Bullet.png", m_Pos, target, 15.0f, 100, 40.0f));
                // 🌟 鐵骰攻速：預設 1.0s，設定極限 0.01s
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
        if (m_Level == 2) {
            m_Image = std::make_shared<Util::Image>(RESOURCE_DIR "/Electric/Electric_Dice_2.png");
        }
    }

    void Update(const std::vector<std::shared_ptr<Monster>>& monsters, std::vector<std::shared_ptr<Bullet>>& bullets) override {
        Dice::Update(monsters, bullets); 
        
        if (m_CooldownTimer <= 0.0f && !monsters.empty()) {
            std::shared_ptr<Monster> target = nullptr;
            
            for (auto& monster : monsters) {
                if (!monster->IsDead()) {
                    target = monster;
                    break;
                }
            }

            if (target) {
                // 🌟 電骰傷害改為 30
                bullets.push_back(std::make_shared<Bullet>(
                    RESOURCE_DIR "/Electric/Electric_Bullet.png", 
                    m_Pos, target, 15.0f, 30, 40.0f, 2 
                ));
                // 🌟 電骰攻速：預設 0.7s，設定極限 0.01s
                m_CooldownTimer = std::max(0.01f, 0.7f / m_Level); 
            }
        }
    }
};

class IceDice : public Dice {
public:
    IceDice(glm::vec2 pos) : Dice(DiceType::Ice, RESOURCE_DIR "/Ice/Ice_Dice.png", pos) {}
};

class PoisonDice : public Dice {
public:
    PoisonDice(glm::vec2 pos) : Dice(DiceType::Poison, RESOURCE_DIR "/Poison/Poison_Dice.png", pos) {}
};

class WindDice : public Dice {
public:
    WindDice(glm::vec2 pos) : Dice(DiceType::Wind, RESOURCE_DIR "/Wind/Wind_Dice.png", pos) {}
};

#endif