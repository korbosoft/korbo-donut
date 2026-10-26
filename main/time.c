#include <gccore.h>
#include <ogc/lwp_watchdog.h>

static u64 first_tick = 0;
static u64 last_tick = 0;
static u64 diff = 0;

u64 ticks_since_start(void) {
	return diff_ticks(first_tick, SYS_Time());
}

void time_init(void) {
	first_tick = SYS_Time();
	last_tick = SYS_Time();
}

void donut_tick(void) {
	u64 current_ticks = SYS_Time();
	diff = diff_ticks(last_tick, current_ticks);
	last_tick = current_ticks;
}

f64 delta_secs(void) {
	return (f64)diff / (f64)PPC_TIMER_CLOCK;
}
