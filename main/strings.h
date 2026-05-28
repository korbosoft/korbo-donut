#ifndef STRINGS_H
#define STRINGS_H

#ifndef GC
#define STRING_CANCEL "Press +/Y to cancel."
#define STRING_CONTROLS "Press 2/B for controls."

#define STRING_CONTROLS_BOX \
"╔═══════════════════════════╦══════════════════════╦═════════════════════════╗" \
"║ +/Y      - Change flavor  ║ -/X - Switch between ║                \e[4mControls\e[0m\e[104;37m ║" \
"║ A        - Toggle music   ║ rendering methods    ║                         ║" \
"║ 1/Z      - Send GBA Donut ║ RESET - I think you  ║                         ║" \
"║ \xfd/START  - Exit           ║ can figure it out :) ║   Press 2/B to go back. ║" \
"╚═══════════════════════════╩══════════════════════╩═════════════════════════╝"
#else
#define STRING_CANCEL "  Press Y to cancel."
#define STRING_CONTROLS "  Press B for controls."
#define STRING_CONTROLS_BOX \
"╔═══════════════════════════╦══════════════════════╦═════════════════════════╗" \
"║ Y        - Change Flavor  ║ X   - Switch between ║                \e[4mControls\e[0m\e[104;37m ║" \
"║ A        - Toggle music   ║ rendering methods    ║                         ║" \
"║ Z        - Send GBA Donut ║ RESET - I think you  ║                         ║" \
"║ START    - Exit           ║ can figure it out :) ║     Press B to go back. ║" \
"╚═══════════════════════════╩══════════════════════╩═════════════════════════╝"
#endif

#endif
