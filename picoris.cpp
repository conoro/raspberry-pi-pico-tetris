#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "pico_display.hpp"
//#include "pico/multicore.h"

using namespace pimoroni;

#define u8 uint8_t
#define u16 uint16_t 
#define u32 uint32_t
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t

const u8 TETROMINOES[7][4][16] = {
	{ // LONG PIECE
		{ 
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0 
		},
		{ 
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0 
		}
	}, 
	{ // BACKWARD L
		{ 
			1, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0 
		}
	}, 
	{ // L
		{ 
			0, 0, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 0, 0,
			1, 1, 1, 0,
			1, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		}
	}, 
	{ // SQUARE
		{ 
			0, 1, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		}
	}, 
	{ // S
		{ 
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 0, 0,
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		}
	}, 
	{ // T
		{ 
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		}
	}, 
	{ // BACKWARD S
		{ 
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0 
		},
		{ 
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0 
		}
	}
};

const u16 SCREEN_WIDTH = PicoDisplay::WIDTH / 2;
const u16 SCREEN_HEIGHT = PicoDisplay::HEIGHT / 2;

uint16_t buffer[PicoDisplay::WIDTH * PicoDisplay::HEIGHT];
PicoDisplay pico_display(buffer);

const u8 BUTTON_X = 0;
const u8 BUTTON_Y = 1;
const u8 BUTTON_A = 2;
const u8 BUTTON_B = 3;

const u8 pico_button_map[] = {
	pico_display.X,
	pico_display.Y,
	pico_display.A,
	pico_display.B
};

u8 buttons[4];
u8 buttons_pressed[4];
u8 buttons_released[4];

bool check_button(u8 button) {
	return buttons[button];
}

bool check_button_pressed(u8 button) {
	return buttons_pressed[button];
}

bool check_button_released(u8 button) {
	return buttons_released[button];
}

u16 color(u8 r, u8 g, u8 b) {
	return pico_display.create_pen(r, g, b);
}

void pixel(u16 x, u16 y, u16 color) {
	x *= 2;
	y *= 2;
	pico_display.frame_buffer[x + y * 240] = color;
	pico_display.frame_buffer[x + 1 + y * 240] = color;
	y++;
	pico_display.frame_buffer[x + y * 240] = color;
	pico_display.frame_buffer[x + 1 + y * 240] = color;
}

void rectangle(u16 x, u16 y, u16 width, u16 height, u16 color) {
	for(u16 i = x; i < x + width; i++) {
		for(u16 t = y; t < y + height; t++) {
			pixel(i, t, color);
		}
	}
}

const u16 background = color(64, 51, 83);
const u16 field_background = color(36, 34, 52);
const u16 block_colors[] = { 
	color(180, 32, 42),
	color(250, 106, 10),
	color(255, 213, 65),
	color(89, 193, 53),
	color(40, 92, 196),
	color(188, 74, 155),
	color(32, 214, 199)
};

u8 piece_index = 0;
u8 piece_angle = 0;
u8 piece_x = 0;
u8 piece_y = 0;
u8 piece_dropping = false;
u8 field[10][20];

inline void next_piece() {
	piece_index = (piece_index + 1) % 7;
	piece_angle = 0;
	piece_x = 3;
	piece_y = 0;
}

inline void init() {
	for(u8 i = 0; i < 10; ++i) {
		for(u8 t = 0; t < 20; ++t) {
			field[i][t] = 0;
		}
	}
	next_piece();
}

inline bool piece_position_allowed() {
	const u8* piece = TETROMINOES[piece_index][piece_angle];
	for(u8 i = 0; i < 4; ++i) {
		for(u8 t = 0; t < 4; ++t) {
			u8 block = piece[i + t * 4];
			if(block) {
				u8 x = piece_x + i;
				u8 y = piece_y + t;
				if(x >= 10 || y >= 20 || field[x][y]) {
					return false;
				}
			}
		}
	}
	return true;
}

inline void place_piece() {
	piece_dropping = false;
	const u8* piece = TETROMINOES[piece_index][piece_angle];
	for(u8 i = 0; i < 4; ++i) {
		for(u8 t = 0; t < 4; ++t) {
			u8 block = piece[i + t * 4];
			if(block) {
				u8 x = piece_x + i;
				u8 y = piece_y + t;
				field[x][y] = piece_index + 1;
			}
		}
	}
	
	for(u8 t = 1; t < 20; ++t) {
		bool line_filled = true;
		for(u8 i = 0; i < 10; ++i) {
			if(field[i][t] == 0) {
				line_filled = false;
				break;
			}
		}
		if(line_filled) {
			for(u8 j = t - 1; j > 0; --j) {
				for(u8 i = 0; i < 10; ++i) {
					field[i][j + 1] = field[i][j];
				}
			}
			for(u8 i = 0; i < 10; ++i) {
				field[i][0] = 0;
			}
			--t;
		}
	}
}

inline void tick(bool update_board) {
	pico_display.set_pen(64, 51, 83);
	pico_display.clear();

	// Turn the piece
	if(check_button_pressed(BUTTON_A)) {
		piece_angle++;
		if(!piece_position_allowed()) {
			piece_angle--;
		}
		piece_angle = piece_angle % 4;
	}

	if(check_button_pressed(BUTTON_B)) {
		piece_dropping = true;
	}
	if(check_button(BUTTON_B) && piece_dropping) {
		update_board = true;
		
	}

	if(check_button_pressed(BUTTON_Y)) {
		piece_x++;
		if(!piece_position_allowed()) {
			piece_x--;
		}
	}
	if(check_button_pressed(BUTTON_X)) {
		piece_x--;
		if(!piece_position_allowed()) {
			piece_x++;
		}
	}

	if(update_board) {
		piece_y++;
		if(!piece_position_allowed()) {
			piece_y--;
			place_piece();
			next_piece();
		}
	}

	u16 offset_x = 9;
	u16 offset_y = 8;

	// Draw the field
	rectangle(offset_x, offset_y, 100, 50, field_background);
	for(u8 i = 0; i < 10; ++i) {
		for(u8 t = 0; t < 20; ++t) {
			u8 tile = field[i][t];
			if(field[i][t] > 0) {
				rectangle(offset_x + t * 5, offset_y + i * 5, 5, 5, block_colors[tile - 1]);
			}
		}
	}

	// Draw the active piece
	const u8* piece = TETROMINOES[piece_index][piece_angle];
	for(u8 i = 0; i < 4; ++i) {
		for(u8 t = 0; t < 4; ++t) {
			u8 block = piece[i + t * 4];
			if(block) {
				u8 x = piece_x + i;
				u8 y = piece_y + t;
				if(x < 10 && y < 20) {
					rectangle(offset_x + (y) * 5, offset_y + (x) * 5, 5, 5, block_colors[piece_index]);
				}
			}
		}
	}
}

void input_init() {
	for(u8 i = 0; i < 4; ++i) {
		buttons[i] = false;
		buttons_pressed[i] = false;
		buttons_released[i] = false;
	}
}

const u8 button_gpio_map[] = { 12, 13, 14, 15 };
#define EVENT_QUEUE_SIZE 128
u16 event_queue[EVENT_QUEUE_SIZE];
u16 event_queue_head = 0;
u16 event_queue_tail = 0;


bool event_queue_has_next() {
	return event_queue_head != event_queue_tail;
}

bool event_queue_full() {
	return (event_queue_head + 1) % EVENT_QUEUE_SIZE == event_queue_tail;
}

void event_queue_append(u16 value) {
	if(!event_queue_full()) {
		event_queue[event_queue_tail] = value;
		event_queue_tail = (event_queue_tail + 1) % EVENT_QUEUE_SIZE;
	}
}

u16 event_queue_next() {
	if(event_queue_has_next()) {
		event_queue_tail = (event_queue_tail - 1) % EVENT_QUEUE_SIZE;
		return event_queue[event_queue_tail];
	}
	return 0;
}

void gpio_callback(unsigned int gpio, u32 events) {
	if(events & (GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL)) {
		u8 button;
		if(gpio == 12) {
			button = BUTTON_A;
		} else if(gpio == 13) {
			button = BUTTON_B;
		} else if(gpio == 14) {
			button = BUTTON_X;
		} else if(gpio == 15) {
			button = BUTTON_Y;
		}
		u8 pressed = GPIO_IRQ_EDGE_RISE & events;

		event_queue_append(button + pressed * 4);
	}
}

void input_start_frame() {
	for(u8 i = 0; i < 4; ++i) {
		buttons_pressed[i] = false;
		buttons_released[i] = false;
	}

	while(event_queue_has_next()) {
		u32 event = event_queue_next();
		u8 button = event % 4;
		u8 pressed = event < 4;

		buttons[button] = pressed;
		if(pressed) {
			buttons_pressed[button] = true;
		} else {
			buttons_released[button] = true;
		}
	}
}

int main() {
	pico_display.init();
	pico_display.set_backlight(255);

	gpio_set_irq_enabled_with_callback(12, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
	gpio_set_irq_enabled_with_callback(13, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
	gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
	gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	u8 tick_counter = 0;
	u8 tick_interval = 30;

	init();
	input_init();

	while(true) {
		input_start_frame();
		tick_counter++;
		if(tick_counter > tick_interval) {
			tick(true);
			tick_counter = 0;
		} else {
			tick(false);
		}
		pico_display.update();
	}

	return 0;
}
