SET(SOURCES ui_MainMenu.c
    ui_BookSelection.c
    ui_ReadingScreen.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_output_png.c
    ui_img_car_png.c
    ui_img_battery_android_frame_full_20dp_000000_fill0_wght400_grad0_opsz20_png.c
    ui_img_battery_android_frame_full_20dp_ffffff_fill0_wght400_grad0_opsz20_png.c
    ui_img_heart_smile_20dp_ea3323_fill0_wght400_grad0_opsz20_png.c
    ui_img_book_17dp_ffffff_fill0_wght400_grad0_opsz20_png.c
    ui_img_nest_clock_farsight_analog_17dp_ffffff_fill0_wght400_grad0_opsz20_png.c
    fonts/ui_font_Font1.c
    fonts/ui_font_clockFont.c
    fonts/ui_font_clockFont1.c
    fonts/ui_font_bookTime.c)

add_library(ui ${SOURCES})
