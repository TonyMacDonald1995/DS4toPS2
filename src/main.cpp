#include "pico/stdio.h"
#include "pico/cyw43_arch.h"

#include "controller_simulator.h"

int main(void) {
	stdio_init_all();

	set_sys_clock_khz(225000, true);

	if (cyw43_arch_init()) {
		printf("Wi-Fi init failed\n");
		return 0;
	}

	psx_main();
}