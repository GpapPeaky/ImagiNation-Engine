#include "auxf/includes.aux"

int main(int, char**){
    KENG::ProvinceRegistry pr;
    pr.ReadProvinceFile();
    pr.Print();

    KENG::RealmRegistry rr;
    rr.ReadRealmFile();
    rr.ReadOwnerFile();
    rr.Print();

    /* Initialise SDL2 and OpenGL */
    SDL2_InitWin();
    OGL_InitContext(SDL2_Win);

    OGL_InitShaderRegistry();

    /* Controller creation */
    OGL_Controller* ctrl = OGL_CreateController(5.0f, 0.1f);

    /* Camera setup */
    OGL_Camera* cam = OGL_CreateCamera({0.0f, 0.0f, 50.0f}, {0, 1, 0}, 0.0f, 0.0f);
    
    /* Bind camera to controller */
    OGL_BindCameraToController(ctrl, cam);
    
    /* Bind a camera to the render view */
    OGL_BindCameraToRenderView(cam);
    
    /* Bind controller keys */
    OGL_BindControllerWASD2D(ctrl);
    
    /* Lightweight root object */
    OGL_Object* rootObj = OGL_CreateObject(OGL_GetShader("rootobj"));
    OGL_Scene = OGL_CreateNode(rootObj, "root");
    
    /* Map creation */
    OGL_Object* provinceMap = OGL_CreateObject(OGL_GetShader("tex"));
    OGL_CreateTextureQuad(*provinceMap->mesh);
    OGL_LoadBitmapToObject(*provinceMap->mesh, "History/provinces/min_province_map.png");
    /* Create the object node */
    OGL_ONode* onodeProvinceMap = OGL_CreateNode(provinceMap, "map");
    TRS::S(*provinceMap, {30.0f * 1.777778f, 30.f, 1.f});
    TRS::R(*provinceMap, {0.f, 0.f, 180.f});
    /* Hierarchy */
    OGL_AttachChild(OGL_Scene, onodeProvinceMap);

    /* Main loop, and timing */
    Uint32 lastTime = SDL_GetTicks();

    bool OGL_GameQuit = false;
    while(!OGL_GameQuit){
        Uint32 now = SDL_GetTicks();
        OGL_GameDt = (now - lastTime) / 1000.0f;            /* Convert to seconds */
        lastTime = now;

        /* Updates to assets / sprites / objects in general */
        SDL2_HandleEvents(OGL_GameQuit, ctrl);      /* Creates a new event to poll per call (Might need to be optimised) */
        
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        OGL_HandleControllerKeyboard(ctrl, keys, OGL_GameDt);
        
        /* Rendering order matters */
        /* Need to pass each uniform before drawing */
        
        /* Updates */

        OGL_SetScreenBackground(0.f, 0.3f, 0.95f, 1.f);

        OGL_RenderVisitChildren(OGL_Scene);
    
        /* Swap frame buffers */
        SDL_GL_SwapWindow(SDL2_Win);

        /* Accumulate time */
        OGL_GameTime += OGL_GameDt;
        /* Frame limiter to OGL_FrameLimit FPS */
        Uint32 frameTime = SDL_GetTicks() - now;
        if(frameTime < 1000 / OGL_FrameLimit){
            SDL_Delay(1000 / OGL_FrameLimit - frameTime);
        }
    }

    /* Cleanup */
    SDL_DestroyRenderer(SDL2_Rnd);
    SDL_DestroyWindow(SDL2_Win);
    SDL_Quit();

    return SUCCESS;
}
