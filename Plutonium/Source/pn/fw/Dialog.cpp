#include <pn/fw/Dialog.hpp>

namespace pn::fw
{
    DialogOption::DialogOption(std::string Name, DialogResult Type)
    {
        this->name = Name;
        this->type = Type;
    }

    std::string DialogOption::GetName()
    {
        return this->name;
    }

    void DialogOption::SetName(std::string Name)
    {
        this->name = Name;
    }

    DialogResult DialogOption::GetType()
    {
        return this->type;
    }

    void DialogOption::SetType(DialogResult Type)
    {
        this->type = Type;
    }

    Dialog::Dialog(std::string Title, std::string Content)
    {
        this->title = Title;
        this->cnt = Content;
        this->osel = 0;
        this->prevosel = 0;
        this->selfact = 255;
        this->pselfact = 0;
    }

    void Dialog::AddOption(DialogOption *Option)
    {
        this->opts.push_back(Option);
    }

    void Dialog::Show(render::Renderer *Drawer)
    {
        if(this->opts.empty()) return;
        u32 dx = 0;
        u32 dy = 0;
        u32 elemw = (1280 / this->opts.size());
        u32 elemh = 60;
        u32 nr = 180;
        u32 ng = 180;
        u32 nb = 200;
        draw::Color nclr(nr, ng, nb, 255);
        while(true)
        {
            hidScanInput();
            u64 k = hidKeysDown(CONTROLLER_P1_AUTO);
            if(k & KEY_LEFT)
            {
                if(this->osel > 0)
                {
                    this->prevosel = this->osel;
                    this->osel--;
                    for(u32 i = 0; i < this->opts.size(); i++)
                    {
                        if(i == this->osel) this->selfact = 0;
                        else if(i == this->prevosel) this->pselfact = 255;
                    }
                }
            }
            else if(k & KEY_RIGHT)
            {
                if(this->osel < (this->opts.size() - 1))
                {
                    this->prevosel = this->osel;
                    this->osel++;
                    for(u32 i = 0; i < this->opts.size(); i++)
                    {
                        if(i == this->osel) this->selfact = 0;
                        else if(i == this->prevosel) this->pselfact = 255;
                    }
                }
            }
            else if(k & KEY_A) break;
            Drawer->DrawRectangleFill({ 225, 225, 225, 255 }, dx, dy, 1280, 720);
            Drawer->DrawText(this->title, draw::Font::NintendoStandard, 35, (dx + 45), (dy + 45), { 10, 10, 10, 255 });
            Drawer->DrawText(this->cnt, draw::Font::NintendoStandard, 25, (dx + 45), (dy + 100), { 10, 10, 10, 255 });
            for(u32 i = 0; i < this->opts.size(); i++)
            {
                DialogOption *opt = this->opts[i];
                u32 tw = Drawer->GetTextWidth(draw::Font::NintendoStandard, opt->GetName(), 20);
                u32 th = Drawer->GetTextHeight(draw::Font::NintendoStandard, opt->GetName(), 20);
                u32 tx = dx + ((elemw - tw) / 2) + (elemw * i);
                u32 ty = dy + (720 - elemh) + ((elemh - th) / 2);
                u32 rx = dx + (elemw * i);
                u32 ry = dy + (720 - elemh);
                if(this->osel == i)
                {
                    if(this->selfact < 255)
                    {
                        Drawer->DrawRectangleFill(draw::Color(nr, ng, nb, this->selfact), rx, ry, elemw, elemh);
                        this->selfact += 48;
                    }
                    else Drawer->DrawRectangleFill(nclr, rx, ry, elemw, elemh);
                }
                else if(this->prevosel == i)
                {
                    if(this->pselfact > 0)
                    {
                        Drawer->DrawRectangleFill(draw::Color(nr, ng, nb, this->pselfact), rx, ry, elemw, elemh);
                        this->pselfact -= 48;
                    }
                }
                Drawer->DrawText(opt->GetName(), draw::Font::NintendoStandard, 20, tx, ty, { 10, 10, 10, 255 });
            }
            Drawer->Render();
        }
    }

    bool Dialog::GotOk()
    {
        return (this->opts[this->osel]->GetType() == DialogResult::Ok);
    }

    bool Dialog::GotCancel()
    {
        return !this->GotOk();
    }

    DialogOption *Dialog::GetSelectedOption()
    {
        return this->opts[this->osel];
    }
}