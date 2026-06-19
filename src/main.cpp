#include "App.hpp"
#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;

            case App::State::MENU: // 新增對選單狀態的處理
                app.Menu();
                break;

            case App::State::BACKPACK: // 新增 BACKPACK 狀態的處理
                app.Backpack();
                break;
            
            case App::State::GAMEOVER: //  新增處理結算畫面狀態
                app.GameOver();
                break;
            
            case App::State::CHEST:
                app.Chest();
                break;
            
            case App::State::UPDATE:
                app.Update();
                break;
            
            case App::State::OPEN:
                app.Open();
                break; 
               
            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
    }
    return 0;
}