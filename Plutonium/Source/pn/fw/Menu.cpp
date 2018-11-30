#include <pn/fw/Menu.hpp>

namespace pn::fw
{
    MenuItem::MenuItem(std::string Name)
    {
        this->name = Name;
    }

    std::string MenuItem::GetName()
    {
        return this->name;
    }

    void MenuItem::SetName(std::string Name)
    {
        this->name = Name;
    }

    void MenuItem::SetOnClick(std::function<void()> Callback)
    {
        this->cb = Callback;
    }

    std::function<void()> MenuItem::GetCallback()
    {
        return this->cb;
    }

    Menu::Menu(u32 X, u32 Y, u32 Width, u32 Height, draw::Color OptionColor, u32 ItemSize)
    {
        this->x = X;
        this->y = Y;
        this->w = Width;
        this->h = Height;
        this->clr = OptionColor;
        this->isize = ItemSize;
        this->previsel = 0;
        this->isel = 0;
        this->selfact = 255;
        this->pselfact = 0;
    }

    u32 Menu::GetX()
    {
        return this->x;
    }

    void Menu::SetX(u32 X)
    {
        this->x = X;
    }

    u32 Menu::GetY()
    {
        return this->y;
    }

    void Menu::SetY(u32 Y)
    {
        this->y = Y;
    }

    u32 Menu::GetWidth()
    {
        return this->w;
    }

    void Menu::SetWidth(u32 Width)
    {
        this->w = Width;
    }

    u32 Menu::GetHeight()
    {
        return this->h;
    }

    void Menu::SetHeight(u32 Height)
    {
        this->h = Height;
    }

    draw::Color Menu::GetColor()
    {
        return this->clr;
    }

    void Menu::SetColor(draw::Color OptionColor)
    {
        this->clr = OptionColor;
    }

    void Menu::AddItem(MenuItem *Item)
    {
        this->itms.push_back(Item);
    }

    void Menu::OnRender(render::Renderer *Drawer)
    {
        if(!this->itms.empty())
        {
            u32 cx = this->x;
            u32 cy = this->y;
            u32 cw = this->w;
            u32 ch = this->isize;
            for(u32 i = 0; i < this->itms.size(); i++)
            {
                s32 clrr = this->clr.R;
                s32 clrg = this->clr.G;
                s32 clrb = this->clr.B;
                s32 nr = clrr - 70;
                if(nr < 0) nr = 0;
                s32 ng = clrg - 70;
                if(ng < 0) ng = 0;
                s32 nb = clrb - 70;
                if(nb < 0) nb = 0;
                draw::Color nclr(nr, ng, nb, this->clr.A);
                if(this->isel == i)
                {
                    Drawer->DrawRectangleFill(this->clr, cx, cy, cw, ch);
                    if(this->selfact < 255)
                    {
                        Drawer->DrawRectangleFill(draw::Color(nr, ng, nb, this->selfact), cx, cy, cw, ch);
                        this->selfact += 48;
                    }
                    else Drawer->DrawRectangleFill(nclr, cx, cy, cw, ch);
                }
                else if(this->previsel == i)
                {
                    Drawer->DrawRectangleFill(this->clr, cx, cy, cw, ch);
                    if(this->pselfact > 0)
                    {
                        Drawer->DrawRectangleFill(draw::Color(nr, ng, nb, this->pselfact), cx, cy, cw, ch);
                        this->pselfact -= 48;
                    }
                    else Drawer->DrawRectangleFill(this->clr, cx, cy, cw, ch);
                }
                else Drawer->DrawRectangleFill(this->clr, cx, cy, cw, ch);
                std::string cnt = this->itms[i]->GetName();
                u32 xw = Drawer->GetTextWidth(draw::Font::Custom, cnt, 25);
                u32 xh = Drawer->GetTextHeight(draw::Font::Custom, cnt, 25);
                u32 tx = ((cw - xw) / 2) + cx;
                u32 ty = ((ch - xh) / 2) + cy;
                Drawer->DrawText(cnt, draw::Font::Custom, 25, tx, ty, { 0, 0, 0, 255 });
                cy += ch;
            }
        }
    }

    void Menu::OnInput(u64 Input)
    {
        if(Input & KEY_DDOWN)
        {
            if(this->isel < (this->itms.size() - 1))
            {
                this->previsel = this->isel;
                this->isel++;
                if(!this->itms.empty()) for(u32 i = 0; i < this->itms.size(); i++)
                {
                    if(i == this->isel) this->selfact = 0;
                    else if(i == this->previsel) this->pselfact = 255;
                }
            }
        }
        else if(Input & KEY_DUP)
        {
            if(this->isel > 0)
            {
                this->previsel = this->isel;
                this->isel--;
                if(!this->itms.empty()) for(u32 i = 0; i < this->itms.size(); i++)
                {
                    if(i == this->isel) this->selfact = 0;
                    else if(i == this->previsel) this->pselfact = 255;
                }
            }
        }
        else if(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) if(!this->itms.empty()) for(u32 i = 0; i < this->itms.size(); i++) if(i == this->isel) (this->itms[i]->GetCallback())();
    }
}