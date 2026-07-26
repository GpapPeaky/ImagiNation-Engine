#include "auxf/includes.aux"

int main(int, char**){
    /* Initialise SDL2 and OpenGL */
    SDL2_InitWin();
    OGL_InitContext(SDL2_Win);
    OGL_InitShaderRegistry();
    OGL_CreateShaderProgram("realmmask", OGLS_TRS_MVP_TextureV, INATE_SHADER_RealmMaskF); /* Pushed into the registry */

    OGL_Controller* ctrl = OGL_CreateController(5.0f, 0.1f);
    OGL_Camera* cam = OGL_CreateCamera({0.0f, 0.0f, 50.0f}, {0, 1, 0}, 0.0f, 0.0f);
    OGL_BindCameraToController(ctrl, cam);
    OGL_BindCameraToRenderView(cam);
    OGL_BindControllerWASD2D(ctrl);

    /* Registries */
    KENG::ProvinceRegistry preg;
    preg.ReadProvinceFile();
    preg.Print();
    KENG::RealmRegistry rreg;
    rreg.ReadRealmFile();
    rreg.Print(preg);

    /* Lightweight root object */
    OGL_Object* rootObj = OGL_CreateObject(OGL_GetShader("rootobj"));
    OGL_Scene = OGL_CreateNode(rootObj, "root");
    
    OGL_Object* provinceMap = OGL_CreateObject(OGL_GetShader("tex"));       /* Not need for an object node */
    OGL_CreateTextureQuad(*provinceMap->mesh);
    OGL_LoadBitmapToObject(*provinceMap, "History/provinces/province_map.png");
    TRS::S(*provinceMap, {50.0f * 1.777777778f, 50.f, 1.f});

    /* Text rendering */
    OGL_Object* provinceHoverInfo = OGL_CreateObject(OGL_GetShader("glyph"));
    OGL_CreateTextQuad(*provinceHoverInfo->mesh);

    OGL_Object* realmMap = OGL_CreateObject(OGL_GetShader("realmmask"));
    OGL_CreateTextureQuad(*realmMap->mesh);
    TRS::S(*realmMap, {50.0f * 1.777777778f, 50.f, 1.f}); // 2. Match scale

    /* Realm mask rendering, shader auxilliaries */
    int provMapW, provMapH;
    GLuint provinceIndexTex = KENG::GPU::BuildProvinceIndexTexture("History/provinces/province_map.png", preg, provMapW, provMapH);
    
    std::vector<std::array<float,3>> paletteCPU = KENG::GPU::BuildPaletteCPU(rreg, preg.Count());
    GLuint realmPaletteTex = KENG::GPU::CreatePaletteTexture(paletteCPU);

    KENG::GPU::RealmMaskMaterial rmm;
    rmm.mat.shader = OGL_GetShader("realmmask");
    rmm.idxTex = provinceIndexTex;
    rmm.palTex = realmPaletteTex;
    rmm.provinceCount = (float)(preg.Count() + 1);

    /* Font */
    unsigned int provinceHoverFontHeight = 18;
    FT_Library provinceHover_FTLib = OGL_InitFreeType();
    FT_Face provinceHover_FTFace = OGL_LoadFont(provinceHover_FTLib, "assets/fonts/BodoniXT.ttf", provinceHoverFontHeight);
    std::map<char, OGL_Character> provinceHover_CharMap = OGL_LoadCharacters(provinceHover_FTFace);

    /* Controllers */
    KENG::ProvinceController provCtrl;

    /* Hierarchy */
    // OGL_AttachChild(OGL_Scene, onodeRealmMap);           // Rendere manually, to avoid some problems

    Uint32 lastTime = SDL_GetTicks();
    bool OGL_GameQuit = false;
    while(!OGL_GameQuit){
        Uint32 now = SDL_GetTicks();
        OGL_GameDt = (now - lastTime) / 1000.0f;            /* Convert to seconds */
        lastTime = now;

        i32 mx, my;
        SDL_GetMouseState(&mx, &my);

        /* Updates to assets / sprites / objects in general */
        KENG::SDL2::HandleEvents(OGL_GameQuit, ctrl);      /* Creates a new event to poll per call (Might need to be optimised) */
        
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        OGL_HandleControllerKeyboard(ctrl, keys, OGL_GameDt);
        
        /* Rendering order matters */
        /* Need to pass each uniform before drawing */
        
        /* Updates */
        OGL_SetScreenBackground(0.f, 0.f, 0.f, 1.f);
        
        OGL_RenderVisitChildren(OGL_Scene); // Normal render visit for other objects

        KENG::GPU::Render(realmMap, rmm); /* Actual coloured realm map rendering */
        
        KENG::Province& hoveredProvince = provCtrl.GetHoveredProvince(preg, *provinceMap);
        KENG::Realm& hoveredRealm = provCtrl.GetHoveredRealm(rreg, hoveredProvince);
        std::string hoveredProvinceName = hoveredProvince.Name();
        std::string hoveredRealmName = hoveredRealm.Name();

        if (hoveredProvinceName != "nullprov" && hoveredRealmName != "nullrlm") {
            OGL_RenderText(*provinceHoverInfo, hoveredProvinceName.c_str(), (float)mx + (float)provinceHoverFontHeight, (float)my + (float)(provinceHoverFontHeight / 2), 1.0f, {1.0f, 1.0f, 1.0f}, provinceHover_CharMap);
            OGL_RenderText(*provinceHoverInfo, hoveredRealmName.c_str(), (float)mx + (float)provinceHoverFontHeight, (float)my + (float)(provinceHoverFontHeight * 2), 1.0f, {1.0f, 1.0f, 1.0f}, provinceHover_CharMap);
        }

        /* TEST */
        // static bool addedOther = false;
        // if (!addedOther) {
        //     KENG::Realm& rlm = rreg.Realms().at(1);

        //     // Added new provs, update realm map mask palette texture
        //     // rlm.AddProvince(0); // -> ADDS THE SEA, AVOID
        //     rlm.AddProvince(1);
        //     KENG::GPU::UpdatePaletteEntry(rmm.palTex, 1, KENG::Utils::UnpackRGB_01(rlm.Color()));
        //     rlm.AddProvince(2);
        //     KENG::GPU::UpdatePaletteEntry(rmm.palTex, 2, KENG::Utils::UnpackRGB_01(rlm.Color()));
        //     rlm.AddProvince(3);
        //     KENG::GPU::UpdatePaletteEntry(rmm.palTex, 3, KENG::Utils::UnpackRGB_01(rlm.Color()));
        // }

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
    SDL_DestroyWindow(SDL2_Win);
    SDL_Quit();

    return SUCCESS; 
}
