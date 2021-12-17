#pragma once

#include <string>
#include "singleton.hpp"
#include "ui.hpp"

struct IDirect3DDevice9;

class menu
    : public singleton<menu>
{
public:
    void initialize(IDirect3DDevice9* device);
    void shutdown();

    void on_device_lost();
    void on_device_reset();

    void render();

    void show();
    void hide();
    void toggle();

    bool visible() const { return m_bVisible; }
private:
    void create_style();
    void render_sidebar();
    void render_body();

    IDirect3DDevice9* m_pDevice;
    ImGuiStyle        m_Style;
    bool              m_bVisible;
};