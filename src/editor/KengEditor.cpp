// Province editor: separate main, built into its own binary.
// Select a province (left click), press P to start typing a new name,
// ENTER writes it back to the province file, ESC cancels capture + selection.

#include "../auxf/includes.aux"

#include <filesystem>
#include <string>
#include <vector>

static const char* PROVINCE_FILE = "History/provinces/province_colors.txt"; // Same file ProvinceRegistry reads
static const size_t MAX_NAME_LEN = 48;

/* Rewrites the line of province `id`, keeping its colour, other lines are untouched */
static bool WriteProvinceName(llui id, const std::string& newName, std::string& err) {
    std::ifstream in(PROVINCE_FILE);
    if (!in.is_open()) {
        err = "Cannot open province file";
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) lines.push_back(line);
    in.close();

    bool found = false;
    for (std::string& l : lines) {
        if (l.empty()) continue;

        bool cr = (l.back() == '\r');            // Preserve CRLF files
        std::stringstream ss(l);

        llui lid;
        if (!(ss >> lid) || lid != id) continue;

        ss >> std::ws;
        if (ss.get() != '"') continue;

        std::string oldName;
        std::getline(ss, oldName, '"');

        int r, g, b;
        if (!(ss >> r >> g >> b)) continue;

        l = std::to_string(id) + " \"" + newName + "\" " +
            std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
        if (cr) l += '\r';

        found = true;
        break;
    }

    if (!found) {
        err = "Province id not found in file";
        return false;
    }

    // Write to a temp file first, so a failure cannot leave a half-written file
    std::string tmpPath = std::string(PROVINCE_FILE) + ".tmp";
    {
        std::ofstream out(tmpPath, std::ios::trunc);
        if (!out.is_open()) {
            err = "Cannot write temp file";
            return false;
        }
        for (const std::string& l : lines) out << l << '\n';
    }

    std::error_code ec;
    std::filesystem::rename(tmpPath, PROVINCE_FILE, ec);
    if (ec) {
        err = "Rename failed: " + ec.message();
        return false;
    }

    return true;
}

/* Filled rectangle without needing any engine geometry: scissored clear. Coordinates from the top-left. */
static void DrawBox(int x, int top, int w, int h, float r, float g, float b) {
    int winW, winH;
    SDL_GetWindowSize(SDL2_Win, &winW, &winH);

    glEnable(GL_SCISSOR_TEST);
    glScissor(x, winH - (top + h), w, h);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

i32 main(i32, ui8**) {
    /* Initialise SDL2 and OpenGL */
    SDL2_InitWin();
    OGL_InitContext(SDL2_Win);
    OGL_InitShaderRegistry();
    OGL_CreateShaderProgram("realmmask", OGLS_TRS_MVP_TextureV, INATE_SHADER_RealmMaskF);
    OGL_CreateShaderProgram("provinceoutline", OGLS_TRS_MVP_ColorV, INATE_SHADER_ProvOutlineMaskF);

    OGL_Controller* ctrl = OGL_CreateController(5.0f, 0.1f);
    OGL_Camera* cam = OGL_CreateCamera({0.0f, 0.0f, 50.0f}, {0, 1, 0}, 0.0f, 0.0f);
    OGL_BindCameraToController(ctrl, cam);
    OGL_BindCameraToRenderView(cam);
    OGL_BindControllerWASD2D(ctrl);

    /* Registries */
    KENG::ProvinceRegistry preg;
    preg.ReadProvinceFile();
    KENG::RealmRegistry rreg;
    rreg.ReadRealmFile();

    OGL_Object* rootObj = OGL_CreateObject(OGL_GetShader("rootobj"));
    OGL_Scene = OGL_CreateNode(rootObj, "root");

    /* Province map, only used for hover picking */
    OGL_Object* provinceMap = OGL_CreateObject(OGL_GetShader("tex"));
    OGL_CreateTextureQuad(*provinceMap->mesh);
    OGL_LoadBitmapToObject(*provinceMap, "History/provinces/province_map.png");
    TRS::S(*provinceMap, {50.0f * 1.777777778f, 50.f, 1.f});

    /* Text */
    OGL_Object* textObj = OGL_CreateObject(OGL_GetShader("glyph"));
    OGL_CreateTextQuad(*textObj->mesh);

    OGL_Object* realmMap = OGL_CreateObject(OGL_GetShader("realmmask"));
    OGL_CreateTextureQuad(*realmMap->mesh);
    TRS::S(*realmMap, {50.0f * 1.777777778f, 50.f, 1.f});

    i32 provMapW, provMapH;
    GLuint provinceIndexTex = KENG::GPU::BuildProvinceIndexTexture("History/provinces/province_map.png", preg, provMapW, provMapH);
    std::vector<std::array<float,3>> paletteCPU = KENG::GPU::BuildPaletteCPU(rreg, preg.Count());
    GLuint realmPaletteTex = KENG::GPU::CreatePaletteTexture(paletteCPU);

    KENG::GPU::RealmMaskMaterial rmm;
    rmm.mat.shader = OGL_GetShader("realmmask");
    rmm.idxTex = provinceIndexTex;
    rmm.palTex = realmPaletteTex;
    rmm.provinceCount = (float)(preg.Count() + 1);

    OGL_Object* provinceOutlines = OGL_CreateObject(OGL_GetShader("provinceoutline"));
    std::vector<glm::vec3> outlineVertices = KENG::GPU::BuildProvinceOutlineVertices("History/provinces/province_map.png", provMapW, provMapH);
    OGL_CreateLineMesh(*provinceOutlines->mesh, outlineVertices);
    TRS::S(*provinceOutlines, {50.0f * 1.777777778f, 50.f, 1.f});

    /* Font */
    ui32 fontHeight = 18;
    FT_Library ftLib = OGL_InitFreeType();
    FT_Face ftFace = OGL_LoadFont(ftLib, "assets/fonts/BodoniXT.ttf", fontHeight);
    std::map<char, OGL_Character> charMap = OGL_LoadCharacters(ftFace);

    KENG::HoverController hoverCtrl;

    /* Editor state */
    bool editing = false;          // Capturing text?
    std::string nameBuffer;        // What has been typed so far
    std::string statusMsg;         // Result of the last save
    float statusUntil = 0.0f;      // OGL_GameTime until which the status is shown

    ui32 lastTime = SDL_GetTicks();
    bool quit = false;
    while (!quit) {
        ui32 now = SDL_GetTicks();
        OGL_GameDt = (now - lastTime) / 1000.0f;
        lastTime = now;

        SDL_GetMouseState(&ctrl->lastX, &ctrl->lastY);

        /* Hover is frozen while typing so the box keeps describing the selected province */
        hoverCtrl.GetHoveredProvince(preg, *provinceMap);
        hoverCtrl.GetHoveredRealm(rreg, hoverCtrl.HoverProv());

        bool startedThisFrame = false; // The 'P' press can also produce a text input event, swallow it

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;

            /* Camera: always available */
            if (e.type == SDL_MOUSEWHEEL && ctrl->mouseZoom) {
                if (e.wheel.y > 0) KENG::SDL2::Zoom(-1.0f);
                else if (e.wheel.y < 0) KENG::SDL2::Zoom(+1.0f);
            }
            if (e.type == SDL_MOUSEMOTION) {
                OGL_HandleControllerMouse(ctrl, (float)e.motion.xrel, (float)e.motion.yrel);
            }

            if (editing) {
                if (e.type == SDL_TEXTINPUT) {
                    if (startedThisFrame) continue;
                    for (const char* c = e.text.text; *c; ++c) {
                        // Printable ASCII only (the font map is char based), no quotes: they break the file format
                        if (*c >= 32 && *c <= 126 && *c != '"' && nameBuffer.size() < MAX_NAME_LEN)
                            nameBuffer.push_back(*c);
                    }
                }

                if (e.type == SDL_KEYDOWN) {
                    SDL_Keycode k = e.key.keysym.sym;

                    if (k == SDLK_ESCAPE) {
                        /* Reset both capture and selection */
                        nameBuffer.clear();
                        editing = false;
                        SDL_StopTextInput();
                        hoverCtrl.ResetClickedProvince();
                    } else if (k == SDLK_BACKSPACE) {
                        if (!nameBuffer.empty()) nameBuffer.pop_back();
                    } else if (k == SDLK_RETURN || k == SDLK_KP_ENTER) {
                        KENG::Province& sel = hoverCtrl.ClickedProvince();
                        // Trim surrounding spaces
                        size_t a = nameBuffer.find_first_not_of(' ');
                        size_t b = nameBuffer.find_last_not_of(' ');
                        std::string newName = (a == std::string::npos) ? "" : nameBuffer.substr(a, b - a + 1);

                        if (newName.empty()) {
                            statusMsg = "Name is empty, nothing written";
                        } else {
                            std::string err;
                            if (WriteProvinceName(sel.Id(), newName, err)) {
                                // Keep the in-memory registry and selection in sync with the file
                                preg.GetProvince(KENG::Utils::UnpackRGB_0255(sel.Color())).Name() = newName;
                                sel.Name() = newName;
                                statusMsg = "Saved: " + newName;
                            } else {
                                statusMsg = "Write failed: " + err;
                            }
                        }
                        statusUntil = OGL_GameTime + 3.0f;
                        nameBuffer.clear();
                        editing = false;
                        SDL_StopTextInput();
                    }
                }
            } else {
                if (e.type == SDL_KEYDOWN) {
                    SDL_Keycode k = e.key.keysym.sym;

                    if (k == SDLK_ESCAPE) hoverCtrl.ResetClickedProvince();
                    if (k == SDLK_MINUS)  KENG::SDL2::Zoom(+1.0f);
                    if (k == SDLK_EQUALS) KENG::SDL2::Zoom(-1.0f);

                    if (k == SDLK_p && hoverCtrl.ClickedProvince().Id() != KENG::PROV::NULL_PROV_ID) {
                        editing = true;
                        nameBuffer.clear();
                        startedThisFrame = true;
                        SDL_StartTextInput();
                    }
                }

                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    hoverCtrl.SetHoveredProvinceAsClicked();
                }
            }
        }

        /* WASD would move the camera while typing a name */
        if (!editing) {
            const ui8* keys = SDL_GetKeyboardState(NULL);
            OGL_HandleControllerKeyboard(ctrl, keys, OGL_GameDt);
        }

        /* Render */
        OGL_SetScreenBackground(0.f, 0.f, 0.f, 1.f);
        OGL_RenderVisitChildren(OGL_Scene);
        KENG::GPU::RenderProvinceOutlines(provinceOutlines, {0.0f, 0.0f, 0.0f});
        KENG::GPU::RenderRealmMask(realmMap, rmm);

        /* Selected province info + input box */
        llui clickedID = hoverCtrl.ClickedProvince().Id();
        if (clickedID != KENG::PROV::NULL_PROV_ID) {
            std::string header = "Selected: " + std::to_string(clickedID) + " " + hoverCtrl.ClickedProvince().Name();

            if (editing) {
                DrawBox(8, 8, 560, 92, 0.08f, 0.08f, 0.12f);
                OGL_RenderText(*textObj, header.c_str(), 16.0f, (f32)fontHeight + 6.0f, 1.0f, {1.0f, 1.0f, 0.4f}, charMap);
                std::string typed = "New name: " + nameBuffer + "_";
                OGL_RenderText(*textObj, typed.c_str(), 16.0f, (f32)fontHeight * 2.5f + 6.0f, 1.0f, {1.0f, 1.0f, 1.0f}, charMap);
                OGL_RenderText(*textObj, "ENTER: save   ESC: cancel", 16.0f, (f32)fontHeight * 4.0f + 6.0f, 1.0f, {0.7f, 0.7f, 0.7f}, charMap);
            } else {
                OGL_RenderText(*textObj, header.c_str(), 0.0f, (f32)fontHeight, 1.0f, {1.0f, 1.0f, 1.0f}, charMap);
                OGL_RenderText(*textObj, "P: rename   ESC: deselect", 0.0f, (f32)fontHeight * 2.0f, 1.0f, {0.7f, 0.7f, 0.7f}, charMap);
            }
        }

        if (!statusMsg.empty() && OGL_GameTime < statusUntil) {
            OGL_RenderText(*textObj, statusMsg.c_str(), 0.0f, (f32)fontHeight * 3.0f + (editing ? 80.0f : 0.0f), 1.0f, {0.4f, 1.0f, 0.4f}, charMap);
        }

        /* Hover info: realm name, province name, province id (hidden while the input box is open) */
        if (!editing && hoverCtrl.HoverProv().Id() != KENG::PROV::NULL_PROV_ID) {
            KENG::Province& hp = hoverCtrl.HoverProv();
            KENG::Realm&    hr = hoverCtrl.HoverRealm();

            std::string realmName = (hr.Id() != KENG::RLM::NULL_RLM_ID) ? hr.Name() : "None";

            std::string lines[3] = {
                "Realm Name: "    + realmName,
                "Province Name: " + hp.Name(),
                "Province ID: "   + std::to_string(hp.Id())
            };

            int   boxX   = ctrl->lastX + 20;
            int   boxTop = ctrl->lastY + 16;
            float lineH  = (f32)fontHeight * 1.5f;

            DrawBox(boxX - 6, boxTop, 300, (int)(lineH * 3.0f) + 8, 0.08f, 0.08f, 0.12f);

            for (int i = 0; i < 3; i++) {
                OGL_RenderText(*textObj, lines[i].c_str(),
                               (f32)boxX, (f32)boxTop + 4.0f + (f32)fontHeight + lineH * (f32)i,
                               1.0f, {1.0f, 1.0f, 1.0f}, charMap);
            }
        }

        SDL_GL_SwapWindow(SDL2_Win);

        OGL_GameTime += OGL_GameDt;
        ui32 frameTime = SDL_GetTicks() - now;
        if (frameTime < 1000 / OGL_FrameLimit) {
            SDL_Delay(1000 / OGL_FrameLimit - frameTime);
        }
    }

    SDL_DestroyWindow(SDL2_Win);
    SDL_Quit();

    return SUCCESS;
}