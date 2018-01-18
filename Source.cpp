#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
using namespace std;

//constants & enumerations
const float FPS = 60;
const int player1_SIZE = 20;
const int player2_SIZE = 20;
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
enum MYKEYS2 {
	KEY_A, KEY_S, KEY_D, KEY_W

};
enum DIRECTIONS {RIGHT, LEFT, UP, DOWN};

int main() {
	//setting up matrix
	int grid[50][50];
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++) {
			//set all 0s except edges
			grid[i][j] = 0;
			if (i == 0 || i == 49 || j == 2 || j == 49)
				grid[i][j] = 1;
		}

	//print to console for testing 
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++)
			cout << grid[i][j];
		cout << endl;
	}

	//set up variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *player1 = NULL;
	ALLEGRO_BITMAP *player2 = NULL;
	ALLEGRO_FONT *font = NULL;
	int player1_x = 300;
	int player1_y = 400;
	int player2_x = 150;
	int player2_y = 200;
	bool key[4] = { false, false, false, false };
	bool key2[4] = { false, false, false, false };
	bool redraw = true;
	bool doexit = false;
	int dx1 = 0;
	int dy1 = 0;
	int dx2 = 0;
	int dy2 = 0;
	int dir = 0;
	int dir2 = 0;
	int liv1 = 3;
	int liv2 = 3;

	//set up allegro stuff
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	al_install_keyboard();

	timer = al_create_timer(1.0 / FPS);

	display = al_create_display(500, 500);

	//load font
	font = al_load_ttf_font("tron.ttf", 25, NULL);

	player1 = al_create_bitmap(player1_SIZE, player1_SIZE);
	player2 = al_create_bitmap(player2_SIZE, player2_SIZE);

	al_set_target_bitmap(player1);

	al_clear_to_color(al_map_rgb(0, 0, 255));//player 1 color

	al_set_target_bitmap(player2);

	al_clear_to_color(al_map_rgb(255, 255, 0));//player 2 color
	
	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//player1 = al_load_bitmap("Blue bike.png");
	//player2 = al_load_bitmap("yellow bike.png");

	//gameloop
	while (!doexit) {

		//cout << "p2s velocity is" << dx2 << " , " << dy2 << endl;
		//cout << "key2 holds" << key2[0] << key2[1] << key2[2] << key2[3] << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			if (liv1 == 0) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_text(font, al_map_rgb(255, 255, 0), 640 / 2, (480 / 4), ALLEGRO_ALIGN_CENTRE, "Player 2 Wins!");
				al_flip_display();
				al_rest(2);
				exit(1);
			}

			if (liv2 == 0) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_text(font, al_map_rgb(20, 20, 255), 640 / 2, (480 / 4), ALLEGRO_ALIGN_CENTRE, "Player 1 Wins!");
				al_flip_display();
				al_rest(2);
				exit(1);
			}

			if (key[KEY_UP]) {
				//set the velocity, not the position!
				dy1 = -1;
				dx1 = 0;
				dir = UP;//used for collision
			}

			if (key[KEY_DOWN]) {
				dy1 = 1;
				dx1 = 0;
				dir = DOWN;
			}

			if (key[KEY_LEFT]) {
				dx1 = -1;
				dy1 = 0;
				dir = LEFT;
			}

			if (key[KEY_RIGHT]) {
				dx1 = 1;
				dy1 = 0;
				dir = RIGHT;
			}

			if (key2[KEY_A]) {
				dx2 = -1;
				dy2 = 0;
				dir2 = LEFT;
			}

			if (key2[KEY_D]) {
				dx2 = 1;
				dy2 = 0;
				dir2 = RIGHT;
			}

			if (key2[KEY_S]) {
				dy2 = 1;
				dx2 = 0;
				dir2 = DOWN;
			}

			if (key2[KEY_W]) {
				//cout << "player 1 thinks w has been pressed" << endl;
				dy2 = -1;
				dx2 = 0;
				dir2 = UP;
			}

			//moves player 1
			player1_x += dx1;
			player1_y += dy1;
			//moves player 2
			player2_x += dx2;
			player2_y += dy2;

			//draw a line behind player 1
			grid[(player1_x) / 10][(player1_y) / 10] = 2;
			//draw a line behind player 2
			grid[(player2_x) / 10][(player2_y) / 10] = 3;

			//check right collision for p1
			if (dir == RIGHT && grid[(player1_x + 10) / 10][player1_y / 10] != 0) {
				cout << "CRASH right" << endl;
				player1_x = 300;
				player1_y = 400;
				liv1--;
			}

			//check left collision for p1
			if (dir == LEFT && grid[(player1_x - 10) / 10][player1_y / 10] != 0) {
				cout << "CRASH left" << endl;
				player1_x = 300;
				player1_y = 400;
				liv1--;
			}

			//check up collision for p1
			if (dir == UP && grid[(player1_y - 30) / 10][player1_x / 10] != 0) {
				cout << "CRASH up at"<< player1_x<<" , "<<player1_y << endl;
				player1_x = 300;
				player1_y = 400;
				liv1--;
			}

			//check down collision for p1
			if (dir == DOWN && grid[(player1_y +10) / 10][player1_x / 10] != 0) {
				cout << "CRASH up at" << player1_x << " , " << player1_y << endl;
				player1_x = 300;
				player1_y = 400;
				liv1--;
			}

			//check right collision for p2
			if (dir == RIGHT && grid[(player2_x + 10) / 10][player2_y / 10] != 0) {
				cout << "CRASH right" << endl;
				player2_x = 150;
				player2_y = 200;
				liv2--;
			}

			//check left collision for p2
			if (dir == LEFT && grid[(player2_x - 10) / 10][player2_y / 10] != 0) {
				cout << "CRASH left" << endl;
				player2_x = 150;
				player2_y = 200;
				liv2--;
			}

			//check up collision for p2
			if (dir == UP && grid[(player2_y - 30) / 10][player2_x / 10] != 0) {
				cout << "CRASH up at" << player2_x << " , " << player2_y << endl;
				player2_x = 150;
				player2_y = 200;
				liv2--;
			}

			//check down collision for p2
			if (dir == DOWN && grid[(player2_y + 10) / 10][player2_x / 10] != 0) {
				cout << "CRASH up at" << player2_x << " , " << player2_y << endl;
				player2_x = 150;
				player2_y = 200;
				liv2--;
			}

			redraw = true;
		}
		//kill program when window x is pressed
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		//check if keys are pressed down
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_A:
				key2[KEY_A] = true;
				break;

			case ALLEGRO_KEY_D:
				key2[KEY_D] = true;
				break;

			case ALLEGRO_KEY_S:
				key2[KEY_S] = true;
				break;

			case ALLEGRO_KEY_W:
				//cout << "w has been pressed" << endl;
				key2[KEY_W] = true;
				break;
			}
		}
		//check if keys are released
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

			case ALLEGRO_KEY_A:
				key2[KEY_A] = false;
				break;

			case ALLEGRO_KEY_D:
				key2[KEY_D] = false;
				break;

			case ALLEGRO_KEY_S:
				key2[KEY_S] = false;
				break;

			case ALLEGRO_KEY_W:
				key2[KEY_W] = false;
				break;
			}
		}

		//RENDER SECTION
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//wipe screen
			al_clear_to_color(al_map_rgb(0, 0, 0));


			//walk through matrix, 1 is white 2 is blue
			for (int i = 0; i < 50; i++) {
				for (int j = 0; j < 50; j++) {
					if (grid[i][j] == 1)
						al_draw_filled_rectangle(i * 10, j * 10, i * 10 + 10, j * 10 + 10, al_map_rgb(255, 255, 255));
					if(grid[i][j] == 2)
						al_draw_filled_rectangle(i * 10, j * 10, i * 10 + 10, j * 10 + 10, al_map_rgb(0, 0, 255));//player 1 streak
					if (grid[i][j] == 3)
						al_draw_filled_rectangle(i * 10, j * 10, i * 10 + 10, j * 10 + 10, al_map_rgb(255, 255, 50));//player 2 streak
				}
			}

			//draw player 1
			al_draw_bitmap(player1, player1_x - 10, player1_y - 10, 0);
			//draw player 2
			al_draw_bitmap(player2, player2_x - 10, player2_y - 10, 0);

			al_draw_textf(font, al_map_rgb(20, 20, 155), 85, -2, ALLEGRO_ALIGN_CENTRE, "P1 lives : %i", liv1);
			al_draw_textf(font, al_map_rgb(255, 255, 50), 325, -2, ALLEGRO_ALIGN_CENTRE, "P2 lives : %i", liv2);
			al_flip_display();

		}//end render section
	}//end game loop

	//clear up memory
	al_destroy_bitmap(player1);
	al_destroy_bitmap(player2);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}//end main