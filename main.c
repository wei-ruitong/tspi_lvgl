#include "lvgl/lvgl.h"
#include "lv_drivers/wayland/wayland.h"
#include "lvgl/demos/lv_demos.h"
#include "ui.h"
#include <limits.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define LV_HOR_RES_MAX (480)
#define LV_VER_RES_MAX (854)
#define DISP_BUF_SIZE (LV_HOR_RES_MAX * LV_VER_RES_MAX)
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DISP_BUF_SIZE];

int main(int argc, char ** argv)
{
    lv_init();
    lv_wayland_init();
    lv_disp_t *disp = lv_wayland_create_window(LV_HOR_RES_MAX, LV_VER_RES_MAX, "Window Title", NULL);
    lv_wayland_window_set_fullscreen(disp,true);
#if 1
    //运行demo
    lv_demo_benchmark();
#else
    // 运行自定义UI
    ui_init();
    lv_obj_t *lottie = lv_rlottie_create_from_file(ui_Panel1, 300, 300, "/root/astronut.json");
    lv_obj_center(lottie);
    lv_rlottie_set_play_mode(lottie, LV_RLOTTIE_CTRL_PLAY | LV_RLOTTIE_CTRL_FORWARD);
    lv_obj_t *lottie1 = lv_rlottie_create_from_file(ui_Screen1, 480, 480, "/root/snowfall.json");
#endif
    struct pollfd pfd;
    uint32_t time_till_next;
    int sleep;
    pfd.fd = lv_wayland_get_fd();
    pfd.events = POLLIN;
    while (1)
    {
        /* Handle any Wayland/LVGL timers/events */
        time_till_next = lv_wayland_timer_handler();
        /* Run until the last window closes */
        if (!lv_wayland_window_is_open(NULL))
        {
            break;
        }
        /* Wait for something interesting to happen */
        if (time_till_next == LV_NO_TIMER_READY)
        {
            sleep = -1;
        }
        else if (time_till_next > INT_MAX)
        {
            sleep = INT_MAX;
        }
        else
        {
            sleep = time_till_next;
        }
        lv_tick_inc(50);
        lv_timer_handler() ;

        while ((poll(&pfd, 1, sleep) < 0) && (errno == EINTR));
    }
    return 0;
}