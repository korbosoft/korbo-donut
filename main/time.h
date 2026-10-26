#ifndef TIME_H
#define TIME_H

extern u64 ticks_since_start(void);
extern void time_init(void);
extern void donut_tick(void);
extern f64 delta_secs(void);

#endif
