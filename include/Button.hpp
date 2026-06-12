#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Transform.hpp"
#include <memory>
#include <string>

class Button {
public:
    Button(const std::string& imagePath, glm::vec2 pos, glm::vec2 size) 
        : m_Pos(pos), m_Size(size) {
        m_Image = std::make_shared<Util::Image>(imagePath);
    }

    // 我們不在這裡畫，改為提供 Image 指標給 App.cpp 畫，這樣最保險
    std::shared_ptr<Util::Image> GetImage() { return m_Image; }
    glm::vec2 GetPos() { return m_Pos; }
    glm::vec2 GetSize() { return m_Size; }

    bool IsClicked() {
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            return (mousePos.x >= m_Pos.x - m_Size.x / 2.5f &&
                    mousePos.x <= m_Pos.x + m_Size.x / 2.5f &&
                    mousePos.y >= m_Pos.y - m_Size.y / 2.5f &&
                    mousePos.y <= m_Pos.y + m_Size.y / 2.5f);
        }
        return false;
    }

    void Draw(const glm::mat4& projection) {
    if (!m_Image) return;

    Core::Matrices data;
    // 使用按鈕自己的位置和尺寸
    data.m_Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0.8f)), glm::vec3(m_Size, 1.0f));
    data.m_Projection = projection;

    m_Image->Draw(data);

    

}

private:
    std::shared_ptr<Util::Image> m_Image;
    glm::vec2 m_Pos;
    glm::vec2 m_Size;
};

#endif