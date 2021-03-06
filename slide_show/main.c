#include <monotron.h>
#include <stdbool.h>

#define PAGE_SECONDS 10

static const char PAGE_1[] = "" \
"\eZ\eY\e^Welcome to the Monotron!\n" \
"\eR\evWelcome to the Monotron!\n" \
"\e-\eW\n" \
"I am a Cortex-M4 powered home computer.\n" \
"My ROM is written in the Rust Programming\n" \
"Language (www.rust-lang.org).\n" \
"\n" \
"I exist to show you can you take a modern\n" \
"language and use it to write hard-real time\n" \
"embedded systems.\n" \
"\n" \
"I drive an 800x600 VGA display through 3 SPI.\n" \
"peripherals at 20 MHz.\n" \
"\n" \
"  \eYo\eC 400x600 effective resolution\n" \
"  \eYo\eC 8 colours \eRR\eGG\eBB\eCC\eMM\eYY\eW\n" \
"  \eYo\eC 48 cols by 36 rows in text mode\n" \
"  \eYo\eC 8px by 16px characters\n" \
"  \eYo\eC CodePage 850 character set \x03\x04\x05\x06\n" \
"  \eYo\eC Alternative 'Teletext' character set\n" \
"  \eYo\eC Support for custom fonts in RAM\n" \
"\eW\n";

static const char PAGE_2[] = "" \
"\eZ\eY\e^Features:\n" \
"\eG\evFeatures:\n" \
"\e-\eW\n" \
"  \eYo\eC 80 MHz Cortex-M4F\n" \
"  \eYo\eC TI TM4C123 Microcontroller\n" \
"  \eYo\eC 32 KiB SRAM (24 KiB reserved for user)\n" \
"  \eYo\eC 256 KiB Flash ROM\n" \
"  \eYo\eC Simple C API for programming\n" \
"  \eYo\eC Serial Input @ 115200 bps\n" \
"  \eYo\eC 9-pin Atari Joystick interface\n" \
"  \eYo\eC 8-bit mono audio output\n" \
"  \eYo\eC PS/2 Keyboard Input*\n" \
"  \eYo\eC SD/MMC Interface*\n" \
"\eW\n" \
"* coming soon\n" \
"\n" \
"Software ports so far include TINY BASIC and\n" \
"Snake.\n" \
"\n";

static const char PAGE_3[] = "" \
"\eZ\eG\e^Learn more:\n" \
"\eR\evLearn more:\n" \
"\e-\n" \
"  \eYo\eC https://github.com/thejpster/monotron\n" \
"\n" \
"  \eYo\eC https://github.com/thejpster/monotron-apps\n" \
"\n" \
"  \eYo\eC https://github.com/rust-embedded\n" \
"\n" \
"  \eYo\eC https://www.rust-lang.org\n" \
"\n" \
"  \eYo\eC https://twitter.com/therealjpster\n" \
"\eW\n";

static void delay_frames(unsigned int frames);
static bool run = true;

int main(void) {
	while(true) {
		puts(PAGE_1);
		delay_frames(FRAMES_PER_SECOND * PAGE_SECONDS);
		if (!run) {
			return 0;
		}
		puts(PAGE_2);
		delay_frames(FRAMES_PER_SECOND * PAGE_SECONDS);
		if (!run) {
			return 0;
		}
		puts(PAGE_3);
		delay_frames(FRAMES_PER_SECOND * PAGE_SECONDS);
		if (!run) {
			return 0;
		}
	}
	return 1;
}

static void delay_frames(unsigned int frames) {
	for(unsigned int x = 0; x < frames; x++) {
		if (kbhit()) {
			getchar();
			run = false;
			return;
		}
		wfvbi();
	}
}
