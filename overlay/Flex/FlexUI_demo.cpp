// dear FlexUI, v1.89.3 WIP
// (demo code)

// Help:
// - Read FAQ at http://dearFlexUI.org/faq
// - Newcomers, read 'Programmer guide' in FlexUI.cpp for notes on how to setup Dear FlexUI in your codebase.
// - Call and read FlexUI::ShowDemoWindow() in FlexUI_demo.cpp. All applications in examples/ are doing that.
// Read FlexUI.cpp for more details, documentation and comments.
// Get the latest version at https://github.com/ocornut/FlexUI

// -------------------------------------------------
// PLEASE DO NOT REMOVE THIS FILE FROM YOUR PROJECT!
// -------------------------------------------------
// Message to the person tempted to delete this file when integrating Dear FlexUI into their codebase:
// Think again! It is the most useful reference code that you and other coders will want to refer to and call.
// Have the FlexUI::ShowDemoWindow() function wired in an always-available debug menu of your game/app!
// Also include Metrics! ItemPicker! DebugLog! and other debug features.
// Removing this file from your project is hindering access to documentation for everyone in your team,
// likely leading you to poorer usage of the library.
// Everything in this file will be stripped out by the linker if you don't call FlexUI::ShowDemoWindow().
// If you want to link core Dear FlexUI in your shipped builds but want a thorough guarantee that the demo will not be
// linked, you can setup your imconfig.h with #define FlexUI_DISABLE_DEMO_WINDOWS and those functions will be empty.
// In another situation, whenever you have Dear FlexUI available you probably want this to be available for reference.
// Thank you,
// -Your beloved friend, FlexUI_demo.cpp (which you won't delete)

// Message to beginner C/C++ programmers about the meaning of the 'static' keyword:
// In this demo code, we frequently use 'static' variables inside functions. A static variable persists across calls,
// so it is essentially like a global variable but declared inside the scope of the function. We do this as a way to
// gather code and data in the same place, to make the demo source code faster to read, faster to write, and smaller
// in size. It also happens to be a convenient way of storing simple UI related information as long as your function
// doesn't need to be reentrant or used in multiple threads. This might be a pattern you will want to use in your code,
// but most of the real data you would be editing is likely going to be stored outside your functions.

// The Demo code in this file is designed to be easy to copy-and-paste into your application!
// Because of this:
// - We never omit the FlexUI:: prefix when calling functions, even though most code here is in the same namespace.
// - We try to declare static variables in the local scope, as close as possible to the code using them.
// - We never use any of the helpers/facilities used internally by Dear FlexUI, unless available in the public API.
// - We never use maths operators on ImVec2/ImVec4. For our other sources files we use them, and they are provided
//   by FlexUI_internal.h using the FlexUI_DEFINE_MATH_OPERATORS define. For your own sources file they are optional
//   and require you either enable those, either provide your own via IM_VEC2_CLASS_EXTRA in imconfig.h.
//   Because we can't assume anything about your support of maths operators, we cannot use them in FlexUI_demo.cpp.

// Navigating this file:
// - In Visual Studio IDE: CTRL+comma ("Edit.GoToAll") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
// - With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.

/*

Index of this file:

// [SECTION] Forward Declarations
// [SECTION] Helpers
// [SECTION] Demo Window / ShowDemoWindow()
// - ShowDemoWindow()
// - sub section: ShowDemoWindowWidgets()
// - sub section: ShowDemoWindowLayout()
// - sub section: ShowDemoWindowPopups()
// - sub section: ShowDemoWindowTables()
// - sub section: ShowDemoWindowInputs()
// [SECTION] About Window / ShowAboutWindow()
// [SECTION] Style Editor / ShowStyleEditor()
// [SECTION] User Guide / ShowUserGuide()
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
// [SECTION] Example App: Fullscreen window / ShowExampleAppFullscreen()
// [SECTION] Example App: Manipulating window titles / ShowExampleAppWindowTitles()
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "FlexUI.h"
#ifndef FlexUI_DISABLE

// System includes
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning (disable: 26451)    // [Static Analyzer] Arithmetic overflow : Using operator 'xxx' on a 4 byte value and then casting the result to an 8 byte value. Cast the value to the wider type before calling operator 'xxx' to avoid overflow(io.2).
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'                     // not all warnings are known by all Clang versions and they tend to be rename-happy.. so ignoring warnings triggers new warnings on some configuration. Great!
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning: use of old-style cast                           // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"        // warning: 'xx' is deprecated: The POSIX name for this..   // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"       // warning: cast to 'void *' from smaller integer type
#pragma clang diagnostic ignored "-Wformat-security"                // warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"          // warning: declaration requires an exit-time destructor    // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. FlexUI coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"                  // warning: macro is not used                               // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning: zero as null pointer constant                   // some standard header variations use #define NULL 0
#pragma clang diagnostic ignored "-Wdouble-promotion"               // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#pragma clang diagnostic ignored "-Wreserved-id-macro"              // warning: macro name is a reserved identifier
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"          // warning: format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wmisleading-indentation"   // [__GNUC__ >= 6] warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif

// Play it nice with Windows users (Update: May 2018, Notepad now supports Unix-style carriage returns!)
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#else
#define IM_NEWLINE  "\n"
#endif

// Helpers
#if defined(_MSC_VER) && !defined(snprintf)
#define snprintf    _snprintf
#endif
#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf   _vsnprintf
#endif

// Format specifiers, printing 64-bit hasn't been decently standardized...
// In a real application you should be using PRId64 and PRIu64 from <inttypes.h> (non-windows) and on Windows define them yourself.
#ifdef _MSC_VER
#define IM_PRId64   "I64d"
#define IM_PRIu64   "I64u"
#else
#define IM_PRId64   "lld"
#define IM_PRIu64   "llu"
#endif

// Helpers macros
// We normally try to not use many helpers in FlexUI_demo.cpp in order to make code easier to copy and paste,
// but making an exception here as those are largely simplifying code...
// In other FlexUI sources we can use nicer internal functions from FlexUI_internal.h (ImMin/ImMax) but not in the demo.
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Enforce cdecl calling convention for functions called by the standard library, in case compilation settings changed the default to e.g. __vectorcall
#ifndef FlexUI_CDECL
#ifdef _MSC_VER
#define FlexUI_CDECL __cdecl
#else
#define FlexUI_CDECL
#endif
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(FlexUI_DISABLE_DEMO_WINDOWS)

// Forward Declarations
static void ShowExampleAppDocuments(bool* p_open);
static void ShowExampleAppMainMenuBar();
static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppSimpleOverlay(bool* p_open);
static void ShowExampleAppFullscreen(bool* p_open);
static void ShowExampleAppWindowTitles(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleMenuFile();

// We split the contents of the big ShowDemoWindow() function into smaller functions
// (because the link time of very large functions grow non-linearly)
static void ShowDemoWindowWidgets();
static void ShowDemoWindowLayout();
static void ShowDemoWindowPopups();
static void ShowDemoWindowTables();
static void ShowDemoWindowColumns();
static void ShowDemoWindowInputs();

//-----------------------------------------------------------------------------
// [SECTION] Helpers
//-----------------------------------------------------------------------------

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    FlexUI::TextDisabled("(?)");
    if (FlexUI::IsItemHovered(FlexUIHoveredFlags_DelayShort))
    {
        FlexUI::BeginTooltip();
        FlexUI::PushTextWrapPos(FlexUI::GetFontSize() * 35.0f);
        FlexUI::TextUnformatted(desc);
        FlexUI::PopTextWrapPos();
        FlexUI::EndTooltip();
    }
}

// Helper to wire demo markers located in code to an interactive browser
typedef void (*FlexUIDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern FlexUIDemoMarkerCallback      GFlexUIDemoMarkerCallback;
extern void* GFlexUIDemoMarkerCallbackUserData;
FlexUIDemoMarkerCallback             GFlexUIDemoMarkerCallback = NULL;
void* GFlexUIDemoMarkerCallbackUserData = NULL;
#define FlexUI_DEMO_MARKER(section)  do { if (GFlexUIDemoMarkerCallback != NULL) GFlexUIDemoMarkerCallback(__FILE__, __LINE__, section, GFlexUIDemoMarkerCallbackUserData); } while (0)

//-----------------------------------------------------------------------------
// [SECTION] Demo Window / ShowDemoWindow()
//-----------------------------------------------------------------------------
// - ShowDemoWindow()
// - ShowDemoWindowWidgets()
// - ShowDemoWindowLayout()
// - ShowDemoWindowPopups()
// - ShowDemoWindowTables()
// - ShowDemoWindowColumns()
// - ShowDemoWindowInputs()
//-----------------------------------------------------------------------------

// Demonstrate most Dear FlexUI features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does.
// You may then search for keywords in the code when you are interested by a specific feature.
void FlexUI::ShowDemoWindow(bool* p_open)
{
    // Exceptionally add an extra assert here for people confused about initial Dear FlexUI setup
    // Most functions would normally just crash if the context is missing.
    IM_ASSERT(FlexUI::GetCurrentContext() != NULL && "Missing dear FlexUI context. Refer to examples app!");

    // Examples Apps (accessible from the "Examples" menu)
    static bool show_app_main_menu_bar = false;
    static bool show_app_documents = false;
    static bool show_app_console = false;
    static bool show_app_log = false;
    static bool show_app_layout = false;
    static bool show_app_property_editor = false;
    static bool show_app_long_text = false;
    static bool show_app_auto_resize = false;
    static bool show_app_constrained_resize = false;
    static bool show_app_simple_overlay = false;
    static bool show_app_fullscreen = false;
    static bool show_app_window_titles = false;
    static bool show_app_custom_rendering = false;

    if (show_app_main_menu_bar)       ShowExampleAppMainMenuBar();
    if (show_app_documents)           ShowExampleAppDocuments(&show_app_documents);
    if (show_app_console)             ShowExampleAppConsole(&show_app_console);
    if (show_app_log)                 ShowExampleAppLog(&show_app_log);
    if (show_app_layout)              ShowExampleAppLayout(&show_app_layout);
    if (show_app_property_editor)     ShowExampleAppPropertyEditor(&show_app_property_editor);
    if (show_app_long_text)           ShowExampleAppLongText(&show_app_long_text);
    if (show_app_auto_resize)         ShowExampleAppAutoResize(&show_app_auto_resize);
    if (show_app_constrained_resize)  ShowExampleAppConstrainedResize(&show_app_constrained_resize);
    if (show_app_simple_overlay)      ShowExampleAppSimpleOverlay(&show_app_simple_overlay);
    if (show_app_fullscreen)          ShowExampleAppFullscreen(&show_app_fullscreen);
    if (show_app_window_titles)       ShowExampleAppWindowTitles(&show_app_window_titles);
    if (show_app_custom_rendering)    ShowExampleAppCustomRendering(&show_app_custom_rendering);

    // Dear FlexUI Tools/Apps (accessible from the "Tools" menu)
    static bool show_app_metrics = false;
    static bool show_app_debug_log = false;
    static bool show_app_stack_tool = false;
    static bool show_app_about = false;
    static bool show_app_style_editor = false;

    if (show_app_metrics)
        FlexUI::ShowMetricsWindow(&show_app_metrics);
    if (show_app_debug_log)
        FlexUI::ShowDebugLogWindow(&show_app_debug_log);
    if (show_app_stack_tool)
        FlexUI::ShowStackToolWindow(&show_app_stack_tool);
    if (show_app_about)
        FlexUI::ShowAboutWindow(&show_app_about);
    if (show_app_style_editor)
    {
        FlexUI::Begin("Dear FlexUI Style Editor", &show_app_style_editor);
        FlexUI::ShowStyleEditor();
        FlexUI::End();
    }

    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool unsaved_document = false;

    FlexUIWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= FlexUIWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= FlexUIWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= FlexUIWindowFlags_MenuBar;
    if (no_move)            window_flags |= FlexUIWindowFlags_NoMove;
    if (no_resize)          window_flags |= FlexUIWindowFlags_NoResize;
    if (no_collapse)        window_flags |= FlexUIWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= FlexUIWindowFlags_NoNav;
    if (no_background)      window_flags |= FlexUIWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= FlexUIWindowFlags_NoBringToFrontOnFocus;
    if (unsaved_document)   window_flags |= FlexUIWindowFlags_UnsavedDocument;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const FlexUIViewport* main_viewport = FlexUI::GetMainViewport();
    FlexUI::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), FlexUICond_FirstUseEver);
    FlexUI::SetNextWindowSize(ImVec2(550, 680), FlexUICond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!FlexUI::Begin("Dear FlexUI Demo", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        FlexUI::End();
        return;
    }

    // Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
    // e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
    //FlexUI::PushItemWidth(-FlexUI::GetWindowWidth() * 0.35f);
    // e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
    FlexUI::PushItemWidth(FlexUI::GetFontSize() * -12);

    // Menu Bar
    if (FlexUI::BeginMenuBar())
    {
        if (FlexUI::BeginMenu("Menu"))
        {
            FlexUI_DEMO_MARKER("Menu/File");
            ShowExampleMenuFile();
            FlexUI::EndMenu();
        }
        if (FlexUI::BeginMenu("Examples"))
        {
            FlexUI_DEMO_MARKER("Menu/Examples");
            FlexUI::MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
            FlexUI::MenuItem("Console", NULL, &show_app_console);
            FlexUI::MenuItem("Log", NULL, &show_app_log);
            FlexUI::MenuItem("Simple layout", NULL, &show_app_layout);
            FlexUI::MenuItem("Property editor", NULL, &show_app_property_editor);
            FlexUI::MenuItem("Long text display", NULL, &show_app_long_text);
            FlexUI::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
            FlexUI::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
            FlexUI::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
            FlexUI::MenuItem("Fullscreen window", NULL, &show_app_fullscreen);
            FlexUI::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
            FlexUI::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
            FlexUI::MenuItem("Documents", NULL, &show_app_documents);
            FlexUI::EndMenu();
        }
        //if (FlexUI::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
        if (FlexUI::BeginMenu("Tools"))
        {
            FlexUI_DEMO_MARKER("Menu/Tools");
#ifndef FlexUI_DISABLE_DEBUG_TOOLS
            const bool has_debug_tools = true;
#else
            const bool has_debug_tools = false;
#endif
            FlexUI::MenuItem("Metrics/Debugger", NULL, &show_app_metrics, has_debug_tools);
            FlexUI::MenuItem("Debug Log", NULL, &show_app_debug_log, has_debug_tools);
            FlexUI::MenuItem("Stack Tool", NULL, &show_app_stack_tool, has_debug_tools);
            FlexUI::MenuItem("Style Editor", NULL, &show_app_style_editor);
            FlexUI::MenuItem("About Dear FlexUI", NULL, &show_app_about);
            FlexUI::EndMenu();
        }
        FlexUI::EndMenuBar();
    }

    FlexUI::Text("dear FlexUI says hello! (%s) (%d)", FlexUI_VERSION, FlexUI_VERSION_NUM);
    FlexUI::Spacing();

    FlexUI_DEMO_MARKER("Help");
    if (FlexUI::CollapsingHeader("Help"))
    {
        FlexUI::Text("ABOUT THIS DEMO:");
        FlexUI::BulletText("Sections below are demonstrating many aspects of the library.");
        FlexUI::BulletText("The \"Examples\" menu above leads to more demo contents.");
        FlexUI::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
            "and Metrics/Debugger (general purpose Dear FlexUI debugging tool).");
        FlexUI::Separator();

        FlexUI::Text("PROGRAMMER GUIDE:");
        FlexUI::BulletText("See the ShowDemoWindow() code in FlexUI_demo.cpp. <- you are here!");
        FlexUI::BulletText("See comments in FlexUI.cpp.");
        FlexUI::BulletText("See example applications in the examples/ folder.");
        FlexUI::BulletText("Read the FAQ at http://www.dearFlexUI.org/faq/");
        FlexUI::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        FlexUI::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
        FlexUI::Separator();

        FlexUI::Text("USER GUIDE:");
        FlexUI::ShowUserGuide();
    }

    FlexUI_DEMO_MARKER("Configuration");
    if (FlexUI::CollapsingHeader("Configuration"))
    {
        FlexUIIO& io = FlexUI::GetIO();

        if (FlexUI::TreeNode("Configuration##2"))
        {
            FlexUI::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", &io.ConfigFlags, FlexUIConfigFlags_NavEnableKeyboard);
            FlexUI::SameLine(); HelpMarker("Enable keyboard controls.");
            FlexUI::CheckboxFlags("io.ConfigFlags: NavEnableGamepad", &io.ConfigFlags, FlexUIConfigFlags_NavEnableGamepad);
            FlexUI::SameLine(); HelpMarker("Enable gamepad controls. Require backend to set io.BackendFlags |= FlexUIBackendFlags_HasGamepad.\n\nRead instructions in FlexUI.cpp for details.");
            FlexUI::CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", &io.ConfigFlags, FlexUIConfigFlags_NavEnableSetMousePos);
            FlexUI::SameLine(); HelpMarker("Instruct navigation to move the mouse cursor. See comment for FlexUIConfigFlags_NavEnableSetMousePos.");
            FlexUI::CheckboxFlags("io.ConfigFlags: NoMouse", &io.ConfigFlags, FlexUIConfigFlags_NoMouse);
            if (io.ConfigFlags & FlexUIConfigFlags_NoMouse)
            {
                // The "NoMouse" option can get us stuck with a disabled mouse! Let's provide an alternative way to fix it:
                if (fmodf((float)FlexUI::GetTime(), 0.40f) < 0.20f)
                {
                    FlexUI::SameLine();
                    FlexUI::Text("<<PRESS SPACE TO DISABLE>>");
                }
                if (FlexUI::IsKeyPressed(FlexUIKey_Space))
                    io.ConfigFlags &= ~FlexUIConfigFlags_NoMouse;
            }
            FlexUI::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", &io.ConfigFlags, FlexUIConfigFlags_NoMouseCursorChange);
            FlexUI::SameLine(); HelpMarker("Instruct backend to not alter mouse cursor shape and visibility.");
            FlexUI::Checkbox("io.ConfigInputTrickleEventQueue", &io.ConfigInputTrickleEventQueue);
            FlexUI::SameLine(); HelpMarker("Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, improving interactions with low framerates.");
            FlexUI::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
            FlexUI::SameLine(); HelpMarker("Enable blinking cursor (optional as some users consider it to be distracting).");
            FlexUI::Checkbox("io.ConfigInputTextEnterKeepActive", &io.ConfigInputTextEnterKeepActive);
            FlexUI::SameLine(); HelpMarker("Pressing Enter will keep item active and select contents (single-line only).");
            FlexUI::Checkbox("io.ConfigDragClickToInputText", &io.ConfigDragClickToInputText);
            FlexUI::SameLine(); HelpMarker("Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving).");
            FlexUI::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
            FlexUI::SameLine(); HelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & FlexUIBackendFlags_HasMouseCursors) because it needs mouse cursor feedback.");
            FlexUI::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
            FlexUI::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
            FlexUI::SameLine(); HelpMarker("Instruct Dear FlexUI to render a mouse cursor itself. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");
            FlexUI::Text("Also see Style->Rendering for rendering options.");
            FlexUI::TreePop();
            FlexUI::Separator();
        }

        FlexUI_DEMO_MARKER("Configuration/Backend Flags");
        if (FlexUI::TreeNode("Backend Flags"))
        {
            HelpMarker(
                "Those flags are set by the backends (FlexUI_impl_xxx files) to specify their capabilities.\n"
                "Here we expose them as read-only fields to avoid breaking interactions with your backend.");

            // Make a local copy to avoid modifying actual backend flags.
            // FIXME: We don't use BeginDisabled() to keep label bright, maybe we need a BeginReadonly() equivalent..
            FlexUIBackendFlags backend_flags = io.BackendFlags;
            FlexUI::CheckboxFlags("io.BackendFlags: HasGamepad", &backend_flags, FlexUIBackendFlags_HasGamepad);
            FlexUI::CheckboxFlags("io.BackendFlags: HasMouseCursors", &backend_flags, FlexUIBackendFlags_HasMouseCursors);
            FlexUI::CheckboxFlags("io.BackendFlags: HasSetMousePos", &backend_flags, FlexUIBackendFlags_HasSetMousePos);
            FlexUI::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", &backend_flags, FlexUIBackendFlags_RendererHasVtxOffset);
            FlexUI::TreePop();
            FlexUI::Separator();
        }

        FlexUI_DEMO_MARKER("Configuration/Style");
        if (FlexUI::TreeNode("Style"))
        {
            HelpMarker("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function.");
            FlexUI::ShowStyleEditor();
            FlexUI::TreePop();
            FlexUI::Separator();
        }

        FlexUI_DEMO_MARKER("Configuration/Capture, Logging");
        if (FlexUI::TreeNode("Capture/Logging"))
        {
            HelpMarker(
                "The logging API redirects all text output so you can easily capture the content of "
                "a window or a block. Tree nodes can be automatically expanded.\n"
                "Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
            FlexUI::LogButtons();

            HelpMarker("You can also call FlexUI::LogText() to output directly to the log without a visual output.");
            if (FlexUI::Button("Copy \"Hello, world!\" to clipboard"))
            {
                FlexUI::LogToClipboard();
                FlexUI::LogText("Hello, world!");
                FlexUI::LogFinish();
            }
            FlexUI::TreePop();
        }
    }

    FlexUI_DEMO_MARKER("Window options");
    if (FlexUI::CollapsingHeader("Window options"))
    {
        if (FlexUI::BeginTable("split", 3))
        {
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No titlebar", &no_titlebar);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No scrollbar", &no_scrollbar);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No menu", &no_menu);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No move", &no_move);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No resize", &no_resize);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No collapse", &no_collapse);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No close", &no_close);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No nav", &no_nav);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No background", &no_background);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("No bring to front", &no_bring_to_front);
            FlexUI::TableNextColumn(); FlexUI::Checkbox("Unsaved document", &unsaved_document);
            FlexUI::EndTable();
        }
    }

    // All demo contents
    ShowDemoWindowWidgets();
    ShowDemoWindowLayout();
    ShowDemoWindowPopups();
    ShowDemoWindowTables();
    ShowDemoWindowInputs();

    // End of ShowDemoWindow()
    FlexUI::PopItemWidth();
    FlexUI::End();
}

static void ShowDemoWindowWidgets()
{
    FlexUI_DEMO_MARKER("Widgets");
    if (!FlexUI::CollapsingHeader("Widgets"))
        return;

    static bool disable_all = false; // The Checkbox for that is inside the "Disabled" section at the bottom
    if (disable_all)
        FlexUI::BeginDisabled();

    FlexUI_DEMO_MARKER("Widgets/Basic");
    if (FlexUI::TreeNode("Basic"))
    {
        FlexUI_DEMO_MARKER("Widgets/Basic/Button");
        static int clicked = 0;
        if (FlexUI::Button("Button"))
            clicked++;
        if (clicked & 1)
        {
            FlexUI::SameLine();
            FlexUI::Text("Thanks for clicking me!");
        }

        FlexUI_DEMO_MARKER("Widgets/Basic/Checkbox");
        static bool check = true;
        FlexUI::Checkbox("checkbox", &check);

        FlexUI_DEMO_MARKER("Widgets/Basic/RadioButton");
        static int e = 0;
        FlexUI::RadioButton("radio a", &e, 0); FlexUI::SameLine();
        FlexUI::RadioButton("radio b", &e, 1); FlexUI::SameLine();
        FlexUI::RadioButton("radio c", &e, 2);

        // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
        FlexUI_DEMO_MARKER("Widgets/Basic/Buttons (Colored)");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0)
                FlexUI::SameLine();
            FlexUI::PushID(i);
            FlexUI::PushStyleColor(FlexUICol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
            FlexUI::PushStyleColor(FlexUICol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
            FlexUI::PushStyleColor(FlexUICol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
            FlexUI::Button("Click");
            FlexUI::PopStyleColor(3);
            FlexUI::PopID();
        }

        // Use AlignTextToFramePadding() to align text baseline to the baseline of framed widgets elements
        // (otherwise a Text+SameLine+Button sequence will have the text a little too high by default!)
        // See 'Demo->Layout->Text Baseline Alignment' for details.
        FlexUI::AlignTextToFramePadding();
        FlexUI::Text("Hold to repeat:");
        FlexUI::SameLine();

        // Arrow buttons with Repeater
        FlexUI_DEMO_MARKER("Widgets/Basic/Buttons (Repeating)");
        static int counter = 0;
        float spacing = FlexUI::GetStyle().ItemInnerSpacing.x;
        FlexUI::PushButtonRepeat(true);
        if (FlexUI::ArrowButton("##left", FlexUIDir_Left)) { counter--; }
        FlexUI::SameLine(0.0f, spacing);
        if (FlexUI::ArrowButton("##right", FlexUIDir_Right)) { counter++; }
        FlexUI::PopButtonRepeat();
        FlexUI::SameLine();
        FlexUI::Text("%d", counter);

        FlexUI::Separator();
        FlexUI::LabelText("label", "Value");

        {
            // Using the _simplified_ one-liner Combo() api here
            // See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
            FlexUI_DEMO_MARKER("Widgets/Basic/Combo");
            const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
            static int item_current = 0;
            FlexUI::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
            FlexUI::SameLine(); HelpMarker(
                "Using the simplified one-liner Combo API here.\nRefer to the \"Combo\" section below for an explanation of how to use the more flexible and general BeginCombo/EndCombo API.");
        }

        {
            // To wire InputText() with std::string or any other custom string type,
            // see the "Text Input > Resize Callback" section of this demo, and the misc/cpp/FlexUI_stdlib.h file.
            FlexUI_DEMO_MARKER("Widgets/Basic/InputText");
            static char str0[128] = "Hello, world!";
            FlexUI::InputText("input text", str0, IM_ARRAYSIZE(str0));
            FlexUI::SameLine(); HelpMarker(
                "USER:\n"
                "Hold SHIFT or use mouse to select text.\n"
                "CTRL+Left/Right to word jump.\n"
                "CTRL+A or Double-Click to select all.\n"
                "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
                "CTRL+Z,CTRL+Y undo/redo.\n"
                "ESCAPE to revert.\n\n"
                "PROGRAMMER:\n"
                "You can use the FlexUIInputTextFlags_CallbackResize facility if you need to wire InputText() "
                "to a dynamic string type. See misc/cpp/FlexUI_stdlib.h for an example (this is not demonstrated "
                "in FlexUI_demo.cpp).");

            static char str1[128] = "";
            FlexUI::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_ARRAYSIZE(str1));

            FlexUI_DEMO_MARKER("Widgets/Basic/InputInt, InputFloat");
            static int i0 = 123;
            FlexUI::InputInt("input int", &i0);

            static float f0 = 0.001f;
            FlexUI::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

            static double d0 = 999999.00000001;
            FlexUI::InputDouble("input double", &d0, 0.01f, 1.0f, "%.8f");

            static float f1 = 1.e10f;
            FlexUI::InputFloat("input scientific", &f1, 0.0f, 0.0f, "%e");
            FlexUI::SameLine(); HelpMarker(
                "You can input value using the scientific notation,\n"
                "  e.g. \"1e+8\" becomes \"100000000\".");

            static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
            FlexUI::InputFloat3("input float3", vec4a);
        }

        {
            FlexUI_DEMO_MARKER("Widgets/Basic/DragInt, DragFloat");
            static int i1 = 50, i2 = 42;
            FlexUI::DragInt("drag int", &i1, 1);
            FlexUI::SameLine(); HelpMarker(
                "Click and drag to edit value.\n"
                "Hold SHIFT/ALT for faster/slower edit.\n"
                "Double-click or CTRL+click to input value.");

            FlexUI::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%", FlexUISliderFlags_AlwaysClamp);

            static float f1 = 1.00f, f2 = 0.0067f;
            FlexUI::DragFloat("drag float", &f1, 0.005f);
            FlexUI::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
        }

        {
            FlexUI_DEMO_MARKER("Widgets/Basic/SliderInt, SliderFloat");
            static int i1 = 0;
            FlexUI::SliderInt("slider int", &i1, -1, 3);
            FlexUI::SameLine(); HelpMarker("CTRL+click to input value.");

            static float f1 = 0.123f, f2 = 0.0f;
            FlexUI::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            FlexUI::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", FlexUISliderFlags_Logarithmic);

            FlexUI_DEMO_MARKER("Widgets/Basic/SliderAngle");
            static float angle = 0.0f;
            FlexUI::SliderAngle("slider angle", &angle);

            // Using the format string to display a name instead of an integer.
            // Here we completely omit '%d' from the format string, so it'll only display a name.
            // This technique can also be used with DragInt().
            FlexUI_DEMO_MARKER("Widgets/Basic/Slider (enum)");
            enum Element { Element_Fire, Element_Earth, Element_Air, Element_Water, Element_COUNT };
            static int elem = Element_Fire;
            const char* elems_names[Element_COUNT] = { "Fire", "Earth", "Air", "Water" };
            const char* elem_name = (elem >= 0 && elem < Element_COUNT) ? elems_names[elem] : "Unknown";
            FlexUI::SliderInt("slider enum", &elem, 0, Element_COUNT - 1, elem_name);
            FlexUI::SameLine(); HelpMarker("Using the format string parameter to display a name instead of the underlying integer.");
        }

        {
            FlexUI_DEMO_MARKER("Widgets/Basic/ColorEdit3, ColorEdit4");
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
            FlexUI::ColorEdit3("color 1", col1);
            FlexUI::SameLine(); HelpMarker(
                "Click on the color square to open a color picker.\n"
                "Click and hold to use drag and drop.\n"
                "Right-click on the color square to show options.\n"
                "CTRL+click on individual component to input value.\n");

            FlexUI::ColorEdit4("color 2", col2);
        }

        {
            // Using the _simplified_ one-liner ListBox() api here
            // See "List boxes" section for examples of how to use the more flexible BeginListBox()/EndListBox() api.
            FlexUI_DEMO_MARKER("Widgets/Basic/ListBox");
            const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
            static int item_current = 1;
            FlexUI::ListBox("listbox", &item_current, items, IM_ARRAYSIZE(items), 4);
            FlexUI::SameLine(); HelpMarker(
                "Using the simplified one-liner ListBox API here.\nRefer to the \"List boxes\" section below for an explanation of how to use the more flexible and general BeginListBox/EndListBox API.");
        }

        {
            // Tooltips
            FlexUI_DEMO_MARKER("Widgets/Basic/Tooltips");
            FlexUI::AlignTextToFramePadding();
            FlexUI::Text("Tooltips:");

            FlexUI::SameLine();
            FlexUI::Button("Button");
            if (FlexUI::IsItemHovered())
                FlexUI::SetTooltip("I am a tooltip");

            FlexUI::SameLine();
            FlexUI::Button("Fancy");
            if (FlexUI::IsItemHovered())
            {
                FlexUI::BeginTooltip();
                FlexUI::Text("I am a fancy tooltip");
                static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
                FlexUI::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
                FlexUI::Text("Sin(time) = %f", sinf((float)FlexUI::GetTime()));
                FlexUI::EndTooltip();
            }

            FlexUI::SameLine();
            FlexUI::Button("Delayed");
            if (FlexUI::IsItemHovered(FlexUIHoveredFlags_DelayNormal)) // Delay best used on items that highlight on hover, so this not a great example!
                FlexUI::SetTooltip("I am a tooltip with a delay.");

            FlexUI::SameLine();
            HelpMarker(
                "Tooltip are created by using the IsItemHovered() function over any kind of item.");

        }

        FlexUI::TreePop();
    }

    // Testing FlexUIOnceUponAFrame helper.
    //static FlexUIOnceUponAFrame once;
    //for (int i = 0; i < 5; i++)
    //    if (once)
    //        FlexUI::Text("This will be displayed only once.");

    FlexUI_DEMO_MARKER("Widgets/Trees");
    if (FlexUI::TreeNode("Trees"))
    {
        FlexUI_DEMO_MARKER("Widgets/Trees/Basic trees");
        if (FlexUI::TreeNode("Basic trees"))
        {
            for (int i = 0; i < 5; i++)
            {
                // Use SetNextItemOpen() so set the default state of a node to be open. We could
                // also use TreeNodeEx() with the FlexUITreeNodeFlags_DefaultOpen flag to achieve the same thing!
                if (i == 0)
                    FlexUI::SetNextItemOpen(true, FlexUICond_Once);

                if (FlexUI::TreeNode((void*)(intptr_t)i, "Child %d", i))
                {
                    FlexUI::Text("blah blah");
                    FlexUI::SameLine();
                    if (FlexUI::SmallButton("button")) {}
                    FlexUI::TreePop();
                }
            }
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Trees/Advanced, with Selectable nodes");
        if (FlexUI::TreeNode("Advanced, with Selectable nodes"))
        {
            HelpMarker(
                "This is a more typical looking tree with selectable nodes.\n"
                "Click to select, CTRL+Click to toggle, click on arrows or double-click to open.");
            static FlexUITreeNodeFlags base_flags = FlexUITreeNodeFlags_OpenOnArrow | FlexUITreeNodeFlags_OpenOnDoubleClick | FlexUITreeNodeFlags_SpanAvailWidth;
            static bool align_label_with_current_x_position = false;
            static bool test_drag_and_drop = false;
            FlexUI::CheckboxFlags("FlexUITreeNodeFlags_OpenOnArrow", &base_flags, FlexUITreeNodeFlags_OpenOnArrow);
            FlexUI::CheckboxFlags("FlexUITreeNodeFlags_OpenOnDoubleClick", &base_flags, FlexUITreeNodeFlags_OpenOnDoubleClick);
            FlexUI::CheckboxFlags("FlexUITreeNodeFlags_SpanAvailWidth", &base_flags, FlexUITreeNodeFlags_SpanAvailWidth); FlexUI::SameLine(); HelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node.");
            FlexUI::CheckboxFlags("FlexUITreeNodeFlags_SpanFullWidth", &base_flags, FlexUITreeNodeFlags_SpanFullWidth);
            FlexUI::Checkbox("Align label with current X position", &align_label_with_current_x_position);
            FlexUI::Checkbox("Test tree node as drag source", &test_drag_and_drop);
            FlexUI::Text("Hello!");
            if (align_label_with_current_x_position)
                FlexUI::Unindent(FlexUI::GetTreeNodeToLabelSpacing());

            // 'selection_mask' is dumb representation of what may be user-side selection state.
            //  You may retain selection state inside or outside your objects in whatever format you see fit.
            // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
            /// of the loop. May be a pointer to your own node type, etc.
            static int selection_mask = (1 << 2);
            int node_clicked = -1;
            for (int i = 0; i < 6; i++)
            {
                // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
                // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
                FlexUITreeNodeFlags node_flags = base_flags;
                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= FlexUITreeNodeFlags_Selected;
                if (i < 3)
                {
                    // Items 0..2 are Tree Node
                    bool node_open = FlexUI::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
                    if (FlexUI::IsItemClicked() && !FlexUI::IsItemToggledOpen())
                        node_clicked = i;
                    if (test_drag_and_drop && FlexUI::BeginDragDropSource())
                    {
                        FlexUI::SetDragDropPayload("_TREENODE", NULL, 0);
                        FlexUI::Text("This is a drag and drop source");
                        FlexUI::EndDragDropSource();
                    }
                    if (node_open)
                    {
                        FlexUI::BulletText("Blah blah\nBlah Blah");
                        FlexUI::TreePop();
                    }
                }
                else
                {
                    // Items 3..5 are Tree Leaves
                    // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
                    // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                    node_flags |= FlexUITreeNodeFlags_Leaf | FlexUITreeNodeFlags_NoTreePushOnOpen; // FlexUITreeNodeFlags_Bullet
                    FlexUI::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
                    if (FlexUI::IsItemClicked() && !FlexUI::IsItemToggledOpen())
                        node_clicked = i;
                    if (test_drag_and_drop && FlexUI::BeginDragDropSource())
                    {
                        FlexUI::SetDragDropPayload("_TREENODE", NULL, 0);
                        FlexUI::Text("This is a drag and drop source");
                        FlexUI::EndDragDropSource();
                    }
                }
            }
            if (node_clicked != -1)
            {
                // Update selection state
                // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
                if (FlexUI::GetIO().KeyCtrl)
                    selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
                else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
                    selection_mask = (1 << node_clicked);           // Click to single-select
            }
            if (align_label_with_current_x_position)
                FlexUI::Indent(FlexUI::GetTreeNodeToLabelSpacing());
            FlexUI::TreePop();
        }
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Collapsing Headers");
    if (FlexUI::TreeNode("Collapsing Headers"))
    {
        static bool closable_group = true;
        FlexUI::Checkbox("Show 2nd header", &closable_group);
        if (FlexUI::CollapsingHeader("Header", FlexUITreeNodeFlags_None))
        {
            FlexUI::Text("IsItemHovered: %d", FlexUI::IsItemHovered());
            for (int i = 0; i < 5; i++)
                FlexUI::Text("Some content %d", i);
        }
        if (FlexUI::CollapsingHeader("Header with a close button", &closable_group))
        {
            FlexUI::Text("IsItemHovered: %d", FlexUI::IsItemHovered());
            for (int i = 0; i < 5; i++)
                FlexUI::Text("More content %d", i);
        }
        /*
        if (FlexUI::CollapsingHeader("Header with a bullet", FlexUITreeNodeFlags_Bullet))
            FlexUI::Text("IsItemHovered: %d", FlexUI::IsItemHovered());
        */
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Bullets");
    if (FlexUI::TreeNode("Bullets"))
    {
        FlexUI::BulletText("Bullet point 1");
        FlexUI::BulletText("Bullet point 2\nOn multiple lines");
        if (FlexUI::TreeNode("Tree node"))
        {
            FlexUI::BulletText("Another bullet point");
            FlexUI::TreePop();
        }
        FlexUI::Bullet(); FlexUI::Text("Bullet point 3 (two calls)");
        FlexUI::Bullet(); FlexUI::SmallButton("Button");
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Text");
    if (FlexUI::TreeNode("Text"))
    {
        FlexUI_DEMO_MARKER("Widgets/Text/Colored Text");
        if (FlexUI::TreeNode("Colorful Text"))
        {
            // Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
            FlexUI::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Pink");
            FlexUI::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
            FlexUI::TextDisabled("Disabled");
            FlexUI::SameLine(); HelpMarker("The TextDisabled color is stored in FlexUIStyle.");
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Text/Word Wrapping");
        if (FlexUI::TreeNode("Word Wrapping"))
        {
            // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
            FlexUI::TextWrapped(
                "This text should automatically wrap on the edge of the window. The current implementation "
                "for text wrapping follows simple rules suitable for English and possibly other languages.");
            FlexUI::Spacing();

            static float wrap_width = 200.0f;
            FlexUI::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

            ImDrawList* draw_list = FlexUI::GetWindowDrawList();
            for (int n = 0; n < 2; n++)
            {
                FlexUI::Text("Test paragraph %d:", n);
                ImVec2 pos = FlexUI::GetCursorScreenPos();
                ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
                ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + FlexUI::GetTextLineHeight());
                FlexUI::PushTextWrapPos(FlexUI::GetCursorPos().x + wrap_width);
                if (n == 0)
                    FlexUI::Text("The lazy dog is a good dog. This paragraph should fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
                else
                    FlexUI::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");

                // Draw actual text bounding box, following by marker of our expected limit (should not overlap!)
                draw_list->AddRect(FlexUI::GetItemRectMin(), FlexUI::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
                draw_list->AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
                FlexUI::PopTextWrapPos();
            }

            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Text/UTF-8 Text");
        if (FlexUI::TreeNode("UTF-8 Text"))
        {
            // UTF-8 test with Japanese characters
            // (Needs a suitable font? Try "Google Noto" or "Arial Unicode". See docs/FONTS.md for details.)
            // - From C++11 you can use the u8"my text" syntax to encode literal strings as UTF-8
            // - For earlier compiler, you may be able to encode your sources as UTF-8 (e.g. in Visual Studio, you
            //   can save your source files as 'UTF-8 without signature').
            // - FOR THIS DEMO FILE ONLY, BECAUSE WE WANT TO SUPPORT OLD COMPILERS, WE ARE *NOT* INCLUDING RAW UTF-8
            //   CHARACTERS IN THIS SOURCE FILE. Instead we are encoding a few strings with hexadecimal constants.
            //   Don't do this in your application! Please use u8"text in any language" in your application!
            // Note that characters values are preserved even by InputText() if the font cannot be displayed,
            // so you can safely copy & paste garbled characters into another application.
            FlexUI::TextWrapped(
                "CJK text will only appear if the font was loaded with the appropriate CJK character ranges. "
                "Call io.Fonts->AddFontFromFileTTF() manually to load extra character ranges. "
                "Read docs/FONTS.md for details.");
            FlexUI::Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)"); // Normally we would use u8"blah blah" with the proper characters directly in the string.
            FlexUI::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
            static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
            //static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
            FlexUI::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
            FlexUI::TreePop();
        }
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Images");
    if (FlexUI::TreeNode("Images"))
    {
        FlexUIIO& io = FlexUI::GetIO();
        FlexUI::TextWrapped(
            "Below we are displaying the font texture (which is the only texture we have access to in this demo). "
            "Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. "
            "Hover the texture for a zoomed view!");

        // Below we are displaying the font texture because it is the only texture we have access to inside the demo!
        // Remember that ImTextureID is just storage for whatever you want it to be. It is essentially a value that
        // will be passed to the rendering backend via the ImDrawCmd structure.
        // If you use one of the default FlexUI_impl_XXXX.cpp rendering backend, they all have comments at the top
        // of their respective source file to specify what they expect to be stored in ImTextureID, for example:
        // - The FlexUI_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer
        // - The FlexUI_impl_opengl3.cpp renderer expect a GLuint OpenGL texture identifier, etc.
        // More:
        // - If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers
        //   to FlexUI::Image(), and gather width/height through your own functions, etc.
        // - You can use ShowMetricsWindow() to inspect the draw data that are being passed to your renderer,
        //   it will help you debug issues if you are confused about it.
        // - Consider using the lower-level ImDrawList::AddImage() API, via FlexUI::GetWindowDrawList()->AddImage().
        // - Read https://github.com/ocornut/FlexUI/blob/master/docs/FAQ.md
        // - Read https://github.com/ocornut/FlexUI/wiki/Image-Loading-and-Displaying-Examples
        ImTextureID my_tex_id = io.Fonts->TexID;
        float my_tex_w = (float)io.Fonts->TexWidth;
        float my_tex_h = (float)io.Fonts->TexHeight;
        {
            FlexUI::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            ImVec2 pos = FlexUI::GetCursorScreenPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            FlexUI::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            if (FlexUI::IsItemHovered())
            {
                FlexUI::BeginTooltip();
                float region_sz = 32.0f;
                float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                float zoom = 4.0f;
                if (region_x < 0.0f) { region_x = 0.0f; }
                else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                if (region_y < 0.0f) { region_y = 0.0f; }
                else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                FlexUI::Text("Min: (%.2f, %.2f)", region_x, region_y);
                FlexUI::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                FlexUI::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                FlexUI::EndTooltip();
            }
        }

        FlexUI_DEMO_MARKER("Widgets/Images/Textured buttons");
        FlexUI::TextWrapped("And now some textured buttons..");
        static int pressed_count = 0;
        for (int i = 0; i < 8; i++)
        {
            // UV coordinates are often (0.0f, 0.0f) and (1.0f, 1.0f) to display an entire textures.
            // Here are trying to display only a 32x32 pixels area of the texture, hence the UV computation.
            // Read about UV coordinates here: https://github.com/ocornut/FlexUI/wiki/Image-Loading-and-Displaying-Examples
            FlexUI::PushID(i);
            if (i > 0)
                FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(i - 1.0f, i - 1.0f));
            ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);    // UV coordinates for (32,32) in our texture
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            if (FlexUI::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
                pressed_count += 1;
            if (i > 0)
                FlexUI::PopStyleVar();
            FlexUI::PopID();
            FlexUI::SameLine();
        }
        FlexUI::NewLine();
        FlexUI::Text("Pressed %d times.", pressed_count);
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Combo");
    if (FlexUI::TreeNode("Combo"))
    {
        // Combo Boxes are also called "Dropdown" in other systems
        // Expose flags as checkbox for the demo
        static FlexUIComboFlags flags = 0;
        FlexUI::CheckboxFlags("FlexUIComboFlags_PopupAlignLeft", &flags, FlexUIComboFlags_PopupAlignLeft);
        FlexUI::SameLine(); HelpMarker("Only makes a difference if the popup is larger than the combo");
        if (FlexUI::CheckboxFlags("FlexUIComboFlags_NoArrowButton", &flags, FlexUIComboFlags_NoArrowButton))
            flags &= ~FlexUIComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
        if (FlexUI::CheckboxFlags("FlexUIComboFlags_NoPreview", &flags, FlexUIComboFlags_NoPreview))
            flags &= ~FlexUIComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

        // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static int item_current_idx = 0; // Here we store our selection data as an index.
        const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
        if (FlexUI::BeginCombo("combo 1", combo_preview_value, flags))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (FlexUI::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    FlexUI::SetItemDefaultFocus();
            }
            FlexUI::EndCombo();
        }

        // Simplified one-liner Combo() API, using values packed in a single constant string
        // This is a convenience for when the selection set is small and known at compile-time.
        static int item_current_2 = 0;
        FlexUI::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

        // Simplified one-liner Combo() using an array of const char*
        // This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
        static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
        FlexUI::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

        // Simplified one-liner Combo() using an accessor function
        struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
        static int item_current_4 = 0;
        FlexUI::Combo("combo 4 (function)", &item_current_4, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/List Boxes");
    if (FlexUI::TreeNode("List boxes"))
    {
        // Using the generic BeginListBox() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static int item_current_idx = 0; // Here we store our selection data as an index.
        if (FlexUI::BeginListBox("listbox 1"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (FlexUI::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    FlexUI::SetItemDefaultFocus();
            }
            FlexUI::EndListBox();
        }

        // Custom size: use all width, 5 items tall
        FlexUI::Text("Full-width:");
        if (FlexUI::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * FlexUI::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (FlexUI::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    FlexUI::SetItemDefaultFocus();
            }
            FlexUI::EndListBox();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Selectables");
    if (FlexUI::TreeNode("Selectables"))
    {
        // Selectable() has 2 overloads:
        // - The one taking "bool selected" as a read-only selection information.
        //   When Selectable() has been clicked it returns true and you can alter selection state accordingly.
        // - The one taking "bool* p_selected" as a read-write selection information (convenient in some cases)
        // The earlier is more flexible, as in real application your selection may be stored in many different ways
        // and not necessarily inside a bool value (e.g. in flags within objects, as an external list, etc).
        FlexUI_DEMO_MARKER("Widgets/Selectables/Basic");
        if (FlexUI::TreeNode("Basic"))
        {
            static bool selection[5] = { false, true, false, false, false };
            FlexUI::Selectable("1. I am selectable", &selection[0]);
            FlexUI::Selectable("2. I am selectable", &selection[1]);
            FlexUI::Text("(I am not selectable)");
            FlexUI::Selectable("4. I am selectable", &selection[3]);
            if (FlexUI::Selectable("5. I am double clickable", selection[4], FlexUISelectableFlags_AllowDoubleClick))
                if (FlexUI::IsMouseDoubleClicked(0))
                    selection[4] = !selection[4];
            FlexUI::TreePop();
        }
        FlexUI_DEMO_MARKER("Widgets/Selectables/Single Selection");
        if (FlexUI::TreeNode("Selection State: Single Selection"))
        {
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (FlexUI::Selectable(buf, selected == n))
                    selected = n;
            }
            FlexUI::TreePop();
        }
        FlexUI_DEMO_MARKER("Widgets/Selectables/Multiple Selection");
        if (FlexUI::TreeNode("Selection State: Multiple Selection"))
        {
            HelpMarker("Hold CTRL and click to select multiple items.");
            static bool selection[5] = { false, false, false, false, false };
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (FlexUI::Selectable(buf, selection[n]))
                {
                    if (!FlexUI::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
                        memset(selection, 0, sizeof(selection));
                    selection[n] ^= 1;
                }
            }
            FlexUI::TreePop();
        }
        FlexUI_DEMO_MARKER("Widgets/Selectables/Rendering more text into the same line");
        if (FlexUI::TreeNode("Rendering more text into the same line"))
        {
            // Using the Selectable() override that takes "bool* p_selected" parameter,
            // this function toggle your bool value automatically.
            static bool selected[3] = { false, false, false };
            FlexUI::Selectable("main.c", &selected[0]); FlexUI::SameLine(300); FlexUI::Text(" 2,345 bytes");
            FlexUI::Selectable("Hello.cpp", &selected[1]); FlexUI::SameLine(300); FlexUI::Text("12,345 bytes");
            FlexUI::Selectable("Hello.h", &selected[2]); FlexUI::SameLine(300); FlexUI::Text(" 2,345 bytes");
            FlexUI::TreePop();
        }
        FlexUI_DEMO_MARKER("Widgets/Selectables/In columns");
        if (FlexUI::TreeNode("In columns"))
        {
            static bool selected[10] = {};

            if (FlexUI::BeginTable("split1", 3, FlexUITableFlags_Resizable | FlexUITableFlags_NoSavedSettings | FlexUITableFlags_Borders))
            {
                for (int i = 0; i < 10; i++)
                {
                    char label[32];
                    sprintf(label, "Item %d", i);
                    FlexUI::TableNextColumn();
                    FlexUI::Selectable(label, &selected[i]); // FIXME-TABLE: Selection overlap
                }
                FlexUI::EndTable();
            }
            FlexUI::Spacing();
            if (FlexUI::BeginTable("split2", 3, FlexUITableFlags_Resizable | FlexUITableFlags_NoSavedSettings | FlexUITableFlags_Borders))
            {
                for (int i = 0; i < 10; i++)
                {
                    char label[32];
                    sprintf(label, "Item %d", i);
                    FlexUI::TableNextRow();
                    FlexUI::TableNextColumn();
                    FlexUI::Selectable(label, &selected[i], FlexUISelectableFlags_SpanAllColumns);
                    FlexUI::TableNextColumn();
                    FlexUI::Text("Some other contents");
                    FlexUI::TableNextColumn();
                    FlexUI::Text("123456");
                }
                FlexUI::EndTable();
            }
            FlexUI::TreePop();
        }
        FlexUI_DEMO_MARKER("Widgets/Selectables/Grid");
        if (FlexUI::TreeNode("Grid"))
        {
            static char selected[4][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };

            // Add in a bit of silly fun...
            const float time = (float)FlexUI::GetTime();
            const bool winning_state = memchr(selected, 0, sizeof(selected)) == NULL; // If all cells are selected...
            if (winning_state)
                FlexUI::PushStyleVar(FlexUIStyleVar_SelectableTextAlign, ImVec2(0.5f + 0.5f * cosf(time * 2.0f), 0.5f + 0.5f * sinf(time * 3.0f)));

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                {
                    if (x > 0)
                        FlexUI::SameLine();
                    FlexUI::PushID(y * 4 + x);
                    if (FlexUI::Selectable("Sailor", selected[y][x] != 0, 0, ImVec2(50, 50)))
                    {
                        // Toggle clicked cell + toggle neighbors
                        selected[y][x] ^= 1;
                        if (x > 0) { selected[y][x - 1] ^= 1; }
                        if (x < 3) { selected[y][x + 1] ^= 1; }
                        if (y > 0) { selected[y - 1][x] ^= 1; }
                        if (y < 3) { selected[y + 1][x] ^= 1; }
                    }
                    FlexUI::PopID();
                }

            if (winning_state)
                FlexUI::PopStyleVar();
            FlexUI::TreePop();
        }
        FlexUI_DEMO_MARKER("Widgets/Selectables/Alignment");
        if (FlexUI::TreeNode("Alignment"))
        {
            HelpMarker(
                "By default, Selectables uses style.SelectableTextAlign but it can be overridden on a per-item "
                "basis using PushStyleVar(). You'll probably want to always keep your default situation to "
                "left-align otherwise it becomes difficult to layout multiple items on a same line");
            static bool selected[3 * 3] = { true, false, true, false, true, false, true, false, true };
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    ImVec2 alignment = ImVec2((float)x / 2.0f, (float)y / 2.0f);
                    char name[32];
                    sprintf(name, "(%.1f,%.1f)", alignment.x, alignment.y);
                    if (x > 0) FlexUI::SameLine();
                    FlexUI::PushStyleVar(FlexUIStyleVar_SelectableTextAlign, alignment);
                    FlexUI::Selectable(name, &selected[3 * y + x], FlexUISelectableFlags_None, ImVec2(80, 80));
                    FlexUI::PopStyleVar();
                }
            }
            FlexUI::TreePop();
        }
        FlexUI::TreePop();
    }

    // To wire InputText() with std::string or any other custom string type,
    // see the "Text Input > Resize Callback" section of this demo, and the misc/cpp/FlexUI_stdlib.h file.
    FlexUI_DEMO_MARKER("Widgets/Text Input");
    if (FlexUI::TreeNode("Text Input"))
    {
        FlexUI_DEMO_MARKER("Widgets/Text Input/Multi-line Text Input");
        if (FlexUI::TreeNode("Multi-line Text Input"))
        {
            // Note: we are using a fixed-sized buffer for simplicity here. See FlexUIInputTextFlags_CallbackResize
            // and the code in misc/cpp/FlexUI_stdlib.h for how to setup InputText() for dynamically resizing strings.
            static char text[1024 * 16] =
                "/*\n"
                " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                " the hexadecimal encoding of one offending instruction,\n"
                " more formally, the invalid operand with locked CMPXCHG8B\n"
                " instruction bug, is a design flaw in the majority of\n"
                " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                " processors (all in the P5 microarchitecture).\n"
                "*/\n\n"
                "label:\n"
                "\tlock cmpxchg8b eax\n";

            static FlexUIInputTextFlags flags = FlexUIInputTextFlags_AllowTabInput;
            HelpMarker("You can use the FlexUIInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/FlexUI_stdlib.h for an example. (This is not demonstrated in FlexUI_demo.cpp because we don't want to include <string> in here)");
            FlexUI::CheckboxFlags("FlexUIInputTextFlags_ReadOnly", &flags, FlexUIInputTextFlags_ReadOnly);
            FlexUI::CheckboxFlags("FlexUIInputTextFlags_AllowTabInput", &flags, FlexUIInputTextFlags_AllowTabInput);
            FlexUI::CheckboxFlags("FlexUIInputTextFlags_CtrlEnterForNewLine", &flags, FlexUIInputTextFlags_CtrlEnterForNewLine);
            FlexUI::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, FlexUI::GetTextLineHeight() * 16), flags);
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Text Input/Filtered Text Input");
        if (FlexUI::TreeNode("Filtered Text Input"))
        {
            struct TextFilters
            {
                // Return 0 (pass) if the character is 'i' or 'm' or 'g' or 'u' or 'i'
                static int FilterFlexUILetters(FlexUIInputTextCallbackData* data)
                {
                    if (data->EventChar < 256 && strchr("FlexUI", (char)data->EventChar))
                        return 0;
                    return 1;
                }
            };

            static char buf1[64] = ""; FlexUI::InputText("default", buf1, 64);
            static char buf2[64] = ""; FlexUI::InputText("decimal", buf2, 64, FlexUIInputTextFlags_CharsDecimal);
            static char buf3[64] = ""; FlexUI::InputText("hexadecimal", buf3, 64, FlexUIInputTextFlags_CharsHexadecimal | FlexUIInputTextFlags_CharsUppercase);
            static char buf4[64] = ""; FlexUI::InputText("uppercase", buf4, 64, FlexUIInputTextFlags_CharsUppercase);
            static char buf5[64] = ""; FlexUI::InputText("no blank", buf5, 64, FlexUIInputTextFlags_CharsNoBlank);
            static char buf6[64] = ""; FlexUI::InputText("\"FlexUI\" letters", buf6, 64, FlexUIInputTextFlags_CallbackCharFilter, TextFilters::FilterFlexUILetters);
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Text Input/Password input");
        if (FlexUI::TreeNode("Password Input"))
        {
            static char password[64] = "password123";
            FlexUI::InputText("password", password, IM_ARRAYSIZE(password), FlexUIInputTextFlags_Password);
            FlexUI::SameLine(); HelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
            FlexUI::InputTextWithHint("password (w/ hint)", "<password>", password, IM_ARRAYSIZE(password), FlexUIInputTextFlags_Password);
            FlexUI::InputText("password (clear)", password, IM_ARRAYSIZE(password));
            FlexUI::TreePop();
        }

        if (FlexUI::TreeNode("Completion, History, Edit Callbacks"))
        {
            struct Funcs
            {
                static int MyCallback(FlexUIInputTextCallbackData* data)
                {
                    if (data->EventFlag == FlexUIInputTextFlags_CallbackCompletion)
                    {
                        data->InsertChars(data->CursorPos, "..");
                    }
                    else if (data->EventFlag == FlexUIInputTextFlags_CallbackHistory)
                    {
                        if (data->EventKey == FlexUIKey_UpArrow)
                        {
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, "Pressed Up!");
                            data->SelectAll();
                        }
                        else if (data->EventKey == FlexUIKey_DownArrow)
                        {
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, "Pressed Down!");
                            data->SelectAll();
                        }
                    }
                    else if (data->EventFlag == FlexUIInputTextFlags_CallbackEdit)
                    {
                        // Toggle casing of first character
                        char c = data->Buf[0];
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) data->Buf[0] ^= 32;
                        data->BufDirty = true;

                        // Increment a counter
                        int* p_int = (int*)data->UserData;
                        *p_int = *p_int + 1;
                    }
                    return 0;
                }
            };
            static char buf1[64];
            FlexUI::InputText("Completion", buf1, 64, FlexUIInputTextFlags_CallbackCompletion, Funcs::MyCallback);
            FlexUI::SameLine(); HelpMarker("Here we append \"..\" each time Tab is pressed. See 'Examples>Console' for a more meaningful demonstration of using this callback.");

            static char buf2[64];
            FlexUI::InputText("History", buf2, 64, FlexUIInputTextFlags_CallbackHistory, Funcs::MyCallback);
            FlexUI::SameLine(); HelpMarker("Here we replace and select text each time Up/Down are pressed. See 'Examples>Console' for a more meaningful demonstration of using this callback.");

            static char buf3[64];
            static int edit_count = 0;
            FlexUI::InputText("Edit", buf3, 64, FlexUIInputTextFlags_CallbackEdit, Funcs::MyCallback, (void*)&edit_count);
            FlexUI::SameLine(); HelpMarker("Here we toggle the casing of the first character on every edit + count edits.");
            FlexUI::SameLine(); FlexUI::Text("(%d)", edit_count);

            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Text Input/Resize Callback");
        if (FlexUI::TreeNode("Resize Callback"))
        {
            // To wire InputText() with std::string or any other custom string type,
            // you can use the FlexUIInputTextFlags_CallbackResize flag + create a custom FlexUI::InputText() wrapper
            // using your preferred type. See misc/cpp/FlexUI_stdlib.h for an implementation of this using std::string.
            HelpMarker(
                "Using FlexUIInputTextFlags_CallbackResize to wire your custom string type to InputText().\n\n"
                "See misc/cpp/FlexUI_stdlib.h for an implementation of this for std::string.");
            struct Funcs
            {
                static int MyResizeCallback(FlexUIInputTextCallbackData* data)
                {
                    if (data->EventFlag == FlexUIInputTextFlags_CallbackResize)
                    {
                        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                        IM_ASSERT(my_str->begin() == data->Buf);
                        my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                        data->Buf = my_str->begin();
                    }
                    return 0;
                }

                // Note: Because FlexUI:: is a namespace you would typically add your own function into the namespace.
                // For example, you code may declare a function 'FlexUI::InputText(const char* label, MyString* my_str)'
                static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), FlexUIInputTextFlags flags = 0)
                {
                    IM_ASSERT((flags & FlexUIInputTextFlags_CallbackResize) == 0);
                    return FlexUI::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | FlexUIInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
                }
            };

            // For this demo we are using ImVector as a string container.
            // Note that because we need to store a terminating zero character, our size/capacity are 1 more
            // than usually reported by a typical string class.
            static ImVector<char> my_str;
            if (my_str.empty())
                my_str.push_back(0);
            Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, FlexUI::GetTextLineHeight() * 16));
            FlexUI::Text("Data: %p\nSize: %d\nCapacity: %d", (void*)my_str.begin(), my_str.size(), my_str.capacity());
            FlexUI::TreePop();
        }

        FlexUI::TreePop();
    }

    // Tabs
    FlexUI_DEMO_MARKER("Widgets/Tabs");
    if (FlexUI::TreeNode("Tabs"))
    {
        FlexUI_DEMO_MARKER("Widgets/Tabs/Basic");
        if (FlexUI::TreeNode("Basic"))
        {
            FlexUITabBarFlags tab_bar_flags = FlexUITabBarFlags_None;
            if (FlexUI::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                if (FlexUI::BeginTabItem("Avocado"))
                {
                    FlexUI::Text("This is the Avocado tab!\nblah blah blah blah blah");
                    FlexUI::EndTabItem();
                }
                if (FlexUI::BeginTabItem("Broccoli"))
                {
                    FlexUI::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                    FlexUI::EndTabItem();
                }
                if (FlexUI::BeginTabItem("Cucumber"))
                {
                    FlexUI::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    FlexUI::EndTabItem();
                }
                FlexUI::EndTabBar();
            }
            FlexUI::Separator();
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Tabs/Advanced & Close Button");
        if (FlexUI::TreeNode("Advanced & Close Button"))
        {
            // Expose a couple of the available flags. In most cases you may just call BeginTabBar() with no flags (0).
            static FlexUITabBarFlags tab_bar_flags = FlexUITabBarFlags_Reorderable;
            FlexUI::CheckboxFlags("FlexUITabBarFlags_Reorderable", &tab_bar_flags, FlexUITabBarFlags_Reorderable);
            FlexUI::CheckboxFlags("FlexUITabBarFlags_AutoSelectNewTabs", &tab_bar_flags, FlexUITabBarFlags_AutoSelectNewTabs);
            FlexUI::CheckboxFlags("FlexUITabBarFlags_TabListPopupButton", &tab_bar_flags, FlexUITabBarFlags_TabListPopupButton);
            FlexUI::CheckboxFlags("FlexUITabBarFlags_NoCloseWithMiddleMouseButton", &tab_bar_flags, FlexUITabBarFlags_NoCloseWithMiddleMouseButton);
            if ((tab_bar_flags & FlexUITabBarFlags_FittingPolicyMask_) == 0)
                tab_bar_flags |= FlexUITabBarFlags_FittingPolicyDefault_;
            if (FlexUI::CheckboxFlags("FlexUITabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, FlexUITabBarFlags_FittingPolicyResizeDown))
                tab_bar_flags &= ~(FlexUITabBarFlags_FittingPolicyMask_ ^ FlexUITabBarFlags_FittingPolicyResizeDown);
            if (FlexUI::CheckboxFlags("FlexUITabBarFlags_FittingPolicyScroll", &tab_bar_flags, FlexUITabBarFlags_FittingPolicyScroll))
                tab_bar_flags &= ~(FlexUITabBarFlags_FittingPolicyMask_ ^ FlexUITabBarFlags_FittingPolicyScroll);

            // Tab Bar
            const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
            static bool opened[4] = { true, true, true, true }; // Persistent user state
            for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            {
                if (n > 0) { FlexUI::SameLine(); }
                FlexUI::Checkbox(names[n], &opened[n]);
            }

            // Passing a bool* to BeginTabItem() is similar to passing one to Begin():
            // the underlying bool will be set to false when the tab is closed.
            if (FlexUI::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
                    if (opened[n] && FlexUI::BeginTabItem(names[n], &opened[n], FlexUITabItemFlags_None))
                    {
                        FlexUI::Text("This is the %s tab!", names[n]);
                        if (n & 1)
                            FlexUI::Text("I am an odd tab.");
                        FlexUI::EndTabItem();
                    }
                FlexUI::EndTabBar();
            }
            FlexUI::Separator();
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Tabs/TabItemButton & Leading-Trailing flags");
        if (FlexUI::TreeNode("TabItemButton & Leading/Trailing flags"))
        {
            static ImVector<int> active_tabs;
            static int next_tab_id = 0;
            if (next_tab_id == 0) // Initialize with some default tabs
                for (int i = 0; i < 3; i++)
                    active_tabs.push_back(next_tab_id++);

            // TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
            // (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
            // but they tend to make more sense together)
            static bool show_leading_button = true;
            static bool show_trailing_button = true;
            FlexUI::Checkbox("Show Leading TabItemButton()", &show_leading_button);
            FlexUI::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

            // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
            static FlexUITabBarFlags tab_bar_flags = FlexUITabBarFlags_AutoSelectNewTabs | FlexUITabBarFlags_Reorderable | FlexUITabBarFlags_FittingPolicyResizeDown;
            FlexUI::CheckboxFlags("FlexUITabBarFlags_TabListPopupButton", &tab_bar_flags, FlexUITabBarFlags_TabListPopupButton);
            if (FlexUI::CheckboxFlags("FlexUITabBarFlags_FittingPolicyResizeDown", &tab_bar_flags, FlexUITabBarFlags_FittingPolicyResizeDown))
                tab_bar_flags &= ~(FlexUITabBarFlags_FittingPolicyMask_ ^ FlexUITabBarFlags_FittingPolicyResizeDown);
            if (FlexUI::CheckboxFlags("FlexUITabBarFlags_FittingPolicyScroll", &tab_bar_flags, FlexUITabBarFlags_FittingPolicyScroll))
                tab_bar_flags &= ~(FlexUITabBarFlags_FittingPolicyMask_ ^ FlexUITabBarFlags_FittingPolicyScroll);

            if (FlexUI::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                // Demo a Leading TabItemButton(): click the "?" button to open a menu
                if (show_leading_button)
                    if (FlexUI::TabItemButton("?", FlexUITabItemFlags_Leading | FlexUITabItemFlags_NoTooltip))
                        FlexUI::OpenPopup("MyHelpMenu");
                if (FlexUI::BeginPopup("MyHelpMenu"))
                {
                    FlexUI::Selectable("Hello!");
                    FlexUI::EndPopup();
                }

                // Demo Trailing Tabs: click the "+" button to add a new tab (in your app you may want to use a font icon instead of the "+")
                // Note that we submit it before the regular tabs, but because of the FlexUITabItemFlags_Trailing flag it will always appear at the end.
                if (show_trailing_button)
                    if (FlexUI::TabItemButton("+", FlexUITabItemFlags_Trailing | FlexUITabItemFlags_NoTooltip))
                        active_tabs.push_back(next_tab_id++); // Add new tab

                // Submit our regular tabs
                for (int n = 0; n < active_tabs.Size; )
                {
                    bool open = true;
                    char name[16];
                    snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
                    if (FlexUI::BeginTabItem(name, &open, FlexUITabItemFlags_None))
                    {
                        FlexUI::Text("This is the %s tab!", name);
                        FlexUI::EndTabItem();
                    }

                    if (!open)
                        active_tabs.erase(active_tabs.Data + n);
                    else
                        n++;
                }

                FlexUI::EndTabBar();
            }
            FlexUI::Separator();
            FlexUI::TreePop();
        }
        FlexUI::TreePop();
    }

    // Plot/Graph widgets are not very good.
    // Consider using a third-party library such as ImPlot: https://github.com/epezent/implot
    // (see others https://github.com/ocornut/FlexUI/wiki/Useful-Extensions)
    FlexUI_DEMO_MARKER("Widgets/Plotting");
    if (FlexUI::TreeNode("Plotting"))
    {
        static bool animate = true;
        FlexUI::Checkbox("Animate", &animate);

        // Plot as lines and plot as histogram
        FlexUI_DEMO_MARKER("Widgets/Plotting/PlotLines, PlotHistogram");
        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        FlexUI::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));
        FlexUI::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));

        // Fill an array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
        // and the sizeof() of your structure in the "stride" parameter.
        static float values[90] = {};
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = FlexUI::GetTime();
        while (refresh_time < FlexUI::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 60.0f;
        }

        // Plots can display overlay texts
        // (in this example, we will display an average value)
        {
            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                average += values[n];
            average /= (float)IM_ARRAYSIZE(values);
            char overlay[32];
            sprintf(overlay, "avg %f", average);
            FlexUI::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
        }

        // Use functions to generate output
        // FIXME: This is rather awkward because current plot API only pass in indices.
        // We probably want an API passing floats and user provide sample rate/count.
        struct Funcs
        {
            static float Sin(void*, int i) { return sinf(i * 0.1f); }
            static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
        };
        static int func_type = 0, display_count = 70;
        FlexUI::Separator();
        FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 8);
        FlexUI::Combo("func", &func_type, "Sin\0Saw\0");
        FlexUI::SameLine();
        FlexUI::SliderInt("Sample count", &display_count, 1, 400);
        float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
        FlexUI::PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
        FlexUI::PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
        FlexUI::Separator();

        // Animate a simple progress bar
        FlexUI_DEMO_MARKER("Widgets/Plotting/ProgressBar");
        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * FlexUI::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        // Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
        // or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        FlexUI::ProgressBar(progress, ImVec2(0.0f, 0.0f));
        FlexUI::SameLine(0.0f, FlexUI::GetStyle().ItemInnerSpacing.x);
        FlexUI::Text("Progress Bar");

        float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
        char buf[32];
        sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
        FlexUI::ProgressBar(progress, ImVec2(0.f, 0.f), buf);
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Color");
    if (FlexUI::TreeNode("Color/Picker Widgets"))
    {
        static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

        static bool alpha_preview = true;
        static bool alpha_half_preview = false;
        static bool drag_and_drop = true;
        static bool options_menu = true;
        static bool hdr = false;
        FlexUI::Checkbox("With Alpha Preview", &alpha_preview);
        FlexUI::Checkbox("With Half Alpha Preview", &alpha_half_preview);
        FlexUI::Checkbox("With Drag and Drop", &drag_and_drop);
        FlexUI::Checkbox("With Options Menu", &options_menu); FlexUI::SameLine(); HelpMarker("Right-click on the individual color widget to show options.");
        FlexUI::Checkbox("With HDR", &hdr); FlexUI::SameLine(); HelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");
        FlexUIColorEditFlags misc_flags = (hdr ? FlexUIColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : FlexUIColorEditFlags_NoDragDrop) | (alpha_half_preview ? FlexUIColorEditFlags_AlphaPreviewHalf : (alpha_preview ? FlexUIColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : FlexUIColorEditFlags_NoOptions);

        FlexUI_DEMO_MARKER("Widgets/Color/ColorEdit");
        FlexUI::Text("Color widget:");
        FlexUI::SameLine(); HelpMarker(
            "Click on the color square to open a color picker.\n"
            "CTRL+click on individual component to input value.\n");
        FlexUI::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

        FlexUI_DEMO_MARKER("Widgets/Color/ColorEdit (HSV, with Alpha)");
        FlexUI::Text("Color widget HSV with Alpha:");
        FlexUI::ColorEdit4("MyColor##2", (float*)&color, FlexUIColorEditFlags_DisplayHSV | misc_flags);

        FlexUI_DEMO_MARKER("Widgets/Color/ColorEdit (float display)");
        FlexUI::Text("Color widget with Float Display:");
        FlexUI::ColorEdit4("MyColor##2f", (float*)&color, FlexUIColorEditFlags_Float | misc_flags);

        FlexUI_DEMO_MARKER("Widgets/Color/ColorButton (with Picker)");
        FlexUI::Text("Color button with Picker:");
        FlexUI::SameLine(); HelpMarker(
            "With the FlexUIColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\n"
            "With the FlexUIColorEditFlags_NoLabel flag you can pass a non-empty label which will only "
            "be used for the tooltip and picker popup.");
        FlexUI::ColorEdit4("MyColor##3", (float*)&color, FlexUIColorEditFlags_NoInputs | FlexUIColorEditFlags_NoLabel | misc_flags);

        FlexUI_DEMO_MARKER("Widgets/Color/ColorButton (with custom Picker popup)");
        FlexUI::Text("Color button with Custom Picker Popup:");

        // Generate a default palette. The palette will persist and can be edited.
        static bool saved_palette_init = true;
        static ImVec4 saved_palette[32] = {};
        if (saved_palette_init)
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                FlexUI::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
                    saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f; // Alpha
            }
            saved_palette_init = false;
        }

        static ImVec4 backup_color;
        bool open_popup = FlexUI::ColorButton("MyColor##3b", color, misc_flags);
        FlexUI::SameLine(0, FlexUI::GetStyle().ItemInnerSpacing.x);
        open_popup |= FlexUI::Button("Palette");
        if (open_popup)
        {
            FlexUI::OpenPopup("mypicker");
            backup_color = color;
        }
        if (FlexUI::BeginPopup("mypicker"))
        {
            FlexUI::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
            FlexUI::Separator();
            FlexUI::ColorPicker4("##picker", (float*)&color, misc_flags | FlexUIColorEditFlags_NoSidePreview | FlexUIColorEditFlags_NoSmallPreview);
            FlexUI::SameLine();

            FlexUI::BeginGroup(); // Lock X position
            FlexUI::Text("Current");
            FlexUI::ColorButton("##current", color, FlexUIColorEditFlags_NoPicker | FlexUIColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
            FlexUI::Text("Previous");
            if (FlexUI::ColorButton("##previous", backup_color, FlexUIColorEditFlags_NoPicker | FlexUIColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
                color = backup_color;
            FlexUI::Separator();
            FlexUI::Text("Palette");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                FlexUI::PushID(n);
                if ((n % 8) != 0)
                    FlexUI::SameLine(0.0f, FlexUI::GetStyle().ItemSpacing.y);

                FlexUIColorEditFlags palette_button_flags = FlexUIColorEditFlags_NoAlpha | FlexUIColorEditFlags_NoPicker | FlexUIColorEditFlags_NoTooltip;
                if (FlexUI::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

                // Allow user to drop colors into each palette entry. Note that ColorButton() is already a
                // drag source by default, unless specifying the FlexUIColorEditFlags_NoDragDrop flag.
                if (FlexUI::BeginDragDropTarget())
                {
                    if (const FlexUIPayload* payload = FlexUI::AcceptDragDropPayload(FlexUI_PAYLOAD_TYPE_COLOR_3F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                    if (const FlexUIPayload* payload = FlexUI::AcceptDragDropPayload(FlexUI_PAYLOAD_TYPE_COLOR_4F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                    FlexUI::EndDragDropTarget();
                }

                FlexUI::PopID();
            }
            FlexUI::EndGroup();
            FlexUI::EndPopup();
        }

        FlexUI_DEMO_MARKER("Widgets/Color/ColorButton (simple)");
        FlexUI::Text("Color button only:");
        static bool no_border = false;
        FlexUI::Checkbox("FlexUIColorEditFlags_NoBorder", &no_border);
        FlexUI::ColorButton("MyColor##3c", *(ImVec4*)&color, misc_flags | (no_border ? FlexUIColorEditFlags_NoBorder : 0), ImVec2(80, 80));

        FlexUI_DEMO_MARKER("Widgets/Color/ColorPicker");
        FlexUI::Text("Color picker:");
        static bool alpha = true;
        static bool alpha_bar = true;
        static bool side_preview = true;
        static bool ref_color = false;
        static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
        static int display_mode = 0;
        static int picker_mode = 0;
        FlexUI::Checkbox("With Alpha", &alpha);
        FlexUI::Checkbox("With Alpha Bar", &alpha_bar);
        FlexUI::Checkbox("With Side Preview", &side_preview);
        if (side_preview)
        {
            FlexUI::SameLine();
            FlexUI::Checkbox("With Ref Color", &ref_color);
            if (ref_color)
            {
                FlexUI::SameLine();
                FlexUI::ColorEdit4("##RefColor", &ref_color_v.x, FlexUIColorEditFlags_NoInputs | misc_flags);
            }
        }
        FlexUI::Combo("Display Mode", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
        FlexUI::SameLine(); HelpMarker(
            "ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, "
            "but the user can change it with a right-click on those inputs.\n\nColorPicker defaults to displaying RGB+HSV+Hex "
            "if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions().");
        FlexUI::SameLine(); HelpMarker("When not specified explicitly (Auto/Current mode), user can right-click the picker to change mode.");
        FlexUIColorEditFlags flags = misc_flags;
        if (!alpha)            flags |= FlexUIColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()
        if (alpha_bar)         flags |= FlexUIColorEditFlags_AlphaBar;
        if (!side_preview)     flags |= FlexUIColorEditFlags_NoSidePreview;
        if (picker_mode == 1)  flags |= FlexUIColorEditFlags_PickerHueBar;
        if (picker_mode == 2)  flags |= FlexUIColorEditFlags_PickerHueWheel;
        if (display_mode == 1) flags |= FlexUIColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
        if (display_mode == 2) flags |= FlexUIColorEditFlags_DisplayRGB;     // Override display mode
        if (display_mode == 3) flags |= FlexUIColorEditFlags_DisplayHSV;
        if (display_mode == 4) flags |= FlexUIColorEditFlags_DisplayHex;
        FlexUI::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

        FlexUI::Text("Set defaults in code:");
        FlexUI::SameLine(); HelpMarker(
            "SetColorEditOptions() is designed to allow you to set boot-time default.\n"
            "We don't have Push/Pop functions because you can force options on a per-widget basis if needed,"
            "and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid"
            "encouraging you to persistently save values that aren't forward-compatible.");
        if (FlexUI::Button("Default: Uint8 + HSV + Hue Bar"))
            FlexUI::SetColorEditOptions(FlexUIColorEditFlags_Uint8 | FlexUIColorEditFlags_DisplayHSV | FlexUIColorEditFlags_PickerHueBar);
        if (FlexUI::Button("Default: Float + HDR + Hue Wheel"))
            FlexUI::SetColorEditOptions(FlexUIColorEditFlags_Float | FlexUIColorEditFlags_HDR | FlexUIColorEditFlags_PickerHueWheel);

        // Always both a small version of both types of pickers (to make it more visible in the demo to people who are skimming quickly through it)
        FlexUI::Text("Both types:");
        float w = (FlexUI::GetContentRegionAvail().x - FlexUI::GetStyle().ItemSpacing.y) * 0.40f;
        FlexUI::SetNextItemWidth(w);
        FlexUI::ColorPicker3("##MyColor##5", (float*)&color, FlexUIColorEditFlags_PickerHueBar | FlexUIColorEditFlags_NoSidePreview | FlexUIColorEditFlags_NoInputs | FlexUIColorEditFlags_NoAlpha);
        FlexUI::SameLine();
        FlexUI::SetNextItemWidth(w);
        FlexUI::ColorPicker3("##MyColor##6", (float*)&color, FlexUIColorEditFlags_PickerHueWheel | FlexUIColorEditFlags_NoSidePreview | FlexUIColorEditFlags_NoInputs | FlexUIColorEditFlags_NoAlpha);

        // HSV encoded support (to avoid RGB<>HSV round trips and singularities when S==0 or V==0)
        static ImVec4 color_hsv(0.23f, 1.0f, 1.0f, 1.0f); // Stored as HSV!
        FlexUI::Spacing();
        FlexUI::Text("HSV encoded colors");
        FlexUI::SameLine(); HelpMarker(
            "By default, colors are given to ColorEdit and ColorPicker in RGB, but FlexUIColorEditFlags_InputHSV"
            "allows you to store colors as HSV and pass them to ColorEdit and ColorPicker as HSV. This comes with the"
            "added benefit that you can manipulate hue values with the picker even when saturation or value are zero.");
        FlexUI::Text("Color widget with InputHSV:");
        FlexUI::ColorEdit4("HSV shown as RGB##1", (float*)&color_hsv, FlexUIColorEditFlags_DisplayRGB | FlexUIColorEditFlags_InputHSV | FlexUIColorEditFlags_Float);
        FlexUI::ColorEdit4("HSV shown as HSV##1", (float*)&color_hsv, FlexUIColorEditFlags_DisplayHSV | FlexUIColorEditFlags_InputHSV | FlexUIColorEditFlags_Float);
        FlexUI::DragFloat4("Raw HSV values", (float*)&color_hsv, 0.01f, 0.0f, 1.0f);

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Drag and Slider Flags");
    if (FlexUI::TreeNode("Drag/Slider Flags"))
    {
        // Demonstrate using advanced flags for DragXXX and SliderXXX functions. Note that the flags are the same!
        static FlexUISliderFlags flags = FlexUISliderFlags_None;
        FlexUI::CheckboxFlags("FlexUISliderFlags_AlwaysClamp", &flags, FlexUISliderFlags_AlwaysClamp);
        FlexUI::SameLine(); HelpMarker("Always clamp value to min/max bounds (if any) when input manually with CTRL+Click.");
        FlexUI::CheckboxFlags("FlexUISliderFlags_Logarithmic", &flags, FlexUISliderFlags_Logarithmic);
        FlexUI::SameLine(); HelpMarker("Enable logarithmic editing (more precision for small values).");
        FlexUI::CheckboxFlags("FlexUISliderFlags_NoRoundToFormat", &flags, FlexUISliderFlags_NoRoundToFormat);
        FlexUI::SameLine(); HelpMarker("Disable rounding underlying value to match precision of the format string (e.g. %.3f values are rounded to those 3 digits).");
        FlexUI::CheckboxFlags("FlexUISliderFlags_NoInput", &flags, FlexUISliderFlags_NoInput);
        FlexUI::SameLine(); HelpMarker("Disable CTRL+Click or Enter key allowing to input text directly into the widget.");

        // Drags
        static float drag_f = 0.5f;
        static int drag_i = 50;
        FlexUI::Text("Underlying float value: %f", drag_f);
        FlexUI::DragFloat("DragFloat (0 -> 1)", &drag_f, 0.005f, 0.0f, 1.0f, "%.3f", flags);
        FlexUI::DragFloat("DragFloat (0 -> +inf)", &drag_f, 0.005f, 0.0f, FLT_MAX, "%.3f", flags);
        FlexUI::DragFloat("DragFloat (-inf -> 1)", &drag_f, 0.005f, -FLT_MAX, 1.0f, "%.3f", flags);
        FlexUI::DragFloat("DragFloat (-inf -> +inf)", &drag_f, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
        FlexUI::DragInt("DragInt (0 -> 100)", &drag_i, 0.5f, 0, 100, "%d", flags);

        // Sliders
        static float slider_f = 0.5f;
        static int slider_i = 50;
        FlexUI::Text("Underlying float value: %f", slider_f);
        FlexUI::SliderFloat("SliderFloat (0 -> 1)", &slider_f, 0.0f, 1.0f, "%.3f", flags);
        FlexUI::SliderInt("SliderInt (0 -> 100)", &slider_i, 0, 100, "%d", flags);

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Range Widgets");
    if (FlexUI::TreeNode("Range Widgets"))
    {
        static float begin = 10, end = 90;
        static int begin_i = 100, end_i = 1000;
        FlexUI::DragFloatRange2("range float", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%", FlexUISliderFlags_AlwaysClamp);
        FlexUI::DragIntRange2("range int", &begin_i, &end_i, 5, 0, 1000, "Min: %d units", "Max: %d units");
        FlexUI::DragIntRange2("range int (no bounds)", &begin_i, &end_i, 5, 0, 0, "Min: %d units", "Max: %d units");
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Data Types");
    if (FlexUI::TreeNode("Data Types"))
    {
        // DragScalar/InputScalar/SliderScalar functions allow various data types
        // - signed/unsigned
        // - 8/16/32/64-bits
        // - integer/float/double
        // To avoid polluting the public API with all possible combinations, we use the FlexUIDataType enum
        // to pass the type, and passing all arguments by pointer.
        // This is the reason the test code below creates local variables to hold "zero" "one" etc. for each type.
        // In practice, if you frequently use a given type that is not covered by the normal API entry points,
        // you can wrap it yourself inside a 1 line function which can take typed argument as value instead of void*,
        // and then pass their address to the generic function. For example:
        //   bool MySliderU64(const char *label, u64* value, u64 min = 0, u64 max = 0, const char* format = "%lld")
        //   {
        //      return SliderScalar(label, FlexUIDataType_U64, value, &min, &max, format);
        //   }

        // Setup limits (as helper variables so we can take their address, as explained above)
        // Note: SliderScalar() functions have a maximum usable range of half the natural type maximum, hence the /2.
#ifndef LLONG_MIN
        ImS64 LLONG_MIN = -9223372036854775807LL - 1;
        ImS64 LLONG_MAX = 9223372036854775807LL;
        ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
#endif
        const char    s8_zero = 0, s8_one = 1, s8_fifty = 50, s8_min = -128, s8_max = 127;
        const ImU8    u8_zero = 0, u8_one = 1, u8_fifty = 50, u8_min = 0, u8_max = 255;
        const short   s16_zero = 0, s16_one = 1, s16_fifty = 50, s16_min = -32768, s16_max = 32767;
        const ImU16   u16_zero = 0, u16_one = 1, u16_fifty = 50, u16_min = 0, u16_max = 65535;
        const ImS32   s32_zero = 0, s32_one = 1, s32_fifty = 50, s32_min = INT_MIN / 2, s32_max = INT_MAX / 2, s32_hi_a = INT_MAX / 2 - 100, s32_hi_b = INT_MAX / 2;
        const ImU32   u32_zero = 0, u32_one = 1, u32_fifty = 50, u32_min = 0, u32_max = UINT_MAX / 2, u32_hi_a = UINT_MAX / 2 - 100, u32_hi_b = UINT_MAX / 2;
        const ImS64   s64_zero = 0, s64_one = 1, s64_fifty = 50, s64_min = LLONG_MIN / 2, s64_max = LLONG_MAX / 2, s64_hi_a = LLONG_MAX / 2 - 100, s64_hi_b = LLONG_MAX / 2;
        const ImU64   u64_zero = 0, u64_one = 1, u64_fifty = 50, u64_min = 0, u64_max = ULLONG_MAX / 2, u64_hi_a = ULLONG_MAX / 2 - 100, u64_hi_b = ULLONG_MAX / 2;
        const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
        const double  f64_zero = 0., f64_one = 1., f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

        // State
        static char   s8_v = 127;
        static ImU8   u8_v = 255;
        static short  s16_v = 32767;
        static ImU16  u16_v = 65535;
        static ImS32  s32_v = -1;
        static ImU32  u32_v = (ImU32)-1;
        static ImS64  s64_v = -1;
        static ImU64  u64_v = (ImU64)-1;
        static float  f32_v = 0.123f;
        static double f64_v = 90000.01234567890123456789;

        const float drag_speed = 0.2f;
        static bool drag_clamp = false;
        FlexUI_DEMO_MARKER("Widgets/Data Types/Drags");
        FlexUI::Text("Drags:");
        FlexUI::Checkbox("Clamp integers to 0..50", &drag_clamp);
        FlexUI::SameLine(); HelpMarker(
            "As with every widget in dear FlexUI, we never modify values unless there is a user interaction.\n"
            "You can override the clamping limits by using CTRL+Click to input a value.");
        FlexUI::DragScalar("drag s8", FlexUIDataType_S8, &s8_v, drag_speed, drag_clamp ? &s8_zero : NULL, drag_clamp ? &s8_fifty : NULL);
        FlexUI::DragScalar("drag u8", FlexUIDataType_U8, &u8_v, drag_speed, drag_clamp ? &u8_zero : NULL, drag_clamp ? &u8_fifty : NULL, "%u ms");
        FlexUI::DragScalar("drag s16", FlexUIDataType_S16, &s16_v, drag_speed, drag_clamp ? &s16_zero : NULL, drag_clamp ? &s16_fifty : NULL);
        FlexUI::DragScalar("drag u16", FlexUIDataType_U16, &u16_v, drag_speed, drag_clamp ? &u16_zero : NULL, drag_clamp ? &u16_fifty : NULL, "%u ms");
        FlexUI::DragScalar("drag s32", FlexUIDataType_S32, &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL);
        FlexUI::DragScalar("drag s32 hex", FlexUIDataType_S32, &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL, "0x%08X");
        FlexUI::DragScalar("drag u32", FlexUIDataType_U32, &u32_v, drag_speed, drag_clamp ? &u32_zero : NULL, drag_clamp ? &u32_fifty : NULL, "%u ms");
        FlexUI::DragScalar("drag s64", FlexUIDataType_S64, &s64_v, drag_speed, drag_clamp ? &s64_zero : NULL, drag_clamp ? &s64_fifty : NULL);
        FlexUI::DragScalar("drag u64", FlexUIDataType_U64, &u64_v, drag_speed, drag_clamp ? &u64_zero : NULL, drag_clamp ? &u64_fifty : NULL);
        FlexUI::DragScalar("drag float", FlexUIDataType_Float, &f32_v, 0.005f, &f32_zero, &f32_one, "%f");
        FlexUI::DragScalar("drag float log", FlexUIDataType_Float, &f32_v, 0.005f, &f32_zero, &f32_one, "%f", FlexUISliderFlags_Logarithmic);
        FlexUI::DragScalar("drag double", FlexUIDataType_Double, &f64_v, 0.0005f, &f64_zero, NULL, "%.10f grams");
        FlexUI::DragScalar("drag double log", FlexUIDataType_Double, &f64_v, 0.0005f, &f64_zero, &f64_one, "0 < %.10f < 1", FlexUISliderFlags_Logarithmic);

        FlexUI_DEMO_MARKER("Widgets/Data Types/Sliders");
        FlexUI::Text("Sliders");
        FlexUI::SliderScalar("slider s8 full", FlexUIDataType_S8, &s8_v, &s8_min, &s8_max, "%d");
        FlexUI::SliderScalar("slider u8 full", FlexUIDataType_U8, &u8_v, &u8_min, &u8_max, "%u");
        FlexUI::SliderScalar("slider s16 full", FlexUIDataType_S16, &s16_v, &s16_min, &s16_max, "%d");
        FlexUI::SliderScalar("slider u16 full", FlexUIDataType_U16, &u16_v, &u16_min, &u16_max, "%u");
        FlexUI::SliderScalar("slider s32 low", FlexUIDataType_S32, &s32_v, &s32_zero, &s32_fifty, "%d");
        FlexUI::SliderScalar("slider s32 high", FlexUIDataType_S32, &s32_v, &s32_hi_a, &s32_hi_b, "%d");
        FlexUI::SliderScalar("slider s32 full", FlexUIDataType_S32, &s32_v, &s32_min, &s32_max, "%d");
        FlexUI::SliderScalar("slider s32 hex", FlexUIDataType_S32, &s32_v, &s32_zero, &s32_fifty, "0x%04X");
        FlexUI::SliderScalar("slider u32 low", FlexUIDataType_U32, &u32_v, &u32_zero, &u32_fifty, "%u");
        FlexUI::SliderScalar("slider u32 high", FlexUIDataType_U32, &u32_v, &u32_hi_a, &u32_hi_b, "%u");
        FlexUI::SliderScalar("slider u32 full", FlexUIDataType_U32, &u32_v, &u32_min, &u32_max, "%u");
        FlexUI::SliderScalar("slider s64 low", FlexUIDataType_S64, &s64_v, &s64_zero, &s64_fifty, "%" IM_PRId64);
        FlexUI::SliderScalar("slider s64 high", FlexUIDataType_S64, &s64_v, &s64_hi_a, &s64_hi_b, "%" IM_PRId64);
        FlexUI::SliderScalar("slider s64 full", FlexUIDataType_S64, &s64_v, &s64_min, &s64_max, "%" IM_PRId64);
        FlexUI::SliderScalar("slider u64 low", FlexUIDataType_U64, &u64_v, &u64_zero, &u64_fifty, "%" IM_PRIu64 " ms");
        FlexUI::SliderScalar("slider u64 high", FlexUIDataType_U64, &u64_v, &u64_hi_a, &u64_hi_b, "%" IM_PRIu64 " ms");
        FlexUI::SliderScalar("slider u64 full", FlexUIDataType_U64, &u64_v, &u64_min, &u64_max, "%" IM_PRIu64 " ms");
        FlexUI::SliderScalar("slider float low", FlexUIDataType_Float, &f32_v, &f32_zero, &f32_one);
        FlexUI::SliderScalar("slider float low log", FlexUIDataType_Float, &f32_v, &f32_zero, &f32_one, "%.10f", FlexUISliderFlags_Logarithmic);
        FlexUI::SliderScalar("slider float high", FlexUIDataType_Float, &f32_v, &f32_lo_a, &f32_hi_a, "%e");
        FlexUI::SliderScalar("slider double low", FlexUIDataType_Double, &f64_v, &f64_zero, &f64_one, "%.10f grams");
        FlexUI::SliderScalar("slider double low log", FlexUIDataType_Double, &f64_v, &f64_zero, &f64_one, "%.10f", FlexUISliderFlags_Logarithmic);
        FlexUI::SliderScalar("slider double high", FlexUIDataType_Double, &f64_v, &f64_lo_a, &f64_hi_a, "%e grams");

        FlexUI::Text("Sliders (reverse)");
        FlexUI::SliderScalar("slider s8 reverse", FlexUIDataType_S8, &s8_v, &s8_max, &s8_min, "%d");
        FlexUI::SliderScalar("slider u8 reverse", FlexUIDataType_U8, &u8_v, &u8_max, &u8_min, "%u");
        FlexUI::SliderScalar("slider s32 reverse", FlexUIDataType_S32, &s32_v, &s32_fifty, &s32_zero, "%d");
        FlexUI::SliderScalar("slider u32 reverse", FlexUIDataType_U32, &u32_v, &u32_fifty, &u32_zero, "%u");
        FlexUI::SliderScalar("slider s64 reverse", FlexUIDataType_S64, &s64_v, &s64_fifty, &s64_zero, "%" IM_PRId64);
        FlexUI::SliderScalar("slider u64 reverse", FlexUIDataType_U64, &u64_v, &u64_fifty, &u64_zero, "%" IM_PRIu64 " ms");

        FlexUI_DEMO_MARKER("Widgets/Data Types/Inputs");
        static bool inputs_step = true;
        FlexUI::Text("Inputs");
        FlexUI::Checkbox("Show step buttons", &inputs_step);
        FlexUI::InputScalar("input s8", FlexUIDataType_S8, &s8_v, inputs_step ? &s8_one : NULL, NULL, "%d");
        FlexUI::InputScalar("input u8", FlexUIDataType_U8, &u8_v, inputs_step ? &u8_one : NULL, NULL, "%u");
        FlexUI::InputScalar("input s16", FlexUIDataType_S16, &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d");
        FlexUI::InputScalar("input u16", FlexUIDataType_U16, &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u");
        FlexUI::InputScalar("input s32", FlexUIDataType_S32, &s32_v, inputs_step ? &s32_one : NULL, NULL, "%d");
        FlexUI::InputScalar("input s32 hex", FlexUIDataType_S32, &s32_v, inputs_step ? &s32_one : NULL, NULL, "%04X");
        FlexUI::InputScalar("input u32", FlexUIDataType_U32, &u32_v, inputs_step ? &u32_one : NULL, NULL, "%u");
        FlexUI::InputScalar("input u32 hex", FlexUIDataType_U32, &u32_v, inputs_step ? &u32_one : NULL, NULL, "%08X");
        FlexUI::InputScalar("input s64", FlexUIDataType_S64, &s64_v, inputs_step ? &s64_one : NULL);
        FlexUI::InputScalar("input u64", FlexUIDataType_U64, &u64_v, inputs_step ? &u64_one : NULL);
        FlexUI::InputScalar("input float", FlexUIDataType_Float, &f32_v, inputs_step ? &f32_one : NULL);
        FlexUI::InputScalar("input double", FlexUIDataType_Double, &f64_v, inputs_step ? &f64_one : NULL);

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Multi-component Widgets");
    if (FlexUI::TreeNode("Multi-component Widgets"))
    {
        static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
        static int vec4i[4] = { 1, 5, 100, 255 };

        FlexUI::InputFloat2("input float2", vec4f);
        FlexUI::DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
        FlexUI::SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
        FlexUI::InputInt2("input int2", vec4i);
        FlexUI::DragInt2("drag int2", vec4i, 1, 0, 255);
        FlexUI::SliderInt2("slider int2", vec4i, 0, 255);
        FlexUI::Spacing();

        FlexUI::InputFloat3("input float3", vec4f);
        FlexUI::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
        FlexUI::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
        FlexUI::InputInt3("input int3", vec4i);
        FlexUI::DragInt3("drag int3", vec4i, 1, 0, 255);
        FlexUI::SliderInt3("slider int3", vec4i, 0, 255);
        FlexUI::Spacing();

        FlexUI::InputFloat4("input float4", vec4f);
        FlexUI::DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
        FlexUI::SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
        FlexUI::InputInt4("input int4", vec4i);
        FlexUI::DragInt4("drag int4", vec4i, 1, 0, 255);
        FlexUI::SliderInt4("slider int4", vec4i, 0, 255);

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Vertical Sliders");
    if (FlexUI::TreeNode("Vertical Sliders"))
    {
        const float spacing = 4;
        FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(spacing, spacing));

        static int int_value = 0;
        FlexUI::VSliderInt("##int", ImVec2(18, 160), &int_value, 0, 5);
        FlexUI::SameLine();

        static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
        FlexUI::PushID("set1");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0) FlexUI::SameLine();
            FlexUI::PushID(i);
            FlexUI::PushStyleColor(FlexUICol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
            FlexUI::PushStyleColor(FlexUICol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
            FlexUI::PushStyleColor(FlexUICol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
            FlexUI::PushStyleColor(FlexUICol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
            FlexUI::VSliderFloat("##v", ImVec2(18, 160), &values[i], 0.0f, 1.0f, "");
            if (FlexUI::IsItemActive() || FlexUI::IsItemHovered())
                FlexUI::SetTooltip("%.3f", values[i]);
            FlexUI::PopStyleColor(4);
            FlexUI::PopID();
        }
        FlexUI::PopID();

        FlexUI::SameLine();
        FlexUI::PushID("set2");
        static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
        const int rows = 3;
        const ImVec2 small_slider_size(18, (float)(int)((160.0f - (rows - 1) * spacing) / rows));
        for (int nx = 0; nx < 4; nx++)
        {
            if (nx > 0) FlexUI::SameLine();
            FlexUI::BeginGroup();
            for (int ny = 0; ny < rows; ny++)
            {
                FlexUI::PushID(nx * rows + ny);
                FlexUI::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
                if (FlexUI::IsItemActive() || FlexUI::IsItemHovered())
                    FlexUI::SetTooltip("%.3f", values2[nx]);
                FlexUI::PopID();
            }
            FlexUI::EndGroup();
        }
        FlexUI::PopID();

        FlexUI::SameLine();
        FlexUI::PushID("set3");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) FlexUI::SameLine();
            FlexUI::PushID(i);
            FlexUI::PushStyleVar(FlexUIStyleVar_GrabMinSize, 40);
            FlexUI::VSliderFloat("##v", ImVec2(40, 160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
            FlexUI::PopStyleVar();
            FlexUI::PopID();
        }
        FlexUI::PopID();
        FlexUI::PopStyleVar();
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Drag and drop");
    if (FlexUI::TreeNode("Drag and Drop"))
    {
        FlexUI_DEMO_MARKER("Widgets/Drag and drop/Standard widgets");
        if (FlexUI::TreeNode("Drag and drop in standard widgets"))
        {
            // ColorEdit widgets automatically act as drag source and drag target.
            // They are using standardized payload strings FlexUI_PAYLOAD_TYPE_COLOR_3F and FlexUI_PAYLOAD_TYPE_COLOR_4F
            // to allow your own widgets to use colors in their drag and drop interaction.
            // Also see 'Demo->Widgets->Color/Picker Widgets->Palette' demo.
            HelpMarker("You can drag from the color squares.");
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
            FlexUI::ColorEdit3("color 1", col1);
            FlexUI::ColorEdit4("color 2", col2);
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Drag and drop/Copy-swap items");
        if (FlexUI::TreeNode("Drag and drop to copy/swap items"))
        {
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (FlexUI::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } FlexUI::SameLine();
            if (FlexUI::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } FlexUI::SameLine();
            if (FlexUI::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
            static const char* names[9] =
            {
                "Bobby", "Beatrice", "Betty",
                "Brianna", "Barry", "Bernard",
                "Bibi", "Blaine", "Bryn"
            };
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                FlexUI::PushID(n);
                if ((n % 3) != 0)
                    FlexUI::SameLine();
                FlexUI::Button(names[n], ImVec2(60, 60));

                // Our buttons are both drag sources and drag targets here!
                if (FlexUI::BeginDragDropSource(FlexUIDragDropFlags_None))
                {
                    // Set payload to carry the index of our item (could be anything)
                    FlexUI::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

                    // Display preview (could be anything, e.g. when dragging an image we could decide to display
                    // the filename and a small preview of the image, etc.)
                    if (mode == Mode_Copy) { FlexUI::Text("Copy %s", names[n]); }
                    if (mode == Mode_Move) { FlexUI::Text("Move %s", names[n]); }
                    if (mode == Mode_Swap) { FlexUI::Text("Swap %s", names[n]); }
                    FlexUI::EndDragDropSource();
                }
                if (FlexUI::BeginDragDropTarget())
                {
                    if (const FlexUIPayload* payload = FlexUI::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    FlexUI::EndDragDropTarget();
                }
                FlexUI::PopID();
            }
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Widgets/Drag and Drop/Drag to reorder items (simple)");
        if (FlexUI::TreeNode("Drag to reorder items (simple)"))
        {
            // Simple reordering
            HelpMarker(
                "We don't use the drag and drop api at all here! "
                "Instead we query when the item is held but not hovered, and order items accordingly.");
            static const char* item_names[] = { "Item One", "Item Two", "Item Three", "Item Four", "Item Five" };
            for (int n = 0; n < IM_ARRAYSIZE(item_names); n++)
            {
                const char* item = item_names[n];
                FlexUI::Selectable(item);

                if (FlexUI::IsItemActive() && !FlexUI::IsItemHovered())
                {
                    int n_next = n + (FlexUI::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                    if (n_next >= 0 && n_next < IM_ARRAYSIZE(item_names))
                    {
                        item_names[n] = item_names[n_next];
                        item_names[n_next] = item;
                        FlexUI::ResetMouseDragDelta();
                    }
                }
            }
            FlexUI::TreePop();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Querying Item Status (Edited,Active,Hovered etc.)");
    if (FlexUI::TreeNode("Querying Item Status (Edited/Active/Hovered etc.)"))
    {
        // Select an item type
        const char* item_names[] =
        {
            "Text", "Button", "Button (w/ repeat)", "Checkbox", "SliderFloat", "InputText", "InputTextMultiline", "InputFloat",
            "InputFloat3", "ColorEdit4", "Selectable", "MenuItem", "TreeNode", "TreeNode (w/ double-click)", "Combo", "ListBox"
        };
        static int item_type = 4;
        static bool item_disabled = false;
        FlexUI::Combo("Item Type", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));
        FlexUI::SameLine();
        HelpMarker("Testing how various types of items are interacting with the IsItemXXX functions. Note that the bool return value of most FlexUI function is generally equivalent to calling FlexUI::IsItemHovered().");
        FlexUI::Checkbox("Item Disabled", &item_disabled);

        // Submit selected items so we can query their status in the code following it.
        bool ret = false;
        static bool b = false;
        static float col4f[4] = { 1.0f, 0.5, 0.0f, 1.0f };
        static char str[16] = {};
        if (item_disabled)
            FlexUI::BeginDisabled(true);
        if (item_type == 0) { FlexUI::Text("ITEM: Text"); }                                              // Testing text items with no identifier/interaction
        if (item_type == 1) { ret = FlexUI::Button("ITEM: Button"); }                                    // Testing button
        if (item_type == 2) { FlexUI::PushButtonRepeat(true); ret = FlexUI::Button("ITEM: Button"); FlexUI::PopButtonRepeat(); } // Testing button (with repeater)
        if (item_type == 3) { ret = FlexUI::Checkbox("ITEM: Checkbox", &b); }                            // Testing checkbox
        if (item_type == 4) { ret = FlexUI::SliderFloat("ITEM: SliderFloat", &col4f[0], 0.0f, 1.0f); }   // Testing basic item
        if (item_type == 5) { ret = FlexUI::InputText("ITEM: InputText", &str[0], IM_ARRAYSIZE(str)); }  // Testing input text (which handles tabbing)
        if (item_type == 6) { ret = FlexUI::InputTextMultiline("ITEM: InputTextMultiline", &str[0], IM_ARRAYSIZE(str)); } // Testing input text (which uses a child window)
        if (item_type == 7) { ret = FlexUI::InputFloat("ITEM: InputFloat", col4f, 1.0f); }               // Testing +/- buttons on scalar input
        if (item_type == 8) { ret = FlexUI::InputFloat3("ITEM: InputFloat3", col4f); }                   // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 9) { ret = FlexUI::ColorEdit4("ITEM: ColorEdit4", col4f); }                     // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 10) { ret = FlexUI::Selectable("ITEM: Selectable"); }                            // Testing selectable item
        if (item_type == 11) { ret = FlexUI::MenuItem("ITEM: MenuItem"); }                                // Testing menu item (they use FlexUIButtonFlags_PressedOnRelease button policy)
        if (item_type == 12) { ret = FlexUI::TreeNode("ITEM: TreeNode"); if (ret) FlexUI::TreePop(); }     // Testing tree node
        if (item_type == 13) { ret = FlexUI::TreeNodeEx("ITEM: TreeNode w/ FlexUITreeNodeFlags_OpenOnDoubleClick", FlexUITreeNodeFlags_OpenOnDoubleClick | FlexUITreeNodeFlags_NoTreePushOnOpen); } // Testing tree node with FlexUIButtonFlags_PressedOnDoubleClick button policy.
        if (item_type == 14) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = FlexUI::Combo("ITEM: Combo", &current, items, IM_ARRAYSIZE(items)); }
        if (item_type == 15) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = FlexUI::ListBox("ITEM: ListBox", &current, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)); }

        bool hovered_delay_none = FlexUI::IsItemHovered();
        bool hovered_delay_short = FlexUI::IsItemHovered(FlexUIHoveredFlags_DelayShort);
        bool hovered_delay_normal = FlexUI::IsItemHovered(FlexUIHoveredFlags_DelayNormal);

        // Display the values of IsItemHovered() and other common item state functions.
        // Note that the FlexUIHoveredFlags_XXX flags can be combined.
        // Because BulletText is an item itself and that would affect the output of IsItemXXX functions,
        // we query every state in a single call to avoid storing them and to simplify the code.
        FlexUI::BulletText(
            "Return value = %d\n"
            "IsItemFocused() = %d\n"
            "IsItemHovered() = %d\n"
            "IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsItemHovered(_AllowWhenOverlapped) = %d\n"
            "IsItemHovered(_AllowWhenDisabled) = %d\n"
            "IsItemHovered(_RectOnly) = %d\n"
            "IsItemActive() = %d\n"
            "IsItemEdited() = %d\n"
            "IsItemActivated() = %d\n"
            "IsItemDeactivated() = %d\n"
            "IsItemDeactivatedAfterEdit() = %d\n"
            "IsItemVisible() = %d\n"
            "IsItemClicked() = %d\n"
            "IsItemToggledOpen() = %d\n"
            "GetItemRectMin() = (%.1f, %.1f)\n"
            "GetItemRectMax() = (%.1f, %.1f)\n"
            "GetItemRectSize() = (%.1f, %.1f)",
            ret,
            FlexUI::IsItemFocused(),
            FlexUI::IsItemHovered(),
            FlexUI::IsItemHovered(FlexUIHoveredFlags_AllowWhenBlockedByPopup),
            FlexUI::IsItemHovered(FlexUIHoveredFlags_AllowWhenBlockedByActiveItem),
            FlexUI::IsItemHovered(FlexUIHoveredFlags_AllowWhenOverlapped),
            FlexUI::IsItemHovered(FlexUIHoveredFlags_AllowWhenDisabled),
            FlexUI::IsItemHovered(FlexUIHoveredFlags_RectOnly),
            FlexUI::IsItemActive(),
            FlexUI::IsItemEdited(),
            FlexUI::IsItemActivated(),
            FlexUI::IsItemDeactivated(),
            FlexUI::IsItemDeactivatedAfterEdit(),
            FlexUI::IsItemVisible(),
            FlexUI::IsItemClicked(),
            FlexUI::IsItemToggledOpen(),
            FlexUI::GetItemRectMin().x, FlexUI::GetItemRectMin().y,
            FlexUI::GetItemRectMax().x, FlexUI::GetItemRectMax().y,
            FlexUI::GetItemRectSize().x, FlexUI::GetItemRectSize().y
        );
        FlexUI::BulletText(
            "w/ Hovering Delay: None = %d, Fast %d, Normal = %d", hovered_delay_none, hovered_delay_short, hovered_delay_normal);

        if (item_disabled)
            FlexUI::EndDisabled();

        char buf[1] = "";
        FlexUI::InputText("unused", buf, IM_ARRAYSIZE(buf), FlexUIInputTextFlags_ReadOnly);
        FlexUI::SameLine();
        HelpMarker("This widget is only here to be able to tab-out of the widgets above and see e.g. Deactivated() status.");

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Querying Window Status (Focused,Hovered etc.)");
    if (FlexUI::TreeNode("Querying Window Status (Focused/Hovered etc.)"))
    {
        static bool embed_all_inside_a_child_window = false;
        FlexUI::Checkbox("Embed everything inside a child window for testing _RootWindow flag.", &embed_all_inside_a_child_window);
        if (embed_all_inside_a_child_window)
            FlexUI::BeginChild("outer_child", ImVec2(0, FlexUI::GetFontSize() * 20.0f), true);

        // Testing IsWindowFocused() function with its various flags.
        FlexUI::BulletText(
            "IsWindowFocused() = %d\n"
            "IsWindowFocused(_ChildWindows) = %d\n"
            "IsWindowFocused(_ChildWindows|_NoPopupHierarchy) = %d\n"
            "IsWindowFocused(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowFocused(_ChildWindows|_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowFocused(_RootWindow) = %d\n"
            "IsWindowFocused(_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowFocused(_AnyWindow) = %d\n",
            FlexUI::IsWindowFocused(),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_ChildWindows),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_ChildWindows | FlexUIFocusedFlags_NoPopupHierarchy),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_ChildWindows | FlexUIFocusedFlags_RootWindow),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_ChildWindows | FlexUIFocusedFlags_RootWindow | FlexUIFocusedFlags_NoPopupHierarchy),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_RootWindow),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_RootWindow | FlexUIFocusedFlags_NoPopupHierarchy),
            FlexUI::IsWindowFocused(FlexUIFocusedFlags_AnyWindow));

        // Testing IsWindowHovered() function with its various flags.
        FlexUI::BulletText(
            "IsWindowHovered() = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsWindowHovered(_ChildWindows) = %d\n"
            "IsWindowHovered(_ChildWindows|_NoPopupHierarchy) = %d\n"
            "IsWindowHovered(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowHovered(_ChildWindows|_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowHovered(_RootWindow) = %d\n"
            "IsWindowHovered(_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowHovered(_ChildWindows|_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_AnyWindow) = %d\n",
            FlexUI::IsWindowHovered(),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_AllowWhenBlockedByPopup),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_AllowWhenBlockedByActiveItem),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_ChildWindows),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_ChildWindows | FlexUIHoveredFlags_NoPopupHierarchy),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_ChildWindows | FlexUIHoveredFlags_RootWindow),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_ChildWindows | FlexUIHoveredFlags_RootWindow | FlexUIHoveredFlags_NoPopupHierarchy),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_RootWindow),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_RootWindow | FlexUIHoveredFlags_NoPopupHierarchy),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_ChildWindows | FlexUIHoveredFlags_AllowWhenBlockedByPopup),
            FlexUI::IsWindowHovered(FlexUIHoveredFlags_AnyWindow));

        FlexUI::BeginChild("child", ImVec2(0, 50), true);
        FlexUI::Text("This is another child window for testing the _ChildWindows flag.");
        FlexUI::EndChild();
        if (embed_all_inside_a_child_window)
            FlexUI::EndChild();

        // Calling IsItemHovered() after begin returns the hovered status of the title bar.
        // This is useful in particular if you want to create a context menu associated to the title bar of a window.
        static bool test_window = false;
        FlexUI::Checkbox("Hovered/Active tests after Begin() for title bar testing", &test_window);
        if (test_window)
        {
            FlexUI::Begin("Title bar Hovered/Active tests", &test_window);
            if (FlexUI::BeginPopupContextItem()) // <-- This is using IsItemHovered()
            {
                if (FlexUI::MenuItem("Close")) { test_window = false; }
                FlexUI::EndPopup();
            }
            FlexUI::Text(
                "IsItemHovered() after begin = %d (== is title bar hovered)\n"
                "IsItemActive() after begin = %d (== is window being clicked/moved)\n",
                FlexUI::IsItemHovered(), FlexUI::IsItemActive());
            FlexUI::End();
        }

        FlexUI::TreePop();
    }

    // Demonstrate BeginDisabled/EndDisabled using a checkbox located at the bottom of the section (which is a bit odd:
    // logically we'd have this checkbox at the top of the section, but we don't want this feature to steal that space)
    if (disable_all)
        FlexUI::EndDisabled();

    FlexUI_DEMO_MARKER("Widgets/Disable Block");
    if (FlexUI::TreeNode("Disable block"))
    {
        FlexUI::Checkbox("Disable entire section above", &disable_all);
        FlexUI::SameLine(); HelpMarker("Demonstrate using BeginDisabled()/EndDisabled() across this section.");
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Widgets/Text Filter");
    if (FlexUI::TreeNode("Text Filter"))
    {
        // Helper class to easy setup a text filter.
        // You may want to implement a more feature-full filtering scheme in your own application.
        HelpMarker("Not a widget per-se, but FlexUITextFilter is a helper to perform simple filtering on text strings.");
        static FlexUITextFilter filter;
        FlexUI::Text("Filter usage:\n"
            "  \"\"         display all lines\n"
            "  \"xxx\"      display lines containing \"xxx\"\n"
            "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
            "  \"-xxx\"     hide lines containing \"xxx\"");
        filter.Draw();
        const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
            if (filter.PassFilter(lines[i]))
                FlexUI::BulletText("%s", lines[i]);
        FlexUI::TreePop();
    }
}

static void ShowDemoWindowLayout()
{
    FlexUI_DEMO_MARKER("Layout");
    if (!FlexUI::CollapsingHeader("Layout & Scrolling"))
        return;

    FlexUI_DEMO_MARKER("Layout/Child windows");
    if (FlexUI::TreeNode("Child windows"))
    {
        HelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window.");
        static bool disable_mouse_wheel = false;
        static bool disable_menu = false;
        FlexUI::Checkbox("Disable Mouse Wheel", &disable_mouse_wheel);
        FlexUI::Checkbox("Disable Menu", &disable_menu);

        // Child 1: no border, enable horizontal scrollbar
        {
            FlexUIWindowFlags window_flags = FlexUIWindowFlags_HorizontalScrollbar;
            if (disable_mouse_wheel)
                window_flags |= FlexUIWindowFlags_NoScrollWithMouse;
            FlexUI::BeginChild("ChildL", ImVec2(FlexUI::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
            for (int i = 0; i < 100; i++)
                FlexUI::Text("%04d: scrollable region", i);
            FlexUI::EndChild();
        }

        FlexUI::SameLine();

        // Child 2: rounded border
        {
            FlexUIWindowFlags window_flags = FlexUIWindowFlags_None;
            if (disable_mouse_wheel)
                window_flags |= FlexUIWindowFlags_NoScrollWithMouse;
            if (!disable_menu)
                window_flags |= FlexUIWindowFlags_MenuBar;
            FlexUI::PushStyleVar(FlexUIStyleVar_ChildRounding, 5.0f);
            FlexUI::BeginChild("ChildR", ImVec2(0, 260), true, window_flags);
            if (!disable_menu && FlexUI::BeginMenuBar())
            {
                if (FlexUI::BeginMenu("Menu"))
                {
                    ShowExampleMenuFile();
                    FlexUI::EndMenu();
                }
                FlexUI::EndMenuBar();
            }
            if (FlexUI::BeginTable("split", 2, FlexUITableFlags_Resizable | FlexUITableFlags_NoSavedSettings))
            {
                for (int i = 0; i < 100; i++)
                {
                    char buf[32];
                    sprintf(buf, "%03d", i);
                    FlexUI::TableNextColumn();
                    FlexUI::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                }
                FlexUI::EndTable();
            }
            FlexUI::EndChild();
            FlexUI::PopStyleVar();
        }

        FlexUI::Separator();

        // Demonstrate a few extra things
        // - Changing FlexUICol_ChildBg (which is transparent black in default styles)
        // - Using SetCursorPos() to position child window (the child window is an item from the POV of parent window)
        //   You can also call SetNextWindowPos() to position the child window. The parent window will effectively
        //   layout from this position.
        // - Using FlexUI::GetItemRectMin/Max() to query the "item" state (because the child window is an item from
        //   the POV of the parent window). See 'Demo->Querying Status (Edited/Active/Hovered etc.)' for details.
        {
            static int offset_x = 0;
            FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 8);
            FlexUI::DragInt("Offset X", &offset_x, 1.0f, -1000, 1000);

            FlexUI::SetCursorPosX(FlexUI::GetCursorPosX() + (float)offset_x);
            FlexUI::PushStyleColor(FlexUICol_ChildBg, IM_COL32(255, 0, 0, 100));
            FlexUI::BeginChild("Red", ImVec2(200, 100), true, FlexUIWindowFlags_None);
            for (int n = 0; n < 50; n++)
                FlexUI::Text("Some test %d", n);
            FlexUI::EndChild();
            bool child_is_hovered = FlexUI::IsItemHovered();
            ImVec2 child_rect_min = FlexUI::GetItemRectMin();
            ImVec2 child_rect_max = FlexUI::GetItemRectMax();
            FlexUI::PopStyleColor();
            FlexUI::Text("Hovered: %d", child_is_hovered);
            FlexUI::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)", child_rect_min.x, child_rect_min.y, child_rect_max.x, child_rect_max.y);
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Layout/Widgets Width");
    if (FlexUI::TreeNode("Widgets Width"))
    {
        static float f = 0.0f;
        static bool show_indented_items = true;
        FlexUI::Checkbox("Show indented items", &show_indented_items);

        // Use SetNextItemWidth() to set the width of a single upcoming item.
        // Use PushItemWidth()/PopItemWidth() to set the width of a group of items.
        // In real code use you'll probably want to choose width values that are proportional to your font size
        // e.g. Using '20.0f * GetFontSize()' as width instead of '200.0f', etc.

        FlexUI::Text("SetNextItemWidth/PushItemWidth(100)");
        FlexUI::SameLine(); HelpMarker("Fixed width.");
        FlexUI::PushItemWidth(100);
        FlexUI::DragFloat("float##1b", &f);
        if (show_indented_items)
        {
            FlexUI::Indent();
            FlexUI::DragFloat("float (indented)##1b", &f);
            FlexUI::Unindent();
        }
        FlexUI::PopItemWidth();

        FlexUI::Text("SetNextItemWidth/PushItemWidth(-100)");
        FlexUI::SameLine(); HelpMarker("Align to right edge minus 100");
        FlexUI::PushItemWidth(-100);
        FlexUI::DragFloat("float##2a", &f);
        if (show_indented_items)
        {
            FlexUI::Indent();
            FlexUI::DragFloat("float (indented)##2b", &f);
            FlexUI::Unindent();
        }
        FlexUI::PopItemWidth();

        FlexUI::Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
        FlexUI::SameLine(); HelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
        FlexUI::PushItemWidth(FlexUI::GetContentRegionAvail().x * 0.5f);
        FlexUI::DragFloat("float##3a", &f);
        if (show_indented_items)
        {
            FlexUI::Indent();
            FlexUI::DragFloat("float (indented)##3b", &f);
            FlexUI::Unindent();
        }
        FlexUI::PopItemWidth();

        FlexUI::Text("SetNextItemWidth/PushItemWidth(-GetContentRegionAvail().x * 0.5f)");
        FlexUI::SameLine(); HelpMarker("Align to right edge minus half");
        FlexUI::PushItemWidth(-FlexUI::GetContentRegionAvail().x * 0.5f);
        FlexUI::DragFloat("float##4a", &f);
        if (show_indented_items)
        {
            FlexUI::Indent();
            FlexUI::DragFloat("float (indented)##4b", &f);
            FlexUI::Unindent();
        }
        FlexUI::PopItemWidth();

        // Demonstrate using PushItemWidth to surround three items.
        // Calling SetNextItemWidth() before each of them would have the same effect.
        FlexUI::Text("SetNextItemWidth/PushItemWidth(-FLT_MIN)");
        FlexUI::SameLine(); HelpMarker("Align to right edge");
        FlexUI::PushItemWidth(-FLT_MIN);
        FlexUI::DragFloat("##float5a", &f);
        if (show_indented_items)
        {
            FlexUI::Indent();
            FlexUI::DragFloat("float (indented)##5b", &f);
            FlexUI::Unindent();
        }
        FlexUI::PopItemWidth();

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Layout/Basic Horizontal Layout");
    if (FlexUI::TreeNode("Basic Horizontal Layout"))
    {
        FlexUI::TextWrapped("(Use FlexUI::SameLine() to keep adding items to the right of the preceding item)");

        // Text
        FlexUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine");
        FlexUI::Text("Two items: Hello"); FlexUI::SameLine();
        FlexUI::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

        // Adjust spacing
        FlexUI::Text("More spacing: Hello"); FlexUI::SameLine(0, 20);
        FlexUI::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

        // Button
        FlexUI::AlignTextToFramePadding();
        FlexUI::Text("Normal buttons"); FlexUI::SameLine();
        FlexUI::Button("Banana"); FlexUI::SameLine();
        FlexUI::Button("Apple"); FlexUI::SameLine();
        FlexUI::Button("Corniflower");

        // Button
        FlexUI::Text("Small buttons"); FlexUI::SameLine();
        FlexUI::SmallButton("Like this one"); FlexUI::SameLine();
        FlexUI::Text("can fit within a text block.");

        // Aligned to arbitrary position. Easy/cheap column.
        FlexUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine (with offset)");
        FlexUI::Text("Aligned");
        FlexUI::SameLine(150); FlexUI::Text("x=150");
        FlexUI::SameLine(300); FlexUI::Text("x=300");
        FlexUI::Text("Aligned");
        FlexUI::SameLine(150); FlexUI::SmallButton("x=150");
        FlexUI::SameLine(300); FlexUI::SmallButton("x=300");

        // Checkbox
        FlexUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine (more)");
        static bool c1 = false, c2 = false, c3 = false, c4 = false;
        FlexUI::Checkbox("My", &c1); FlexUI::SameLine();
        FlexUI::Checkbox("Tailor", &c2); FlexUI::SameLine();
        FlexUI::Checkbox("Is", &c3); FlexUI::SameLine();
        FlexUI::Checkbox("Rich", &c4);

        // Various
        static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
        FlexUI::PushItemWidth(80);
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
        static int item = -1;
        FlexUI::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); FlexUI::SameLine();
        FlexUI::SliderFloat("X", &f0, 0.0f, 5.0f); FlexUI::SameLine();
        FlexUI::SliderFloat("Y", &f1, 0.0f, 5.0f); FlexUI::SameLine();
        FlexUI::SliderFloat("Z", &f2, 0.0f, 5.0f);
        FlexUI::PopItemWidth();

        FlexUI::PushItemWidth(80);
        FlexUI::Text("Lists:");
        static int selection[4] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) FlexUI::SameLine();
            FlexUI::PushID(i);
            FlexUI::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
            FlexUI::PopID();
            //if (FlexUI::IsItemHovered()) FlexUI::SetTooltip("ListBox %d hovered", i);
        }
        FlexUI::PopItemWidth();

        // Dummy
        FlexUI_DEMO_MARKER("Layout/Basic Horizontal Layout/Dummy");
        ImVec2 button_sz(40, 40);
        FlexUI::Button("A", button_sz); FlexUI::SameLine();
        FlexUI::Dummy(button_sz); FlexUI::SameLine();
        FlexUI::Button("B", button_sz);

        // Manually wrapping
        // (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
        FlexUI_DEMO_MARKER("Layout/Basic Horizontal Layout/Manual wrapping");
        FlexUI::Text("Manual wrapping:");
        FlexUIStyle& style = FlexUI::GetStyle();
        int buttons_count = 20;
        float window_visible_x2 = FlexUI::GetWindowPos().x + FlexUI::GetWindowContentRegionMax().x;
        for (int n = 0; n < buttons_count; n++)
        {
            FlexUI::PushID(n);
            FlexUI::Button("Box", button_sz);
            float last_button_x2 = FlexUI::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                FlexUI::SameLine();
            FlexUI::PopID();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Layout/Groups");
    if (FlexUI::TreeNode("Groups"))
    {
        HelpMarker(
            "BeginGroup() basically locks the horizontal position for new line. "
            "EndGroup() bundles the whole group so that you can use \"item\" functions such as "
            "IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group.");
        FlexUI::BeginGroup();
        {
            FlexUI::BeginGroup();
            FlexUI::Button("AAA");
            FlexUI::SameLine();
            FlexUI::Button("BBB");
            FlexUI::SameLine();
            FlexUI::BeginGroup();
            FlexUI::Button("CCC");
            FlexUI::Button("DDD");
            FlexUI::EndGroup();
            FlexUI::SameLine();
            FlexUI::Button("EEE");
            FlexUI::EndGroup();
            if (FlexUI::IsItemHovered())
                FlexUI::SetTooltip("First group hovered");
        }
        // Capture the group size and create widgets using the same size
        ImVec2 size = FlexUI::GetItemRectSize();
        const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
        FlexUI::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

        FlexUI::Button("ACTION", ImVec2((size.x - FlexUI::GetStyle().ItemSpacing.x) * 0.5f, size.y));
        FlexUI::SameLine();
        FlexUI::Button("REACTION", ImVec2((size.x - FlexUI::GetStyle().ItemSpacing.x) * 0.5f, size.y));
        FlexUI::EndGroup();
        FlexUI::SameLine();

        FlexUI::Button("LEVERAGE\nBUZZWORD", size);
        FlexUI::SameLine();

        if (FlexUI::BeginListBox("List", size))
        {
            FlexUI::Selectable("Selected", true);
            FlexUI::Selectable("Not Selected", false);
            FlexUI::EndListBox();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Layout/Text Baseline Alignment");
    if (FlexUI::TreeNode("Text Baseline Alignment"))
    {
        {
            FlexUI::BulletText("Text baseline:");
            FlexUI::SameLine(); HelpMarker(
                "This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. "
                "Lines only composed of text or \"small\" widgets use less vertical space than lines with framed widgets.");
            FlexUI::Indent();

            FlexUI::Text("KO Blahblah"); FlexUI::SameLine();
            FlexUI::Button("Some framed item"); FlexUI::SameLine();
            HelpMarker("Baseline of button will look misaligned with text..");

            // If your line starts with text, call AlignTextToFramePadding() to align text to upcoming widgets.
            // (because we don't know what's coming after the Text() statement, we need to move the text baseline
            // down by FramePadding.y ahead of time)
            FlexUI::AlignTextToFramePadding();
            FlexUI::Text("OK Blahblah"); FlexUI::SameLine();
            FlexUI::Button("Some framed item"); FlexUI::SameLine();
            HelpMarker("We call AlignTextToFramePadding() to vertically align the text baseline by +FramePadding.y");

            // SmallButton() uses the same vertical padding as Text
            FlexUI::Button("TEST##1"); FlexUI::SameLine();
            FlexUI::Text("TEST"); FlexUI::SameLine();
            FlexUI::SmallButton("TEST##2");

            // If your line starts with text, call AlignTextToFramePadding() to align text to upcoming widgets.
            FlexUI::AlignTextToFramePadding();
            FlexUI::Text("Text aligned to framed item"); FlexUI::SameLine();
            FlexUI::Button("Item##1"); FlexUI::SameLine();
            FlexUI::Text("Item"); FlexUI::SameLine();
            FlexUI::SmallButton("Item##2"); FlexUI::SameLine();
            FlexUI::Button("Item##3");

            FlexUI::Unindent();
        }

        FlexUI::Spacing();

        {
            FlexUI::BulletText("Multi-line text:");
            FlexUI::Indent();
            FlexUI::Text("One\nTwo\nThree"); FlexUI::SameLine();
            FlexUI::Text("Hello\nWorld"); FlexUI::SameLine();
            FlexUI::Text("Banana");

            FlexUI::Text("Banana"); FlexUI::SameLine();
            FlexUI::Text("Hello\nWorld"); FlexUI::SameLine();
            FlexUI::Text("One\nTwo\nThree");

            FlexUI::Button("HOP##1"); FlexUI::SameLine();
            FlexUI::Text("Banana"); FlexUI::SameLine();
            FlexUI::Text("Hello\nWorld"); FlexUI::SameLine();
            FlexUI::Text("Banana");

            FlexUI::Button("HOP##2"); FlexUI::SameLine();
            FlexUI::Text("Hello\nWorld"); FlexUI::SameLine();
            FlexUI::Text("Banana");
            FlexUI::Unindent();
        }

        FlexUI::Spacing();

        {
            FlexUI::BulletText("Misc items:");
            FlexUI::Indent();

            // SmallButton() sets FramePadding to zero. Text baseline is aligned to match baseline of previous Button.
            FlexUI::Button("80x80", ImVec2(80, 80));
            FlexUI::SameLine();
            FlexUI::Button("50x50", ImVec2(50, 50));
            FlexUI::SameLine();
            FlexUI::Button("Button()");
            FlexUI::SameLine();
            FlexUI::SmallButton("SmallButton()");

            // Tree
            const float spacing = FlexUI::GetStyle().ItemInnerSpacing.x;
            FlexUI::Button("Button##1");
            FlexUI::SameLine(0.0f, spacing);
            if (FlexUI::TreeNode("Node##1"))
            {
                // Placeholder tree data
                for (int i = 0; i < 6; i++)
                    FlexUI::BulletText("Item %d..", i);
                FlexUI::TreePop();
            }

            // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget.
            // Otherwise you can use SmallButton() (smaller fit).
            FlexUI::AlignTextToFramePadding();

            // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add
            // other contents below the node.
            bool node_open = FlexUI::TreeNode("Node##2");
            FlexUI::SameLine(0.0f, spacing); FlexUI::Button("Button##2");
            if (node_open)
            {
                // Placeholder tree data
                for (int i = 0; i < 6; i++)
                    FlexUI::BulletText("Item %d..", i);
                FlexUI::TreePop();
            }

            // Bullet
            FlexUI::Button("Button##3");
            FlexUI::SameLine(0.0f, spacing);
            FlexUI::BulletText("Bullet text");

            FlexUI::AlignTextToFramePadding();
            FlexUI::BulletText("Node");
            FlexUI::SameLine(0.0f, spacing); FlexUI::Button("Button##4");
            FlexUI::Unindent();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Layout/Scrolling");
    if (FlexUI::TreeNode("Scrolling"))
    {
        // Vertical scroll functions
        FlexUI_DEMO_MARKER("Layout/Scrolling/Vertical");
        HelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given vertical position.");

        static int track_item = 50;
        static bool enable_track = true;
        static bool enable_extra_decorations = false;
        static float scroll_to_off_px = 0.0f;
        static float scroll_to_pos_px = 200.0f;

        FlexUI::Checkbox("Decoration", &enable_extra_decorations);

        FlexUI::Checkbox("Track", &enable_track);
        FlexUI::PushItemWidth(100);
        FlexUI::SameLine(140); enable_track |= FlexUI::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");

        bool scroll_to_off = FlexUI::Button("Scroll Offset");
        FlexUI::SameLine(140); scroll_to_off |= FlexUI::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, FLT_MAX, "+%.0f px");

        bool scroll_to_pos = FlexUI::Button("Scroll To Pos");
        FlexUI::SameLine(140); scroll_to_pos |= FlexUI::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, FLT_MAX, "X/Y = %.0f px");
        FlexUI::PopItemWidth();

        if (scroll_to_off || scroll_to_pos)
            enable_track = false;

        FlexUIStyle& style = FlexUI::GetStyle();
        float child_w = (FlexUI::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
        if (child_w < 1.0f)
            child_w = 1.0f;
        FlexUI::PushID("##VerticalScrolling");
        for (int i = 0; i < 5; i++)
        {
            if (i > 0) FlexUI::SameLine();
            FlexUI::BeginGroup();
            const char* names[] = { "Top", "25%", "Center", "75%", "Bottom" };
            FlexUI::TextUnformatted(names[i]);

            const FlexUIWindowFlags child_flags = enable_extra_decorations ? FlexUIWindowFlags_MenuBar : 0;
            const FlexUIID child_id = FlexUI::GetID((void*)(intptr_t)i);
            const bool child_is_visible = FlexUI::BeginChild(child_id, ImVec2(child_w, 200.0f), true, child_flags);
            if (FlexUI::BeginMenuBar())
            {
                FlexUI::TextUnformatted("abc");
                FlexUI::EndMenuBar();
            }
            if (scroll_to_off)
                FlexUI::SetScrollY(scroll_to_off_px);
            if (scroll_to_pos)
                FlexUI::SetScrollFromPosY(FlexUI::GetCursorStartPos().y + scroll_to_pos_px, i * 0.25f);
            if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
            {
                for (int item = 0; item < 100; item++)
                {
                    if (enable_track && item == track_item)
                    {
                        FlexUI::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
                        FlexUI::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
                    }
                    else
                    {
                        FlexUI::Text("Item %d", item);
                    }
                }
            }
            float scroll_y = FlexUI::GetScrollY();
            float scroll_max_y = FlexUI::GetScrollMaxY();
            FlexUI::EndChild();
            FlexUI::Text("%.0f/%.0f", scroll_y, scroll_max_y);
            FlexUI::EndGroup();
        }
        FlexUI::PopID();

        // Horizontal scroll functions
        FlexUI_DEMO_MARKER("Layout/Scrolling/Horizontal");
        FlexUI::Spacing();
        HelpMarker(
            "Use SetScrollHereX() or SetScrollFromPosX() to scroll to a given horizontal position.\n\n"
            "Because the clipping rectangle of most window hides half worth of WindowPadding on the "
            "left/right, using SetScrollFromPosX(+1) will usually result in clipped text whereas the "
            "equivalent SetScrollFromPosY(+1) wouldn't.");
        FlexUI::PushID("##HorizontalScrolling");
        for (int i = 0; i < 5; i++)
        {
            float child_height = FlexUI::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
            FlexUIWindowFlags child_flags = FlexUIWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? FlexUIWindowFlags_AlwaysVerticalScrollbar : 0);
            FlexUIID child_id = FlexUI::GetID((void*)(intptr_t)i);
            bool child_is_visible = FlexUI::BeginChild(child_id, ImVec2(-100, child_height), true, child_flags);
            if (scroll_to_off)
                FlexUI::SetScrollX(scroll_to_off_px);
            if (scroll_to_pos)
                FlexUI::SetScrollFromPosX(FlexUI::GetCursorStartPos().x + scroll_to_pos_px, i * 0.25f);
            if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
            {
                for (int item = 0; item < 100; item++)
                {
                    if (item > 0)
                        FlexUI::SameLine();
                    if (enable_track && item == track_item)
                    {
                        FlexUI::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
                        FlexUI::SetScrollHereX(i * 0.25f); // 0.0f:left, 0.5f:center, 1.0f:right
                    }
                    else
                    {
                        FlexUI::Text("Item %d", item);
                    }
                }
            }
            float scroll_x = FlexUI::GetScrollX();
            float scroll_max_x = FlexUI::GetScrollMaxX();
            FlexUI::EndChild();
            FlexUI::SameLine();
            const char* names[] = { "Left", "25%", "Center", "75%", "Right" };
            FlexUI::Text("%s\n%.0f/%.0f", names[i], scroll_x, scroll_max_x);
            FlexUI::Spacing();
        }
        FlexUI::PopID();

        // Miscellaneous Horizontal Scrolling Demo
        FlexUI_DEMO_MARKER("Layout/Scrolling/Horizontal (more)");
        HelpMarker(
            "Horizontal scrolling for a window is enabled via the FlexUIWindowFlags_HorizontalScrollbar flag.\n\n"
            "You may want to also explicitly specify content width by using SetNextWindowContentWidth() before Begin().");
        static int lines = 7;
        FlexUI::SliderInt("Lines", &lines, 1, 15);
        FlexUI::PushStyleVar(FlexUIStyleVar_FrameRounding, 3.0f);
        FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
        ImVec2 scrolling_child_size = ImVec2(0, FlexUI::GetFrameHeightWithSpacing() * 7 + 30);
        FlexUI::BeginChild("scrolling", scrolling_child_size, true, FlexUIWindowFlags_HorizontalScrollbar);
        for (int line = 0; line < lines; line++)
        {
            // Display random stuff. For the sake of this trivial demo we are using basic Button() + SameLine()
            // If you want to create your own time line for a real application you may be better off manipulating
            // the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets
            // yourself. You may also want to use the lower-level ImDrawList API.
            int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            for (int n = 0; n < num_buttons; n++)
            {
                if (n > 0) FlexUI::SameLine();
                FlexUI::PushID(n + line * 1000);
                char num_buf[16];
                sprintf(num_buf, "%d", n);
                const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf;
                float hue = n * 0.05f;
                FlexUI::PushStyleColor(FlexUICol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
                FlexUI::PushStyleColor(FlexUICol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
                FlexUI::PushStyleColor(FlexUICol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
                FlexUI::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                FlexUI::PopStyleColor(3);
                FlexUI::PopID();
            }
        }
        float scroll_x = FlexUI::GetScrollX();
        float scroll_max_x = FlexUI::GetScrollMaxX();
        FlexUI::EndChild();
        FlexUI::PopStyleVar(2);
        float scroll_x_delta = 0.0f;
        FlexUI::SmallButton("<<");
        if (FlexUI::IsItemActive())
            scroll_x_delta = -FlexUI::GetIO().DeltaTime * 1000.0f;
        FlexUI::SameLine();
        FlexUI::Text("Scroll from code"); FlexUI::SameLine();
        FlexUI::SmallButton(">>");
        if (FlexUI::IsItemActive())
            scroll_x_delta = +FlexUI::GetIO().DeltaTime * 1000.0f;
        FlexUI::SameLine();
        FlexUI::Text("%.0f/%.0f", scroll_x, scroll_max_x);
        if (scroll_x_delta != 0.0f)
        {
            // Demonstrate a trick: you can use Begin to set yourself in the context of another window
            // (here we are already out of your child window)
            FlexUI::BeginChild("scrolling");
            FlexUI::SetScrollX(FlexUI::GetScrollX() + scroll_x_delta);
            FlexUI::EndChild();
        }
        FlexUI::Spacing();

        static bool show_horizontal_contents_size_demo_window = false;
        FlexUI::Checkbox("Show Horizontal contents size demo window", &show_horizontal_contents_size_demo_window);

        if (show_horizontal_contents_size_demo_window)
        {
            static bool show_h_scrollbar = true;
            static bool show_button = true;
            static bool show_tree_nodes = true;
            static bool show_text_wrapped = false;
            static bool show_columns = true;
            static bool show_tab_bar = true;
            static bool show_child = false;
            static bool explicit_content_size = false;
            static float contents_size_x = 300.0f;
            if (explicit_content_size)
                FlexUI::SetNextWindowContentSize(ImVec2(contents_size_x, 0.0f));
            FlexUI::Begin("Horizontal contents size demo window", &show_horizontal_contents_size_demo_window, show_h_scrollbar ? FlexUIWindowFlags_HorizontalScrollbar : 0);
            FlexUI_DEMO_MARKER("Layout/Scrolling/Horizontal contents size demo window");
            FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(2, 0));
            FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(2, 0));
            HelpMarker("Test of different widgets react and impact the work rectangle growing when horizontal scrolling is enabled.\n\nUse 'Metrics->Tools->Show windows rectangles' to visualize rectangles.");
            FlexUI::Checkbox("H-scrollbar", &show_h_scrollbar);
            FlexUI::Checkbox("Button", &show_button);            // Will grow contents size (unless explicitly overwritten)
            FlexUI::Checkbox("Tree nodes", &show_tree_nodes);    // Will grow contents size and display highlight over full width
            FlexUI::Checkbox("Text wrapped", &show_text_wrapped);// Will grow and use contents size
            FlexUI::Checkbox("Columns", &show_columns);          // Will use contents size
            FlexUI::Checkbox("Tab bar", &show_tab_bar);          // Will use contents size
            FlexUI::Checkbox("Child", &show_child);              // Will grow and use contents size
            FlexUI::Checkbox("Explicit content size", &explicit_content_size);
            FlexUI::Text("Scroll %.1f/%.1f %.1f/%.1f", FlexUI::GetScrollX(), FlexUI::GetScrollMaxX(), FlexUI::GetScrollY(), FlexUI::GetScrollMaxY());
            if (explicit_content_size)
            {
                FlexUI::SameLine();
                FlexUI::SetNextItemWidth(100);
                FlexUI::DragFloat("##csx", &contents_size_x);
                ImVec2 p = FlexUI::GetCursorScreenPos();
                FlexUI::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 10, p.y + 10), IM_COL32_WHITE);
                FlexUI::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + contents_size_x - 10, p.y), ImVec2(p.x + contents_size_x, p.y + 10), IM_COL32_WHITE);
                FlexUI::Dummy(ImVec2(0, 10));
            }
            FlexUI::PopStyleVar(2);
            FlexUI::Separator();
            if (show_button)
            {
                FlexUI::Button("this is a 300-wide button", ImVec2(300, 0));
            }
            if (show_tree_nodes)
            {
                bool open = true;
                if (FlexUI::TreeNode("this is a tree node"))
                {
                    if (FlexUI::TreeNode("another one of those tree node..."))
                    {
                        FlexUI::Text("Some tree contents");
                        FlexUI::TreePop();
                    }
                    FlexUI::TreePop();
                }
                FlexUI::CollapsingHeader("CollapsingHeader", &open);
            }
            if (show_text_wrapped)
            {
                FlexUI::TextWrapped("This text should automatically wrap on the edge of the work rectangle.");
            }
            if (show_columns)
            {
                FlexUI::Text("Tables:");
                if (FlexUI::BeginTable("table", 4, FlexUITableFlags_Borders))
                {
                    for (int n = 0; n < 4; n++)
                    {
                        FlexUI::TableNextColumn();
                        FlexUI::Text("Width %.2f", FlexUI::GetContentRegionAvail().x);
                    }
                    FlexUI::EndTable();
                }
                FlexUI::Text("Columns:");
                FlexUI::Columns(4);
                for (int n = 0; n < 4; n++)
                {
                    FlexUI::Text("Width %.2f", FlexUI::GetColumnWidth());
                    FlexUI::NextColumn();
                }
                FlexUI::Columns(1);
            }
            if (show_tab_bar && FlexUI::BeginTabBar("Hello"))
            {
                if (FlexUI::BeginTabItem("OneOneOne")) { FlexUI::EndTabItem(); }
                if (FlexUI::BeginTabItem("TwoTwoTwo")) { FlexUI::EndTabItem(); }
                if (FlexUI::BeginTabItem("ThreeThreeThree")) { FlexUI::EndTabItem(); }
                if (FlexUI::BeginTabItem("FourFourFour")) { FlexUI::EndTabItem(); }
                FlexUI::EndTabBar();
            }
            if (show_child)
            {
                FlexUI::BeginChild("child", ImVec2(0, 0), true);
                FlexUI::EndChild();
            }
            FlexUI::End();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Layout/Clipping");
    if (FlexUI::TreeNode("Clipping"))
    {
        static ImVec2 size(100.0f, 100.0f);
        static ImVec2 offset(30.0f, 30.0f);
        FlexUI::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
        FlexUI::TextWrapped("(Click and drag to scroll)");

        HelpMarker(
            "(Left) Using FlexUI::PushClipRect():\n"
            "Will alter FlexUI hit-testing logic + ImDrawList rendering.\n"
            "(use this if you want your clipping rectangle to affect interactions)\n\n"
            "(Center) Using ImDrawList::PushClipRect():\n"
            "Will alter ImDrawList rendering only.\n"
            "(use this as a shortcut if you are only using ImDrawList calls)\n\n"
            "(Right) Using ImDrawList::AddText() with a fine ClipRect:\n"
            "Will alter only this specific ImDrawList::AddText() rendering.\n"
            "This is often used internally to avoid altering the clipping rectangle and minimize draw calls.");

        for (int n = 0; n < 3; n++)
        {
            if (n > 0)
                FlexUI::SameLine();

            FlexUI::PushID(n);
            FlexUI::InvisibleButton("##canvas", size);
            if (FlexUI::IsItemActive() && FlexUI::IsMouseDragging(FlexUIMouseButton_Left))
            {
                offset.x += FlexUI::GetIO().MouseDelta.x;
                offset.y += FlexUI::GetIO().MouseDelta.y;
            }
            FlexUI::PopID();
            if (!FlexUI::IsItemVisible()) // Skip rendering as ImDrawList elements are not clipped.
                continue;

            const ImVec2 p0 = FlexUI::GetItemRectMin();
            const ImVec2 p1 = FlexUI::GetItemRectMax();
            const char* text_str = "Line 1 hello\nLine 2 clip me!";
            const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);
            ImDrawList* draw_list = FlexUI::GetWindowDrawList();
            switch (n)
            {
            case 0:
                FlexUI::PushClipRect(p0, p1, true);
                draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
                FlexUI::PopClipRect();
                break;
            case 1:
                draw_list->PushClipRect(p0, p1, true);
                draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
                draw_list->PopClipRect();
                break;
            case 2:
                ImVec4 clip_rect(p0.x, p0.y, p1.x, p1.y); // AddText() takes a ImVec4* here so let's convert.
                draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                draw_list->AddText(FlexUI::GetFont(), FlexUI::GetFontSize(), text_pos, IM_COL32_WHITE, text_str, NULL, 0.0f, &clip_rect);
                break;
            }
        }

        FlexUI::TreePop();
    }
}

static void ShowDemoWindowPopups()
{
    FlexUI_DEMO_MARKER("Popups");
    if (!FlexUI::CollapsingHeader("Popups & Modal windows"))
        return;

    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by Dear FlexUI instead of being held by the programmer as
    //   we are used to with regular Begin() calls. User can manipulate the visibility state by calling OpenPopup().
    // (*) One can use IsItemHovered(FlexUIHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even
    //     when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state BECAUSE it can close
    // popups at any time.

    // Typical use for regular windows:
    //   bool my_tool_is_active = false; if (FlexUI::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
    // Typical use for popups:
    //   if (FlexUI::Button("Open")) FlexUI::OpenPopup("MyPopup"); if (FlexUI::BeginPopup("MyPopup") { [...] EndPopup(); }

    // With popups we have to go through a library call (here OpenPopup) to manipulate the visibility state.
    // This may be a bit confusing at first but it should quickly make sense. Follow on the examples below.

    FlexUI_DEMO_MARKER("Popups/Popups");
    if (FlexUI::TreeNode("Popups"))
    {
        FlexUI::TextWrapped(
            "When a popup is active, it inhibits interacting with windows that are behind the popup. "
            "Clicking outside the popup closes it.");

        static int selected_fish = -1;
        const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
        static bool toggles[] = { true, false, false, false, false };

        // Simple selection popup (if you want to show the current selection inside the Button itself,
        // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
        if (FlexUI::Button("Select.."))
            FlexUI::OpenPopup("my_select_popup");
        FlexUI::SameLine();
        FlexUI::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (FlexUI::BeginPopup("my_select_popup"))
        {
            FlexUI::Text("Aquarium");
            FlexUI::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (FlexUI::Selectable(names[i]))
                    selected_fish = i;
            FlexUI::EndPopup();
        }

        // Showing a menu with toggles
        if (FlexUI::Button("Toggle.."))
            FlexUI::OpenPopup("my_toggle_popup");
        if (FlexUI::BeginPopup("my_toggle_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                FlexUI::MenuItem(names[i], "", &toggles[i]);
            if (FlexUI::BeginMenu("Sub-menu"))
            {
                FlexUI::MenuItem("Click me");
                FlexUI::EndMenu();
            }

            FlexUI::Separator();
            FlexUI::Text("Tooltip here");
            if (FlexUI::IsItemHovered())
                FlexUI::SetTooltip("I am a tooltip over a popup");

            if (FlexUI::Button("Stacked Popup"))
                FlexUI::OpenPopup("another popup");
            if (FlexUI::BeginPopup("another popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    FlexUI::MenuItem(names[i], "", &toggles[i]);
                if (FlexUI::BeginMenu("Sub-menu"))
                {
                    FlexUI::MenuItem("Click me");
                    if (FlexUI::Button("Stacked Popup"))
                        FlexUI::OpenPopup("another popup");
                    if (FlexUI::BeginPopup("another popup"))
                    {
                        FlexUI::Text("I am the last one here.");
                        FlexUI::EndPopup();
                    }
                    FlexUI::EndMenu();
                }
                FlexUI::EndPopup();
            }
            FlexUI::EndPopup();
        }

        // Call the more complete ShowExampleMenuFile which we use in various places of this demo
        if (FlexUI::Button("With a menu.."))
            FlexUI::OpenPopup("my_file_popup");
        if (FlexUI::BeginPopup("my_file_popup", FlexUIWindowFlags_MenuBar))
        {
            if (FlexUI::BeginMenuBar())
            {
                if (FlexUI::BeginMenu("File"))
                {
                    ShowExampleMenuFile();
                    FlexUI::EndMenu();
                }
                if (FlexUI::BeginMenu("Edit"))
                {
                    FlexUI::MenuItem("Dummy");
                    FlexUI::EndMenu();
                }
                FlexUI::EndMenuBar();
            }
            FlexUI::Text("Hello from popup!");
            FlexUI::Button("This is a dummy button..");
            FlexUI::EndPopup();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Popups/Context menus");
    if (FlexUI::TreeNode("Context menus"))
    {
        HelpMarker("\"Context\" functions are simple helpers to associate a Popup to a given Item or Window identifier.");

        // BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
        //     if (id == 0)
        //         id = GetItemID(); // Use last item id
        //     if (IsItemHovered() && IsMouseReleased(FlexUIMouseButton_Right))
        //         OpenPopup(id);
        //     return BeginPopup(id);
        // For advanced uses you may want to replicate and customize this code.
        // See more details in BeginPopupContextItem().

        // Example 1
        // When used after an item that has an ID (e.g. Button), we can skip providing an ID to BeginPopupContextItem(),
        // and BeginPopupContextItem() will use the last item ID as the popup ID.
        {
            const char* names[5] = { "Label1", "Label2", "Label3", "Label4", "Label5" };
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                if (FlexUI::Selectable(names[n], selected == n))
                    selected = n;
                if (FlexUI::BeginPopupContextItem()) // <-- use last item id as popup id
                {
                    selected = n;
                    FlexUI::Text("This a popup for \"%s\"!", names[n]);
                    if (FlexUI::Button("Close"))
                        FlexUI::CloseCurrentPopup();
                    FlexUI::EndPopup();
                }
                if (FlexUI::IsItemHovered())
                    FlexUI::SetTooltip("Right-click to open popup");
            }
        }

        // Example 2
        // Popup on a Text() element which doesn't have an identifier: we need to provide an identifier to BeginPopupContextItem().
        // Using an explicit identifier is also convenient if you want to activate the popups from different locations.
        {
            HelpMarker("Text() elements don't have stable identifiers so we need to provide one.");
            static float value = 0.5f;
            FlexUI::Text("Value = %.3f <-- (1) right-click this text", value);
            if (FlexUI::BeginPopupContextItem("my popup"))
            {
                if (FlexUI::Selectable("Set to zero")) value = 0.0f;
                if (FlexUI::Selectable("Set to PI")) value = 3.1415f;
                FlexUI::SetNextItemWidth(-FLT_MIN);
                FlexUI::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
                FlexUI::EndPopup();
            }

            // We can also use OpenPopupOnItemClick() to toggle the visibility of a given popup.
            // Here we make it that right-clicking this other text element opens the same popup as above.
            // The popup itself will be submitted by the code above.
            FlexUI::Text("(2) Or right-click this text");
            FlexUI::OpenPopupOnItemClick("my popup", FlexUIPopupFlags_MouseButtonRight);

            // Back to square one: manually open the same popup.
            if (FlexUI::Button("(3) Or click this button"))
                FlexUI::OpenPopup("my popup");
        }

        // Example 3
        // When using BeginPopupContextItem() with an implicit identifier (NULL == use last item ID),
        // we need to make sure your item identifier is stable.
        // In this example we showcase altering the item label while preserving its identifier, using the ### operator (see FAQ).
        {
            HelpMarker("Showcase using a popup ID linked to item ID, with the item having a changing label + stable ID using the ### operator.");
            static char name[32] = "Label1";
            char buf[64];
            sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
            FlexUI::Button(buf);
            if (FlexUI::BeginPopupContextItem())
            {
                FlexUI::Text("Edit name:");
                FlexUI::InputText("##edit", name, IM_ARRAYSIZE(name));
                if (FlexUI::Button("Close"))
                    FlexUI::CloseCurrentPopup();
                FlexUI::EndPopup();
            }
            FlexUI::SameLine(); FlexUI::Text("(<-- right-click here)");
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Popups/Modals");
    if (FlexUI::TreeNode("Modals"))
    {
        FlexUI::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside.");

        if (FlexUI::Button("Delete.."))
            FlexUI::OpenPopup("Delete?");

        // Always center this window when appearing
        ImVec2 center = FlexUI::GetMainViewport()->GetCenter();
        FlexUI::SetNextWindowPos(center, FlexUICond_Appearing, ImVec2(0.5f, 0.5f));

        if (FlexUI::BeginPopupModal("Delete?", NULL, FlexUIWindowFlags_AlwaysAutoResize))
        {
            FlexUI::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
            FlexUI::Separator();

            //static int unused_i = 0;
            //FlexUI::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(0, 0));
            FlexUI::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            FlexUI::PopStyleVar();

            if (FlexUI::Button("OK", ImVec2(120, 0))) { FlexUI::CloseCurrentPopup(); }
            FlexUI::SetItemDefaultFocus();
            FlexUI::SameLine();
            if (FlexUI::Button("Cancel", ImVec2(120, 0))) { FlexUI::CloseCurrentPopup(); }
            FlexUI::EndPopup();
        }

        if (FlexUI::Button("Stacked modals.."))
            FlexUI::OpenPopup("Stacked 1");
        if (FlexUI::BeginPopupModal("Stacked 1", NULL, FlexUIWindowFlags_MenuBar))
        {
            if (FlexUI::BeginMenuBar())
            {
                if (FlexUI::BeginMenu("File"))
                {
                    if (FlexUI::MenuItem("Some menu item")) {}
                    FlexUI::EndMenu();
                }
                FlexUI::EndMenuBar();
            }
            FlexUI::Text("Hello from Stacked The First\nUsing style.Colors[FlexUICol_ModalWindowDimBg] behind it.");

            // Testing behavior of widgets stacking their own regular popups over the modal.
            static int item = 1;
            static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
            FlexUI::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
            FlexUI::ColorEdit4("color", color);

            if (FlexUI::Button("Add another modal.."))
                FlexUI::OpenPopup("Stacked 2");

            // Also demonstrate passing a bool* to BeginPopupModal(), this will create a regular close button which
            // will close the popup. Note that the visibility state of popups is owned by FlexUI, so the input value
            // of the bool actually doesn't matter here.
            bool unused_open = true;
            if (FlexUI::BeginPopupModal("Stacked 2", &unused_open))
            {
                FlexUI::Text("Hello from Stacked The Second!");
                if (FlexUI::Button("Close"))
                    FlexUI::CloseCurrentPopup();
                FlexUI::EndPopup();
            }

            if (FlexUI::Button("Close"))
                FlexUI::CloseCurrentPopup();
            FlexUI::EndPopup();
        }

        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Popups/Menus inside a regular window");
    if (FlexUI::TreeNode("Menus inside a regular window"))
    {
        FlexUI::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
        FlexUI::Separator();

        FlexUI::MenuItem("Menu item", "CTRL+M");
        if (FlexUI::BeginMenu("Menu inside a regular window"))
        {
            ShowExampleMenuFile();
            FlexUI::EndMenu();
        }
        FlexUI::Separator();
        FlexUI::TreePop();
    }
}

// Dummy data structure that we use for the Table demo.
// (pre-C++11 doesn't allow us to instantiate ImVector<MyItem> template if this structure is defined inside the demo function)
namespace
{
    // We are passing our own identifier to TableSetupColumn() to facilitate identifying columns in the sorting code.
    // This identifier will be passed down into FlexUITableSortSpec::ColumnUserID.
    // But it is possible to omit the user id parameter of TableSetupColumn() and just use the column index instead! (FlexUITableSortSpec::ColumnIndex)
    // If you don't use sorting, you will generally never care about giving column an ID!
    enum MyItemColumnID
    {
        MyItemColumnID_ID,
        MyItemColumnID_Name,
        MyItemColumnID_Action,
        MyItemColumnID_Quantity,
        MyItemColumnID_Description
    };

    struct MyItem
    {
        int         ID;
        const char* Name;
        int         Quantity;

        // We have a problem which is affecting _only this demo_ and should not affect your code:
        // As we don't rely on std:: or other third-party library to compile dear FlexUI, we only have reliable access to qsort(),
        // however qsort doesn't allow passing user data to comparing function.
        // As a workaround, we are storing the sort specs in a static/global for the comparing function to access.
        // In your own use case you would probably pass the sort specs to your sorting/comparing functions directly and not use a global.
        // We could technically call FlexUI::TableGetSortSpecs() in CompareWithSortSpecs(), but considering that this function is called
        // very often by the sorting algorithm it would be a little wasteful.
        static const FlexUITableSortSpecs* s_current_sort_specs;

        // Compare function to be used by qsort()
        static int FlexUI_CDECL CompareWithSortSpecs(const void* lhs, const void* rhs)
        {
            const MyItem* a = (const MyItem*)lhs;
            const MyItem* b = (const MyItem*)rhs;
            for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
            {
                // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
                // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
                const FlexUITableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
                int delta = 0;
                switch (sort_spec->ColumnUserID)
                {
                case MyItemColumnID_ID:             delta = (a->ID - b->ID);                break;
                case MyItemColumnID_Name:           delta = (strcmp(a->Name, b->Name));     break;
                case MyItemColumnID_Quantity:       delta = (a->Quantity - b->Quantity);    break;
                case MyItemColumnID_Description:    delta = (strcmp(a->Name, b->Name));     break;
                default: IM_ASSERT(0); break;
                }
                if (delta > 0)
                    return (sort_spec->SortDirection == FlexUISortDirection_Ascending) ? +1 : -1;
                if (delta < 0)
                    return (sort_spec->SortDirection == FlexUISortDirection_Ascending) ? -1 : +1;
            }

            // qsort() is instable so always return a way to differenciate items.
            // Your own compare function may want to avoid fallback on implicit sort specs e.g. a Name compare if it wasn't already part of the sort specs.
            return (a->ID - b->ID);
        }
    };
    const FlexUITableSortSpecs* MyItem::s_current_sort_specs = NULL;
}

// Make the UI compact because there are so many fields
static void PushStyleCompact()
{
    FlexUIStyle& style = FlexUI::GetStyle();
    FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
    FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
}

static void PopStyleCompact()
{
    FlexUI::PopStyleVar(2);
}

// Show a combo box with a choice of sizing policies
static void EditTableSizingFlags(FlexUITableFlags* p_flags)
{
    struct EnumDesc { FlexUITableFlags Value; const char* Name; const char* Tooltip; };
    static const EnumDesc policies[] =
    {
        { FlexUITableFlags_None,               "Default",                            "Use default sizing policy:\n- FlexUITableFlags_SizingFixedFit if ScrollX is on or if host window has FlexUIWindowFlags_AlwaysAutoResize.\n- FlexUITableFlags_SizingStretchSame otherwise." },
        { FlexUITableFlags_SizingFixedFit,     "FlexUITableFlags_SizingFixedFit",     "Columns default to _WidthFixed (if resizable) or _WidthAuto (if not resizable), matching contents width." },
        { FlexUITableFlags_SizingFixedSame,    "FlexUITableFlags_SizingFixedSame",    "Columns are all the same width, matching the maximum contents width.\nImplicitly disable FlexUITableFlags_Resizable and enable FlexUITableFlags_NoKeepColumnsVisible." },
        { FlexUITableFlags_SizingStretchProp,  "FlexUITableFlags_SizingStretchProp",  "Columns default to _WidthStretch with weights proportional to their widths." },
        { FlexUITableFlags_SizingStretchSame,  "FlexUITableFlags_SizingStretchSame",  "Columns default to _WidthStretch with same weights." }
    };
    int idx;
    for (idx = 0; idx < IM_ARRAYSIZE(policies); idx++)
        if (policies[idx].Value == (*p_flags & FlexUITableFlags_SizingMask_))
            break;
    const char* preview_text = (idx < IM_ARRAYSIZE(policies)) ? policies[idx].Name + (idx > 0 ? strlen("FlexUITableFlags") : 0) : "";
    if (FlexUI::BeginCombo("Sizing Policy", preview_text))
    {
        for (int n = 0; n < IM_ARRAYSIZE(policies); n++)
            if (FlexUI::Selectable(policies[n].Name, idx == n))
                *p_flags = (*p_flags & ~FlexUITableFlags_SizingMask_) | policies[n].Value;
        FlexUI::EndCombo();
    }
    FlexUI::SameLine();
    FlexUI::TextDisabled("(?)");
    if (FlexUI::IsItemHovered())
    {
        FlexUI::BeginTooltip();
        FlexUI::PushTextWrapPos(FlexUI::GetFontSize() * 50.0f);
        for (int m = 0; m < IM_ARRAYSIZE(policies); m++)
        {
            FlexUI::Separator();
            FlexUI::Text("%s:", policies[m].Name);
            FlexUI::Separator();
            FlexUI::SetCursorPosX(FlexUI::GetCursorPosX() + FlexUI::GetStyle().IndentSpacing * 0.5f);
            FlexUI::TextUnformatted(policies[m].Tooltip);
        }
        FlexUI::PopTextWrapPos();
        FlexUI::EndTooltip();
    }
}

static void EditTableColumnsFlags(FlexUITableColumnFlags* p_flags)
{
    FlexUI::CheckboxFlags("_Disabled", p_flags, FlexUITableColumnFlags_Disabled); FlexUI::SameLine(); HelpMarker("Master disable flag (also hide from context menu)");
    FlexUI::CheckboxFlags("_DefaultHide", p_flags, FlexUITableColumnFlags_DefaultHide);
    FlexUI::CheckboxFlags("_DefaultSort", p_flags, FlexUITableColumnFlags_DefaultSort);
    if (FlexUI::CheckboxFlags("_WidthStretch", p_flags, FlexUITableColumnFlags_WidthStretch))
        *p_flags &= ~(FlexUITableColumnFlags_WidthMask_ ^ FlexUITableColumnFlags_WidthStretch);
    if (FlexUI::CheckboxFlags("_WidthFixed", p_flags, FlexUITableColumnFlags_WidthFixed))
        *p_flags &= ~(FlexUITableColumnFlags_WidthMask_ ^ FlexUITableColumnFlags_WidthFixed);
    FlexUI::CheckboxFlags("_NoResize", p_flags, FlexUITableColumnFlags_NoResize);
    FlexUI::CheckboxFlags("_NoReorder", p_flags, FlexUITableColumnFlags_NoReorder);
    FlexUI::CheckboxFlags("_NoHide", p_flags, FlexUITableColumnFlags_NoHide);
    FlexUI::CheckboxFlags("_NoClip", p_flags, FlexUITableColumnFlags_NoClip);
    FlexUI::CheckboxFlags("_NoSort", p_flags, FlexUITableColumnFlags_NoSort);
    FlexUI::CheckboxFlags("_NoSortAscending", p_flags, FlexUITableColumnFlags_NoSortAscending);
    FlexUI::CheckboxFlags("_NoSortDescending", p_flags, FlexUITableColumnFlags_NoSortDescending);
    FlexUI::CheckboxFlags("_NoHeaderLabel", p_flags, FlexUITableColumnFlags_NoHeaderLabel);
    FlexUI::CheckboxFlags("_NoHeaderWidth", p_flags, FlexUITableColumnFlags_NoHeaderWidth);
    FlexUI::CheckboxFlags("_PreferSortAscending", p_flags, FlexUITableColumnFlags_PreferSortAscending);
    FlexUI::CheckboxFlags("_PreferSortDescending", p_flags, FlexUITableColumnFlags_PreferSortDescending);
    FlexUI::CheckboxFlags("_IndentEnable", p_flags, FlexUITableColumnFlags_IndentEnable); FlexUI::SameLine(); HelpMarker("Default for column 0");
    FlexUI::CheckboxFlags("_IndentDisable", p_flags, FlexUITableColumnFlags_IndentDisable); FlexUI::SameLine(); HelpMarker("Default for column >0");
}

static void ShowTableColumnsStatusFlags(FlexUITableColumnFlags flags)
{
    FlexUI::CheckboxFlags("_IsEnabled", &flags, FlexUITableColumnFlags_IsEnabled);
    FlexUI::CheckboxFlags("_IsVisible", &flags, FlexUITableColumnFlags_IsVisible);
    FlexUI::CheckboxFlags("_IsSorted", &flags, FlexUITableColumnFlags_IsSorted);
    FlexUI::CheckboxFlags("_IsHovered", &flags, FlexUITableColumnFlags_IsHovered);
}

static void ShowDemoWindowTables()
{
    //FlexUI::SetNextItemOpen(true, FlexUICond_Once);
    FlexUI_DEMO_MARKER("Tables");
    if (!FlexUI::CollapsingHeader("Tables & Columns"))
        return;

    // Using those as a base value to create width/height that are factor of the size of our font
    const float TEXT_BASE_WIDTH = FlexUI::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = FlexUI::GetTextLineHeightWithSpacing();

    FlexUI::PushID("Tables");

    int open_action = -1;
    if (FlexUI::Button("Open all"))
        open_action = 1;
    FlexUI::SameLine();
    if (FlexUI::Button("Close all"))
        open_action = 0;
    FlexUI::SameLine();

    // Options
    static bool disable_indent = false;
    FlexUI::Checkbox("Disable tree indentation", &disable_indent);
    FlexUI::SameLine();
    HelpMarker("Disable the indenting of tree nodes so demo tables can use the full window width.");
    FlexUI::Separator();
    if (disable_indent)
        FlexUI::PushStyleVar(FlexUIStyleVar_IndentSpacing, 0.0f);

    // About Styling of tables
    // Most settings are configured on a per-table basis via the flags passed to BeginTable() and TableSetupColumns APIs.
    // There are however a few settings that a shared and part of the FlexUIStyle structure:
    //   style.CellPadding                          // Padding within each cell
    //   style.Colors[FlexUICol_TableHeaderBg]       // Table header background
    //   style.Colors[FlexUICol_TableBorderStrong]   // Table outer and header borders
    //   style.Colors[FlexUICol_TableBorderLight]    // Table inner borders
    //   style.Colors[FlexUICol_TableRowBg]          // Table row background when FlexUITableFlags_RowBg is enabled (even rows)
    //   style.Colors[FlexUICol_TableRowBgAlt]       // Table row background when FlexUITableFlags_RowBg is enabled (odds rows)

    // Demos
    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Basic");
    if (FlexUI::TreeNode("Basic"))
    {
        // Here we will showcase three different ways to output a table.
        // They are very simple variations of a same thing!

        // [Method 1] Using TableNextRow() to create a new row, and TableSetColumnIndex() to select the column.
        // In many situations, this is the most flexible and easy to use pattern.
        HelpMarker("Using TableNextRow() + calling TableSetColumnIndex() _before_ each cell, in a loop.");
        if (FlexUI::BeginTable("table1", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Row %d Column %d", row, column);
                }
            }
            FlexUI::EndTable();
        }

        // [Method 2] Using TableNextColumn() called multiple times, instead of using a for loop + TableSetColumnIndex().
        // This is generally more convenient when you have code manually submitting the contents of each column.
        HelpMarker("Using TableNextRow() + calling TableNextColumn() _before_ each cell, manually.");
        if (FlexUI::BeginTable("table2", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                FlexUI::TableNextRow();
                FlexUI::TableNextColumn();
                FlexUI::Text("Row %d", row);
                FlexUI::TableNextColumn();
                FlexUI::Text("Some contents");
                FlexUI::TableNextColumn();
                FlexUI::Text("123.456");
            }
            FlexUI::EndTable();
        }

        // [Method 3] We call TableNextColumn() _before_ each cell. We never call TableNextRow(),
        // as TableNextColumn() will automatically wrap around and create new rows as needed.
        // This is generally more convenient when your cells all contains the same type of data.
        HelpMarker(
            "Only using TableNextColumn(), which tends to be convenient for tables where every cell contains the same type of contents.\n"
            "This is also more similar to the old NextColumn() function of the Columns API, and provided to facilitate the Columns->Tables API transition.");
        if (FlexUI::BeginTable("table3", 3))
        {
            for (int item = 0; item < 14; item++)
            {
                FlexUI::TableNextColumn();
                FlexUI::Text("Item %d", item);
            }
            FlexUI::EndTable();
        }

        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Borders, background");
    if (FlexUI::TreeNode("Borders, background"))
    {
        // Expose a few Borders related flags interactively
        enum ContentsType { CT_Text, CT_FillButton };
        static FlexUITableFlags flags = FlexUITableFlags_Borders | FlexUITableFlags_RowBg;
        static bool display_headers = false;
        static int contents_type = CT_Text;

        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_RowBg", &flags, FlexUITableFlags_RowBg);
        FlexUI::CheckboxFlags("FlexUITableFlags_Borders", &flags, FlexUITableFlags_Borders);
        FlexUI::SameLine(); HelpMarker("FlexUITableFlags_Borders\n = FlexUITableFlags_BordersInnerV\n | FlexUITableFlags_BordersOuterV\n | FlexUITableFlags_BordersInnerV\n | FlexUITableFlags_BordersOuterH");
        FlexUI::Indent();

        FlexUI::CheckboxFlags("FlexUITableFlags_BordersH", &flags, FlexUITableFlags_BordersH);
        FlexUI::Indent();
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuterH", &flags, FlexUITableFlags_BordersOuterH);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersInnerH", &flags, FlexUITableFlags_BordersInnerH);
        FlexUI::Unindent();

        FlexUI::CheckboxFlags("FlexUITableFlags_BordersV", &flags, FlexUITableFlags_BordersV);
        FlexUI::Indent();
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuterV", &flags, FlexUITableFlags_BordersOuterV);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersInnerV", &flags, FlexUITableFlags_BordersInnerV);
        FlexUI::Unindent();

        FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuter", &flags, FlexUITableFlags_BordersOuter);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersInner", &flags, FlexUITableFlags_BordersInner);
        FlexUI::Unindent();

        FlexUI::AlignTextToFramePadding(); FlexUI::Text("Cell contents:");
        FlexUI::SameLine(); FlexUI::RadioButton("Text", &contents_type, CT_Text);
        FlexUI::SameLine(); FlexUI::RadioButton("FillButton", &contents_type, CT_FillButton);
        FlexUI::Checkbox("Display headers", &display_headers);
        FlexUI::CheckboxFlags("FlexUITableFlags_NoBordersInBody", &flags, FlexUITableFlags_NoBordersInBody); FlexUI::SameLine(); HelpMarker("Disable vertical borders in columns Body (borders will always appear in Headers");
        PopStyleCompact();

        if (FlexUI::BeginTable("table1", 3, flags))
        {
            // Display headers so we can inspect their interaction with borders.
            // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
            if (display_headers)
            {
                FlexUI::TableSetupColumn("One");
                FlexUI::TableSetupColumn("Two");
                FlexUI::TableSetupColumn("Three");
                FlexUI::TableHeadersRow();
            }

            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    char buf[32];
                    sprintf(buf, "Hello %d,%d", column, row);
                    if (contents_type == CT_Text)
                        FlexUI::TextUnformatted(buf);
                    else if (contents_type == CT_FillButton)
                        FlexUI::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Resizable, stretch");
    if (FlexUI::TreeNode("Resizable, stretch"))
    {
        // By default, if we don't enable ScrollX the sizing policy for each column is "Stretch"
        // All columns maintain a sizing weight, and they will occupy all available width.
        static FlexUITableFlags flags = FlexUITableFlags_SizingStretchSame | FlexUITableFlags_Resizable | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV | FlexUITableFlags_ContextMenuInBody;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags, FlexUITableFlags_Resizable);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersV", &flags, FlexUITableFlags_BordersV);
        FlexUI::SameLine(); HelpMarker("Using the _Resizable flag automatically enables the _BordersInnerV flag as well, this is why the resize borders are still showing when unchecking this.");
        PopStyleCompact();

        if (FlexUI::BeginTable("table1", 3, flags))
        {
            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Hello %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Resizable, fixed");
    if (FlexUI::TreeNode("Resizable, fixed"))
    {
        // Here we use FlexUITableFlags_SizingFixedFit (even though _ScrollX is not set)
        // So columns will adopt the "Fixed" policy and will maintain a fixed width regardless of the whole available width (unless table is small)
        // If there is not enough available width to fit all columns, they will however be resized down.
        // FIXME-TABLE: Providing a stretch-on-init would make sense especially for tables which don't have saved settings
        HelpMarker(
            "Using _Resizable + _SizingFixedFit flags.\n"
            "Fixed-width columns generally makes more sense if you want to use horizontal scrolling.\n\n"
            "Double-click a column border to auto-fit the column to its contents.");
        PushStyleCompact();
        static FlexUITableFlags flags = FlexUITableFlags_SizingFixedFit | FlexUITableFlags_Resizable | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV | FlexUITableFlags_ContextMenuInBody;
        FlexUI::CheckboxFlags("FlexUITableFlags_NoHostExtendX", &flags, FlexUITableFlags_NoHostExtendX);
        PopStyleCompact();

        if (FlexUI::BeginTable("table1", 3, flags))
        {
            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Hello %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Resizable, mixed");
    if (FlexUI::TreeNode("Resizable, mixed"))
    {
        HelpMarker(
            "Using TableSetupColumn() to alter resizing policy on a per-column basis.\n\n"
            "When combining Fixed and Stretch columns, generally you only want one, maybe two trailing columns to use _WidthStretch.");
        static FlexUITableFlags flags = FlexUITableFlags_SizingFixedFit | FlexUITableFlags_RowBg | FlexUITableFlags_Borders | FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable;

        if (FlexUI::BeginTable("table1", 3, flags))
        {
            FlexUI::TableSetupColumn("AAA", FlexUITableColumnFlags_WidthFixed);
            FlexUI::TableSetupColumn("BBB", FlexUITableColumnFlags_WidthFixed);
            FlexUI::TableSetupColumn("CCC", FlexUITableColumnFlags_WidthStretch);
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
                }
            }
            FlexUI::EndTable();
        }
        if (FlexUI::BeginTable("table2", 6, flags))
        {
            FlexUI::TableSetupColumn("AAA", FlexUITableColumnFlags_WidthFixed);
            FlexUI::TableSetupColumn("BBB", FlexUITableColumnFlags_WidthFixed);
            FlexUI::TableSetupColumn("CCC", FlexUITableColumnFlags_WidthFixed | FlexUITableColumnFlags_DefaultHide);
            FlexUI::TableSetupColumn("DDD", FlexUITableColumnFlags_WidthStretch);
            FlexUI::TableSetupColumn("EEE", FlexUITableColumnFlags_WidthStretch);
            FlexUI::TableSetupColumn("FFF", FlexUITableColumnFlags_WidthStretch | FlexUITableColumnFlags_DefaultHide);
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 6; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("%s %d,%d", (column >= 3) ? "Stretch" : "Fixed", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Reorderable, hideable, with headers");
    if (FlexUI::TreeNode("Reorderable, hideable, with headers"))
    {
        HelpMarker(
            "Click and drag column headers to reorder columns.\n\n"
            "Right-click on a header to open a context menu.");
        static FlexUITableFlags flags = FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags, FlexUITableFlags_Resizable);
        FlexUI::CheckboxFlags("FlexUITableFlags_Reorderable", &flags, FlexUITableFlags_Reorderable);
        FlexUI::CheckboxFlags("FlexUITableFlags_Hideable", &flags, FlexUITableFlags_Hideable);
        FlexUI::CheckboxFlags("FlexUITableFlags_NoBordersInBody", &flags, FlexUITableFlags_NoBordersInBody);
        FlexUI::CheckboxFlags("FlexUITableFlags_NoBordersInBodyUntilResize", &flags, FlexUITableFlags_NoBordersInBodyUntilResize); FlexUI::SameLine(); HelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers)");
        PopStyleCompact();

        if (FlexUI::BeginTable("table1", 3, flags))
        {
            // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
            // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
            FlexUI::TableSetupColumn("One");
            FlexUI::TableSetupColumn("Two");
            FlexUI::TableSetupColumn("Three");
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Hello %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }

        // Use outer_size.x == 0.0f instead of default to make the table as tight as possible (only valid when no scrolling and no stretch column)
        if (FlexUI::BeginTable("table2", 3, flags | FlexUITableFlags_SizingFixedFit, ImVec2(0.0f, 0.0f)))
        {
            FlexUI::TableSetupColumn("One");
            FlexUI::TableSetupColumn("Two");
            FlexUI::TableSetupColumn("Three");
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Fixed %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Padding");
    if (FlexUI::TreeNode("Padding"))
    {
        // First example: showcase use of padding flags and effect of BorderOuterV/BorderInnerV on X padding.
        // We don't expose BorderOuterH/BorderInnerH here because they have no effect on X padding.
        HelpMarker(
            "We often want outer padding activated when any using features which makes the edges of a column visible:\n"
            "e.g.:\n"
            "- BorderOuterV\n"
            "- any form of row selection\n"
            "Because of this, activating BorderOuterV sets the default to PadOuterX. Using PadOuterX or NoPadOuterX you can override the default.\n\n"
            "Actual padding values are using style.CellPadding.\n\n"
            "In this demo we don't show horizontal borders to emphasize how they don't affect default horizontal padding.");

        static FlexUITableFlags flags1 = FlexUITableFlags_BordersV;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_PadOuterX", &flags1, FlexUITableFlags_PadOuterX);
        FlexUI::SameLine(); HelpMarker("Enable outer-most padding (default if FlexUITableFlags_BordersOuterV is set)");
        FlexUI::CheckboxFlags("FlexUITableFlags_NoPadOuterX", &flags1, FlexUITableFlags_NoPadOuterX);
        FlexUI::SameLine(); HelpMarker("Disable outer-most padding (default if FlexUITableFlags_BordersOuterV is not set)");
        FlexUI::CheckboxFlags("FlexUITableFlags_NoPadInnerX", &flags1, FlexUITableFlags_NoPadInnerX);
        FlexUI::SameLine(); HelpMarker("Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off)");
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuterV", &flags1, FlexUITableFlags_BordersOuterV);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersInnerV", &flags1, FlexUITableFlags_BordersInnerV);
        static bool show_headers = false;
        FlexUI::Checkbox("show_headers", &show_headers);
        PopStyleCompact();

        if (FlexUI::BeginTable("table_padding", 3, flags1))
        {
            if (show_headers)
            {
                FlexUI::TableSetupColumn("One");
                FlexUI::TableSetupColumn("Two");
                FlexUI::TableSetupColumn("Three");
                FlexUI::TableHeadersRow();
            }

            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    if (row == 0)
                    {
                        FlexUI::Text("Avail %.2f", FlexUI::GetContentRegionAvail().x);
                    }
                    else
                    {
                        char buf[32];
                        sprintf(buf, "Hello %d,%d", column, row);
                        FlexUI::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                    }
                    //if (FlexUI::TableGetColumnFlags() & FlexUITableColumnFlags_IsHovered)
                    //    FlexUI::TableSetBgColor(FlexUITableBgTarget_CellBg, IM_COL32(0, 100, 0, 255));
                }
            }
            FlexUI::EndTable();
        }

        // Second example: set style.CellPadding to (0.0) or a custom value.
        // FIXME-TABLE: Vertical border effectively not displayed the same way as horizontal one...
        HelpMarker("Setting style.CellPadding to (0,0) or a custom value.");
        static FlexUITableFlags flags2 = FlexUITableFlags_Borders | FlexUITableFlags_RowBg;
        static ImVec2 cell_padding(0.0f, 0.0f);
        static bool show_widget_frame_bg = true;

        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Borders", &flags2, FlexUITableFlags_Borders);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersH", &flags2, FlexUITableFlags_BordersH);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersV", &flags2, FlexUITableFlags_BordersV);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersInner", &flags2, FlexUITableFlags_BordersInner);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuter", &flags2, FlexUITableFlags_BordersOuter);
        FlexUI::CheckboxFlags("FlexUITableFlags_RowBg", &flags2, FlexUITableFlags_RowBg);
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags2, FlexUITableFlags_Resizable);
        FlexUI::Checkbox("show_widget_frame_bg", &show_widget_frame_bg);
        FlexUI::SliderFloat2("CellPadding", &cell_padding.x, 0.0f, 10.0f, "%.0f");
        PopStyleCompact();

        FlexUI::PushStyleVar(FlexUIStyleVar_CellPadding, cell_padding);
        if (FlexUI::BeginTable("table_padding_2", 3, flags2))
        {
            static char text_bufs[3 * 5][16]; // Mini text storage for 3x5 cells
            static bool init = true;
            if (!show_widget_frame_bg)
                FlexUI::PushStyleColor(FlexUICol_FrameBg, 0);
            for (int cell = 0; cell < 3 * 5; cell++)
            {
                FlexUI::TableNextColumn();
                if (init)
                    strcpy(text_bufs[cell], "edit me");
                FlexUI::SetNextItemWidth(-FLT_MIN);
                FlexUI::PushID(cell);
                FlexUI::InputText("##cell", text_bufs[cell], IM_ARRAYSIZE(text_bufs[cell]));
                FlexUI::PopID();
            }
            if (!show_widget_frame_bg)
                FlexUI::PopStyleColor();
            init = false;
            FlexUI::EndTable();
        }
        FlexUI::PopStyleVar();

        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Explicit widths");
    if (FlexUI::TreeNode("Sizing policies"))
    {
        static FlexUITableFlags flags1 = FlexUITableFlags_BordersV | FlexUITableFlags_BordersOuterH | FlexUITableFlags_RowBg | FlexUITableFlags_ContextMenuInBody;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags1, FlexUITableFlags_Resizable);
        FlexUI::CheckboxFlags("FlexUITableFlags_NoHostExtendX", &flags1, FlexUITableFlags_NoHostExtendX);
        PopStyleCompact();

        static FlexUITableFlags sizing_policy_flags[4] = { FlexUITableFlags_SizingFixedFit, FlexUITableFlags_SizingFixedSame, FlexUITableFlags_SizingStretchProp, FlexUITableFlags_SizingStretchSame };
        for (int table_n = 0; table_n < 4; table_n++)
        {
            FlexUI::PushID(table_n);
            FlexUI::SetNextItemWidth(TEXT_BASE_WIDTH * 30);
            EditTableSizingFlags(&sizing_policy_flags[table_n]);

            // To make it easier to understand the different sizing policy,
            // For each policy: we display one table where the columns have equal contents width, and one where the columns have different contents width.
            if (FlexUI::BeginTable("table1", 3, sizing_policy_flags[table_n] | flags1))
            {
                for (int row = 0; row < 3; row++)
                {
                    FlexUI::TableNextRow();
                    FlexUI::TableNextColumn(); FlexUI::Text("Oh dear");
                    FlexUI::TableNextColumn(); FlexUI::Text("Oh dear");
                    FlexUI::TableNextColumn(); FlexUI::Text("Oh dear");
                }
                FlexUI::EndTable();
            }
            if (FlexUI::BeginTable("table2", 3, sizing_policy_flags[table_n] | flags1))
            {
                for (int row = 0; row < 3; row++)
                {
                    FlexUI::TableNextRow();
                    FlexUI::TableNextColumn(); FlexUI::Text("AAAA");
                    FlexUI::TableNextColumn(); FlexUI::Text("BBBBBBBB");
                    FlexUI::TableNextColumn(); FlexUI::Text("CCCCCCCCCCCC");
                }
                FlexUI::EndTable();
            }
            FlexUI::PopID();
        }

        FlexUI::Spacing();
        FlexUI::TextUnformatted("Advanced");
        FlexUI::SameLine();
        HelpMarker("This section allows you to interact and see the effect of various sizing policies depending on whether Scroll is enabled and the contents of your columns.");

        enum ContentsType { CT_ShowWidth, CT_ShortText, CT_LongText, CT_Button, CT_FillButton, CT_InputText };
        static FlexUITableFlags flags = FlexUITableFlags_ScrollY | FlexUITableFlags_Borders | FlexUITableFlags_RowBg | FlexUITableFlags_Resizable;
        static int contents_type = CT_ShowWidth;
        static int column_count = 3;

        PushStyleCompact();
        FlexUI::PushID("Advanced");
        FlexUI::PushItemWidth(TEXT_BASE_WIDTH * 30);
        EditTableSizingFlags(&flags);
        FlexUI::Combo("Contents", &contents_type, "Show width\0Short Text\0Long Text\0Button\0Fill Button\0InputText\0");
        if (contents_type == CT_FillButton)
        {
            FlexUI::SameLine();
            HelpMarker("Be mindful that using right-alignment (e.g. size.x = -FLT_MIN) creates a feedback loop where contents width can feed into auto-column width can feed into contents width.");
        }
        FlexUI::DragInt("Columns", &column_count, 0.1f, 1, 64, "%d", FlexUISliderFlags_AlwaysClamp);
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags, FlexUITableFlags_Resizable);
        FlexUI::CheckboxFlags("FlexUITableFlags_PreciseWidths", &flags, FlexUITableFlags_PreciseWidths);
        FlexUI::SameLine(); HelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollX", &flags, FlexUITableFlags_ScrollX);
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollY", &flags, FlexUITableFlags_ScrollY);
        FlexUI::CheckboxFlags("FlexUITableFlags_NoClip", &flags, FlexUITableFlags_NoClip);
        FlexUI::PopItemWidth();
        FlexUI::PopID();
        PopStyleCompact();

        if (FlexUI::BeginTable("table2", column_count, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 7)))
        {
            for (int cell = 0; cell < 10 * column_count; cell++)
            {
                FlexUI::TableNextColumn();
                int column = FlexUI::TableGetColumnIndex();
                int row = FlexUI::TableGetRowIndex();

                FlexUI::PushID(cell);
                char label[32];
                static char text_buf[32] = "";
                sprintf(label, "Hello %d,%d", column, row);
                switch (contents_type)
                {
                case CT_ShortText:  FlexUI::TextUnformatted(label); break;
                case CT_LongText:   FlexUI::Text("Some %s text %d,%d\nOver two lines..", column == 0 ? "long" : "longeeer", column, row); break;
                case CT_ShowWidth:  FlexUI::Text("W: %.1f", FlexUI::GetContentRegionAvail().x); break;
                case CT_Button:     FlexUI::Button(label); break;
                case CT_FillButton: FlexUI::Button(label, ImVec2(-FLT_MIN, 0.0f)); break;
                case CT_InputText:  FlexUI::SetNextItemWidth(-FLT_MIN); FlexUI::InputText("##", text_buf, IM_ARRAYSIZE(text_buf)); break;
                }
                FlexUI::PopID();
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Vertical scrolling, with clipping");
    if (FlexUI::TreeNode("Vertical scrolling, with clipping"))
    {
        HelpMarker("Here we activate ScrollY, which will create a child window container to allow hosting scrollable contents.\n\nWe also demonstrate using FlexUIListClipper to virtualize the submission of many items.");
        static FlexUITableFlags flags = FlexUITableFlags_ScrollY | FlexUITableFlags_RowBg | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV | FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable;

        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollY", &flags, FlexUITableFlags_ScrollY);
        PopStyleCompact();

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (FlexUI::BeginTable("table_scrolly", 3, flags, outer_size))
        {
            FlexUI::TableSetupScrollFreeze(0, 1); // Make top row always visible
            FlexUI::TableSetupColumn("One", FlexUITableColumnFlags_None);
            FlexUI::TableSetupColumn("Two", FlexUITableColumnFlags_None);
            FlexUI::TableSetupColumn("Three", FlexUITableColumnFlags_None);
            FlexUI::TableHeadersRow();

            // Demonstrate using clipper for large vertical lists
            FlexUIListClipper clipper;
            clipper.Begin(1000);
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    FlexUI::TableNextRow();
                    for (int column = 0; column < 3; column++)
                    {
                        FlexUI::TableSetColumnIndex(column);
                        FlexUI::Text("Hello %d,%d", column, row);
                    }
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Horizontal scrolling");
    if (FlexUI::TreeNode("Horizontal scrolling"))
    {
        HelpMarker(
            "When ScrollX is enabled, the default sizing policy becomes FlexUITableFlags_SizingFixedFit, "
            "as automatically stretching columns doesn't make much sense with horizontal scrolling.\n\n"
            "Also note that as of the current version, you will almost always want to enable ScrollY along with ScrollX,"
            "because the container window won't automatically extend vertically to fix contents (this may be improved in future versions).");
        static FlexUITableFlags flags = FlexUITableFlags_ScrollX | FlexUITableFlags_ScrollY | FlexUITableFlags_RowBg | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV | FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable;
        static int freeze_cols = 1;
        static int freeze_rows = 1;

        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags, FlexUITableFlags_Resizable);
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollX", &flags, FlexUITableFlags_ScrollX);
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollY", &flags, FlexUITableFlags_ScrollY);
        FlexUI::SetNextItemWidth(FlexUI::GetFrameHeight());
        FlexUI::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, FlexUISliderFlags_NoInput);
        FlexUI::SetNextItemWidth(FlexUI::GetFrameHeight());
        FlexUI::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, FlexUISliderFlags_NoInput);
        PopStyleCompact();

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (FlexUI::BeginTable("table_scrollx", 7, flags, outer_size))
        {
            FlexUI::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            FlexUI::TableSetupColumn("Line #", FlexUITableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
            FlexUI::TableSetupColumn("One");
            FlexUI::TableSetupColumn("Two");
            FlexUI::TableSetupColumn("Three");
            FlexUI::TableSetupColumn("Four");
            FlexUI::TableSetupColumn("Five");
            FlexUI::TableSetupColumn("Six");
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 20; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 7; column++)
                {
                    // Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
                    // Because here we know that:
                    // - A) all our columns are contributing the same to row height
                    // - B) column 0 is always visible,
                    // We only always submit this one column and can skip others.
                    // More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
                    if (!FlexUI::TableSetColumnIndex(column) && column > 0)
                        continue;
                    if (column == 0)
                        FlexUI::Text("Line %d", row);
                    else
                        FlexUI::Text("Hello world %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }

        FlexUI::Spacing();
        FlexUI::TextUnformatted("Stretch + ScrollX");
        FlexUI::SameLine();
        HelpMarker(
            "Showcase using Stretch columns + ScrollX together: "
            "this is rather unusual and only makes sense when specifying an 'inner_width' for the table!\n"
            "Without an explicit value, inner_width is == outer_size.x and therefore using Stretch columns + ScrollX together doesn't make sense.");
        static FlexUITableFlags flags2 = FlexUITableFlags_SizingStretchSame | FlexUITableFlags_ScrollX | FlexUITableFlags_ScrollY | FlexUITableFlags_BordersOuter | FlexUITableFlags_RowBg | FlexUITableFlags_ContextMenuInBody;
        static float inner_width = 1000.0f;
        PushStyleCompact();
        FlexUI::PushID("flags3");
        FlexUI::PushItemWidth(TEXT_BASE_WIDTH * 30);
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollX", &flags2, FlexUITableFlags_ScrollX);
        FlexUI::DragFloat("inner_width", &inner_width, 1.0f, 0.0f, FLT_MAX, "%.1f");
        FlexUI::PopItemWidth();
        FlexUI::PopID();
        PopStyleCompact();
        if (FlexUI::BeginTable("table2", 7, flags2, outer_size, inner_width))
        {
            for (int cell = 0; cell < 20 * 7; cell++)
            {
                FlexUI::TableNextColumn();
                FlexUI::Text("Hello world %d,%d", FlexUI::TableGetColumnIndex(), FlexUI::TableGetRowIndex());
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Columns flags");
    if (FlexUI::TreeNode("Columns flags"))
    {
        // Create a first table just to show all the options/flags we want to make visible in our example!
        const int column_count = 3;
        const char* column_names[column_count] = { "One", "Two", "Three" };
        static FlexUITableColumnFlags column_flags[column_count] = { FlexUITableColumnFlags_DefaultSort, FlexUITableColumnFlags_None, FlexUITableColumnFlags_DefaultHide };
        static FlexUITableColumnFlags column_flags_out[column_count] = { 0, 0, 0 }; // Output from TableGetColumnFlags()

        if (FlexUI::BeginTable("table_columns_flags_checkboxes", column_count, FlexUITableFlags_None))
        {
            PushStyleCompact();
            for (int column = 0; column < column_count; column++)
            {
                FlexUI::TableNextColumn();
                FlexUI::PushID(column);
                FlexUI::AlignTextToFramePadding(); // FIXME-TABLE: Workaround for wrong text baseline propagation across columns
                FlexUI::Text("'%s'", column_names[column]);
                FlexUI::Spacing();
                FlexUI::Text("Input flags:");
                EditTableColumnsFlags(&column_flags[column]);
                FlexUI::Spacing();
                FlexUI::Text("Output flags:");
                FlexUI::BeginDisabled();
                ShowTableColumnsStatusFlags(column_flags_out[column]);
                FlexUI::EndDisabled();
                FlexUI::PopID();
            }
            PopStyleCompact();
            FlexUI::EndTable();
        }

        // Create the real table we care about for the example!
        // We use a scrolling table to be able to showcase the difference between the _IsEnabled and _IsVisible flags above, otherwise in
        // a non-scrolling table columns are always visible (unless using FlexUITableFlags_NoKeepColumnsVisible + resizing the parent window down)
        const FlexUITableFlags flags
            = FlexUITableFlags_SizingFixedFit | FlexUITableFlags_ScrollX | FlexUITableFlags_ScrollY
            | FlexUITableFlags_RowBg | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV
            | FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable | FlexUITableFlags_Sortable;
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 9);
        if (FlexUI::BeginTable("table_columns_flags", column_count, flags, outer_size))
        {
            for (int column = 0; column < column_count; column++)
                FlexUI::TableSetupColumn(column_names[column], column_flags[column]);
            FlexUI::TableHeadersRow();
            for (int column = 0; column < column_count; column++)
                column_flags_out[column] = FlexUI::TableGetColumnFlags(column);
            float indent_step = (float)((int)TEXT_BASE_WIDTH / 2);
            for (int row = 0; row < 8; row++)
            {
                FlexUI::Indent(indent_step); // Add some indentation to demonstrate usage of per-column IndentEnable/IndentDisable flags.
                FlexUI::TableNextRow();
                for (int column = 0; column < column_count; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("%s %s", (column == 0) ? "Indented" : "Hello", FlexUI::TableGetColumnName(column));
                }
            }
            FlexUI::Unindent(indent_step * 8.0f);

            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Columns widths");
    if (FlexUI::TreeNode("Columns widths"))
    {
        HelpMarker("Using TableSetupColumn() to setup default width.");

        static FlexUITableFlags flags1 = FlexUITableFlags_Borders | FlexUITableFlags_NoBordersInBodyUntilResize;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags1, FlexUITableFlags_Resizable);
        FlexUI::CheckboxFlags("FlexUITableFlags_NoBordersInBodyUntilResize", &flags1, FlexUITableFlags_NoBordersInBodyUntilResize);
        PopStyleCompact();
        if (FlexUI::BeginTable("table1", 3, flags1))
        {
            // We could also set FlexUITableFlags_SizingFixedFit on the table and all columns will default to FlexUITableColumnFlags_WidthFixed.
            FlexUI::TableSetupColumn("one", FlexUITableColumnFlags_WidthFixed, 100.0f); // Default to 100.0f
            FlexUI::TableSetupColumn("two", FlexUITableColumnFlags_WidthFixed, 200.0f); // Default to 200.0f
            FlexUI::TableSetupColumn("three", FlexUITableColumnFlags_WidthFixed);       // Default to auto
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 4; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    if (row == 0)
                        FlexUI::Text("(w: %5.1f)", FlexUI::GetContentRegionAvail().x);
                    else
                        FlexUI::Text("Hello %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }

        HelpMarker("Using TableSetupColumn() to setup explicit width.\n\nUnless _NoKeepColumnsVisible is set, fixed columns with set width may still be shrunk down if there's not enough space in the host.");

        static FlexUITableFlags flags2 = FlexUITableFlags_None;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_NoKeepColumnsVisible", &flags2, FlexUITableFlags_NoKeepColumnsVisible);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersInnerV", &flags2, FlexUITableFlags_BordersInnerV);
        FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuterV", &flags2, FlexUITableFlags_BordersOuterV);
        PopStyleCompact();
        if (FlexUI::BeginTable("table2", 4, flags2))
        {
            // We could also set FlexUITableFlags_SizingFixedFit on the table and all columns will default to FlexUITableColumnFlags_WidthFixed.
            FlexUI::TableSetupColumn("", FlexUITableColumnFlags_WidthFixed, 100.0f);
            FlexUI::TableSetupColumn("", FlexUITableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            FlexUI::TableSetupColumn("", FlexUITableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 30.0f);
            FlexUI::TableSetupColumn("", FlexUITableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    if (row == 0)
                        FlexUI::Text("(w: %5.1f)", FlexUI::GetContentRegionAvail().x);
                    else
                        FlexUI::Text("Hello %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Nested tables");
    if (FlexUI::TreeNode("Nested tables"))
    {
        HelpMarker("This demonstrates embedding a table into another table cell.");

        if (FlexUI::BeginTable("table_nested1", 2, FlexUITableFlags_Borders | FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable))
        {
            FlexUI::TableSetupColumn("A0");
            FlexUI::TableSetupColumn("A1");
            FlexUI::TableHeadersRow();

            FlexUI::TableNextColumn();
            FlexUI::Text("A0 Row 0");
            {
                float rows_height = TEXT_BASE_HEIGHT * 2;
                if (FlexUI::BeginTable("table_nested2", 2, FlexUITableFlags_Borders | FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable))
                {
                    FlexUI::TableSetupColumn("B0");
                    FlexUI::TableSetupColumn("B1");
                    FlexUI::TableHeadersRow();

                    FlexUI::TableNextRow(FlexUITableRowFlags_None, rows_height);
                    FlexUI::TableNextColumn();
                    FlexUI::Text("B0 Row 0");
                    FlexUI::TableNextColumn();
                    FlexUI::Text("B1 Row 0");
                    FlexUI::TableNextRow(FlexUITableRowFlags_None, rows_height);
                    FlexUI::TableNextColumn();
                    FlexUI::Text("B0 Row 1");
                    FlexUI::TableNextColumn();
                    FlexUI::Text("B1 Row 1");

                    FlexUI::EndTable();
                }
            }
            FlexUI::TableNextColumn(); FlexUI::Text("A1 Row 0");
            FlexUI::TableNextColumn(); FlexUI::Text("A0 Row 1");
            FlexUI::TableNextColumn(); FlexUI::Text("A1 Row 1");
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Row height");
    if (FlexUI::TreeNode("Row height"))
    {
        HelpMarker("You can pass a 'min_row_height' to TableNextRow().\n\nRows are padded with 'style.CellPadding.y' on top and bottom, so effectively the minimum row height will always be >= 'style.CellPadding.y * 2.0f'.\n\nWe cannot honor a _maximum_ row height as that would require a unique clipping rectangle per row.");
        if (FlexUI::BeginTable("table_row_height", 1, FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersInnerV))
        {
            for (int row = 0; row < 10; row++)
            {
                float min_row_height = (float)(int)(TEXT_BASE_HEIGHT * 0.30f * row);
                FlexUI::TableNextRow(FlexUITableRowFlags_None, min_row_height);
                FlexUI::TableNextColumn();
                FlexUI::Text("min_row_height = %.2f", min_row_height);
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Outer size");
    if (FlexUI::TreeNode("Outer size"))
    {
        // Showcasing use of FlexUITableFlags_NoHostExtendX and FlexUITableFlags_NoHostExtendY
        // Important to that note how the two flags have slightly different behaviors!
        FlexUI::Text("Using NoHostExtendX and NoHostExtendY:");
        PushStyleCompact();
        static FlexUITableFlags flags = FlexUITableFlags_Borders | FlexUITableFlags_Resizable | FlexUITableFlags_ContextMenuInBody | FlexUITableFlags_RowBg | FlexUITableFlags_SizingFixedFit | FlexUITableFlags_NoHostExtendX;
        FlexUI::CheckboxFlags("FlexUITableFlags_NoHostExtendX", &flags, FlexUITableFlags_NoHostExtendX);
        FlexUI::SameLine(); HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
        FlexUI::CheckboxFlags("FlexUITableFlags_NoHostExtendY", &flags, FlexUITableFlags_NoHostExtendY);
        FlexUI::SameLine(); HelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
        PopStyleCompact();

        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 5.5f);
        if (FlexUI::BeginTable("table1", 3, flags, outer_size))
        {
            for (int row = 0; row < 10; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableNextColumn();
                    FlexUI::Text("Cell %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::SameLine();
        FlexUI::Text("Hello!");

        FlexUI::Spacing();

        FlexUI::Text("Using explicit size:");
        if (FlexUI::BeginTable("table2", 3, FlexUITableFlags_Borders | FlexUITableFlags_RowBg, ImVec2(TEXT_BASE_WIDTH * 30, 0.0f)))
        {
            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableNextColumn();
                    FlexUI::Text("Cell %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::SameLine();
        if (FlexUI::BeginTable("table3", 3, FlexUITableFlags_Borders | FlexUITableFlags_RowBg, ImVec2(TEXT_BASE_WIDTH * 30, 0.0f)))
        {
            for (int row = 0; row < 3; row++)
            {
                FlexUI::TableNextRow(0, TEXT_BASE_HEIGHT * 1.5f);
                for (int column = 0; column < 3; column++)
                {
                    FlexUI::TableNextColumn();
                    FlexUI::Text("Cell %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }

        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Background color");
    if (FlexUI::TreeNode("Background color"))
    {
        static FlexUITableFlags flags = FlexUITableFlags_RowBg;
        static int row_bg_type = 1;
        static int row_bg_target = 1;
        static int cell_bg_type = 1;

        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_Borders", &flags, FlexUITableFlags_Borders);
        FlexUI::CheckboxFlags("FlexUITableFlags_RowBg", &flags, FlexUITableFlags_RowBg);
        FlexUI::SameLine(); HelpMarker("FlexUITableFlags_RowBg automatically sets RowBg0 to alternative colors pulled from the Style.");
        FlexUI::Combo("row bg type", (int*)&row_bg_type, "None\0Red\0Gradient\0");
        FlexUI::Combo("row bg target", (int*)&row_bg_target, "RowBg0\0RowBg1\0"); FlexUI::SameLine(); HelpMarker("Target RowBg0 to override the alternating odd/even colors,\nTarget RowBg1 to blend with them.");
        FlexUI::Combo("cell bg type", (int*)&cell_bg_type, "None\0Blue\0"); FlexUI::SameLine(); HelpMarker("We are colorizing cells to B1->C2 here.");
        IM_ASSERT(row_bg_type >= 0 && row_bg_type <= 2);
        IM_ASSERT(row_bg_target >= 0 && row_bg_target <= 1);
        IM_ASSERT(cell_bg_type >= 0 && cell_bg_type <= 1);
        PopStyleCompact();

        if (FlexUI::BeginTable("table1", 5, flags))
        {
            for (int row = 0; row < 6; row++)
            {
                FlexUI::TableNextRow();

                // Demonstrate setting a row background color with 'FlexUI::TableSetBgColor(FlexUITableBgTarget_RowBgX, ...)'
                // We use a transparent color so we can see the one behind in case our target is RowBg1 and RowBg0 was already targeted by the FlexUITableFlags_RowBg flag.
                if (row_bg_type != 0)
                {
                    ImU32 row_bg_color = FlexUI::GetColorU32(row_bg_type == 1 ? ImVec4(0.7f, 0.3f, 0.3f, 0.65f) : ImVec4(0.2f + row * 0.1f, 0.2f, 0.2f, 0.65f)); // Flat or Gradient?
                    FlexUI::TableSetBgColor(FlexUITableBgTarget_RowBg0 + row_bg_target, row_bg_color);
                }

                // Fill cells
                for (int column = 0; column < 5; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("%c%c", 'A' + row, '0' + column);

                    // Change background of Cells B1->C2
                    // Demonstrate setting a cell background color with 'FlexUI::TableSetBgColor(FlexUITableBgTarget_CellBg, ...)'
                    // (the CellBg color will be blended over the RowBg and ColumnBg colors)
                    // We can also pass a column number as a third parameter to TableSetBgColor() and do this outside the column loop.
                    if (row >= 1 && row <= 2 && column >= 1 && column <= 2 && cell_bg_type == 1)
                    {
                        ImU32 cell_bg_color = FlexUI::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f));
                        FlexUI::TableSetBgColor(FlexUITableBgTarget_CellBg, cell_bg_color);
                    }
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Tree view");
    if (FlexUI::TreeNode("Tree view"))
    {
        static FlexUITableFlags flags = FlexUITableFlags_BordersV | FlexUITableFlags_BordersOuterH | FlexUITableFlags_Resizable | FlexUITableFlags_RowBg | FlexUITableFlags_NoBordersInBody;

        if (FlexUI::BeginTable("3ways", 3, flags))
        {
            // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
            FlexUI::TableSetupColumn("Name", FlexUITableColumnFlags_NoHide);
            FlexUI::TableSetupColumn("Size", FlexUITableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            FlexUI::TableSetupColumn("Type", FlexUITableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            FlexUI::TableHeadersRow();

            // Simple storage to output a dummy file-system.
            struct MyTreeNode
            {
                const char* Name;
                const char* Type;
                int             Size;
                int             ChildIdx;
                int             ChildCount;
                static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
                {
                    FlexUI::TableNextRow();
                    FlexUI::TableNextColumn();
                    const bool is_folder = (node->ChildCount > 0);
                    if (is_folder)
                    {
                        bool open = FlexUI::TreeNodeEx(node->Name, FlexUITreeNodeFlags_SpanFullWidth);
                        FlexUI::TableNextColumn();
                        FlexUI::TextDisabled("--");
                        FlexUI::TableNextColumn();
                        FlexUI::TextUnformatted(node->Type);
                        if (open)
                        {
                            for (int child_n = 0; child_n < node->ChildCount; child_n++)
                                DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
                            FlexUI::TreePop();
                        }
                    }
                    else
                    {
                        FlexUI::TreeNodeEx(node->Name, FlexUITreeNodeFlags_Leaf | FlexUITreeNodeFlags_Bullet | FlexUITreeNodeFlags_NoTreePushOnOpen | FlexUITreeNodeFlags_SpanFullWidth);
                        FlexUI::TableNextColumn();
                        FlexUI::Text("%d", node->Size);
                        FlexUI::TableNextColumn();
                        FlexUI::TextUnformatted(node->Type);
                    }
                }
            };
            static const MyTreeNode nodes[] =
            {
                { "Root",                         "Folder",       -1,       1, 3    }, // 0
                { "Music",                        "Folder",       -1,       4, 2    }, // 1
                { "Textures",                     "Folder",       -1,       6, 3    }, // 2
                { "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
                { "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
                { "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
                { "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
                { "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
                { "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
            };

            MyTreeNode::DisplayNode(&nodes[0], nodes);

            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Item width");
    if (FlexUI::TreeNode("Item width"))
    {
        HelpMarker(
            "Showcase using PushItemWidth() and how it is preserved on a per-column basis.\n\n"
            "Note that on auto-resizing non-resizable fixed columns, querying the content width for e.g. right-alignment doesn't make sense.");
        if (FlexUI::BeginTable("table_item_width", 3, FlexUITableFlags_Borders))
        {
            FlexUI::TableSetupColumn("small");
            FlexUI::TableSetupColumn("half");
            FlexUI::TableSetupColumn("right-align");
            FlexUI::TableHeadersRow();

            for (int row = 0; row < 3; row++)
            {
                FlexUI::TableNextRow();
                if (row == 0)
                {
                    // Setup ItemWidth once (instead of setting up every time, which is also possible but less efficient)
                    FlexUI::TableSetColumnIndex(0);
                    FlexUI::PushItemWidth(TEXT_BASE_WIDTH * 3.0f); // Small
                    FlexUI::TableSetColumnIndex(1);
                    FlexUI::PushItemWidth(-FlexUI::GetContentRegionAvail().x * 0.5f);
                    FlexUI::TableSetColumnIndex(2);
                    FlexUI::PushItemWidth(-FLT_MIN); // Right-aligned
                }

                // Draw our contents
                static float dummy_f = 0.0f;
                FlexUI::PushID(row);
                FlexUI::TableSetColumnIndex(0);
                FlexUI::SliderFloat("float0", &dummy_f, 0.0f, 1.0f);
                FlexUI::TableSetColumnIndex(1);
                FlexUI::SliderFloat("float1", &dummy_f, 0.0f, 1.0f);
                FlexUI::TableSetColumnIndex(2);
                FlexUI::SliderFloat("##float2", &dummy_f, 0.0f, 1.0f); // No visible label since right-aligned
                FlexUI::PopID();
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    // Demonstrate using TableHeader() calls instead of TableHeadersRow()
    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Custom headers");
    if (FlexUI::TreeNode("Custom headers"))
    {
        const int COLUMNS_COUNT = 3;
        if (FlexUI::BeginTable("table_custom_headers", COLUMNS_COUNT, FlexUITableFlags_Borders | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable))
        {
            FlexUI::TableSetupColumn("Apricot");
            FlexUI::TableSetupColumn("Banana");
            FlexUI::TableSetupColumn("Cherry");

            // Dummy entire-column selection storage
            // FIXME: It would be nice to actually demonstrate full-featured selection using those checkbox.
            static bool column_selected[3] = {};

            // Instead of calling TableHeadersRow() we'll submit custom headers ourselves
            FlexUI::TableNextRow(FlexUITableRowFlags_Headers);
            for (int column = 0; column < COLUMNS_COUNT; column++)
            {
                FlexUI::TableSetColumnIndex(column);
                const char* column_name = FlexUI::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
                FlexUI::PushID(column);
                FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(0, 0));
                FlexUI::Checkbox("##checkall", &column_selected[column]);
                FlexUI::PopStyleVar();
                FlexUI::SameLine(0.0f, FlexUI::GetStyle().ItemInnerSpacing.x);
                FlexUI::TableHeader(column_name);
                FlexUI::PopID();
            }

            for (int row = 0; row < 5; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    char buf[32];
                    sprintf(buf, "Cell %d,%d", column, row);
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Selectable(buf, column_selected[column]);
                }
            }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    // Demonstrate creating custom context menus inside columns, while playing it nice with context menus provided by TableHeadersRow()/TableHeader()
    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Context menus");
    if (FlexUI::TreeNode("Context menus"))
    {
        HelpMarker("By default, right-clicking over a TableHeadersRow()/TableHeader() line will open the default context-menu.\nUsing FlexUITableFlags_ContextMenuInBody we also allow right-clicking over columns body.");
        static FlexUITableFlags flags1 = FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable | FlexUITableFlags_Borders | FlexUITableFlags_ContextMenuInBody;

        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_ContextMenuInBody", &flags1, FlexUITableFlags_ContextMenuInBody);
        PopStyleCompact();

        // Context Menus: first example
        // [1.1] Right-click on the TableHeadersRow() line to open the default table context menu.
        // [1.2] Right-click in columns also open the default table context menu (if FlexUITableFlags_ContextMenuInBody is set)
        const int COLUMNS_COUNT = 3;
        if (FlexUI::BeginTable("table_context_menu", COLUMNS_COUNT, flags1))
        {
            FlexUI::TableSetupColumn("One");
            FlexUI::TableSetupColumn("Two");
            FlexUI::TableSetupColumn("Three");

            // [1.1]] Right-click on the TableHeadersRow() line to open the default table context menu.
            FlexUI::TableHeadersRow();

            // Submit dummy contents
            for (int row = 0; row < 4; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < COLUMNS_COUNT; column++)
                {
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Cell %d,%d", column, row);
                }
            }
            FlexUI::EndTable();
        }

        // Context Menus: second example
        // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
        // [2.2] Right-click on the ".." to open a custom popup
        // [2.3] Right-click in columns to open another custom popup
        HelpMarker("Demonstrate mixing table context menu (over header), item context button (over button) and custom per-colum context menu (over column body).");
        FlexUITableFlags flags2 = FlexUITableFlags_Resizable | FlexUITableFlags_SizingFixedFit | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable | FlexUITableFlags_Borders;
        if (FlexUI::BeginTable("table_context_menu_2", COLUMNS_COUNT, flags2))
        {
            FlexUI::TableSetupColumn("One");
            FlexUI::TableSetupColumn("Two");
            FlexUI::TableSetupColumn("Three");

            // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
            FlexUI::TableHeadersRow();
            for (int row = 0; row < 4; row++)
            {
                FlexUI::TableNextRow();
                for (int column = 0; column < COLUMNS_COUNT; column++)
                {
                    // Submit dummy contents
                    FlexUI::TableSetColumnIndex(column);
                    FlexUI::Text("Cell %d,%d", column, row);
                    FlexUI::SameLine();

                    // [2.2] Right-click on the ".." to open a custom popup
                    FlexUI::PushID(row * COLUMNS_COUNT + column);
                    FlexUI::SmallButton("..");
                    if (FlexUI::BeginPopupContextItem())
                    {
                        FlexUI::Text("This is the popup for Button(\"..\") in Cell %d,%d", column, row);
                        if (FlexUI::Button("Close"))
                            FlexUI::CloseCurrentPopup();
                        FlexUI::EndPopup();
                    }
                    FlexUI::PopID();
                }
            }

            // [2.3] Right-click anywhere in columns to open another custom popup
            // (instead of testing for !IsAnyItemHovered() we could also call OpenPopup() with FlexUIPopupFlags_NoOpenOverExistingPopup
            // to manage popup priority as the popups triggers, here "are we hovering a column" are overlapping)
            int hovered_column = -1;
            for (int column = 0; column < COLUMNS_COUNT + 1; column++)
            {
                FlexUI::PushID(column);
                if (FlexUI::TableGetColumnFlags(column) & FlexUITableColumnFlags_IsHovered)
                    hovered_column = column;
                if (hovered_column == column && !FlexUI::IsAnyItemHovered() && FlexUI::IsMouseReleased(1))
                    FlexUI::OpenPopup("MyPopup");
                if (FlexUI::BeginPopup("MyPopup"))
                {
                    if (column == COLUMNS_COUNT)
                        FlexUI::Text("This is a custom popup for unused space after the last column.");
                    else
                        FlexUI::Text("This is a custom popup for Column %d", column);
                    if (FlexUI::Button("Close"))
                        FlexUI::CloseCurrentPopup();
                    FlexUI::EndPopup();
                }
                FlexUI::PopID();
            }

            FlexUI::EndTable();
            FlexUI::Text("Hovered column: %d", hovered_column);
        }
        FlexUI::TreePop();
    }

    // Demonstrate creating multiple tables with the same ID
    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Synced instances");
    if (FlexUI::TreeNode("Synced instances"))
    {
        HelpMarker("Multiple tables with the same identifier will share their settings, width, visibility, order etc.");

        static FlexUITableFlags flags = FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable | FlexUITableFlags_Borders | FlexUITableFlags_SizingFixedFit | FlexUITableFlags_NoSavedSettings;
        FlexUI::CheckboxFlags("FlexUITableFlags_ScrollY", &flags, FlexUITableFlags_ScrollY);
        FlexUI::CheckboxFlags("FlexUITableFlags_SizingFixedFit", &flags, FlexUITableFlags_SizingFixedFit);
        for (int n = 0; n < 3; n++)
        {
            char buf[32];
            sprintf(buf, "Synced Table %d", n);
            bool open = FlexUI::CollapsingHeader(buf, FlexUITreeNodeFlags_DefaultOpen);
            if (open && FlexUI::BeginTable("Table", 3, flags, ImVec2(0.0f, FlexUI::GetTextLineHeightWithSpacing() * 5)))
            {
                FlexUI::TableSetupColumn("One");
                FlexUI::TableSetupColumn("Two");
                FlexUI::TableSetupColumn("Three");
                FlexUI::TableHeadersRow();
                const int cell_count = (n == 1) ? 27 : 9; // Make second table have a scrollbar to verify that additional decoration is not affecting column positions.
                for (int cell = 0; cell < cell_count; cell++)
                {
                    FlexUI::TableNextColumn();
                    FlexUI::Text("this cell %d", cell);
                }
                FlexUI::EndTable();
            }
        }
        FlexUI::TreePop();
    }

    // Demonstrate using Sorting facilities
    // This is a simplified version of the "Advanced" example, where we mostly focus on the code necessary to handle sorting.
    // Note that the "Advanced" example also showcase manually triggering a sort (e.g. if item quantities have been modified)
    static const char* template_items_names[] =
    {
        "Banana", "Apple", "Cherry", "Watermelon", "Grapefruit", "Strawberry", "Mango",
        "Kiwi", "Orange", "Pineapple", "Blueberry", "Plum", "Coconut", "Pear", "Apricot"
    };
    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Sorting");
    if (FlexUI::TreeNode("Sorting"))
    {
        // Create item list
        static ImVector<MyItem> items;
        if (items.Size == 0)
        {
            items.resize(50, MyItem());
            for (int n = 0; n < items.Size; n++)
            {
                const int template_n = n % IM_ARRAYSIZE(template_items_names);
                MyItem& item = items[n];
                item.ID = n;
                item.Name = template_items_names[template_n];
                item.Quantity = (n * n - n) % 20; // Assign default quantities
            }
        }

        // Options
        static FlexUITableFlags flags =
            FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable | FlexUITableFlags_Sortable | FlexUITableFlags_SortMulti
            | FlexUITableFlags_RowBg | FlexUITableFlags_BordersOuter | FlexUITableFlags_BordersV | FlexUITableFlags_NoBordersInBody
            | FlexUITableFlags_ScrollY;
        PushStyleCompact();
        FlexUI::CheckboxFlags("FlexUITableFlags_SortMulti", &flags, FlexUITableFlags_SortMulti);
        FlexUI::SameLine(); HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
        FlexUI::CheckboxFlags("FlexUITableFlags_SortTristate", &flags, FlexUITableFlags_SortTristate);
        FlexUI::SameLine(); HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
        PopStyleCompact();

        if (FlexUI::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 15), 0.0f))
        {
            // Declare columns
            // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
            // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
            // Demonstrate using a mixture of flags among available sort-related flags:
            // - FlexUITableColumnFlags_DefaultSort
            // - FlexUITableColumnFlags_NoSort / FlexUITableColumnFlags_NoSortAscending / FlexUITableColumnFlags_NoSortDescending
            // - FlexUITableColumnFlags_PreferSortAscending / FlexUITableColumnFlags_PreferSortDescending
            FlexUI::TableSetupColumn("ID", FlexUITableColumnFlags_DefaultSort | FlexUITableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_ID);
            FlexUI::TableSetupColumn("Name", FlexUITableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
            FlexUI::TableSetupColumn("Action", FlexUITableColumnFlags_NoSort | FlexUITableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
            FlexUI::TableSetupColumn("Quantity", FlexUITableColumnFlags_PreferSortDescending | FlexUITableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Quantity);
            FlexUI::TableSetupScrollFreeze(0, 1); // Make row always visible
            FlexUI::TableHeadersRow();

            // Sort our data if sort specs have been changed!
            if (FlexUITableSortSpecs* sorts_specs = FlexUI::TableGetSortSpecs())
                if (sorts_specs->SpecsDirty)
                {
                    MyItem::s_current_sort_specs = sorts_specs; // Store in variable accessible by the sort function.
                    if (items.Size > 1)
                        qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
                    MyItem::s_current_sort_specs = NULL;
                    sorts_specs->SpecsDirty = false;
                }

            // Demonstrate using clipper for large vertical lists
            FlexUIListClipper clipper;
            clipper.Begin(items.Size);
            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {
                    // Display a data item
                    MyItem* item = &items[row_n];
                    FlexUI::PushID(item->ID);
                    FlexUI::TableNextRow();
                    FlexUI::TableNextColumn();
                    FlexUI::Text("%04d", item->ID);
                    FlexUI::TableNextColumn();
                    FlexUI::TextUnformatted(item->Name);
                    FlexUI::TableNextColumn();
                    FlexUI::SmallButton("None");
                    FlexUI::TableNextColumn();
                    FlexUI::Text("%d", item->Quantity);
                    FlexUI::PopID();
                }
            FlexUI::EndTable();
        }
        FlexUI::TreePop();
    }

    // In this example we'll expose most table flags and settings.
    // For specific flags and settings refer to the corresponding section for more detailed explanation.
    // This section is mostly useful to experiment with combining certain flags or settings with each others.
    //FlexUI::SetNextItemOpen(true, FlexUICond_Once); // [DEBUG]
    if (open_action != -1)
        FlexUI::SetNextItemOpen(open_action != 0);
    FlexUI_DEMO_MARKER("Tables/Advanced");
    if (FlexUI::TreeNode("Advanced"))
    {
        static FlexUITableFlags flags =
            FlexUITableFlags_Resizable | FlexUITableFlags_Reorderable | FlexUITableFlags_Hideable
            | FlexUITableFlags_Sortable | FlexUITableFlags_SortMulti
            | FlexUITableFlags_RowBg | FlexUITableFlags_Borders | FlexUITableFlags_NoBordersInBody
            | FlexUITableFlags_ScrollX | FlexUITableFlags_ScrollY
            | FlexUITableFlags_SizingFixedFit;

        enum ContentsType { CT_Text, CT_Button, CT_SmallButton, CT_FillButton, CT_Selectable, CT_SelectableSpanRow };
        static int contents_type = CT_SelectableSpanRow;
        const char* contents_type_names[] = { "Text", "Button", "SmallButton", "FillButton", "Selectable", "Selectable (span row)" };
        static int freeze_cols = 1;
        static int freeze_rows = 1;
        static int items_count = IM_ARRAYSIZE(template_items_names) * 2;
        static ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);
        static float row_min_height = 0.0f; // Auto
        static float inner_width_with_scroll = 0.0f; // Auto-extend
        static bool outer_size_enabled = true;
        static bool show_headers = true;
        static bool show_wrapped_text = false;
        //static FlexUITextFilter filter;
        //FlexUI::SetNextItemOpen(true, FlexUICond_Once); // FIXME-TABLE: Enabling this results in initial clipped first pass on table which tend to affect column sizing
        if (FlexUI::TreeNode("Options"))
        {
            // Make the UI compact because there are so many fields
            PushStyleCompact();
            FlexUI::PushItemWidth(TEXT_BASE_WIDTH * 28.0f);

            if (FlexUI::TreeNodeEx("Features:", FlexUITreeNodeFlags_DefaultOpen))
            {
                FlexUI::CheckboxFlags("FlexUITableFlags_Resizable", &flags, FlexUITableFlags_Resizable);
                FlexUI::CheckboxFlags("FlexUITableFlags_Reorderable", &flags, FlexUITableFlags_Reorderable);
                FlexUI::CheckboxFlags("FlexUITableFlags_Hideable", &flags, FlexUITableFlags_Hideable);
                FlexUI::CheckboxFlags("FlexUITableFlags_Sortable", &flags, FlexUITableFlags_Sortable);
                FlexUI::CheckboxFlags("FlexUITableFlags_NoSavedSettings", &flags, FlexUITableFlags_NoSavedSettings);
                FlexUI::CheckboxFlags("FlexUITableFlags_ContextMenuInBody", &flags, FlexUITableFlags_ContextMenuInBody);
                FlexUI::TreePop();
            }

            if (FlexUI::TreeNodeEx("Decorations:", FlexUITreeNodeFlags_DefaultOpen))
            {
                FlexUI::CheckboxFlags("FlexUITableFlags_RowBg", &flags, FlexUITableFlags_RowBg);
                FlexUI::CheckboxFlags("FlexUITableFlags_BordersV", &flags, FlexUITableFlags_BordersV);
                FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuterV", &flags, FlexUITableFlags_BordersOuterV);
                FlexUI::CheckboxFlags("FlexUITableFlags_BordersInnerV", &flags, FlexUITableFlags_BordersInnerV);
                FlexUI::CheckboxFlags("FlexUITableFlags_BordersH", &flags, FlexUITableFlags_BordersH);
                FlexUI::CheckboxFlags("FlexUITableFlags_BordersOuterH", &flags, FlexUITableFlags_BordersOuterH);
                FlexUI::CheckboxFlags("FlexUITableFlags_BordersInnerH", &flags, FlexUITableFlags_BordersInnerH);
                FlexUI::CheckboxFlags("FlexUITableFlags_NoBordersInBody", &flags, FlexUITableFlags_NoBordersInBody); FlexUI::SameLine(); HelpMarker("Disable vertical borders in columns Body (borders will always appear in Headers");
                FlexUI::CheckboxFlags("FlexUITableFlags_NoBordersInBodyUntilResize", &flags, FlexUITableFlags_NoBordersInBodyUntilResize); FlexUI::SameLine(); HelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers)");
                FlexUI::TreePop();
            }

            if (FlexUI::TreeNodeEx("Sizing:", FlexUITreeNodeFlags_DefaultOpen))
            {
                EditTableSizingFlags(&flags);
                FlexUI::SameLine(); HelpMarker("In the Advanced demo we override the policy of each column so those table-wide settings have less effect that typical.");
                FlexUI::CheckboxFlags("FlexUITableFlags_NoHostExtendX", &flags, FlexUITableFlags_NoHostExtendX);
                FlexUI::SameLine(); HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
                FlexUI::CheckboxFlags("FlexUITableFlags_NoHostExtendY", &flags, FlexUITableFlags_NoHostExtendY);
                FlexUI::SameLine(); HelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
                FlexUI::CheckboxFlags("FlexUITableFlags_NoKeepColumnsVisible", &flags, FlexUITableFlags_NoKeepColumnsVisible);
                FlexUI::SameLine(); HelpMarker("Only available if ScrollX is disabled.");
                FlexUI::CheckboxFlags("FlexUITableFlags_PreciseWidths", &flags, FlexUITableFlags_PreciseWidths);
                FlexUI::SameLine(); HelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
                FlexUI::CheckboxFlags("FlexUITableFlags_NoClip", &flags, FlexUITableFlags_NoClip);
                FlexUI::SameLine(); HelpMarker("Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with ScrollFreeze options.");
                FlexUI::TreePop();
            }

            if (FlexUI::TreeNodeEx("Padding:", FlexUITreeNodeFlags_DefaultOpen))
            {
                FlexUI::CheckboxFlags("FlexUITableFlags_PadOuterX", &flags, FlexUITableFlags_PadOuterX);
                FlexUI::CheckboxFlags("FlexUITableFlags_NoPadOuterX", &flags, FlexUITableFlags_NoPadOuterX);
                FlexUI::CheckboxFlags("FlexUITableFlags_NoPadInnerX", &flags, FlexUITableFlags_NoPadInnerX);
                FlexUI::TreePop();
            }

            if (FlexUI::TreeNodeEx("Scrolling:", FlexUITreeNodeFlags_DefaultOpen))
            {
                FlexUI::CheckboxFlags("FlexUITableFlags_ScrollX", &flags, FlexUITableFlags_ScrollX);
                FlexUI::SameLine();
                FlexUI::SetNextItemWidth(FlexUI::GetFrameHeight());
                FlexUI::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, FlexUISliderFlags_NoInput);
                FlexUI::CheckboxFlags("FlexUITableFlags_ScrollY", &flags, FlexUITableFlags_ScrollY);
                FlexUI::SameLine();
                FlexUI::SetNextItemWidth(FlexUI::GetFrameHeight());
                FlexUI::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, FlexUISliderFlags_NoInput);
                FlexUI::TreePop();
            }

            if (FlexUI::TreeNodeEx("Sorting:", FlexUITreeNodeFlags_DefaultOpen))
            {
                FlexUI::CheckboxFlags("FlexUITableFlags_SortMulti", &flags, FlexUITableFlags_SortMulti);
                FlexUI::SameLine(); HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
                FlexUI::CheckboxFlags("FlexUITableFlags_SortTristate", &flags, FlexUITableFlags_SortTristate);
                FlexUI::SameLine(); HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
                FlexUI::TreePop();
            }

            if (FlexUI::TreeNodeEx("Other:", FlexUITreeNodeFlags_DefaultOpen))
            {
                FlexUI::Checkbox("show_headers", &show_headers);
                FlexUI::Checkbox("show_wrapped_text", &show_wrapped_text);

                FlexUI::DragFloat2("##OuterSize", &outer_size_value.x);
                FlexUI::SameLine(0.0f, FlexUI::GetStyle().ItemInnerSpacing.x);
                FlexUI::Checkbox("outer_size", &outer_size_enabled);
                FlexUI::SameLine();
                HelpMarker("If scrolling is disabled (ScrollX and ScrollY not set):\n"
                    "- The table is output directly in the parent window.\n"
                    "- OuterSize.x < 0.0f will right-align the table.\n"
                    "- OuterSize.x = 0.0f will narrow fit the table unless there are any Stretch columns.\n"
                    "- OuterSize.y then becomes the minimum size for the table, which will extend vertically if there are more rows (unless NoHostExtendY is set).");

                // From a user point of view we will tend to use 'inner_width' differently depending on whether our table is embedding scrolling.
                // To facilitate toying with this demo we will actually pass 0.0f to the BeginTable() when ScrollX is disabled.
                FlexUI::DragFloat("inner_width (when ScrollX active)", &inner_width_with_scroll, 1.0f, 0.0f, FLT_MAX);

                FlexUI::DragFloat("row_min_height", &row_min_height, 1.0f, 0.0f, FLT_MAX);
                FlexUI::SameLine(); HelpMarker("Specify height of the Selectable item.");

                FlexUI::DragInt("items_count", &items_count, 0.1f, 0, 9999);
                FlexUI::Combo("items_type (first column)", &contents_type, contents_type_names, IM_ARRAYSIZE(contents_type_names));
                //filter.Draw("filter");
                FlexUI::TreePop();
            }

            FlexUI::PopItemWidth();
            PopStyleCompact();
            FlexUI::Spacing();
            FlexUI::TreePop();
        }

        // Update item list if we changed the number of items
        static ImVector<MyItem> items;
        static ImVector<int> selection;
        static bool items_need_sort = false;
        if (items.Size != items_count)
        {
            items.resize(items_count, MyItem());
            for (int n = 0; n < items_count; n++)
            {
                const int template_n = n % IM_ARRAYSIZE(template_items_names);
                MyItem& item = items[n];
                item.ID = n;
                item.Name = template_items_names[template_n];
                item.Quantity = (template_n == 3) ? 10 : (template_n == 4) ? 20 : 0; // Assign default quantities
            }
        }

        const ImDrawList* parent_draw_list = FlexUI::GetWindowDrawList();
        const int parent_draw_list_draw_cmd_count = parent_draw_list->CmdBuffer.Size;
        ImVec2 table_scroll_cur, table_scroll_max; // For debug display
        const ImDrawList* table_draw_list = NULL;  // "

        // Submit table
        const float inner_width_to_use = (flags & FlexUITableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
        if (FlexUI::BeginTable("table_advanced", 6, flags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
        {
            // Declare columns
            // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
            // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
            FlexUI::TableSetupColumn("ID", FlexUITableColumnFlags_DefaultSort | FlexUITableColumnFlags_WidthFixed | FlexUITableColumnFlags_NoHide, 0.0f, MyItemColumnID_ID);
            FlexUI::TableSetupColumn("Name", FlexUITableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
            FlexUI::TableSetupColumn("Action", FlexUITableColumnFlags_NoSort | FlexUITableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
            FlexUI::TableSetupColumn("Quantity", FlexUITableColumnFlags_PreferSortDescending, 0.0f, MyItemColumnID_Quantity);
            FlexUI::TableSetupColumn("Description", (flags & FlexUITableFlags_NoHostExtendX) ? 0 : FlexUITableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Description);
            FlexUI::TableSetupColumn("Hidden", FlexUITableColumnFlags_DefaultHide | FlexUITableColumnFlags_NoSort);
            FlexUI::TableSetupScrollFreeze(freeze_cols, freeze_rows);

            // Sort our data if sort specs have been changed!
            FlexUITableSortSpecs* sorts_specs = FlexUI::TableGetSortSpecs();
            if (sorts_specs && sorts_specs->SpecsDirty)
                items_need_sort = true;
            if (sorts_specs && items_need_sort && items.Size > 1)
            {
                MyItem::s_current_sort_specs = sorts_specs; // Store in variable accessible by the sort function.
                qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
                MyItem::s_current_sort_specs = NULL;
                sorts_specs->SpecsDirty = false;
            }
            items_need_sort = false;

            // Take note of whether we are currently sorting based on the Quantity field,
            // we will use this to trigger sorting when we know the data of this column has been modified.
            const bool sorts_specs_using_quantity = (FlexUI::TableGetColumnFlags(3) & FlexUITableColumnFlags_IsSorted) != 0;

            // Show headers
            if (show_headers)
                FlexUI::TableHeadersRow();

            // Show data
            // FIXME-TABLE FIXME-NAV: How we can get decent up/down even though we have the buttons here?
            FlexUI::PushButtonRepeat(true);
#if 1
            // Demonstrate using clipper for large vertical lists
            FlexUIListClipper clipper;
            clipper.Begin(items.Size);
            while (clipper.Step())
            {
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
#else
            // Without clipper
                {
                    for (int row_n = 0; row_n < items.Size; row_n++)
#endif
                    {
                        MyItem* item = &items[row_n];
                        //if (!filter.PassFilter(item->Name))
                        //    continue;

                        const bool item_is_selected = selection.contains(item->ID);
                        FlexUI::PushID(item->ID);
                        FlexUI::TableNextRow(FlexUITableRowFlags_None, row_min_height);

                        // For the demo purpose we can select among different type of items submitted in the first column
                        FlexUI::TableSetColumnIndex(0);
                        char label[32];
                        sprintf(label, "%04d", item->ID);
                        if (contents_type == CT_Text)
                            FlexUI::TextUnformatted(label);
                        else if (contents_type == CT_Button)
                            FlexUI::Button(label);
                        else if (contents_type == CT_SmallButton)
                            FlexUI::SmallButton(label);
                        else if (contents_type == CT_FillButton)
                            FlexUI::Button(label, ImVec2(-FLT_MIN, 0.0f));
                        else if (contents_type == CT_Selectable || contents_type == CT_SelectableSpanRow)
                        {
                            FlexUISelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? FlexUISelectableFlags_SpanAllColumns | FlexUISelectableFlags_AllowItemOverlap : FlexUISelectableFlags_None;
                            if (FlexUI::Selectable(label, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
                            {
                                if (FlexUI::GetIO().KeyCtrl)
                                {
                                    if (item_is_selected)
                                        selection.find_erase_unsorted(item->ID);
                                    else
                                        selection.push_back(item->ID);
                                }
                                else
                                {
                                    selection.clear();
                                    selection.push_back(item->ID);
                                }
                            }
                        }

                        if (FlexUI::TableSetColumnIndex(1))
                            FlexUI::TextUnformatted(item->Name);

                        // Here we demonstrate marking our data set as needing to be sorted again if we modified a quantity,
                        // and we are currently sorting on the column showing the Quantity.
                        // To avoid triggering a sort while holding the button, we only trigger it when the button has been released.
                        // You will probably need a more advanced system in your code if you want to automatically sort when a specific entry changes.
                        if (FlexUI::TableSetColumnIndex(2))
                        {
                            if (FlexUI::SmallButton("Chop")) { item->Quantity += 1; }
                            if (sorts_specs_using_quantity && FlexUI::IsItemDeactivated()) { items_need_sort = true; }
                            FlexUI::SameLine();
                            if (FlexUI::SmallButton("Eat")) { item->Quantity -= 1; }
                            if (sorts_specs_using_quantity && FlexUI::IsItemDeactivated()) { items_need_sort = true; }
                        }

                        if (FlexUI::TableSetColumnIndex(3))
                            FlexUI::Text("%d", item->Quantity);

                        FlexUI::TableSetColumnIndex(4);
                        if (show_wrapped_text)
                            FlexUI::TextWrapped("Lorem ipsum dolor sit amet");
                        else
                            FlexUI::Text("Lorem ipsum dolor sit amet");

                        if (FlexUI::TableSetColumnIndex(5))
                            FlexUI::Text("1234");

                        FlexUI::PopID();
                    }
                }
                FlexUI::PopButtonRepeat();

                // Store some info to display debug details below
                table_scroll_cur = ImVec2(FlexUI::GetScrollX(), FlexUI::GetScrollY());
                table_scroll_max = ImVec2(FlexUI::GetScrollMaxX(), FlexUI::GetScrollMaxY());
                table_draw_list = FlexUI::GetWindowDrawList();
                FlexUI::EndTable();
            }
        static bool show_debug_details = false;
        FlexUI::Checkbox("Debug details", &show_debug_details);
        if (show_debug_details && table_draw_list)
        {
            FlexUI::SameLine(0.0f, 0.0f);
            const int table_draw_list_draw_cmd_count = table_draw_list->CmdBuffer.Size;
            if (table_draw_list == parent_draw_list)
                FlexUI::Text(": DrawCmd: +%d (in same window)",
                    table_draw_list_draw_cmd_count - parent_draw_list_draw_cmd_count);
            else
                FlexUI::Text(": DrawCmd: +%d (in child window), Scroll: (%.f/%.f) (%.f/%.f)",
                    table_draw_list_draw_cmd_count - 1, table_scroll_cur.x, table_scroll_max.x, table_scroll_cur.y, table_scroll_max.y);
        }
        FlexUI::TreePop();
        }

    FlexUI::PopID();

    ShowDemoWindowColumns();

    if (disable_indent)
        FlexUI::PopStyleVar();
    }

// Demonstrate old/legacy Columns API!
// [2020: Columns are under-featured and not maintained. Prefer using the more flexible and powerful BeginTable() API!]
static void ShowDemoWindowColumns()
{
    FlexUI_DEMO_MARKER("Columns (legacy API)");
    bool open = FlexUI::TreeNode("Legacy Columns API");
    FlexUI::SameLine();
    HelpMarker("Columns() is an old API! Prefer using the more flexible and powerful BeginTable() API!");
    if (!open)
        return;

    // Basic columns
    FlexUI_DEMO_MARKER("Columns (legacy API)/Basic");
    if (FlexUI::TreeNode("Basic"))
    {
        FlexUI::Text("Without border:");
        FlexUI::Columns(3, "mycolumns3", false);  // 3-ways, no border
        FlexUI::Separator();
        for (int n = 0; n < 14; n++)
        {
            char label[32];
            sprintf(label, "Item %d", n);
            if (FlexUI::Selectable(label)) {}
            //if (FlexUI::Button(label, ImVec2(-FLT_MIN,0.0f))) {}
            FlexUI::NextColumn();
        }
        FlexUI::Columns(1);
        FlexUI::Separator();

        FlexUI::Text("With border:");
        FlexUI::Columns(4, "mycolumns"); // 4-ways, with border
        FlexUI::Separator();
        FlexUI::Text("ID"); FlexUI::NextColumn();
        FlexUI::Text("Name"); FlexUI::NextColumn();
        FlexUI::Text("Path"); FlexUI::NextColumn();
        FlexUI::Text("Hovered"); FlexUI::NextColumn();
        FlexUI::Separator();
        const char* names[3] = { "One", "Two", "Three" };
        const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
        static int selected = -1;
        for (int i = 0; i < 3; i++)
        {
            char label[32];
            sprintf(label, "%04d", i);
            if (FlexUI::Selectable(label, selected == i, FlexUISelectableFlags_SpanAllColumns))
                selected = i;
            bool hovered = FlexUI::IsItemHovered();
            FlexUI::NextColumn();
            FlexUI::Text(names[i]); FlexUI::NextColumn();
            FlexUI::Text(paths[i]); FlexUI::NextColumn();
            FlexUI::Text("%d", hovered); FlexUI::NextColumn();
        }
        FlexUI::Columns(1);
        FlexUI::Separator();
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Columns (legacy API)/Borders");
    if (FlexUI::TreeNode("Borders"))
    {
        // NB: Future columns API should allow automatic horizontal borders.
        static bool h_borders = true;
        static bool v_borders = true;
        static int columns_count = 4;
        const int lines_count = 3;
        FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 8);
        FlexUI::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
        if (columns_count < 2)
            columns_count = 2;
        FlexUI::SameLine();
        FlexUI::Checkbox("horizontal", &h_borders);
        FlexUI::SameLine();
        FlexUI::Checkbox("vertical", &v_borders);
        FlexUI::Columns(columns_count, NULL, v_borders);
        for (int i = 0; i < columns_count * lines_count; i++)
        {
            if (h_borders && FlexUI::GetColumnIndex() == 0)
                FlexUI::Separator();
            FlexUI::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
            FlexUI::Text("Width %.2f", FlexUI::GetColumnWidth());
            FlexUI::Text("Avail %.2f", FlexUI::GetContentRegionAvail().x);
            FlexUI::Text("Offset %.2f", FlexUI::GetColumnOffset());
            FlexUI::Text("Long text that is likely to clip");
            FlexUI::Button("Button", ImVec2(-FLT_MIN, 0.0f));
            FlexUI::NextColumn();
        }
        FlexUI::Columns(1);
        if (h_borders)
            FlexUI::Separator();
        FlexUI::TreePop();
    }

    // Create multiple items in a same cell before switching to next column
    FlexUI_DEMO_MARKER("Columns (legacy API)/Mixed items");
    if (FlexUI::TreeNode("Mixed items"))
    {
        FlexUI::Columns(3, "mixed");
        FlexUI::Separator();

        FlexUI::Text("Hello");
        FlexUI::Button("Banana");
        FlexUI::NextColumn();

        FlexUI::Text("FlexUI");
        FlexUI::Button("Apple");
        static float foo = 1.0f;
        FlexUI::InputFloat("red", &foo, 0.05f, 0, "%.3f");
        FlexUI::Text("An extra line here.");
        FlexUI::NextColumn();

        FlexUI::Text("Sailor");
        FlexUI::Button("Corniflower");
        static float bar = 1.0f;
        FlexUI::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
        FlexUI::NextColumn();

        if (FlexUI::CollapsingHeader("Category A")) { FlexUI::Text("Blah blah blah"); } FlexUI::NextColumn();
        if (FlexUI::CollapsingHeader("Category B")) { FlexUI::Text("Blah blah blah"); } FlexUI::NextColumn();
        if (FlexUI::CollapsingHeader("Category C")) { FlexUI::Text("Blah blah blah"); } FlexUI::NextColumn();
        FlexUI::Columns(1);
        FlexUI::Separator();
        FlexUI::TreePop();
    }

    // Word wrapping
    FlexUI_DEMO_MARKER("Columns (legacy API)/Word-wrapping");
    if (FlexUI::TreeNode("Word-wrapping"))
    {
        FlexUI::Columns(2, "word-wrapping");
        FlexUI::Separator();
        FlexUI::TextWrapped("The quick brown fox jumps over the lazy dog.");
        FlexUI::TextWrapped("Hello Left");
        FlexUI::NextColumn();
        FlexUI::TextWrapped("The quick brown fox jumps over the lazy dog.");
        FlexUI::TextWrapped("Hello Right");
        FlexUI::Columns(1);
        FlexUI::Separator();
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Columns (legacy API)/Horizontal Scrolling");
    if (FlexUI::TreeNode("Horizontal Scrolling"))
    {
        FlexUI::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
        ImVec2 child_size = ImVec2(0, FlexUI::GetFontSize() * 20.0f);
        FlexUI::BeginChild("##ScrollingRegion", child_size, false, FlexUIWindowFlags_HorizontalScrollbar);
        FlexUI::Columns(10);

        // Also demonstrate using clipper for large vertical lists
        int ITEMS_COUNT = 2000;
        FlexUIListClipper clipper;
        clipper.Begin(ITEMS_COUNT);
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                for (int j = 0; j < 10; j++)
                {
                    FlexUI::Text("Line %d Column %d...", i, j);
                    FlexUI::NextColumn();
                }
        }
        FlexUI::Columns(1);
        FlexUI::EndChild();
        FlexUI::TreePop();
    }

    FlexUI_DEMO_MARKER("Columns (legacy API)/Tree");
    if (FlexUI::TreeNode("Tree"))
    {
        FlexUI::Columns(2, "tree", true);
        for (int x = 0; x < 3; x++)
        {
            bool open1 = FlexUI::TreeNode((void*)(intptr_t)x, "Node%d", x);
            FlexUI::NextColumn();
            FlexUI::Text("Node contents");
            FlexUI::NextColumn();
            if (open1)
            {
                for (int y = 0; y < 3; y++)
                {
                    bool open2 = FlexUI::TreeNode((void*)(intptr_t)y, "Node%d.%d", x, y);
                    FlexUI::NextColumn();
                    FlexUI::Text("Node contents");
                    if (open2)
                    {
                        FlexUI::Text("Even more contents");
                        if (FlexUI::TreeNode("Tree in column"))
                        {
                            FlexUI::Text("The quick brown fox jumps over the lazy dog");
                            FlexUI::TreePop();
                        }
                    }
                    FlexUI::NextColumn();
                    if (open2)
                        FlexUI::TreePop();
                }
                FlexUI::TreePop();
            }
        }
        FlexUI::Columns(1);
        FlexUI::TreePop();
    }

    FlexUI::TreePop();
}

namespace FlexUI { extern FlexUIKeyData* GetKeyData(FlexUIKey key); }

static void ShowDemoWindowInputs()
{
    FlexUI_DEMO_MARKER("Inputs & Focus");
    if (FlexUI::CollapsingHeader("Inputs & Focus"))
    {
        FlexUIIO& io = FlexUI::GetIO();

        // Display inputs submitted to FlexUIIO
        FlexUI_DEMO_MARKER("Inputs & Focus/Inputs");
        FlexUI::SetNextItemOpen(true, FlexUICond_Once);
        if (FlexUI::TreeNode("Inputs"))
        {
            HelpMarker(
                "This is a simplified view. See more detailed input state:\n"
                "- in 'Tools->Metrics/Debugger->Inputs'.\n"
                "- in 'Tools->Debug Log->IO'.");
            if (FlexUI::IsMousePosValid())
                FlexUI::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                FlexUI::Text("Mouse pos: <INVALID>");
            FlexUI::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            FlexUI::Text("Mouse down:");
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (FlexUI::IsMouseDown(i)) { FlexUI::SameLine(); FlexUI::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            FlexUI::Text("Mouse wheel: %.1f", io.MouseWheel);

            // We iterate both legacy native range and named FlexUIKey ranges, which is a little odd but this allows displaying the data for old/new backends.
            // User code should never have to go through such hoops: old code may use native keycodes, new code may use FlexUIKey codes.
#ifdef FlexUI_DISABLE_OBSOLETE_KEYIO
            struct funcs { static bool IsLegacyNativeDupe(FlexUIKey) { return false; } };
#else
            struct funcs { static bool IsLegacyNativeDupe(FlexUIKey key) { return key < 512 && FlexUI::GetIO().KeyMap[key] != -1; } }; // Hide Native<>FlexUIKey duplicates when both exists in the array
#endif
            FlexUI::Text("Keys down:");         for (FlexUIKey key = FlexUIKey_KeysData_OFFSET; key < FlexUIKey_COUNT; key = (FlexUIKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !FlexUI::IsKeyDown(key)) continue; FlexUI::SameLine(); FlexUI::Text((key < FlexUIKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", FlexUI::GetKeyName(key), key); FlexUI::SameLine(); FlexUI::Text("(%.02f)", FlexUI::GetKeyData(key)->DownDuration); }
            FlexUI::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            FlexUI::Text("Chars queue:");       for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; FlexUI::SameLine();  FlexUI::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

            FlexUI::TreePop();
        }

        // Display FlexUIIO output flags
        FlexUI_DEMO_MARKER("Inputs & Focus/Outputs");
        FlexUI::SetNextItemOpen(true, FlexUICond_Once);
        if (FlexUI::TreeNode("Outputs"))
        {
            HelpMarker(
                "The value of io.WantCaptureMouse and io.WantCaptureKeyboard are normally set by Dear FlexUI "
                "to instruct your application of how to route inputs. Typically, when a value is true, it means "
                "Dear FlexUI wants the corresponding inputs and we expect the underlying application to ignore them.\n\n"
                "The most typical case is: when hovering a window, Dear FlexUI set io.WantCaptureMouse to true, "
                "and underlying application should ignore mouse inputs (in practice there are many and more subtle "
                "rules leading to how those flags are set).");
            FlexUI::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
            FlexUI::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
            FlexUI::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
            FlexUI::Text("io.WantTextInput: %d", io.WantTextInput);
            FlexUI::Text("io.WantSetMousePos: %d", io.WantSetMousePos);
            FlexUI::Text("io.NavActive: %d, io.NavVisible: %d", io.NavActive, io.NavVisible);

            FlexUI_DEMO_MARKER("Inputs & Focus/Outputs/WantCapture override");
            if (FlexUI::TreeNode("WantCapture override"))
            {
                HelpMarker(
                    "Hovering the colored canvas will override io.WantCaptureXXX fields.\n"
                    "Notice how normally (when set to none), the value of io.WantCaptureKeyboard would be false when hovering and true when clicking.");
                static int capture_override_mouse = -1;
                static int capture_override_keyboard = -1;
                const char* capture_override_desc[] = { "None", "Set to false", "Set to true" };
                FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 15);
                FlexUI::SliderInt("SetNextFrameWantCaptureMouse() on hover", &capture_override_mouse, -1, +1, capture_override_desc[capture_override_mouse + 1], FlexUISliderFlags_AlwaysClamp);
                FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 15);
                FlexUI::SliderInt("SetNextFrameWantCaptureKeyboard() on hover", &capture_override_keyboard, -1, +1, capture_override_desc[capture_override_keyboard + 1], FlexUISliderFlags_AlwaysClamp);

                FlexUI::ColorButton("##panel", ImVec4(0.7f, 0.1f, 0.7f, 1.0f), FlexUIColorEditFlags_NoTooltip | FlexUIColorEditFlags_NoDragDrop, ImVec2(128.0f, 96.0f)); // Dummy item
                if (FlexUI::IsItemHovered() && capture_override_mouse != -1)
                    FlexUI::SetNextFrameWantCaptureMouse(capture_override_mouse == 1);
                if (FlexUI::IsItemHovered() && capture_override_keyboard != -1)
                    FlexUI::SetNextFrameWantCaptureKeyboard(capture_override_keyboard == 1);

                FlexUI::TreePop();
            }
            FlexUI::TreePop();
        }

        // Display mouse cursors
        FlexUI_DEMO_MARKER("Inputs & Focus/Mouse Cursors");
        if (FlexUI::TreeNode("Mouse Cursors"))
        {
            const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed" };
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == FlexUIMouseCursor_COUNT);

            FlexUIMouseCursor current = FlexUI::GetMouseCursor();
            FlexUI::Text("Current mouse cursor = %d: %s", current, mouse_cursors_names[current]);
            FlexUI::BeginDisabled(true);
            FlexUI::CheckboxFlags("io.BackendFlags: HasMouseCursors", &io.BackendFlags, FlexUIBackendFlags_HasMouseCursors);
            FlexUI::EndDisabled();

            FlexUI::Text("Hover to see mouse cursors:");
            FlexUI::SameLine(); HelpMarker(
                "Your application can render a different mouse cursor based on what FlexUI::GetMouseCursor() returns. "
                "If software cursor rendering (io.MouseDrawCursor) is set FlexUI will draw the right cursor for you, "
                "otherwise your backend needs to handle it.");
            for (int i = 0; i < FlexUIMouseCursor_COUNT; i++)
            {
                char label[32];
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
                FlexUI::Bullet(); FlexUI::Selectable(label, false);
                if (FlexUI::IsItemHovered())
                    FlexUI::SetMouseCursor(i);
            }
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Inputs & Focus/Tabbing");
        if (FlexUI::TreeNode("Tabbing"))
        {
            FlexUI::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
            static char buf[32] = "hello";
            FlexUI::InputText("1", buf, IM_ARRAYSIZE(buf));
            FlexUI::InputText("2", buf, IM_ARRAYSIZE(buf));
            FlexUI::InputText("3", buf, IM_ARRAYSIZE(buf));
            FlexUI::PushAllowKeyboardFocus(false);
            FlexUI::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
            FlexUI::SameLine(); HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
            FlexUI::PopAllowKeyboardFocus();
            FlexUI::InputText("5", buf, IM_ARRAYSIZE(buf));
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Inputs & Focus/Focus from code");
        if (FlexUI::TreeNode("Focus from code"))
        {
            bool focus_1 = FlexUI::Button("Focus on 1"); FlexUI::SameLine();
            bool focus_2 = FlexUI::Button("Focus on 2"); FlexUI::SameLine();
            bool focus_3 = FlexUI::Button("Focus on 3");
            int has_focus = 0;
            static char buf[128] = "click on a button to set focus";

            if (focus_1) FlexUI::SetKeyboardFocusHere();
            FlexUI::InputText("1", buf, IM_ARRAYSIZE(buf));
            if (FlexUI::IsItemActive()) has_focus = 1;

            if (focus_2) FlexUI::SetKeyboardFocusHere();
            FlexUI::InputText("2", buf, IM_ARRAYSIZE(buf));
            if (FlexUI::IsItemActive()) has_focus = 2;

            FlexUI::PushAllowKeyboardFocus(false);
            if (focus_3) FlexUI::SetKeyboardFocusHere();
            FlexUI::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
            if (FlexUI::IsItemActive()) has_focus = 3;
            FlexUI::SameLine(); HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
            FlexUI::PopAllowKeyboardFocus();

            if (has_focus)
                FlexUI::Text("Item with focus: %d", has_focus);
            else
                FlexUI::Text("Item with focus: <none>");

            // Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
            static float f3[3] = { 0.0f, 0.0f, 0.0f };
            int focus_ahead = -1;
            if (FlexUI::Button("Focus on X")) { focus_ahead = 0; } FlexUI::SameLine();
            if (FlexUI::Button("Focus on Y")) { focus_ahead = 1; } FlexUI::SameLine();
            if (FlexUI::Button("Focus on Z")) { focus_ahead = 2; }
            if (focus_ahead != -1) FlexUI::SetKeyboardFocusHere(focus_ahead);
            FlexUI::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

            FlexUI::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
            FlexUI::TreePop();
        }

        FlexUI_DEMO_MARKER("Inputs & Focus/Dragging");
        if (FlexUI::TreeNode("Dragging"))
        {
            FlexUI::TextWrapped("You can use FlexUI::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
            for (int button = 0; button < 3; button++)
            {
                FlexUI::Text("IsMouseDragging(%d):", button);
                FlexUI::Text("  w/ default threshold: %d,", FlexUI::IsMouseDragging(button));
                FlexUI::Text("  w/ zero threshold: %d,", FlexUI::IsMouseDragging(button, 0.0f));
                FlexUI::Text("  w/ large threshold: %d,", FlexUI::IsMouseDragging(button, 20.0f));
            }

            FlexUI::Button("Drag Me");
            if (FlexUI::IsItemActive())
                FlexUI::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, FlexUI::GetColorU32(FlexUICol_Button), 4.0f); // Draw a line between the button and the mouse cursor

            // Drag operations gets "unlocked" when the mouse has moved past a certain threshold
            // (the default threshold is stored in io.MouseDragThreshold). You can request a lower or higher
            // threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta().
            ImVec2 value_raw = FlexUI::GetMouseDragDelta(0, 0.0f);
            ImVec2 value_with_lock_threshold = FlexUI::GetMouseDragDelta(0);
            ImVec2 mouse_delta = io.MouseDelta;
            FlexUI::Text("GetMouseDragDelta(0):");
            FlexUI::Text("  w/ default threshold: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y);
            FlexUI::Text("  w/ zero threshold: (%.1f, %.1f)", value_raw.x, value_raw.y);
            FlexUI::Text("io.MouseDelta: (%.1f, %.1f)", mouse_delta.x, mouse_delta.y);
            FlexUI::TreePop();
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] About Window / ShowAboutWindow()
// Access from Dear FlexUI Demo -> Tools -> About
//-----------------------------------------------------------------------------

void FlexUI::ShowAboutWindow(bool* p_open)
{
    if (!FlexUI::Begin("About Dear FlexUI", p_open, FlexUIWindowFlags_AlwaysAutoResize))
    {
        FlexUI::End();
        return;
    }
    FlexUI_DEMO_MARKER("Tools/About Dear FlexUI");
    FlexUI::Text("Dear FlexUI %s", FlexUI::GetVersion());
    FlexUI::Separator();
    FlexUI::Text("By Omar Cornut and all Dear FlexUI contributors.");
    FlexUI::Text("Dear FlexUI is licensed under the MIT License, see LICENSE for more information.");

    static bool show_config_info = false;
    FlexUI::Checkbox("Config/Build Information", &show_config_info);
    if (show_config_info)
    {
        FlexUIIO& io = FlexUI::GetIO();
        FlexUIStyle& style = FlexUI::GetStyle();

        bool copy_to_clipboard = FlexUI::Button("Copy to clipboard");
        ImVec2 child_size = ImVec2(0, FlexUI::GetTextLineHeightWithSpacing() * 18);
        FlexUI::BeginChildFrame(FlexUI::GetID("cfg_infos"), child_size, FlexUIWindowFlags_NoMove);
        if (copy_to_clipboard)
        {
            FlexUI::LogToClipboard();
            FlexUI::LogText("```\n"); // Back quotes will make text appears without formatting when pasting on GitHub
        }

        FlexUI::Text("Dear FlexUI %s (%d)", FlexUI_VERSION, FlexUI_VERSION_NUM);
        FlexUI::Separator();
        FlexUI::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
        FlexUI::Text("define: __cplusplus=%d", (int)__cplusplus);
#ifdef FlexUI_DISABLE_OBSOLETE_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_OBSOLETE_KEYIO
        FlexUI::Text("define: FlexUI_DISABLE_OBSOLETE_KEYIO");
#endif
#ifdef FlexUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_WIN32_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_DEFAULT_MATH_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_DEFAULT_MATH_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_DEFAULT_FILE_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_DEFAULT_FILE_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_FILE_FUNCTIONS
        FlexUI::Text("define: FlexUI_DISABLE_FILE_FUNCTIONS");
#endif
#ifdef FlexUI_DISABLE_DEFAULT_ALLOCATORS
        FlexUI::Text("define: FlexUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef FlexUI_USE_BGRA_PACKED_COLOR
        FlexUI::Text("define: FlexUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
        FlexUI::Text("define: _WIN32");
#endif
#ifdef _WIN64
        FlexUI::Text("define: _WIN64");
#endif
#ifdef __linux__
        FlexUI::Text("define: __linux__");
#endif
#ifdef __APPLE__
        FlexUI::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
        FlexUI::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
#ifdef _MSVC_LANG
        FlexUI::Text("define: _MSVC_LANG=%d", (int)_MSVC_LANG);
#endif
#ifdef __MINGW32__
        FlexUI::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
        FlexUI::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
        FlexUI::Text("define: __GNUC__=%d", (int)__GNUC__);
#endif
#ifdef __clang_version__
        FlexUI::Text("define: __clang_version__=%s", __clang_version__);
#endif
        FlexUI::Separator();
        FlexUI::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
        FlexUI::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
        FlexUI::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
        if (io.ConfigFlags & FlexUIConfigFlags_NavEnableKeyboard)        FlexUI::Text(" NavEnableKeyboard");
        if (io.ConfigFlags & FlexUIConfigFlags_NavEnableGamepad)         FlexUI::Text(" NavEnableGamepad");
        if (io.ConfigFlags & FlexUIConfigFlags_NavEnableSetMousePos)     FlexUI::Text(" NavEnableSetMousePos");
        if (io.ConfigFlags & FlexUIConfigFlags_NavNoCaptureKeyboard)     FlexUI::Text(" NavNoCaptureKeyboard");
        if (io.ConfigFlags & FlexUIConfigFlags_NoMouse)                  FlexUI::Text(" NoMouse");
        if (io.ConfigFlags & FlexUIConfigFlags_NoMouseCursorChange)      FlexUI::Text(" NoMouseCursorChange");
        if (io.MouseDrawCursor)                                         FlexUI::Text("io.MouseDrawCursor");
        if (io.ConfigMacOSXBehaviors)                                   FlexUI::Text("io.ConfigMacOSXBehaviors");
        if (io.ConfigInputTextCursorBlink)                              FlexUI::Text("io.ConfigInputTextCursorBlink");
        if (io.ConfigWindowsResizeFromEdges)                            FlexUI::Text("io.ConfigWindowsResizeFromEdges");
        if (io.ConfigWindowsMoveFromTitleBarOnly)                       FlexUI::Text("io.ConfigWindowsMoveFromTitleBarOnly");
        if (io.ConfigMemoryCompactTimer >= 0.0f)                        FlexUI::Text("io.ConfigMemoryCompactTimer = %.1f", io.ConfigMemoryCompactTimer);
        FlexUI::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
        if (io.BackendFlags & FlexUIBackendFlags_HasGamepad)             FlexUI::Text(" HasGamepad");
        if (io.BackendFlags & FlexUIBackendFlags_HasMouseCursors)        FlexUI::Text(" HasMouseCursors");
        if (io.BackendFlags & FlexUIBackendFlags_HasSetMousePos)         FlexUI::Text(" HasSetMousePos");
        if (io.BackendFlags & FlexUIBackendFlags_RendererHasVtxOffset)   FlexUI::Text(" RendererHasVtxOffset");
        FlexUI::Separator();
        FlexUI::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
        FlexUI::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
        FlexUI::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        FlexUI::Separator();
        FlexUI::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
        FlexUI::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
        FlexUI::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
        FlexUI::Text("style.FrameRounding: %.2f", style.FrameRounding);
        FlexUI::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
        FlexUI::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
        FlexUI::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

        if (copy_to_clipboard)
        {
            FlexUI::LogText("\n```\n");
            FlexUI::LogFinish();
        }
        FlexUI::EndChildFrame();
    }
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Style Editor / ShowStyleEditor()
//-----------------------------------------------------------------------------
// - ShowFontSelector()
// - ShowStyleSelector()
// - ShowStyleEditor()
//-----------------------------------------------------------------------------

// Forward declare ShowFontAtlas() which isn't worth putting in public API yet
namespace FlexUI { FlexUI_API void ShowFontAtlas(ImFontAtlas* atlas); }

// Demo helper function to select among loaded fonts.
// Here we use the regular BeginCombo()/EndCombo() api which is the more flexible one.
void FlexUI::ShowFontSelector(const char* label)
{
    FlexUIIO& io = FlexUI::GetIO();
    ImFont* font_current = FlexUI::GetFont();
    if (FlexUI::BeginCombo(label, font_current->GetDebugName()))
    {
        for (int n = 0; n < io.Fonts->Fonts.Size; n++)
        {
            ImFont* font = io.Fonts->Fonts[n];
            FlexUI::PushID((void*)font);
            if (FlexUI::Selectable(font->GetDebugName(), font == font_current))
                io.FontDefault = font;
            FlexUI::PopID();
        }
        FlexUI::EndCombo();
    }
    FlexUI::SameLine();
    HelpMarker(
        "- Load additional fonts with io.Fonts->AddFontFromFileTTF().\n"
        "- The font atlas is built when calling io.Fonts->GetTexDataAsXXXX() or io.Fonts->Build().\n"
        "- Read FAQ and docs/FONTS.md for more details.\n"
        "- If you need to add/remove fonts at runtime (e.g. for DPI change), do it before calling NewFrame().");
}

// Demo helper function to select among default colors. See ShowStyleEditor() for more advanced options.
// Here we use the simplified Combo() api that packs items into a single literal string.
// Useful for quick combo boxes where the choices are known locally.
bool FlexUI::ShowStyleSelector(const char* label)
{
    static int style_idx = -1;
    if (FlexUI::Combo(label, &style_idx, "Dark\0Light\0Classic\0"))
    {
        switch (style_idx)
        {
        case 0: FlexUI::StyleColorsDark(); break;
        case 1: FlexUI::StyleColorsLight(); break;
        case 2: FlexUI::StyleColorsClassic(); break;
        }
        return true;
    }
    return false;
}

void FlexUI::ShowStyleEditor(FlexUIStyle * ref)
{
    FlexUI_DEMO_MARKER("Tools/Style Editor");
    // You can pass in a reference FlexUIStyle structure to compare to, revert to and save to
    // (without a reference style pointer, we will use one compared locally as a reference)
    FlexUIStyle& style = FlexUI::GetStyle();
    static FlexUIStyle ref_saved_style;

    // Default to using internal storage as reference
    static bool init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    FlexUI::PushItemWidth(FlexUI::GetWindowWidth() * 0.50f);

    if (FlexUI::ShowStyleSelector("Colors##Selector"))
        ref_saved_style = style;
    FlexUI::ShowFontSelector("Fonts##Selector");

    // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
    if (FlexUI::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
        style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
    { bool border = (style.WindowBorderSize > 0.0f); if (FlexUI::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
    FlexUI::SameLine();
    { bool border = (style.FrameBorderSize > 0.0f);  if (FlexUI::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
    FlexUI::SameLine();
    { bool border = (style.PopupBorderSize > 0.0f);  if (FlexUI::Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }

    // Save/Revert button
    if (FlexUI::Button("Save Ref"))
        *ref = ref_saved_style = style;
    FlexUI::SameLine();
    if (FlexUI::Button("Revert Ref"))
        style = *ref;
    FlexUI::SameLine();
    HelpMarker(
        "Save/Revert in local non-persistent storage. Default Colors definition are not affected. "
        "Use \"Export\" below to save them somewhere.");

    FlexUI::Separator();

    if (FlexUI::BeginTabBar("##tabs", FlexUITabBarFlags_None))
    {
        if (FlexUI::BeginTabItem("Sizes"))
        {
            FlexUI::Text("Main");
            FlexUI::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            FlexUI::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            FlexUI::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
            FlexUI::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            FlexUI::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            FlexUI::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            FlexUI::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            FlexUI::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            FlexUI::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
            FlexUI::Text("Borders");
            FlexUI::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            FlexUI::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            FlexUI::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            FlexUI::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
            FlexUI::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
            FlexUI::Text("Rounding");
            FlexUI::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
            FlexUI::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
            FlexUI::Text("Alignment");
            FlexUI::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
            if (FlexUI::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                style.WindowMenuButtonPosition = window_menu_button_position - 1;
            FlexUI::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
            FlexUI::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
            FlexUI::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
            FlexUI::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
            FlexUI::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
            FlexUI::Text("Safe Area Padding");
            FlexUI::SameLine(); HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
            FlexUI::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
            FlexUI::EndTabItem();
        }

        if (FlexUI::BeginTabItem("Colors"))
        {
            static int output_dest = 0;
            static bool output_only_modified = true;
            if (FlexUI::Button("Export"))
            {
                if (output_dest == 0)
                    FlexUI::LogToClipboard();
                else
                    FlexUI::LogToTTY();
                FlexUI::LogText("ImVec4* colors = FlexUI::GetStyle().Colors;" IM_NEWLINE);
                for (int i = 0; i < FlexUICol_COUNT; i++)
                {
                    const ImVec4& col = style.Colors[i];
                    const char* name = FlexUI::GetStyleColorName(i);
                    if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                        FlexUI::LogText("colors[FlexUICol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE,
                            name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                }
                FlexUI::LogFinish();
            }
            FlexUI::SameLine(); FlexUI::SetNextItemWidth(120); FlexUI::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
            FlexUI::SameLine(); FlexUI::Checkbox("Only Modified Colors", &output_only_modified);

            static FlexUITextFilter filter;
            filter.Draw("Filter colors", FlexUI::GetFontSize() * 16);

            static FlexUIColorEditFlags alpha_flags = 0;
            if (FlexUI::RadioButton("Opaque", alpha_flags == FlexUIColorEditFlags_None)) { alpha_flags = FlexUIColorEditFlags_None; } FlexUI::SameLine();
            if (FlexUI::RadioButton("Alpha", alpha_flags == FlexUIColorEditFlags_AlphaPreview)) { alpha_flags = FlexUIColorEditFlags_AlphaPreview; } FlexUI::SameLine();
            if (FlexUI::RadioButton("Both", alpha_flags == FlexUIColorEditFlags_AlphaPreviewHalf)) { alpha_flags = FlexUIColorEditFlags_AlphaPreviewHalf; } FlexUI::SameLine();
            HelpMarker(
                "In the color list:\n"
                "Left-click on color square to open color picker,\n"
                "Right-click to open edit options menu.");

            FlexUI::BeginChild("##colors", ImVec2(0, 0), true, FlexUIWindowFlags_AlwaysVerticalScrollbar | FlexUIWindowFlags_AlwaysHorizontalScrollbar | FlexUIWindowFlags_NavFlattened);
            FlexUI::PushItemWidth(-160);
            for (int i = 0; i < FlexUICol_COUNT; i++)
            {
                const char* name = FlexUI::GetStyleColorName(i);
                if (!filter.PassFilter(name))
                    continue;
                FlexUI::PushID(i);
                FlexUI::ColorEdit4("##color", (float*)&style.Colors[i], FlexUIColorEditFlags_AlphaBar | alpha_flags);
                if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                {
                    // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                    // so instead of "Save"/"Revert" you'd use icons!
                    // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                    FlexUI::SameLine(0.0f, style.ItemInnerSpacing.x); if (FlexUI::Button("Save")) { ref->Colors[i] = style.Colors[i]; }
                    FlexUI::SameLine(0.0f, style.ItemInnerSpacing.x); if (FlexUI::Button("Revert")) { style.Colors[i] = ref->Colors[i]; }
                }
                FlexUI::SameLine(0.0f, style.ItemInnerSpacing.x);
                FlexUI::TextUnformatted(name);
                FlexUI::PopID();
            }
            FlexUI::PopItemWidth();
            FlexUI::EndChild();

            FlexUI::EndTabItem();
        }

        if (FlexUI::BeginTabItem("Fonts"))
        {
            FlexUIIO& io = FlexUI::GetIO();
            ImFontAtlas* atlas = io.Fonts;
            HelpMarker("Read FAQ and docs/FONTS.md for details on font loading.");
            FlexUI::ShowFontAtlas(atlas);

            // Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
            // (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
            const float MIN_SCALE = 0.3f;
            const float MAX_SCALE = 2.0f;
            HelpMarker(
                "Those are old settings provided for convenience.\n"
                "However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
                "rebuild the font atlas, and call style.ScaleAllSizes() on a reference FlexUIStyle structure.\n"
                "Using those settings here will give you poor quality results.");
            static float window_scale = 1.0f;
            FlexUI::PushItemWidth(FlexUI::GetFontSize() * 8);
            if (FlexUI::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", FlexUISliderFlags_AlwaysClamp)) // Scale only this window
                FlexUI::SetWindowFontScale(window_scale);
            FlexUI::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", FlexUISliderFlags_AlwaysClamp); // Scale everything
            FlexUI::PopItemWidth();

            FlexUI::EndTabItem();
        }

        if (FlexUI::BeginTabItem("Rendering"))
        {
            FlexUI::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
            FlexUI::SameLine();
            HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");

            FlexUI::Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
            FlexUI::SameLine();
            HelpMarker("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering).");

            FlexUI::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
            FlexUI::PushItemWidth(FlexUI::GetFontSize() * 8);
            FlexUI::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
            if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

            // When editing the "Circle Segment Max Error" value, draw a preview of its effect on auto-tessellated circles.
            FlexUI::DragFloat("Circle Tessellation Max Error", &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f, "%.2f", FlexUISliderFlags_AlwaysClamp);
            if (FlexUI::IsItemActive())
            {
                FlexUI::SetNextWindowPos(FlexUI::GetCursorScreenPos());
                FlexUI::BeginTooltip();
                FlexUI::TextUnformatted("(R = radius, N = number of segments)");
                FlexUI::Spacing();
                ImDrawList* draw_list = FlexUI::GetWindowDrawList();
                const float min_widget_width = FlexUI::CalcTextSize("N: MMM\nR: MMM").x;
                for (int n = 0; n < 8; n++)
                {
                    const float RAD_MIN = 5.0f;
                    const float RAD_MAX = 70.0f;
                    const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

                    FlexUI::BeginGroup();

                    FlexUI::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

                    const float canvas_width = IM_MAX(min_widget_width, rad * 2.0f);
                    const float offset_x = floorf(canvas_width * 0.5f);
                    const float offset_y = floorf(RAD_MAX);

                    const ImVec2 p1 = FlexUI::GetCursorScreenPos();
                    draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, FlexUI::GetColorU32(FlexUICol_Text));
                    FlexUI::Dummy(ImVec2(canvas_width, RAD_MAX * 2));

                    /*
                    const ImVec2 p2 = FlexUI::GetCursorScreenPos();
                    draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, FlexUI::GetColorU32(FlexUICol_Text));
                    FlexUI::Dummy(ImVec2(canvas_width, RAD_MAX * 2));
                    */

                    FlexUI::EndGroup();
                    FlexUI::SameLine();
                }
                FlexUI::EndTooltip();
            }
            FlexUI::SameLine();
            HelpMarker("When drawing circle primitives with \"num_segments == 0\" tesselation will be calculated automatically.");

            FlexUI::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
            FlexUI::DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f, "%.2f"); FlexUI::SameLine(); HelpMarker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");
            FlexUI::PopItemWidth();

            FlexUI::EndTabItem();
        }

        FlexUI::EndTabBar();
    }

    FlexUI::PopItemWidth();
}

//-----------------------------------------------------------------------------
// [SECTION] User Guide / ShowUserGuide()
//-----------------------------------------------------------------------------

void FlexUI::ShowUserGuide()
{
    FlexUIIO& io = FlexUI::GetIO();
    FlexUI::BulletText("Double-click on title bar to collapse window.");
    FlexUI::BulletText(
        "Click and drag on lower corner to resize window\n"
        "(double-click to auto fit window to its contents).");
    FlexUI::BulletText("CTRL+Click on a slider or drag box to input value as text.");
    FlexUI::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
    FlexUI::BulletText("CTRL+Tab to select a window.");
    if (io.FontAllowUserScaling)
        FlexUI::BulletText("CTRL+Mouse Wheel to zoom window contents.");
    FlexUI::BulletText("While inputing text:\n");
    FlexUI::Indent();
    FlexUI::BulletText("CTRL+Left/Right to word jump.");
    FlexUI::BulletText("CTRL+A or double-click to select all.");
    FlexUI::BulletText("CTRL+X/C/V to use clipboard cut/copy/paste.");
    FlexUI::BulletText("CTRL+Z,CTRL+Y to undo/redo.");
    FlexUI::BulletText("ESCAPE to revert.");
    FlexUI::Unindent();
    FlexUI::BulletText("With keyboard navigation enabled:");
    FlexUI::Indent();
    FlexUI::BulletText("Arrow keys to navigate.");
    FlexUI::BulletText("Space to activate a widget.");
    FlexUI::BulletText("Return to input text into a widget.");
    FlexUI::BulletText("Escape to deactivate a widget, close popup, exit child window.");
    FlexUI::BulletText("Alt to jump to the menu layer of a window.");
    FlexUI::Unindent();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// Note the difference between BeginMainMenuBar() and BeginMenuBar():
// - BeginMenuBar() = menu-bar inside current window (which needs the FlexUIWindowFlags_MenuBar flag!)
// - BeginMainMenuBar() = helper to create menu-bar-sized window at the top of the main viewport + call BeginMenuBar() into it.
static void ShowExampleAppMainMenuBar()
{
    if (FlexUI::BeginMainMenuBar())
    {
        if (FlexUI::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            FlexUI::EndMenu();
        }
        if (FlexUI::BeginMenu("Edit"))
        {
            if (FlexUI::MenuItem("Undo", "CTRL+Z")) {}
            if (FlexUI::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            FlexUI::Separator();
            if (FlexUI::MenuItem("Cut", "CTRL+X")) {}
            if (FlexUI::MenuItem("Copy", "CTRL+C")) {}
            if (FlexUI::MenuItem("Paste", "CTRL+V")) {}
            FlexUI::EndMenu();
        }
        FlexUI::EndMainMenuBar();
    }
}

// Note that shortcuts are currently provided for display only
// (future version will add explicit flags to BeginMenu() to request processing shortcuts)
static void ShowExampleMenuFile()
{
    FlexUI_DEMO_MARKER("Examples/Menu");
    FlexUI::MenuItem("(demo menu)", NULL, false, false);
    if (FlexUI::MenuItem("New")) {}
    if (FlexUI::MenuItem("Open", "Ctrl+O")) {}
    if (FlexUI::BeginMenu("Open Recent"))
    {
        FlexUI::MenuItem("fish_hat.c");
        FlexUI::MenuItem("fish_hat.inl");
        FlexUI::MenuItem("fish_hat.h");
        if (FlexUI::BeginMenu("More.."))
        {
            FlexUI::MenuItem("Hello");
            FlexUI::MenuItem("Sailor");
            if (FlexUI::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                FlexUI::EndMenu();
            }
            FlexUI::EndMenu();
        }
        FlexUI::EndMenu();
    }
    if (FlexUI::MenuItem("Save", "Ctrl+S")) {}
    if (FlexUI::MenuItem("Save As..")) {}

    FlexUI::Separator();
    FlexUI_DEMO_MARKER("Examples/Menu/Options");
    if (FlexUI::BeginMenu("Options"))
    {
        static bool enabled = true;
        FlexUI::MenuItem("Enabled", "", &enabled);
        FlexUI::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            FlexUI::Text("Scrolling Text %d", i);
        FlexUI::EndChild();
        static float f = 0.5f;
        static int n = 0;
        FlexUI::SliderFloat("Value", &f, 0.0f, 1.0f);
        FlexUI::InputFloat("Input", &f, 0.1f);
        FlexUI::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        FlexUI::EndMenu();
    }

    FlexUI_DEMO_MARKER("Examples/Menu/Colors");
    if (FlexUI::BeginMenu("Colors"))
    {
        float sz = FlexUI::GetTextLineHeight();
        for (int i = 0; i < FlexUICol_COUNT; i++)
        {
            const char* name = FlexUI::GetStyleColorName((FlexUICol)i);
            ImVec2 p = FlexUI::GetCursorScreenPos();
            FlexUI::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), FlexUI::GetColorU32((FlexUICol)i));
            FlexUI::Dummy(ImVec2(sz, sz));
            FlexUI::SameLine();
            FlexUI::MenuItem(name);
        }
        FlexUI::EndMenu();
    }

    // Here we demonstrate appending again to the "Options" menu (which we already created above)
    // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
    // In a real code-base using it would make senses to use this feature from very different code locations.
    if (FlexUI::BeginMenu("Options")) // <-- Append!
    {
        FlexUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
        static bool b = true;
        FlexUI::Checkbox("SomeOption", &b);
        FlexUI::EndMenu();
    }

    if (FlexUI::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (FlexUI::MenuItem("Checked", NULL, true)) {}
    if (FlexUI::MenuItem("Quit", "Alt+F4")) {}
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, we are using a more C++ like approach of declaring a class to hold both data and functions.
struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    FlexUITextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    ExampleAppConsole()
    {
        FlexUI_DEMO_MARKER("Examples/Console");
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");
        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("Welcome to Dear FlexUI!");
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void    Draw(const char* title, bool* p_open)
    {
        FlexUI::SetNextWindowSize(ImVec2(520, 600), FlexUICond_FirstUseEver);
        if (!FlexUI::Begin(title, p_open))
        {
            FlexUI::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (FlexUI::BeginPopupContextItem())
        {
            if (FlexUI::MenuItem("Close Console"))
                *p_open = false;
            FlexUI::EndPopup();
        }

        FlexUI::TextWrapped(
            "This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
            "implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        FlexUI::TextWrapped("Enter 'HELP' for help.");

        // TODO: display items starting from the bottom

        if (FlexUI::SmallButton("Add Debug Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); }
        FlexUI::SameLine();
        if (FlexUI::SmallButton("Add Debug Error")) { AddLog("[error] something went wrong"); }
        FlexUI::SameLine();
        if (FlexUI::SmallButton("Clear")) { ClearLog(); }
        FlexUI::SameLine();
        bool copy_to_clipboard = FlexUI::SmallButton("Copy");
        //static float t = 0.0f; if (FlexUI::GetTime() - t > 0.02f) { t = FlexUI::GetTime(); AddLog("Spam %f", t); }

        FlexUI::Separator();

        // Options menu
        if (FlexUI::BeginPopup("Options"))
        {
            FlexUI::Checkbox("Auto-scroll", &AutoScroll);
            FlexUI::EndPopup();
        }

        // Options, Filter
        if (FlexUI::Button("Options"))
            FlexUI::OpenPopup("Options");
        FlexUI::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        FlexUI::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = FlexUI::GetStyle().ItemSpacing.y + FlexUI::GetFrameHeightWithSpacing();
        if (FlexUI::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, FlexUIWindowFlags_HorizontalScrollbar))
        {
            if (FlexUI::BeginPopupContextWindow())
            {
                if (FlexUI::Selectable("Clear")) ClearLog();
                FlexUI::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use FlexUI::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      FlexUIListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                FlexUI::LogToClipboard();
            for (int i = 0; i < Items.Size; i++)
            {
                const char* item = Items[i];
                if (!Filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                if (has_color)
                    FlexUI::PushStyleColor(FlexUICol_Text, color);
                FlexUI::TextUnformatted(item);
                if (has_color)
                    FlexUI::PopStyleColor();
            }
            if (copy_to_clipboard)
                FlexUI::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && FlexUI::GetScrollY() >= FlexUI::GetScrollMaxY()))
                FlexUI::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            FlexUI::PopStyleVar();
        }
        FlexUI::EndChild();
        FlexUI::Separator();

        // Command-line
        bool reclaim_focus = false;
        FlexUIInputTextFlags input_text_flags = FlexUIInputTextFlags_EnterReturnsTrue | FlexUIInputTextFlags_EscapeClearsAll | FlexUIInputTextFlags_CallbackCompletion | FlexUIInputTextFlags_CallbackHistory;
        if (FlexUI::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        FlexUI::SetItemDefaultFocus();
        if (reclaim_focus)
            FlexUI::SetKeyboardFocusHere(-1); // Auto focus previous widget

        FlexUI::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(FlexUIInputTextCallbackData* data)
    {
        ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(FlexUIInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case FlexUIInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            for (int i = 0; i < Commands.Size; i++)
                if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(Commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }
        case FlexUIInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = HistoryPos;
            if (data->EventKey == FlexUIKey_UpArrow)
            {
                if (HistoryPos == -1)
                    HistoryPos = History.Size - 1;
                else if (HistoryPos > 0)
                    HistoryPos--;
            }
            else if (data->EventKey == FlexUIKey_DownArrow)
            {
                if (HistoryPos != -1)
                    if (++HistoryPos >= History.Size)
                        HistoryPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != HistoryPos)
            {
                const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
        }
        return 0;
    }
};

static void ShowExampleAppConsole(bool* p_open)
{
    static ExampleAppConsole console;
    console.Draw("Example: Console", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ExampleAppLog
{
    FlexUITextBuffer     Buf;
    FlexUITextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!FlexUI::Begin(title, p_open))
        {
            FlexUI::End();
            return;
        }

        // Options menu
        if (FlexUI::BeginPopup("Options"))
        {
            FlexUI::Checkbox("Auto-scroll", &AutoScroll);
            FlexUI::EndPopup();
        }

        // Main window
        if (FlexUI::Button("Options"))
            FlexUI::OpenPopup("Options");
        FlexUI::SameLine();
        bool clear = FlexUI::Button("Clear");
        FlexUI::SameLine();
        bool copy = FlexUI::Button("Copy");
        FlexUI::SameLine();
        Filter.Draw("Filter", -100.0f);

        FlexUI::Separator();

        if (FlexUI::BeginChild("scrolling", ImVec2(0, 0), false, FlexUIWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                FlexUI::LogToClipboard();

            FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        FlexUI::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   FlexUI::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using FlexUIListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we have an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                FlexUIListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        FlexUI::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            FlexUI::PopStyleVar();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (AutoScroll && FlexUI::GetScrollY() >= FlexUI::GetScrollMaxY())
                FlexUI::SetScrollHereY(1.0f);
        }
        FlexUI::EndChild();
        FlexUI::End();
    }
};

// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open)
{
    static ExampleAppLog log;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    FlexUI::SetNextWindowSize(ImVec2(500, 400), FlexUICond_FirstUseEver);
    FlexUI::Begin("Example: Log", p_open);
    FlexUI_DEMO_MARKER("Examples/Log");
    if (FlexUI::SmallButton("[Debug] Add 5 entries"))
    {
        static int counter = 0;
        const char* categories[3] = { "info", "warn", "error" };
        const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
        for (int n = 0; n < 5; n++)
        {
            const char* category = categories[counter % IM_ARRAYSIZE(categories)];
            const char* word = words[counter % IM_ARRAYSIZE(words)];
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                FlexUI::GetFrameCount(), category, FlexUI::GetTime(), word);
            counter++;
        }
    }
    FlexUI::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    log.Draw("Example: Log", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
//-----------------------------------------------------------------------------

// Demonstrate create a window with multiple child windows.
static void ShowExampleAppLayout(bool* p_open)
{
    FlexUI::SetNextWindowSize(ImVec2(500, 440), FlexUICond_FirstUseEver);
    if (FlexUI::Begin("Example: Simple layout", p_open, FlexUIWindowFlags_MenuBar))
    {
        FlexUI_DEMO_MARKER("Examples/Simple layout");
        if (FlexUI::BeginMenuBar())
        {
            if (FlexUI::BeginMenu("File"))
            {
                if (FlexUI::MenuItem("Close", "Ctrl+W")) { *p_open = false; }
                FlexUI::EndMenu();
            }
            FlexUI::EndMenuBar();
        }

        // Left
        static int selected = 0;
        {
            FlexUI::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < 100; i++)
            {
                // FIXME: Good candidate to use FlexUISelectableFlags_SelectOnNav
                char label[128];
                sprintf(label, "MyObject %d", i);
                if (FlexUI::Selectable(label, selected == i))
                    selected = i;
            }
            FlexUI::EndChild();
        }
        FlexUI::SameLine();

        // Right
        {
            FlexUI::BeginGroup();
            FlexUI::BeginChild("item view", ImVec2(0, -FlexUI::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            FlexUI::Text("MyObject: %d", selected);
            FlexUI::Separator();
            if (FlexUI::BeginTabBar("##Tabs", FlexUITabBarFlags_None))
            {
                if (FlexUI::BeginTabItem("Description"))
                {
                    FlexUI::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    FlexUI::EndTabItem();
                }
                if (FlexUI::BeginTabItem("Details"))
                {
                    FlexUI::Text("ID: 0123456789");
                    FlexUI::EndTabItem();
                }
                FlexUI::EndTabBar();
            }
            FlexUI::EndChild();
            if (FlexUI::Button("Revert")) {}
            FlexUI::SameLine();
            if (FlexUI::Button("Save")) {}
            FlexUI::EndGroup();
        }
    }
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
//-----------------------------------------------------------------------------

static void ShowPlaceholderObject(const char* prefix, int uid)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    FlexUI::PushID(uid);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    FlexUI::TableNextRow();
    FlexUI::TableSetColumnIndex(0);
    FlexUI::AlignTextToFramePadding();
    bool node_open = FlexUI::TreeNode("Object", "%s_%u", prefix, uid);
    FlexUI::TableSetColumnIndex(1);
    FlexUI::Text("my sailor is rich");

    if (node_open)
    {
        static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
        for (int i = 0; i < 8; i++)
        {
            FlexUI::PushID(i); // Use field index as identifier.
            if (i < 2)
            {
                ShowPlaceholderObject("Child", 424242);
            }
            else
            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                FlexUI::TableNextRow();
                FlexUI::TableSetColumnIndex(0);
                FlexUI::AlignTextToFramePadding();
                FlexUITreeNodeFlags flags = FlexUITreeNodeFlags_Leaf | FlexUITreeNodeFlags_NoTreePushOnOpen | FlexUITreeNodeFlags_Bullet;
                FlexUI::TreeNodeEx("Field", flags, "Field_%d", i);

                FlexUI::TableSetColumnIndex(1);
                FlexUI::SetNextItemWidth(-FLT_MIN);
                if (i >= 5)
                    FlexUI::InputFloat("##value", &placeholder_members[i], 1.0f);
                else
                    FlexUI::DragFloat("##value", &placeholder_members[i], 0.01f);
                FlexUI::NextColumn();
            }
            FlexUI::PopID();
        }
        FlexUI::TreePop();
    }
    FlexUI::PopID();
}

// Demonstrate create a simple property editor.
static void ShowExampleAppPropertyEditor(bool* p_open)
{
    FlexUI::SetNextWindowSize(ImVec2(430, 450), FlexUICond_FirstUseEver);
    if (!FlexUI::Begin("Example: Property editor", p_open))
    {
        FlexUI::End();
        return;
    }
    FlexUI_DEMO_MARKER("Examples/Property Editor");

    HelpMarker(
        "This example shows how you may implement a property editor using two columns.\n"
        "All objects/fields data are dummies here.\n"
        "Remember that in many simple cases, you can use FlexUI::SameLine(xxx) to position\n"
        "your cursor horizontally instead of using the Columns() API.");

    FlexUI::PushStyleVar(FlexUIStyleVar_FramePadding, ImVec2(2, 2));
    if (FlexUI::BeginTable("split", 2, FlexUITableFlags_BordersOuter | FlexUITableFlags_Resizable))
    {
        // Iterate placeholder objects (all the same data)
        for (int obj_i = 0; obj_i < 4; obj_i++)
        {
            ShowPlaceholderObject("Object", obj_i);
            //FlexUI::Separator();
        }
        FlexUI::EndTable();
    }
    FlexUI::PopStyleVar();
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
//-----------------------------------------------------------------------------

// Demonstrate/test rendering huge amount of text, and the incidence of clipping.
static void ShowExampleAppLongText(bool* p_open)
{
    FlexUI::SetNextWindowSize(ImVec2(520, 600), FlexUICond_FirstUseEver);
    if (!FlexUI::Begin("Example: Long text display", p_open))
    {
        FlexUI::End();
        return;
    }
    FlexUI_DEMO_MARKER("Examples/Long text display");

    static int test_type = 0;
    static FlexUITextBuffer log;
    static int lines = 0;
    FlexUI::Text("Printing unusually long amount of text.");
    FlexUI::Combo("Test type", &test_type,
        "Single call to TextUnformatted()\0"
        "Multiple calls to Text(), clipped\0"
        "Multiple calls to Text(), not clipped (slow)\0");
    FlexUI::Text("Buffer contents: %d lines, %d bytes", lines, log.size());
    if (FlexUI::Button("Clear")) { log.clear(); lines = 0; }
    FlexUI::SameLine();
    if (FlexUI::Button("Add 1000 lines"))
    {
        for (int i = 0; i < 1000; i++)
            log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines + i);
        lines += 1000;
    }
    FlexUI::BeginChild("Log");
    switch (test_type)
    {
    case 0:
        // Single call to TextUnformatted() with a big buffer
        FlexUI::TextUnformatted(log.begin(), log.end());
        break;
    case 1:
    {
        // Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the FlexUIListClipper helper.
        FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(0, 0));
        FlexUIListClipper clipper;
        clipper.Begin(lines);
        while (clipper.Step())
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                FlexUI::Text("%i The quick brown fox jumps over the lazy dog", i);
        FlexUI::PopStyleVar();
        break;
    }
    case 2:
        // Multiple calls to Text(), not clipped (slow)
        FlexUI::PushStyleVar(FlexUIStyleVar_ItemSpacing, ImVec2(0, 0));
        for (int i = 0; i < lines; i++)
            FlexUI::Text("%i The quick brown fox jumps over the lazy dog", i);
        FlexUI::PopStyleVar();
        break;
    }
    FlexUI::EndChild();
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window which gets auto-resized according to its content.
static void ShowExampleAppAutoResize(bool* p_open)
{
    if (!FlexUI::Begin("Example: Auto-resizing window", p_open, FlexUIWindowFlags_AlwaysAutoResize))
    {
        FlexUI::End();
        return;
    }
    FlexUI_DEMO_MARKER("Examples/Auto-resizing window");

    static int lines = 10;
    FlexUI::TextUnformatted(
        "Window will resize every-frame to the size of its content.\n"
        "Note that you probably don't want to query the window size to\n"
        "output your content because that would create a feedback loop.");
    FlexUI::SliderInt("Number of lines", &lines, 1, 20);
    for (int i = 0; i < lines; i++)
        FlexUI::Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window with custom resize constraints.
// Note that size constraints currently don't work on a docked window (when in 'docking' branch)
static void ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints
    {
        // Helper functions to demonstrate programmatic constraints
        // FIXME: This doesn't take account of decoration size (e.g. title bar), library should make this easier.
        static void AspectRatio(FlexUISizeCallbackData* data) { float aspect_ratio = *(float*)data->UserData; data->DesiredSize.x = IM_MAX(data->CurrentSize.x, data->CurrentSize.y); data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio); }
        static void Square(FlexUISizeCallbackData* data) { data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->CurrentSize.x, data->CurrentSize.y); }
        static void Step(FlexUISizeCallbackData* data) { float step = *(float*)data->UserData; data->DesiredSize = ImVec2((int)(data->CurrentSize.x / step + 0.5f) * step, (int)(data->CurrentSize.y / step + 0.5f) * step); }
    };

    const char* test_desc[] =
    {
        "Between 100x100 and 500x500",
        "At least 100x100",
        "Resize vertical only",
        "Resize horizontal only",
        "Width Between 400 and 500",
        "Custom: Aspect Ratio 16:9",
        "Custom: Always Square",
        "Custom: Fixed Steps (100)",
    };

    // Options
    static bool auto_resize = false;
    static bool window_padding = true;
    static int type = 5; // Aspect Ratio
    static int display_lines = 10;

    // Submit constraint
    float aspect_ratio = 16.0f / 9.0f;
    float fixed_step = 100.0f;
    if (type == 0) FlexUI::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(500, 500));         // Between 100x100 and 500x500
    if (type == 1) FlexUI::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
    if (type == 2) FlexUI::SetNextWindowSizeConstraints(ImVec2(-1, 0), ImVec2(-1, FLT_MAX));      // Vertical only
    if (type == 3) FlexUI::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));      // Horizontal only
    if (type == 4) FlexUI::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(500, -1));          // Width Between and 400 and 500
    if (type == 5) FlexUI::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::AspectRatio, (void*)&aspect_ratio);   // Aspect ratio
    if (type == 6) FlexUI::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);                              // Always Square
    if (type == 7) FlexUI::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)&fixed_step);            // Fixed Step

    // Submit window
    if (!window_padding)
        FlexUI::PushStyleVar(FlexUIStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    const FlexUIWindowFlags window_flags = auto_resize ? FlexUIWindowFlags_AlwaysAutoResize : 0;
    const bool window_open = FlexUI::Begin("Example: Constrained Resize", p_open, window_flags);
    if (!window_padding)
        FlexUI::PopStyleVar();
    if (window_open)
    {
        FlexUI_DEMO_MARKER("Examples/Constrained Resizing window");
        if (FlexUI::GetIO().KeyShift)
        {
            // Display a dummy viewport (in your real app you would likely use ImageButton() to display a texture.
            ImVec2 avail_size = FlexUI::GetContentRegionAvail();
            ImVec2 pos = FlexUI::GetCursorScreenPos();
            FlexUI::ColorButton("viewport", ImVec4(0.5f, 0.2f, 0.5f, 1.0f), FlexUIColorEditFlags_NoTooltip | FlexUIColorEditFlags_NoDragDrop, avail_size);
            FlexUI::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y + 10));
            FlexUI::Text("%.2f x %.2f", avail_size.x, avail_size.y);
        }
        else
        {
            FlexUI::Text("(Hold SHIFT to display a dummy viewport)");
            if (FlexUI::Button("Set 200x200")) { FlexUI::SetWindowSize(ImVec2(200, 200)); } FlexUI::SameLine();
            if (FlexUI::Button("Set 500x500")) { FlexUI::SetWindowSize(ImVec2(500, 500)); } FlexUI::SameLine();
            if (FlexUI::Button("Set 800x200")) { FlexUI::SetWindowSize(ImVec2(800, 200)); }
            FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 20);
            FlexUI::Combo("Constraint", &type, test_desc, IM_ARRAYSIZE(test_desc));
            FlexUI::SetNextItemWidth(FlexUI::GetFontSize() * 20);
            FlexUI::DragInt("Lines", &display_lines, 0.2f, 1, 100);
            FlexUI::Checkbox("Auto-resize", &auto_resize);
            FlexUI::Checkbox("Window padding", &window_padding);
            for (int i = 0; i < display_lines; i++)
                FlexUI::Text("%*sHello, sailor! Making this line long enough for the example.", i * 4, "");
        }
    }
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple static window with no decoration
// + a context-menu to choose which corner of the screen to use.
static void ShowExampleAppSimpleOverlay(bool* p_open)
{
    static int location = 0;
    FlexUIIO& io = FlexUI::GetIO();
    FlexUIWindowFlags window_flags = FlexUIWindowFlags_NoDecoration | FlexUIWindowFlags_AlwaysAutoResize | FlexUIWindowFlags_NoSavedSettings | FlexUIWindowFlags_NoFocusOnAppearing | FlexUIWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const FlexUIViewport* viewport = FlexUI::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        FlexUI::SetNextWindowPos(window_pos, FlexUICond_Always, window_pos_pivot);
        window_flags |= FlexUIWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        FlexUI::SetNextWindowPos(FlexUI::GetMainViewport()->GetCenter(), FlexUICond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= FlexUIWindowFlags_NoMove;
    }
    FlexUI::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (FlexUI::Begin("Example: Simple overlay", p_open, window_flags))
    {
        FlexUI_DEMO_MARKER("Examples/Simple Overlay");
        FlexUI::Text("Simple overlay\n" "(right-click to change position)");
        FlexUI::Separator();
        if (FlexUI::IsMousePosValid())
            FlexUI::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            FlexUI::Text("Mouse Position: <invalid>");
        if (FlexUI::BeginPopupContextWindow())
        {
            if (FlexUI::MenuItem("Custom", NULL, location == -1)) location = -1;
            if (FlexUI::MenuItem("Center", NULL, location == -2)) location = -2;
            if (FlexUI::MenuItem("Top-left", NULL, location == 0)) location = 0;
            if (FlexUI::MenuItem("Top-right", NULL, location == 1)) location = 1;
            if (FlexUI::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
            if (FlexUI::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (p_open && FlexUI::MenuItem("Close")) *p_open = false;
            FlexUI::EndPopup();
        }
    }
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Fullscreen window / ShowExampleAppFullscreen()
//-----------------------------------------------------------------------------

// Demonstrate creating a window covering the entire screen/viewport
static void ShowExampleAppFullscreen(bool* p_open)
{
    static bool use_work_area = true;
    static FlexUIWindowFlags flags = FlexUIWindowFlags_NoDecoration | FlexUIWindowFlags_NoMove | FlexUIWindowFlags_NoSavedSettings;

    // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
    // Based on your use case you may want one of the other.
    const FlexUIViewport* viewport = FlexUI::GetMainViewport();
    FlexUI::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    FlexUI::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (FlexUI::Begin("Example: Fullscreen window", p_open, flags))
    {
        FlexUI::Checkbox("Use work area instead of main area", &use_work_area);
        FlexUI::SameLine();
        HelpMarker("Main Area = entire viewport,\nWork Area = entire viewport minus sections used by the main menu bars, task bars etc.\n\nEnable the main-menu bar in Examples menu to see the difference.");

        FlexUI::CheckboxFlags("FlexUIWindowFlags_NoBackground", &flags, FlexUIWindowFlags_NoBackground);
        FlexUI::CheckboxFlags("FlexUIWindowFlags_NoDecoration", &flags, FlexUIWindowFlags_NoDecoration);
        FlexUI::Indent();
        FlexUI::CheckboxFlags("FlexUIWindowFlags_NoTitleBar", &flags, FlexUIWindowFlags_NoTitleBar);
        FlexUI::CheckboxFlags("FlexUIWindowFlags_NoCollapse", &flags, FlexUIWindowFlags_NoCollapse);
        FlexUI::CheckboxFlags("FlexUIWindowFlags_NoScrollbar", &flags, FlexUIWindowFlags_NoScrollbar);
        FlexUI::Unindent();

        if (p_open && FlexUI::Button("Close this window"))
            *p_open = false;
    }
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
//-----------------------------------------------------------------------------

// Demonstrate the use of "##" and "###" in identifiers to manipulate ID generation.
// This applies to all regular items as well.
// Read FAQ section "How can I have multiple widgets with the same label?" for details.
static void ShowExampleAppWindowTitles(bool*)
{
    const FlexUIViewport* viewport = FlexUI::GetMainViewport();
    const ImVec2 base_pos = viewport->Pos;

    // By default, Windows are uniquely identified by their title.
    // You can use the "##" and "###" markers to manipulate the display/ID.

    // Using "##" to display same title but have unique identifier.
    FlexUI::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 100), FlexUICond_FirstUseEver);
    FlexUI::Begin("Same title as another window##1");
    FlexUI_DEMO_MARKER("Examples/Manipulating window titles");
    FlexUI::Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
    FlexUI::End();

    FlexUI::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 200), FlexUICond_FirstUseEver);
    FlexUI::Begin("Same title as another window##2");
    FlexUI::Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique.");
    FlexUI::End();

    // Using "###" to display a changing title but keep a static identifier "AnimatedTitle"
    char buf[128];
    sprintf(buf, "Animated title %c %d###AnimatedTitle", "|/-\\"[(int)(FlexUI::GetTime() / 0.25f) & 3], FlexUI::GetFrameCount());
    FlexUI::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 300), FlexUICond_FirstUseEver);
    FlexUI::Begin(buf);
    FlexUI::Text("This window has a changing title.");
    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
//-----------------------------------------------------------------------------

// Demonstrate using the low-level ImDrawList to draw custom shapes.
static void ShowExampleAppCustomRendering(bool* p_open)
{
    if (!FlexUI::Begin("Example: Custom rendering", p_open))
    {
        FlexUI::End();
        return;
    }
    FlexUI_DEMO_MARKER("Examples/Custom Rendering");

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of
    // overloaded operators, etc. Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your
    // types and ImVec2/ImVec4. Dear FlexUI defines overloaded operators but they are internal to FlexUI.cpp and not
    // exposed outside (to avoid messing with your types) In this example we are not using the maths operators!

    if (FlexUI::BeginTabBar("##TabBar"))
    {
        if (FlexUI::BeginTabItem("Primitives"))
        {
            FlexUI::PushItemWidth(-FlexUI::GetFontSize() * 15);
            ImDrawList* draw_list = FlexUI::GetWindowDrawList();

            // Draw gradients
            // (note that those are currently exacerbating our sRGB/Linear issues)
            // Calling FlexUI::GetColorU32() multiplies the given colors by the current Style Alpha, but you may pass the IM_COL32() directly as well..
            FlexUI::Text("Gradients");
            ImVec2 gradient_size = ImVec2(FlexUI::CalcItemWidth(), FlexUI::GetFrameHeight());
            {
                ImVec2 p0 = FlexUI::GetCursorScreenPos();
                ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32 col_a = FlexUI::GetColorU32(IM_COL32(0, 0, 0, 255));
                ImU32 col_b = FlexUI::GetColorU32(IM_COL32(255, 255, 255, 255));
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                FlexUI::InvisibleButton("##gradient1", gradient_size);
            }
            {
                ImVec2 p0 = FlexUI::GetCursorScreenPos();
                ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32 col_a = FlexUI::GetColorU32(IM_COL32(0, 255, 0, 255));
                ImU32 col_b = FlexUI::GetColorU32(IM_COL32(255, 0, 0, 255));
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                FlexUI::InvisibleButton("##gradient2", gradient_size);
            }

            // Draw a bunch of primitives
            FlexUI::Text("All primitives");
            static float sz = 36.0f;
            static float thickness = 3.0f;
            static int ngon_sides = 6;
            static bool circle_segments_override = false;
            static int circle_segments_override_v = 12;
            static bool curve_segments_override = false;
            static int curve_segments_override_v = 8;
            static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
            FlexUI::DragFloat("Size", &sz, 0.2f, 2.0f, 100.0f, "%.0f");
            FlexUI::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
            FlexUI::SliderInt("N-gon sides", &ngon_sides, 3, 12);
            FlexUI::Checkbox("##circlesegmentoverride", &circle_segments_override);
            FlexUI::SameLine(0.0f, FlexUI::GetStyle().ItemInnerSpacing.x);
            circle_segments_override |= FlexUI::SliderInt("Circle segments override", &circle_segments_override_v, 3, 40);
            FlexUI::Checkbox("##curvessegmentoverride", &curve_segments_override);
            FlexUI::SameLine(0.0f, FlexUI::GetStyle().ItemInnerSpacing.x);
            curve_segments_override |= FlexUI::SliderInt("Curves segments override", &curve_segments_override_v, 3, 40);
            FlexUI::ColorEdit4("Color", &colf.x);

            const ImVec2 p = FlexUI::GetCursorScreenPos();
            const ImU32 col = ImColor(colf);
            const float spacing = 10.0f;
            const ImDrawFlags corners_tl_br = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight;
            const float rounding = sz / 5.0f;
            const int circle_segments = circle_segments_override ? circle_segments_override_v : 0;
            const int curve_segments = curve_segments_override ? curve_segments_override_v : 0;
            float x = p.x + 4.0f;
            float y = p.y + 4.0f;
            for (int n = 0; n < 2; n++)
            {
                // First line uses a thickness of 1.0f, second line uses the configurable thickness
                float th = (n == 0) ? 1.0f : thickness;
                draw_list->AddNgon(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides, th);                 x += sz + spacing;  // N-gon
                draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments, th);          x += sz + spacing;  // Circle
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 0.0f, ImDrawFlags_None, th);          x += sz + spacing;  // Square
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, ImDrawFlags_None, th);      x += sz + spacing;  // Square with all rounded corners
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, corners_tl_br, th);         x += sz + spacing;  // Square with two rounded corners
                draw_list->AddTriangle(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col, th); x += sz + spacing;  // Triangle
                //draw_list->AddTriangle(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col, th);x+= sz*0.4f + spacing; // Thin triangle
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col, th);                                       x += sz + spacing;  // Horizontal line (note: drawing a filled rectangle will be faster!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col, th);                                       x += spacing;       // Vertical line (note: drawing a filled rectangle will be faster!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col, th);                                  x += sz + spacing;  // Diagonal line

                // Quadratic Bezier Curve (3 control points)
                ImVec2 cp3[3] = { ImVec2(x, y + sz * 0.6f), ImVec2(x + sz * 0.5f, y - sz * 0.4f), ImVec2(x + sz, y + sz) };
                draw_list->AddBezierQuadratic(cp3[0], cp3[1], cp3[2], col, th, curve_segments); x += sz + spacing;

                // Cubic Bezier Curve (4 control points)
                ImVec2 cp4[4] = { ImVec2(x, y), ImVec2(x + sz * 1.3f, y + sz * 0.3f), ImVec2(x + sz - sz * 1.3f, y + sz - sz * 0.3f), ImVec2(x + sz, y + sz) };
                draw_list->AddBezierCubic(cp4[0], cp4[1], cp4[2], cp4[3], col, th, curve_segments);

                x = p.x + 4;
                y += sz + spacing;
            }
            draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides);               x += sz + spacing;  // N-gon
            draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments);            x += sz + spacing;  // Circle
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);                                    x += sz + spacing;  // Square
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f);                             x += sz + spacing;  // Square with all rounded corners
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_tl_br);              x += sz + spacing;  // Square with two rounded corners
            draw_list->AddTriangleFilled(ImVec2(x + sz * 0.5f, y), ImVec2(x + sz, y + sz - 0.5f), ImVec2(x, y + sz - 0.5f), col);  x += sz + spacing;  // Triangle
            //draw_list->AddTriangleFilled(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col); x += sz*0.4f + spacing; // Thin triangle
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + thickness), col);                             x += sz + spacing;  // Horizontal line (faster than AddLine, but only handle integer thickness)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y + sz), col);                             x += spacing * 2.0f;// Vertical line (faster than AddLine, but only handle integer thickness)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 1, y + 1), col);                                      x += sz;            // Pixel (faster than AddLine)
            draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + sz, y + sz), IM_COL32(0, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255));

            FlexUI::Dummy(ImVec2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));
            FlexUI::PopItemWidth();
            FlexUI::EndTabItem();
        }

        if (FlexUI::BeginTabItem("Canvas"))
        {
            static ImVector<ImVec2> points;
            static ImVec2 scrolling(0.0f, 0.0f);
            static bool opt_enable_grid = true;
            static bool opt_enable_context_menu = true;
            static bool adding_line = false;

            FlexUI::Checkbox("Enable grid", &opt_enable_grid);
            FlexUI::Checkbox("Enable context menu", &opt_enable_context_menu);
            FlexUI::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

            // Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
            // Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
            // To use a child window instead we could use, e.g:
            //      FlexUI::PushStyleVar(FlexUIStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
            //      FlexUI::PushStyleColor(FlexUICol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
            //      FlexUI::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, FlexUIWindowFlags_NoMove);
            //      FlexUI::PopStyleColor();
            //      FlexUI::PopStyleVar();
            //      [...]
            //      FlexUI::EndChild();

            // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
            ImVec2 canvas_p0 = FlexUI::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
            ImVec2 canvas_sz = FlexUI::GetContentRegionAvail();   // Resize canvas to what's available
            if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
            if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
            ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

            // Draw border and background color
            FlexUIIO& io = FlexUI::GetIO();
            ImDrawList* draw_list = FlexUI::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
            draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

            // This will catch our interactions
            FlexUI::InvisibleButton("canvas", canvas_sz, FlexUIButtonFlags_MouseButtonLeft | FlexUIButtonFlags_MouseButtonRight);
            const bool is_hovered = FlexUI::IsItemHovered(); // Hovered
            const bool is_active = FlexUI::IsItemActive();   // Held
            const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
            const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            // Add first and second point
            if (is_hovered && !adding_line && FlexUI::IsMouseClicked(FlexUIMouseButton_Left))
            {
                points.push_back(mouse_pos_in_canvas);
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (adding_line)
            {
                points.back() = mouse_pos_in_canvas;
                if (!FlexUI::IsMouseDown(FlexUIMouseButton_Left))
                    adding_line = false;
            }

            // Pan (we use a zero mouse threshold when there's no context menu)
            // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
            const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
            if (is_active && FlexUI::IsMouseDragging(FlexUIMouseButton_Right, mouse_threshold_for_pan))
            {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }

            // Context menu (under default mouse threshold)
            ImVec2 drag_delta = FlexUI::GetMouseDragDelta(FlexUIMouseButton_Right);
            if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
                FlexUI::OpenPopupOnItemClick("context", FlexUIPopupFlags_MouseButtonRight);
            if (FlexUI::BeginPopup("context"))
            {
                if (adding_line)
                    points.resize(points.size() - 2);
                adding_line = false;
                if (FlexUI::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
                if (FlexUI::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
                FlexUI::EndPopup();
            }

            // Draw grid + all lines in the canvas
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid)
            {
                const float GRID_STEP = 64.0f;
                for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
            }
            for (int n = 0; n < points.Size; n += 2)
                draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
            draw_list->PopClipRect();

            FlexUI::EndTabItem();
        }

        if (FlexUI::BeginTabItem("BG/FG draw lists"))
        {
            static bool draw_bg = true;
            static bool draw_fg = true;
            FlexUI::Checkbox("Draw in Background draw list", &draw_bg);
            FlexUI::SameLine(); HelpMarker("The Background draw list will be rendered below every Dear FlexUI windows.");
            FlexUI::Checkbox("Draw in Foreground draw list", &draw_fg);
            FlexUI::SameLine(); HelpMarker("The Foreground draw list will be rendered over every Dear FlexUI windows.");
            ImVec2 window_pos = FlexUI::GetWindowPos();
            ImVec2 window_size = FlexUI::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg)
                FlexUI::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10 + 4);
            if (draw_fg)
                FlexUI::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0, 10);
            FlexUI::EndTabItem();
        }

        FlexUI::EndTabBar();
    }

    FlexUI::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
//-----------------------------------------------------------------------------

// Simplified structure to mimic a Document model
struct MyDocument
{
    const char* Name;       // Document title
    bool        Open;       // Set when open (we keep an array of all available documents to simplify demo code!)
    bool        OpenPrev;   // Copy of Open from last update.
    bool        Dirty;      // Set when the document has been modified
    bool        WantClose;  // Set when the document
    ImVec4      Color;      // An arbitrary variable associated to the document

    MyDocument(const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        Name = name;
        Open = OpenPrev = open;
        Dirty = false;
        WantClose = false;
        Color = color;
    }
    void DoOpen() { Open = true; }
    void DoQueueClose() { WantClose = true; }
    void DoForceClose() { Open = false; Dirty = false; }
    void DoSave() { Dirty = false; }

    // Display placeholder contents for the Document
    static void DisplayContents(MyDocument* doc)
    {
        FlexUI::PushID(doc);
        FlexUI::Text("Document \"%s\"", doc->Name);
        FlexUI::PushStyleColor(FlexUICol_Text, doc->Color);
        FlexUI::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
        FlexUI::PopStyleColor();
        if (FlexUI::Button("Modify", ImVec2(100, 0)))
            doc->Dirty = true;
        FlexUI::SameLine();
        if (FlexUI::Button("Save", ImVec2(100, 0)))
            doc->DoSave();
        FlexUI::ColorEdit3("color", &doc->Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
        FlexUI::PopID();
    }

    // Display context menu for the Document
    static void DisplayContextMenu(MyDocument* doc)
    {
        if (!FlexUI::BeginPopupContextItem())
            return;

        char buf[256];
        sprintf(buf, "Save %s", doc->Name);
        if (FlexUI::MenuItem(buf, "CTRL+S", false, doc->Open))
            doc->DoSave();
        if (FlexUI::MenuItem("Close", "CTRL+W", false, doc->Open))
            doc->DoQueueClose();
        FlexUI::EndPopup();
    }
};

struct ExampleAppDocuments
{
    ImVector<MyDocument> Documents;

    ExampleAppDocuments()
    {
        Documents.push_back(MyDocument("Lettuce", true, ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("Eggplant", true, ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
        Documents.push_back(MyDocument("Carrot", true, ImVec4(1.0f, 0.8f, 0.5f, 1.0f)));
        Documents.push_back(MyDocument("Tomato", false, ImVec4(1.0f, 0.3f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("A Rather Long Title", false));
        Documents.push_back(MyDocument("Some Document", false));
    }
};

// [Optional] Notify the system of Tabs/Windows closure that happened outside the regular tab interface.
// If a tab has been closed programmatically (aka closed from another source such as the Checkbox() in the demo,
// as opposed to clicking on the regular tab closing button) and stops being submitted, it will take a frame for
// the tab bar to notice its absence. During this frame there will be a gap in the tab bar, and if the tab that has
// disappeared was the selected one, the tab bar will report no selected tab during the frame. This will effectively
// give the impression of a flicker for one frame.
// We call SetTabItemClosed() to manually notify the Tab Bar or Docking system of removed tabs to avoid this glitch.
// Note that this completely optional, and only affect tab bars with the FlexUITabBarFlags_Reorderable flag.
static void NotifyOfDocumentsClosedElsewhere(ExampleAppDocuments & app)
{
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (!doc->Open && doc->OpenPrev)
            FlexUI::SetTabItemClosed(doc->Name);
        doc->OpenPrev = doc->Open;
    }
}

void ShowExampleAppDocuments(bool* p_open)
{
    static ExampleAppDocuments app;

    // Options
    static bool opt_reorderable = true;
    static FlexUITabBarFlags opt_fitting_flags = FlexUITabBarFlags_FittingPolicyDefault_;

    bool window_contents_visible = FlexUI::Begin("Example: Documents", p_open, FlexUIWindowFlags_MenuBar);
    if (!window_contents_visible)
    {
        FlexUI::End();
        return;
    }

    // Menu
    if (FlexUI::BeginMenuBar())
    {
        if (FlexUI::BeginMenu("File"))
        {
            int open_count = 0;
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                open_count += app.Documents[doc_n].Open ? 1 : 0;

            if (FlexUI::BeginMenu("Open", open_count < app.Documents.Size))
            {
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                {
                    MyDocument* doc = &app.Documents[doc_n];
                    if (!doc->Open)
                        if (FlexUI::MenuItem(doc->Name))
                            doc->DoOpen();
                }
                FlexUI::EndMenu();
            }
            if (FlexUI::MenuItem("Close All Documents", NULL, false, open_count > 0))
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                    app.Documents[doc_n].DoQueueClose();
            if (FlexUI::MenuItem("Exit", "Ctrl+F4") && p_open)
                *p_open = false;
            FlexUI::EndMenu();
        }
        FlexUI::EndMenuBar();
    }

    // [Debug] List documents with one checkbox for each
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (doc_n > 0)
            FlexUI::SameLine();
        FlexUI::PushID(doc);
        if (FlexUI::Checkbox(doc->Name, &doc->Open))
            if (!doc->Open)
                doc->DoForceClose();
        FlexUI::PopID();
    }

    FlexUI::Separator();

    // About the FlexUIWindowFlags_UnsavedDocument / FlexUITabItemFlags_UnsavedDocument flags.
    // They have multiple effects:
    // - Display a dot next to the title.
    // - Tab is selected when clicking the X close button.
    // - Closure is not assumed (will wait for user to stop submitting the tab).
    //   Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    //   We need to assume closure by default otherwise waiting for "lack of submission" on the next frame would leave an empty
    //   hole for one-frame, both in the tab-bar and in tab-contents when closing a tab/window.
    //   The rarely used SetTabItemClosed() function is a way to notify of programmatic closure to avoid the one-frame hole.

    // Submit Tab Bar and Tabs
    {
        FlexUITabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? FlexUITabBarFlags_Reorderable : 0);
        if (FlexUI::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (opt_reorderable)
                NotifyOfDocumentsClosedElsewhere(app);

            // [DEBUG] Stress tests
            //if ((FlexUI::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
            //if (FlexUI::GetIO().KeyCtrl) FlexUI::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..

            // Submit Tabs
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
            {
                MyDocument* doc = &app.Documents[doc_n];
                if (!doc->Open)
                    continue;

                FlexUITabItemFlags tab_flags = (doc->Dirty ? FlexUITabItemFlags_UnsavedDocument : 0);
                bool visible = FlexUI::BeginTabItem(doc->Name, &doc->Open, tab_flags);

                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!doc->Open && doc->Dirty)
                {
                    doc->Open = true;
                    doc->DoQueueClose();
                }

                MyDocument::DisplayContextMenu(doc);
                if (visible)
                {
                    MyDocument::DisplayContents(doc);
                    FlexUI::EndTabItem();
                }
            }

            FlexUI::EndTabBar();
        }
    }

    // Update closing queue
    static ImVector<MyDocument*> close_queue;
    if (close_queue.empty())
    {
        // Close queue is locked once we started a popup
        for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
        {
            MyDocument* doc = &app.Documents[doc_n];
            if (doc->WantClose)
            {
                doc->WantClose = false;
                close_queue.push_back(doc);
            }
        }
    }

    // Display closing confirmation UI
    if (!close_queue.empty())
    {
        int close_queue_unsaved_documents = 0;
        for (int n = 0; n < close_queue.Size; n++)
            if (close_queue[n]->Dirty)
                close_queue_unsaved_documents++;

        if (close_queue_unsaved_documents == 0)
        {
            // Close documents when all are unsaved
            for (int n = 0; n < close_queue.Size; n++)
                close_queue[n]->DoForceClose();
            close_queue.clear();
        }
        else
        {
            if (!FlexUI::IsPopupOpen("Save?"))
                FlexUI::OpenPopup("Save?");
            if (FlexUI::BeginPopupModal("Save?", NULL, FlexUIWindowFlags_AlwaysAutoResize))
            {
                FlexUI::Text("Save change to the following items?");
                float item_height = FlexUI::GetTextLineHeightWithSpacing();
                if (FlexUI::BeginChildFrame(FlexUI::GetID("frame"), ImVec2(-FLT_MIN, 6.25f * item_height)))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        if (close_queue[n]->Dirty)
                            FlexUI::Text("%s", close_queue[n]->Name);
                    FlexUI::EndChildFrame();
                }

                ImVec2 button_size(FlexUI::GetFontSize() * 7.0f, 0.0f);
                if (FlexUI::Button("Yes", button_size))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                    {
                        if (close_queue[n]->Dirty)
                            close_queue[n]->DoSave();
                        close_queue[n]->DoForceClose();
                    }
                    close_queue.clear();
                    FlexUI::CloseCurrentPopup();
                }
                FlexUI::SameLine();
                if (FlexUI::Button("No", button_size))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        close_queue[n]->DoForceClose();
                    close_queue.clear();
                    FlexUI::CloseCurrentPopup();
                }
                FlexUI::SameLine();
                if (FlexUI::Button("Cancel", button_size))
                {
                    close_queue.clear();
                    FlexUI::CloseCurrentPopup();
                }
                FlexUI::EndPopup();
            }
        }
    }

    FlexUI::End();
}

// End of Demo code
#else

void FlexUI::ShowAboutWindow(bool*) {}
void FlexUI::ShowDemoWindow(bool*) {}
void FlexUI::ShowUserGuide() {}
void FlexUI::ShowStyleEditor(FlexUIStyle*) {}

#endif

#endif // #ifndef FlexUI_DISABLE
