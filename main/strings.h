#ifndef STRINGS_H
#define STRINGS_H

#define SPLASH_COUNT 8

const char *splashMessages[SPLASH_COUNT] = {
	[0] = "Also try DS Donut!",
	[1] = "Also try 3DS Donut!",
	[2] = "Also try Lily Skate!",
	[3] = "Better than Wii Donut!",
	[4] = "oh man please to help i am not good with co",
	[5] = "(\"Doughnut\" if you're british)",
	[6] = "Korbo loves you <3",
	[7] = "Did you know you can change the music?",
};

#define STRING_MAIN_BOX \
"╔════════════════════════════════════════════════════════════════════════════╗" \
"║ \x1b[4mKorbo's Donut Shop :3 %s   %s\x1b[0m\x1b[104;37m "                 "║" \
"║ Originally based off \"Wii Donut\" by emilydaemon                            ║" \
"║ Written, and otherwise created by Korbo Q. Lamp                            ║" \
"║ Default Music is \"Addiction\" by Jogeir Liljedahl   " STRING_CONTROLS   " ║" \
"╚════════════════════════════════════════════════════════════════════════════╝"

#ifndef GC
#define STRING_CANCEL "Press +/Y to cancel."
#define STRING_CONTROLS "Press 2/B for controls."

#define STRING_CONTROLS_BOX \
"╔═══════════════════════════╦══════════════════════╦═════════════════════════╗" \
"║ +/Y      - Change flavor  ║ -/X - Switch between ║                \e[4mControls\e[0m\e[104;37m ║" \
"║ A        - Toggle music   ║ rendering methods    ║                         ║" \
"║                           ║ RESET - I think you  ║                         ║" \
"║ \xfd/START  - Exit           ║ can figure it out :) ║   Press 2/B to go back. ║" \
"╚═══════════════════════════╩══════════════════════╩═════════════════════════╝"
#else
#define STRING_CANCEL "  Press Y to cancel."
#define STRING_CONTROLS "  Press B for controls."
#define STRING_CONTROLS_BOX \
"╔═══════════════════════════╦══════════════════════╦═════════════════════════╗" \
"║ Y        - Change Flavor  ║ X   - Switch between ║                \e[4mControls\e[0m\e[104;37m ║" \
"║ A        - Toggle music   ║ rendering methods    ║                         ║" \
"║                           ║ RESET - I think you  ║                         ║" \
"║ START    - Exit           ║ can figure it out :) ║     Press B to go back. ║" \
"╚═══════════════════════════╩══════════════════════╩═════════════════════════╝"
#endif

#endif
