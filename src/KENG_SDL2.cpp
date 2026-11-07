#include "KENG_SDL2.hpp"

void KENG::SDL2::Zoom(float direction) {
    float z = OGL_RenderView->pos.z;

    // Distance-based zoom speed
    float zoomSpeed = z * 0.1f;

    z += direction * zoomSpeed;

    // Clamp zoom range
    if (z < MIN_ZOOM) z = MIN_ZOOM;
    if (z > MAX_ZOOM) z = MAX_ZOOM;

    OGL_RenderView->pos.z = z;
}

void KENG::SDL2::HandleEvents(bool& quit, OGL_Controller* ctrl, HoverController& hoverCtrl, ProvinceRegistry& preg, \
                              RealmRegistry& rreg, OGL_Object* provMap, KENG::GPU::MapMaskMaterial& mmm) {
    /* Update hover */
    
    hoverCtrl.GetHoveredProvince(preg, *provMap);
    hoverCtrl.GetHoveredRealm(rreg, hoverCtrl.HoverProv());
    
    SDL_Event e;

    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT ){
            quit = true;
        }

        if ((e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_0 ))) {
            mmm.mm = GPU::OWNERSHIP;
        } else if ((e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_1 ))) {
            mmm.mm = GPU::DEV;
        }

        /* ESCAPE */
        if ((e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE ))) {
            hoverCtrl.ResetClickedProvince(); // RESET
            hoverCtrl.ResetClickedRealm();

            mmm.selectedRealmID = -1; // Reset
        }

        /* - */
        if ((e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_MINUS ))) {
            Zoom(+1.0f);
        }

        /* + */
        if ((e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_EQUALS ))) {
            Zoom(-1.0f);
        }

        /* Basic mouse zoom in/out */
        if (e.type == SDL_MOUSEWHEEL) {
            if (ctrl->mouseZoom) {
                if (e.wheel.y > 0.0f) {
                    Zoom(-1.0f);
                } else if (e.wheel.y < 0.0f) {
                    Zoom(+1.0f);
                }
            }
        }

        /* Mouse movement */
        if(e.type == SDL_MOUSEMOTION){
            float xRel = e.motion.xrel;
            float yRel = e.motion.yrel;
            
            OGL_HandleControllerMouse(ctrl, xRel, yRel);
        }

        /* Mouse buttons */
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if (e.button.button == SDL_BUTTON_LEFT){
                hoverCtrl.SetHoveredProvinceAsClicked();
                // std::cout << hoverCtrl.ClickedProvince().Name() << "\n";
            } 

            else if (e.button.button == SDL_BUTTON_RIGHT) {
                hoverCtrl.SetHoveredRealmAsClicked();
                mmm.selectedRealmID = hoverCtrl.ClickedRealm().Id();
                // std::cout << hoverCtrl.ClickedRealm().Name() << "\n";
            }
        }
    }

    return;
}