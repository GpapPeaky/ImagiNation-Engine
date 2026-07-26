#include "KENG_SDL2.hpp"

void KENG::SDL2::HandleEvents(bool& quit, OGL_Controller* ctrl, HoverController& provCtrl, ProvinceRegistry& preg, RealmRegistry& rreg, OGL_Object* provMap) {
    /* Update hover */
    
    provCtrl.GetHoveredProvince(preg, *provMap);
    provCtrl.GetHoveredRealm(rreg, provCtrl.HoverProv());
    
    SDL_Event e;

    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT ){
            quit = true;
        }

        if ((e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE ))) {
            provCtrl.ResetClickedProvince(); // RESET
        }
            
        /* Mouse movement */
        if(e.type == SDL_MOUSEMOTION){
            float xRel = e.motion.xrel;
            float yRel = e.motion.yrel;
            
            OGL_HandleControllerMouse(ctrl, xRel, yRel);
        }

        /* Basic mouse zoom in/out */
        if(e.type == SDL_MOUSEWHEEL){
            if(ctrl->mouseZoom){
                if(e.wheel.y > 0.0f){
                    OGL_RenderView->pos.z -= 0.5f;
                }else if(e.wheel.y < 0.0f){
                    // zoom out
                    OGL_RenderView->pos.z += 0.5f;
                }
            }
        }

        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT){
                provCtrl.SetHoveredProvinceAsClicked();

                std::cout << provCtrl.ClickedProvince().Name() << std::endl;
            }
            
            if(e.button.button == SDL_BUTTON_RIGHT){

            }
        }
    }

    return;
}