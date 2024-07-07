#include "million.h"

#pragma warning (disable : 4244) // O_o

struct tab_anim
{
    float active_text_anim;
    float active_rect_alpha;
};

bool figma_frame::tab(const char* label, bool selected)
{
    FlexUIWindow* window = FlexUI::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    FlexUIContext& g = *GFlexUI;
    const FlexUIStyle& style = g.Style;
    const FlexUIID id = window->GetID(label);
    const ImVec2 label_size = FlexUI::CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x + 10, pos.y + 23));
    FlexUI::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y), style.FramePadding.y);
    if (!FlexUI::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = FlexUI::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <FlexUIID, tab_anim> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, {0.5f, 0.0f} });
        it_anim = anim.find(id);
    }

    it_anim->second.active_text_anim = ImLerp(it_anim->second.active_text_anim, selected ? 1.0f : hovered ? 0.8f : 0.5f, 0.06f * (1.0f - FlexUI::GetIO().DeltaTime));
    it_anim->second.active_rect_alpha = ImLerp(it_anim->second.active_rect_alpha, selected ? 1.0f : 0.0f, 0.08f * (1.0f - FlexUI::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(ImVec2(rect.Min.x, rect.Max.y + 10), ImVec2(rect.Max.x, rect.Max.y + 12), ImColor(157 / 255.0f, 176 / 255.0f, 242 / 255.0f, it_anim->second.active_rect_alpha), 2.0f, ImDrawFlags_RoundCornersTop);
    window->DrawList->AddText(ImVec2((rect.Min.x + rect.Max.x) / 2 - (label_size.x) / 2 - 1, (rect.Min.y + rect.Max.y + 10) / 2.f - (label_size.y / 2.f)), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.active_text_anim), label);

    return pressed;
}
