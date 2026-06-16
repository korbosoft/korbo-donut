#ifndef STRINGS_H
#define STRINGS_H

#define STRING_DONUT_ERROR \
"\x1b[3;22H"  "                                    " \
"\x1b[4;22H"  "        FLAGRANT DONUT ERROR        " \
"\x1b[5;22H"  "                                    " \
"\x1b[6;22H"  "          Donut Shop Over.          " \
"\x1b[7;22H"  "        Buffer = Very Blank.        " \
"\x1b[8;22H"  "                                    " \
"\x1b[9;22H"  " If you're seeing this, the donut   " \
"\x1b[10;22H" " failed to render. Please disable   " \
"\x1b[11;22H" " \"Store EFB Copies to Texture Only\" " \
"\x1b[12;22H" " (Settings -> Graphics -> Hacks)    " \
"\x1b[13;22H" " if you're running this on Dolphin. " \
"\x1b[14;22H" " If this is not the case, or if     " \
"\x1b[15;22H" " disabling the setting didn't work, " \
"\x1b[16;22H" " something has gone horribly wrong. " \
"\x1b[17;22H" " Please contact me via e-mail or on " \
"\x1b[18;22H" " the GitHub repository, which is at " \
"\x1b[19;22H" " korbosoft/korbo-donut.             " \
"\x1b[20;22H" "                                    "

#define STRING_MAIN_BOX \
"╔════════════════════════════════════════════════════════════════════════════╗" \
"║ \x1b[4mKorbo's Donut Shop v"VERSION" :3\x1b[0;104;97m   \x1b[93m%s\x1b[97m ║" \
"║ Originally based off \"Wii Donut\" by emilydaemon                            ║" \
"║ Written, and otherwise created by Korbo Q. Lamp                            ║" \
"║ Default Music is \"Addiction\" by Jogeir Liljedahl   " STRING_CONTROLS   " ║" \
"╚════════════════════════════════════════════════════════════════════════════╝"

#ifdef HW_RVL
#define STRING_CANCEL "Press +/Y to cancel."
#define STRING_CONTROLS "Press 2/B for controls."

#define STRING_CONTROLS_BOX \
"╔═══════════════════════════╦══════════════════════╦═════════════════════════╗" \
"║ -/X      - Toggle manual  ║ 1/Z   - Use classic  ║                \x1b[4mControls\x1b[0;104;97m ║" \
"║ +/Y      - Change flavor  ║ rendering method     ║ Manual mode: Sticks     ║" \
"║ A        - Toggle music   ║ RESET - I think you  ║ rotate donut.           ║" \
"║ \xfd/START  - Exit           ║ can figure it out :) ║   Press 2/B to go back. ║" \
"╚═══════════════════════════╩══════════════════════╩═════════════════════════╝"
#else
#define STRING_CANCEL "  Press Y to cancel."
#define STRING_CONTROLS "  Press B for controls."
#define STRING_CONTROLS_BOX \
"╔═══════════════════════════╦══════════════════════╦═════════════════════════╗" \
"║ X        - Toggle manual  ║ Z     - Use classic  ║                \x1b[4mControls\x1b[0;104;97m ║" \
"║ Y        - Change flavor  ║ rendering method     ║ Manual mode: Sticks     ║" \
"║ A        - Toggle music   ║ RESET - I think you  ║ rotate donut.           ║" \
"║ START    - Exit           ║ can figure it out :) ║     Press B to go back. ║" \
"╚═══════════════════════════╩══════════════════════╩═════════════════════════╝"
#endif

#endif
