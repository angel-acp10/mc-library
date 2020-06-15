#include "test-draw.h"
#include "mc-draw.h"

void test_draw()
{
    //filled rectangle
    mcGeo_t fRec;
    fRec.x = 1200;//10-60
    fRec.y = 600;//10-30
    fRec.w = 50;
    fRec.h = 20;
    fRec.color = white;
    mcDraw_fRectangle(&fRec);

    //rectangle
    mcGeo_t rec;
    rec.x = 110;
    rec.y = 20;
    rec.w = 10;
    rec.h = 40;
    rec.color = white;
    mcDraw_rectangle(&rec);

    //hLine
    mcGeo_t hLine;
    hLine.x = 10;
    hLine.y = 40;
    hLine.w = 60;
    hLine.color = white;
    mcDraw_xLine(&hLine);

    //vLine
    mcGeo_t vLine;
    vLine.x = 100;
    vLine.y = 5;
    vLine.h = 25;
    vLine.color = white;
    mcDraw_yLine(&vLine);

    //draw a single pixel
    mcDraw_pixel(100, 60, white);

}