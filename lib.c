#include <monotron.h>
#include <stdint.h>

#define FONT_MODE_NORMAL 0
#define FONT_MODE_TELETEXT 1
#define FONT_MODE_CUSTOM 2

struct callbacks_t {
	void* p_context;
	int32_t(*putchar)(void* p_context, char ch);
	int32_t(*puts)(void* p_context, const char*);
	int32_t(*readc)(void* p_context);
	void(*wfvbi)(void* p_context);
	int32_t(*kbhit)(void* p_context);
	void (*move_cursor)(void* p_context, unsigned char row, unsigned char col);
	int32_t (*play)(void* p_context, uint32_t frequency, uint8_t channel, uint8_t waveform, uint8_t volume);
	void (*change_font)(void* p_context, uint32_t mode, const void* p_font);
	uint8_t (*get_joystick)(void* p_context);
};

typedef int32_t(*entry_point_t)(const struct callbacks_t*);

const struct callbacks_t* p_callbacks;
void* p_context;

static unsigned int rand_seed = 0;

static unsigned int strlen(const char*s);
static void reverse(char s[]);

int32_t entry(const struct callbacks_t* callbacks) {
	p_callbacks = callbacks;
	return main();
}

/* Write 8-bit char to stdout */
int putchar(char ch) {
	return p_callbacks->putchar(p_callbacks->p_context, ch);
}

/* Write a connected sixel to the screen. Assumes you have the Teletext font selected. */
void put_connected_sixel(uint8_t ch) {
	ch = ch & 63;
	if (ch >= 32) {
		putchar(ch - 32 + 0xC0);
	} else {
		putchar(ch + 0x80);
	}
}

/* Write a separated sixel to the screen. Assumes you have the Teletext font selected. */
void put_separated_sixel(uint8_t ch) {
	ch = ch & 63;
	if (ch >= 32) {
		putchar(ch - 32 + 0xE0);
	} else {
		putchar(ch + 0xA0);
	}
}

/* Write 8-bit string to stdout. */
int puts(const char* s) {
	return p_callbacks->puts(p_callbacks->p_context, s);
}

/* Blocking character read from stdin. Returns a char or EOF */
int getchar(void) {
	return p_callbacks->readc(p_callbacks->p_context);
}

/* Blocking character read from stdin. Returns a char or EOF */
int kbhit(void) {
	return p_callbacks->kbhit(p_callbacks->p_context);
}

/* Wait For Vertical Blanking Interval. */
void wfvbi(void) {
	p_callbacks->wfvbi(p_callbacks->p_context);
}

int rand(void) {
	rand_seed = (rand_seed * 1103515245) + 12345;
	return rand_seed % RAND_MAX;
}

void srand(unsigned int seed) {
	rand_seed = seed;
}

void move_cursor(unsigned char row, unsigned char col) {
	p_callbacks->move_cursor(p_callbacks->p_context, row, col);
}

void itoa(int n, char s[]) {
	int i, sign;
	if ((sign = n) < 0) {  /* record sign */
		n = -n;          /* make n positive */
	}
	i = 0;
	do {       /* generate digits in reverse order */
		s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
	if (sign < 0) {
		s[i++] = '-';
	}
	s[i] = '\0';
	reverse(s);
}

int play(uint32_t frequency, channel_t channel, waveform_t waveform, uint8_t volume) {
	return p_callbacks->play(p_callbacks->p_context, frequency, (uint8_t) channel, (uint8_t) waveform, volume);
}

/* Switch to the CodePage 850 font */
void font_normal(void) {
	p_callbacks->change_font(p_callbacks->p_context, FONT_MODE_NORMAL, NULL);
}

/* Switch to the Teletext font */
void font_teletext(void) {
	p_callbacks->change_font(p_callbacks->p_context, FONT_MODE_TELETEXT, NULL);
}

/* Supply 4096 bytes of font data (16 bytes per char, 256 chars) */
void font_custom(const void* p_font) {
	p_callbacks->change_font(p_callbacks->p_context, FONT_MODE_CUSTOM, p_font);
}

/* Fetch joystick state */
uint8_t get_joystick(void) {
	return p_callbacks->get_joystick(p_callbacks->p_context);
}

/* Check joystick state */
bool joystick_is_up(uint8_t state) {
	return ((state & (1 << 4)) != 0);
}

bool joystick_is_down(uint8_t state) {
	return ((state & (1 << 3)) != 0);
}

bool joystick_is_left(uint8_t state) {
	return ((state & (1 << 2)) != 0);
}

bool joystick_is_right(uint8_t state) {
	return ((state & (1 << 1)) != 0);
}

bool joystick_fire_pressed(uint8_t state) {
	return ((state & (1 << 0)) != 0);
}

static unsigned int strlen(const char*s) {
	unsigned int result = 0;
	while(*s++ != '\0') {
		result++;
	}
	return result;
}

static void reverse(char s[]) {
	int i, j;
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		char c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

__attribute__ ((section(".entry_point")))
const entry_point_t entry_point = entry;
