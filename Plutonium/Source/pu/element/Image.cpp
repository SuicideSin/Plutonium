#include <pu/element/Image.hpp>

namespace pu::element
{
    Image::Image(u32 X, u32 Y, std::string Image) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->w = 0;
        this->h = 0;
        std::ifstream ifs(Image);
        if(ifs.good())
        {
            this->img = Image;
            this->ntex = render::LoadImage(Image);
        }
        ifs.close();
    }

    Image::~Image()
    {
        render::DeleteTexture(this->ntex);
    }

    u32 Image::GetX()
    {
        return this->x;
    }

    void Image::SetX(u32 X)
    {
        this->x = X;
    }

    u32 Image::GetY()
    {
        return this->y;
    }

    void Image::SetY(u32 Y)
    {
        this->y = Y;
    }

    u32 Image::GetWidth()
    {
        return this->w;
    }

    void Image::SetWidth(u32 Width)
    {
        this->w = Width;
    }

    u32 Image::GetHeight()
    {
        return this->h;
    }

    void Image::SetHeight(u32 Height)
    {
        this->h = Height;
    }

    std::string Image::GetImage()
    {
        return this->img;
    }

    void Image::SetImage(std::string Image)
    {
        std::ifstream ifs(Image);
        if(ifs.good())
        {
            this->img = Image;
            render::DeleteTexture(this->ntex);
            this->ntex = render::LoadImage(Image);
        }
        ifs.close();
    }

    bool Image::IsImageValid()
    {
        std::ifstream ifs(this->img);
        bool ok = ifs.good();
        ifs.close();
        return ok;
    }

    void Image::OnRender(render::Renderer *Drawer)
    {
        u32 iw = this->w;
        u32 ih = this->h;
        if((iw == 0) || (ih == 0)) Drawer->RenderTexture(this->ntex, this->x, this->y);
        else Drawer->RenderTextureScaled(this->ntex, this->x, this->y, iw, ih);
    }

    void Image::OnInput(u64 Down, u64 Up, u64 Held)
    {
    }
}