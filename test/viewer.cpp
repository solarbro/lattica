#include "gl_core_3_3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <thread>
#include <imgui/imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <cmath>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void window_close_callback(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, 1);
}

constexpr int minSizeX = 1280;
constexpr int minSizeY = 720;

void window_size_callback(GLFWwindow *window, int w, int h)
{
    int neww = w;
    int newh = h;
    if(neww < minSizeX)
        neww = minSizeX;
    if(newh < minSizeY)
        newh = minSizeY;

    if(neww != w || newh != h)
        glfwSetWindowSize(window, neww, newh);
}

ImVec4 lerp(const ImVec4 &v0, const ImVec4 &v1, float t);

void SetThemeDark(ImGuiStyle *style);
void SetFont(ImFontAtlas *fonts);

void DrawUI(unsigned windowWidth, unsigned windowHeight);

int main (int argc, char *argv[])
{
    printf("Hello, World!\n");

    // Initialize a window
    GLFWwindow *mainWindow = nullptr;

    if(!glfwInit())
        printf("Failed to initialize GLFW\n");

    glfwSetErrorCallback(error_callback);

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    mainWindow = glfwCreateWindow(minSizeX, minSizeY, "Pi3D Viewer", nullptr, nullptr);
    if(!mainWindow)
    {
        printf("Window creation failed!\n");
        return -1;
    }

    // printf("Starting loop\n");
    printf("Init graphics\n");

    // Load OpenGl
    int err = ogl_LoadFunctions();
    if(!err)
    {
        printf("OpenGl load failed\n");
        return -1;
    }
    glfwSetWindowCloseCallback(mainWindow, window_close_callback);

    glfwMakeContextCurrent(mainWindow);
    const GLubyte *driver = glGetString(GL_VERSION);
    printf("OpenGL version: %s\n", driver);

    // Initialize imgui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGuiContext *imguiCtx = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;

    SetThemeDark(&ImGui::GetStyle());

    // Setup platform/renderer bindings
    ImGui_Glfw_Context *imguiGlfwCtx;
    ImGui_Opengl_Context *imguiOpenGlCtx;
    ImGui_ImplGlfw_InitForOpenGL(&imguiGlfwCtx, mainWindow, true);
    ImGui_ImplOpenGL3_Init(&imguiOpenGlCtx, "#version 330");

    SetFont(io.Fonts);

    // Enable vsync
    glfwSwapInterval(1); 

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        // Always make context current as the child windows will draw in their own context
        glfwMakeContextCurrent(mainWindow);

        static const double targetFps = 1.0 / 60.0;
        double start = glfwGetTime();

        // Do rendering
        int w, h;
        glfwGetWindowSize(mainWindow, &w, &h);
        glViewport(0, 0, w, h);

        // glClearColor(std::sin(start), std::cos(start), std::abs(1.f - std::cos(start)), 1.f);
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set current ImGui context
        ImGui::SetCurrentContext(imguiCtx);

        ImGui_ImplOpenGL3_NewFrame(imguiOpenGlCtx);
        ImGui_ImplGlfw_NewFrame(imguiGlfwCtx);

        int width, height;
        glfwGetWindowSize(mainWindow, &width, &height);
        DrawUI(width, height);

        ImGui_ImplOpenGL3_RenderDrawData(imguiOpenGlCtx, ImGui::GetDrawData());

        double end = glfwGetTime();

        // Swap buffers
        glfwSwapBuffers(mainWindow);
    }

    ImGui_ImplOpenGL3_Shutdown(imguiOpenGlCtx);
    ImGui_ImplGlfw_Shutdown(imguiGlfwCtx);
    ImGui::DestroyContext(imguiCtx);

    // Shutdown window system
    if(mainWindow)
        glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}

/* 

int Viewer::Run()
{

    m_renderer.Run(m_mainWindow);

    // m_renderer.StartRenderThread(m_mainWindow);

    // while(!glfwWindowShouldClose(m_mainWindow))
    // {
        // glfwPollEvents();

        // Keep running
        // std::this_thread::sleep_for(std::chrono::milliseconds(16));

    // }

    // m_renderer.StopRenderThread();

    printf("\nExiting update loop!\n");
}
*/

ImVec4 lerp(const ImVec4 &v0, const ImVec4 &v1, float t)
{
    ImVec4 res;
    for(unsigned i = 0; i < 4; ++i)
    {
        *(&res.x + i) = t * *(&res.x) + (1 - t) * *(&res.x);
    }
    return res;
}

void SetThemeDark(ImGuiStyle *style)
{
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.10f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.26f, 0.29f, 0.28f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.36f, 0.39f, 0.38f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.37f, 0.40f, 0.39f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.54f, 0.52f, 0.58f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.76f, 0.79f, 0.78f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.00f, 0.60f, 1.00f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.01f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.90f, 0.40f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = lerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.80f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.11f, 0.11f, 0.11f, 1.f); //lerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.4f, 0.4f, 0.4f, 1.f);  //lerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.5f, 0.5f, 0.5f, 1.f);  //lerp(colors[ImGuiCol_TabActive],    colors[ImGuiCol_TitleBg], 0.40f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style->WindowRounding = 0.f;
    style->TabRounding = 0.f;
}

void SetFont(ImFontAtlas *fonts)
{
    fonts->AddFontDefault();
}

void DrawUIPlane();
void DrawUICuboid();
void DrawUISphere();
void DrawUICylinder();
void DrawUICone();
void DrawUIPrism();
void DrawUIPyramid();
void DrawUITube();
void DrawUICapsule();
void DrawUITorus();
void DrawUITorusKnot();

typedef void (*pfnDrawUIShape)(void);
static const pfnDrawUIShape drawUIShapeFn[] = {
    DrawUIPlane,
    DrawUICuboid,
    DrawUISphere,
    DrawUICylinder,
    DrawUICone,
    DrawUIPrism,
    DrawUIPyramid,
    DrawUITube,
    DrawUICapsule,
    DrawUITorus,
    DrawUITorusKnot,
};

void DrawUI(unsigned windowWidth, unsigned windowHeight)
{
    ImGui::NewFrame();

    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup
    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(360, windowHeight));

    // Control panel window items
    // Shape selection
    static const char * const items[] = { 
        "Plane", 
        "Cuboid",
        "Sphere", 
        "Cylinder", 
        "Cone", 
        "Prism", 
        "Pyramid", 
        "Tube", 
        "Capsule", 
        "Torus", 
        "Torus Knot"
    }; 

    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::CollapsingHeader("Generate", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static const char* current_item = NULL;
        static int selection = 0;
        static int prevSelection = 0;
        if(ImGui::Combo("Shape", &selection, items, IM_ARRAYSIZE(items)))
        {
            if(prevSelection != selection)
            {
                printf("New selection: %s\n", items[selection]);
                prevSelection = selection;
            }
        }

        static const char * const windingOrders[] = {
            "Counter-Clockwise",
            "Clockwise"
        };
        static int windingOrderSelection = 0;
        static int prevWindingOrderSelection = 0;
        if(ImGui::Combo("Winding Order", &windingOrderSelection, windingOrders, IM_ARRAYSIZE(windingOrders)))
        {
            if(prevWindingOrderSelection != windingOrderSelection)
            {
                printf("Update winding order: %s\n", windingOrders[windingOrderSelection]);
                prevWindingOrderSelection = windingOrderSelection;
            }
        }

        drawUIShapeFn[selection]();
    }

    if (ImGui::CollapsingHeader("Info"))
    {
        ImGui::Text("Vertex count:");
        ImGui::Text("Polygon count:");
        ImGui::Text("Memory usage:");
    }

    if (ImGui::CollapsingHeader("Visualize"))
    {
        static bool showShaded = true;
        ImGui::Checkbox("Show shaded", &showShaded);
        static bool showEdges = false;
        ImGui::Checkbox("Show polygon edges", &showEdges);
        static bool showNormals = false;
        ImGui::Checkbox("Show normals", &showNormals);

        static float translation[3] = {};
        ImGui::DragFloat3("Translate", translation, 0.1f);
        static float rotation[3] = {};
        ImGui::DragFloat3("Rotate", rotation, 0.1f, -M_PI, M_PI);
        static float scale[3] = {1.f, 1.f, 1.f};
        ImGui::DragFloat3("Scale", scale, 0.1f, 0.01f, 10.f);

        if(ImGui::Button("Reset Transforms"))
        {
            translation[0] = translation[1] = translation[2] = 0.f;
            rotation[0] = rotation[1] = rotation[2] = 0.f;
            scale[0] = scale[1] = scale[2] = 1.f;
        }
    }

    ImGui::End();

    ImGui::Render();
}

void DrawUIPlane()
{
    static int div[2] = {1, 1};
    ImGui::SliderInt2("Subdiv", div, 1, 256);
    static float size[2] = {1.f, 1.f};
    ImGui::SliderFloat2("Size", size, 0.01f, 10.f);
}

void DrawUICuboid()
{
    static int div[3] = {1, 1, 1};
    ImGui::SliderInt3("Subdiv", div, 1, 256);
    static float size[3] = {1.f, 1.f, 1.f};
    ImGui::SliderFloat3("Size", size, 0.01f, 10.f);
}

void DrawUISphere()
{
    static int divLong = 8;
    static int divLat = 8;
    ImGui::SliderInt("Subdiv longitude", &divLong, 3, 128);
    ImGui::SliderInt("Subdiv latitude", &divLat, 3, 128);
    static float radius = 0.5f;
    ImGui::SliderFloat("Radius", &radius, 0.01f, 10.f);
}

void DrawUICylinder()
{
    static int divRadial = 8;
    static int divAxial = 1;
    static int divRings = 1;
    ImGui::SliderInt("Subdiv radial", &divRadial, 3, 128);
    ImGui::SliderInt("Subdiv axial", &divAxial, 1, 128);
    ImGui::SliderInt("Subdiv rings", &divRings, 1, 64);
    static float radius = 0.5f;
    static float length = 1.f;
    ImGui::SliderFloat("Radius", &radius, 0.01f, 10.f);
    ImGui::SliderFloat("Length", &length, 0.01f, 10.f);
}

void DrawUICone()
{
    static int divRadial = 8;
    static int divAxial = 1;
    static int divRings = 1;
    ImGui::SliderInt("Subdiv radial", &divRadial, 3, 128);
    ImGui::SliderInt("Subdiv axial", &divAxial, 1, 128);
    ImGui::SliderInt("Subdiv rings", &divRings, 1, 64);
    static float radius = 0.5f;
    static float length = 1.f;
    ImGui::SliderFloat("Radius", &radius, 0.01f, 10.f);
    ImGui::SliderFloat("Length", &length, 0.01f, 10.f);
}

void DrawUIPrism()
{
    static int numFacets = 3;
    ImGui::SliderInt("Facets", &numFacets, 3, 64);
    static int divRadialPerFacet = 8;
    static int divAxial = 1;
    static int divRings = 1;
    ImGui::SliderInt("Subdiv radial (per facet)", &divRadialPerFacet, 1, 32);
    ImGui::SliderInt("Subdiv axial", &divAxial, 1, 64);
    ImGui::SliderInt("Subdiv rings", &divRings, 1, 64);
    static float radius = 0.5f;
    static float length = 1.f;
    ImGui::SliderFloat("Radius", &radius, 0.01f, 10.f);
    ImGui::SliderFloat("Length", &length, 0.01f, 10.f);
}

void DrawUIPyramid()
{
    static int numFacets = 3;
    ImGui::SliderInt("Facets", &numFacets, 3, 64);
    static int divRadialPerFacet = 8;
    static int divAxial = 1;
    static int divRings = 1;
    ImGui::SliderInt("Subdiv radial (per facet)", &divRadialPerFacet, 1, 32);
    ImGui::SliderInt("Subdiv axial", &divAxial, 1, 64);
    ImGui::SliderInt("Subdiv rings", &divRings, 1, 64);
    static float radius = 0.5f;
    static float length = 1.f;
    ImGui::SliderFloat("Radius", &radius, 0.01f, 10.f);
    ImGui::SliderFloat("Length", &length, 0.01f, 10.f);
}

void DrawUITube()
{
    static int divRadial = 8;
    static int divAxial = 1;
    static int divRings = 1;
    ImGui::SliderInt("Subdiv radial", &divRadial, 3, 128);
    ImGui::SliderInt("Subdiv axial", &divAxial, 1, 128);
    ImGui::SliderInt("Subdiv rings", &divRings, 1, 64);
    static float innerRadius = 0.25f;
    static float outerRadius = 0.5f;
    static float length = 1.f;
    ImGui::SliderFloat("Outer radius", &outerRadius, 0.01f, 10.f);
    ImGui::SliderFloat("Inner radius", &innerRadius, 0.01f, 10.f);
    ImGui::SliderFloat("Length", &length, 0.01f, 10.f);
}

void DrawUICapsule()
{
    static int divRadial = 8;
    static int divAxial = 1;
    static int divLatitude = 1;
    ImGui::SliderInt("Subdiv radial", &divRadial, 3, 128);
    ImGui::SliderInt("Subdiv axial", &divAxial, 1, 128);
    ImGui::SliderInt("Subdiv latitude", &divLatitude, 1, 64);
    static float radius = 0.25f;
    static float length = 0.5f;
    ImGui::SliderFloat("Outer radius", &radius, 0.01f, 10.f);
    ImGui::SliderFloat("Length", &length, 0.01f, 10.f);
}

void DrawUITorus()
{
    static int divRadial = 8;
    static int divTubular = 6;
    ImGui::SliderInt("Subdiv radial", &divRadial, 3, 128);
    ImGui::SliderInt("Subdiv tubular", &divTubular, 3, 128);
    static float majorRadius = 0.5f;
    static float minorRadius = 0.125f;
    ImGui::SliderFloat("Major radius", &majorRadius, 0.01f, 10.f);
    ImGui::SliderFloat("Minor radius", &minorRadius, 0.01f, 10.f);
}

void DrawUITorusKnot()
{
    static int divRadial = 8;
    static int divTubular = 6;
    ImGui::SliderInt("Subdiv radial", &divRadial, 3, 128);
    ImGui::SliderInt("Subdiv tubular", &divTubular, 3, 128);
    static float radius = 0.5f;
    static float torusRadius = 0.25f;
    static float tubeRadius = 0.05f;
    ImGui::SliderFloat("Radius", &radius, 0.1f, 10.f);
    ImGui::SliderFloat("Torus radius", &torusRadius, 0.01f, 10.f);
    ImGui::SliderFloat("Tube radius", &tubeRadius, 0.01f, 1.f);
    static int p = 2; 
    static int q = 3;
    ImGui::SliderInt("P", &p, 1, 64);
    ImGui::SliderInt("Q", &q, 1, 64);
}