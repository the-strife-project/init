#include <tasks>
#include <cstdio>
#include <loader>
#include <registry>
#include <fs>

inline void print(const char* str) {
	std::printf("\033\x0D%s\033\x0A", str);
	std::flushTerm();
}
inline void ok() { std::printf("\033\x09[OK]\033\x0A\n"); }
inline void failed() { std::printf("\033\x0C""Failed!\033\x0A\n"); }

extern "C" void _start() {
	print("This is init\n");

	auto pid = std::run("/cd/bin/splash");
	if(std::getLastLoaderError()) {
		failed();
		std::exit(1);
	}
	std::wait(pid);

	print("Running registry... ");
	std::run("/cd/bin/registry");
	if(std::getLastLoaderError()) {
		failed();
		std::exit(2);
	}
	while(!std::resolve("registry"));
	ok();

	print("Running keyboard... ");
	std::run("/cd/bin/keyboard");
	if(std::getLastLoaderError()) {
		failed();
		std::exit(3);
	}
	while(!std::resolve("keyboard"));
	ok();

	bool first = true;
	while(true) {
		if(first) {
			print("Starting system shell");
			// The following line is apart for clearing the row while scrolling
			// with the 0x0A color byte, so the cursor is actually green
			std::printf("\n");
			first = false;
		} else {
			print("[init] Restarting system shell");
			std::printf("\n");
		}

		pid = std::run("/cd/bin/shell");
		if(std::getLastLoaderError()) {
			failed();
			std::exit(4);
		}
		std::wait(pid);
	}
}
