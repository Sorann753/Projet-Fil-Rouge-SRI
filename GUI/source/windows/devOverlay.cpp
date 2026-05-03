// ─────────────────────────────────────────────────────────────────
//  DevOverlay.cpp
// ─────────────────────────────────────────────────────────────────
#include "windows/devOverlay.hpp"
#include "utils/utils.hpp"

#include <FL/fl_draw.H>
#include <cstdio>

AppData gApp;

// Palette terminal
static const Fl_Color COL_BG     = fl_rgb_color(10,  10,  30);
static const Fl_Color COL_BORDER = fl_rgb_color(0,  220, 100);
static const Fl_Color COL_SEP    = fl_rgb_color(0,  120,  60);
static const Fl_Color COL_TEXT   = fl_rgb_color(180, 255, 180);

static constexpr int PANEL_W = 260;
static constexpr int PANEL_H = 210;
static constexpr int MARGIN  = 10;

// ─────────────────────────────────────────────
DevOverlay::DevOverlay(int x, int y, int w, int h)
    : Fl_Widget(x, y, w, h, nullptr)
{
    hide(); // masqué par défaut
}

// ─────────────────────────────────────────────
void DevOverlay::toggle() {
    gApp.devOverlayVisible = !gApp.devOverlayVisible;
    gApp.devOverlayVisible ? show() : hide();
}

// ─────────────────────────────────────────────
void DevOverlay::draw() {
    if (!visible()) return;

    const int px = x() + MARGIN;
    const int py = y() + h() - PANEL_H - MARGIN;

    drawPanel(px, py, PANEL_W, PANEL_H);
    drawMetrics(px + 10, py + 38, 18);
}

// ─────────────────────────────────────────────
void DevOverlay::drawPanel(int px, int py, int pw, int ph) {
    // Fond
    fl_color(COL_BG);
    fl_rectf(px, py, pw, ph);

    // Bordure
    fl_color(COL_BORDER);
    fl_rect(px, py, pw, ph);

    // Titre
    fl_font(FL_COURIER_BOLD, 12);
    fl_color(COL_BORDER);
    fl_draw("[ DEV OVERLAY ]", px + 8, py + 16);

    // Séparateur
    fl_color(COL_SEP);
    fl_line(px + 4, py + 22, px + pw - 4, py + 22);
}

// ─────────────────────────────────────────────
void DevOverlay::drawMetrics(int tx, int ty, int lineH) {
    fl_font(FL_COURIER, 11);
    fl_color(COL_TEXT);

    char buf[128];

    auto line = [&](const char* fmt, auto... args) {
        snprintf(buf, sizeof(buf), fmt, args...);
        fl_draw(buf, tx, ty);
        ty += lineH;
    };

    line("State      : %s",    gApp.currentState);
    line("Entities   : %d",    gApp.entityCount);
}

// ─────────────────────────────────────────────
int DevOverlay::handle(int) {
    return 0; // laisse passer tous les événements
}