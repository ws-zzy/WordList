// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imguifilesystem.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
bool OpenButtonPressed, SaveButtonPressed;
static void ShowMenuFile()
{
    if (ImGui::MenuItem("Open", "Ctrl+O")) {
        OpenButtonPressed = true;
    }
    
    if (ImGui::MenuItem("Save As", "Ctrl+S")) {
        SaveButtonPressed = true;
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Quit", "Alt+F4")) {
        exit(0);
    }
}

// Demonstrate creating a fullscreen menu bar and populating it.
static void ShowMainMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

static void OpenButtonMonitor() {
    static ImGuiFs::Dialog openDlg;                             // one per dialog (and must be static)
    // ImGui::Text("Chosen file: \"%s\"", OpenButtonPressed ? "True" : "False");
    openDlg.chooseFileDialog(OpenButtonPressed);              // see other dialog types and the full list of arguments for advanced usage
    if (strlen(openDlg.getChosenPath())>0) {
        ImGui::Text("Open file: \"%s\"",openDlg.getChosenPath());
    }

    // If you want to copy the (valid) returned path somewhere, you can use something like:
    char myPath[ImGuiFs::MAX_PATH_BYTES];
    if (strlen(openDlg.getChosenPath())>0) {
        strcpy(myPath,openDlg.getChosenPath());
    }
}

static void SaveButtonMonitor() {
    static ImGuiFs::Dialog saveDlg;                             // one per dialog (and must be static)
    // ImGui::Text("Chosen file: \"%s\"", SaveButtonPressed ? "True" : "False");
    saveDlg.saveFileDialog(SaveButtonPressed);              // see other dialog types and the full list of arguments for advanced usage
    if (strlen(saveDlg.getChosenPath())>0) {
        ImGui::Text("Save file: \"%s\"",saveDlg.getChosenPath());
    }

    // If you want to copy the (valid) returned path somewhere, you can use something like:
    char myPath[ImGuiFs::MAX_PATH_BYTES];
    if (strlen(saveDlg.getChosenPath())>0) {
        strcpy(myPath,saveDlg.getChosenPath());
    }
}

bool w , c , h , t , r;
static void FlagCheckBox() {
    ImGui::Checkbox("-w", &w);      // Edit bools storing our window open/close state
    c = c & !w;
    ImGui::SameLine();
    ImGui::Checkbox("-c", &c);      // Edit bools storing our window open/close state
    w = w & !c;
    ImGui::SameLine();
    ImGui::Checkbox("-h", &h);      // Edit bools storing our window open/close state
    t = t & !h;
    ImGui::SameLine();
    ImGui::Checkbox("-t", &t);      // Edit bools storing our window open/close state
    h = h & !t;
    ImGui::SameLine();
    ImGui::Checkbox("-r", &r);      // Edit bools storing our window open/close state
    ImGui::SameLine();
    ImGui::Text("char:");
    ImGui::SameLine();
    static char word[16] = "1";
    ImGui::InputTextEx("##source", word, 2, ImVec2(ImGui::GetTextLineHeight() * 1.5, 0), ImGuiInputTextFlags_AllowTabInput);
    ImGui::SameLine();
    ImGui::Button("submit");
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(540, 320, "Longest Word List", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_another_window = true;
    ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec2 window_size = ImVec2(540.0f, 320.0f);
    ImVec2 window_pos  = ImVec2(0.0f, 0.0f); 

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::SetNextWindowSize(window_size);
            ImGui::SetNextWindowPos(window_pos);
            if (!ImGui::Begin("ImGui Demo", &show_another_window, window_flag))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                break;
            }
            ShowMainMenuBar();
            OpenButtonMonitor();
            SaveButtonMonitor();
            ImGui::Text("Input");
            static char inputText[1024*16] = 
            "zzy\n"
            "ycd\n"
            "wb\n";
            ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AllowTabInput;
            ImGui::InputTextMultiline("Input", inputText, IM_ARRAYSIZE(inputText), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 8), inputFlags);
            
            ImGui::Separator();

            ImGui::Text("Output");
            static char outputText[1024*16] = 
            "zzy\n"
            "ycd\n";
            ImGuiInputTextFlags outputFlags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
            ImGui::InputTextMultiline("outputput", outputText, IM_ARRAYSIZE(outputText), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 8), outputFlags); 
            //ImGui::Text("context: \"%s\"", outputText);
            FlagCheckBox();
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
