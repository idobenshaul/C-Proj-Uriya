//TODO need to free widget
//TODO need to free gameoptions!

#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_video.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "TreeUtils.h"
#include "logic.h"

#define WIN_W 1000
#define WIN_H 1000
#define IMG_W 240
#define IMG_H 296

//menu defines
#define BUTTON_WIDTH 270
#define BUTTON_HEIGHT 67
#define BUTTON_H_GAP 7
#define SELECTED BUTTON_WIDTH*2
#define SELECTED_WORLD -BUTTON_WIDTH-5
#define NEW_GAME_LOCATION 200 ... 200+BUTTON_HEIGHT
#define LOAD_GAME_LOCATION 10*1+200+1*(BUTTON_HEIGHT+BUTTON_H_GAP) ...10*1+200+2*(BUTTON_HEIGHT+BUTTON_H_GAP)
#define CREATE_GAME_LOCATION 10*2+200+2*(BUTTON_HEIGHT+BUTTON_H_GAP) ... 10*2+200+3*(BUTTON_HEIGHT+BUTTON_H_GAP)
#define EDIT_GAME_LOCATION 10*3+200+3*(BUTTON_HEIGHT+BUTTON_H_GAP) ... 10*3+200+4*(BUTTON_HEIGHT+BUTTON_H_GAP)
#define QUIT_LOCATION 10*4+200+4*(BUTTON_HEIGHT+BUTTON_H_GAP) ... 10*4+200+5*(BUTTON_HEIGHT+BUTTON_H_GAP)
#define HUMAN_LOCATION NEW_GAME_LOCATION
#define MACHINE_LOCATION LOAD_GAME_LOCATION
#define U_SELECT_BACK_LOCATION CREATE_GAME_LOCATION
#define SKILL_LOCATION NEW_GAME_LOCATION
#define S_SELECT_BACK_LOCATION LOAD_GAME_LOCATION
#define DONE_LOCATION CREATE_GAME_LOCATION
#define UP_ARROW 200 ... 237
#define DOWN_ARROW 239 ... 280
#define CAPTION_LOCATION_X 400
#define LOAD_GAME_CAPTION_LOCATION_Y 23
#define SKILL_CAT_CAPTION_LOCATION_Y 2*CAPTION_HEIGHT
#define SKILL_MOUSE_CAPTION_LOCATION_Y 20+3*CAPTION_HEIGHT-2
#define SELECT_MOUSE_CAPTION_LOCATION_Y 20+6*CAPTION_HEIGHT-10
#define SELECT_CAT_CAPTION_LOCATION_Y 20+5*CAPTION_HEIGHT-5

//game and editor window defines

#define GRID_WIDTH 830
#define GRID_HEIGHT 663
#define BOARD_BUTTON_WIDTH 160
#define BOARD_BUTTON_HEIGHT 49
#define TOP_PANEL_HEIGHT 115
#define SIDE_PANEL_WIDTH 207
#define CELL_HEIGHT 78
#define CELL_WIDTH 88
#define PAUSE_LOCATION_Y 5+STATUS_CAPTION_HEIGHT*2
#define PAUSE_LOCATION_X 300
#define RECONF_MOUSE_LOCATION 130 ... (130 + BOARD_BUTTON_HEIGHT)
#define RECONF_CAT_LOCATION (130 + BOARD_BUTTON_HEIGHT + 50) ... (130 + 2*BOARD_BUTTON_HEIGHT + 50)
#define RESTART_GAME_LOCATION (130 + 2*BOARD_BUTTON_HEIGHT + 100) ... (130 + 3*BOARD_BUTTON_HEIGHT + 100)
#define GOTO_MAIN_MENU_LOCATION (130 + 3*BOARD_BUTTON_HEIGHT + 150) ... (130 + 4*BOARD_BUTTON_HEIGHT + 150)
#define QUIT_PROG_LOCATION (130 + 4*BOARD_BUTTON_HEIGHT + 200) ... (130 + 5*BOARD_BUTTON_HEIGHT + 200)
#define SAVE_GAME_LOCATION 10 ... 10+BOARD_BUTTON_WIDTH
#define RETURN_TO_MAIN_EDIT_LOCATION 10+300 ... 10+300+BOARD_BUTTON_WIDTH
#define QUIT_PROG_EDIT_LOCATION 10+600 ... 10+600+BOARD_BUTTON_WIDTH
#define PLACE_MOUSE_LOCATION RECONF_MOUSE_LOCATION
#define PLACE_CAT_LOCATION RECONF_CAT_LOCATION
#define PLACE_CHEESE_LOCATION RESTART_GAME_LOCATION
#define PLACE_WALL_LOCATION GOTO_MAIN_MENU_LOCATION
#define PLACE_EMPTY_LOCATION QUIT_PROG_LOCATION
#define CAPTION_HEIGHT 70
#define CAPTION_WIDTH 264
#define CAPTION_X 687
#define CAPTION_DELTA 7
#define MOUSE_LOCATION 1624
#define CAT_LOCATION 1536
#define CHEESE_LOCATION 1362
#define WALL_LOCATION 1450
#define PINK_DELTA 85
#define STATUS_CAPTION_WIDTH 300
#define STATUS_CAPTION_HEIGHT 24
#define STATUS_CAPTIONS_X 1112
#define STATUS_CAPTIONS_Y 187
#define SPACE_BUTTON_Y 2*STATUS_CAPTION_HEIGHT+9
#define SPACE_BUTTON_X 1094
#define SPACE_BUTTON_HEIGHT 48
#define SPACE_BUTTON_WIDTH 265
#define GAME_OVER_MOUSE_Y 386
#define GAME_OVER_CAT_Y 453
#define GAME_OVER_TIMEOUT_Y 530
#define BIG_MSG_WIDTH 600
#define BIG_MSG_HEIGHT 70
#define WORLD_SELECT_X 1590
#define WORLD_SELECT_UNMARK_X 1366
#define PAUSED_SIDE_PANEL_LOCATION 1155
#define NUM_HEIGHT 179
#define DIGIT_X 1125
#define DIGIT_Y 597
#define DIGIT_DELTA 48
#define MOUSE_1ST_DIGIT_X 135
#define DIGIT_WIDTH 12
#define CAT_1ST_DIGIT_X 110
#define PINK_CELL_X 1360
#define PINK_CELL_Y 171
#define YELLOW_Y 83
#define WORLD_CAPTION_X 83
#define WORLD_CAPTION_Y 700
#define WORLD_CAPTION_WIDTH 115
#define WORLD_CAPTION_HEIGHT 33

//player definitions
#define MOUSE 0
#define CAT 1
#define MID_GAME 10

//actions definitions
#define NEW_GAME 1
#define LOAD_GAME 2
#define CREATE_GAME 3
#define EDIT_GAME 4
#define QUIT 5
#define HUMAN 6
#define MACHINE 7
#define BACK 8
#define LEVEL_UP 9
#define LEVEL_DOWN 10
#define DONE 11
#define MOVE 12
#define PAUSE 13
#define UNPAUSE 14
#define SAVE_GAME 15
#define RESTART_GAME 16
#define RECONF_CAT 18
#define RECONF_MOUSE 17
#define PLACE_CAT 19
#define PLACE_MOUSE 20
#define PLACE_CHEESE 21
#define PLACE_WALL 22
#define PLACE_EMPTY 23
#define WARP 24
#define MAIN_MENU 25
#define RECONF_CAT_SKILL 26
#define RECONF_MOUSE_SKILL 27

//macro defines
#define WindowInitMacro	 	if (SDL_Init(SDL_INIT_VIDEO) < 0) {\
								printf("ERROR: unable to init SDL: %s\n", SDL_GetError());\
								return 1;\
							} \
							SDL_WM_SetIcon(SDL_LoadBMP("catMouse.bmp"), NULL);\
							atexit(SDL_Quit);

#define PauseMacro if (!gameOver) { \
						if (pause)\
							action = UNPAUSE;\
						else\
							action = PAUSE;\
					}

struct Widget {
	int srcX, srcY, width, height;
	int x, y;
	int isSelected;
	char* img_filename;
	char* caption;
};

typedef struct Widget Widget;

typedef Widget* WidgetRef;



WidgetRef mouse, cat, pink; //Widgets for quick board updating
void displayCaptions(int action);
int humanSelect(int player);
int openGameWindow(int gameNum);
//int skillSelect(int player);
void updateGridWarp(char** board, int prevRow, int prevCol, int newRow,
		int newCol);
SDL_Surface * display;

WidgetRef getChildWidget(TreeRef root, int childNum) {
	int i = 0;
	ListRef cur = getChildren(root);
	for (i = 1; i < childNum; i++) {
		if (isEmpty(cur)) {
			return NULL;
		} else {
			cur = tail(cur);
		}
	}
	TreeRef temp = (TreeRef) (headData(cur));
	void** p = rootData(temp);
	return (WidgetRef) p;
}

int printWidget(void* data) {
	WidgetRef temp = (WidgetRef) data;
	printf(
			"x=%d,  y=%d, srcX=%d, srcY=%d, width=%d,  height=%d, isSelected=%d, filename equals= %s\n",
			temp->x, temp->y, temp->srcX, temp->srcY, temp->width, temp->height,
			temp->isSelected, temp->img_filename);
	//printf("isSelected equals= %d\n",((WidgetRef)rootData(temp))->isSelected);
	//printf("filename equals= %p\n",((WidgetRef)rootData(temp))->img_filename);
	return 1;
}

int displayWidget(void* data) {
	WidgetRef widget = (WidgetRef) data;
	SDL_Rect imgrect = {
			widget->isSelected ? widget->srcX + SELECTED : widget->srcX,
			widget->srcY, widget->width, widget->height };
	SDL_Rect dstrect = { widget->x, widget->y, widget->width, widget->height };

	SDL_Surface *img = SDL_LoadBMP(widget->img_filename);
	if (widget->img_filename == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		if (SDL_FillRect(display, &dstrect,
				SDL_MapRGB(display->format, 130, 119, 108)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			SDL_FreeSurface(img);
			exit(0);
		}
	} else {

		//puts(widget->img_filename);
		// Apply the image to the display
		if (SDL_BlitSurface(img, &imgrect, display, &dstrect) != 0) {
			SDL_FreeSurface(img);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return 0;
		}
	}

	// Create window surface

	if (SDL_Flip(display) != 0) {
		//printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		SDL_FreeSurface(img);
		return 0;
	}
	SDL_FreeSurface(img);
	return 1;
}

Widget* createWidget(int x, int y, int srcX, int srcY, int width, int height,
		int isSelected, char* filename, char* caption) {
//printf ("%s\n", filename);
	Widget* temp = malloc(sizeof(Widget));
	temp->srcX = srcX;
	temp->srcY = srcY;
	temp->x = x;
	temp->y = y;
	temp->width = width;
	temp->height = height;
	temp->isSelected = isSelected;
	temp->img_filename = filename;
	temp->caption = caption;
	//printf ("%s\n", temp->img_filename);
	//printf("new widget address: %d\n", (int) temp);
	return temp;
}

int openMainWindow() {
	int i, action = 0, gameNum = 1;

	// Initialize SDL and make sure it quits
	WindowInitMacro;
	display = SDL_SetVideoMode(500, 700, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	TreeRef t = newTree(createWidget(0, 0, 0, 0, 600, 7000, 0, NULL, "hgh"));
	i = 0;

	for (i = 0; i < 5; i++) {
		insertChild(t,
				createWidget(100, i * 10 + 200 + i * BUTTON_HEIGHT, 6,
						9 + i * (BUTTON_HEIGHT + BUTTON_H_GAP), BUTTON_WIDTH,
						BUTTON_HEIGHT, 0, "Buttons.bmp", ""));
	}
	getChildWidget(t, 1)->isSelected = 1;

	nonRecDFS(t, displayWidget);
	//display caption
	displayCaptions(MAIN_MENU);

	// Poll for keyboard & mouse events
	SDL_Event e;
	WidgetRef cur;
	while (action != QUIT) {

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;

			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					action = QUIT;
					break;
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							printWidget(cur);
							action = i;
							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t, i);
					} //emd while*/
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t, i);
					while (cur != NULL) {
						printWidget(cur);
						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t, i);
					}
					break;
				default:
					break;
				} //end keyup switch
				break;
			case (SDL_MOUSEBUTTONUP):
				printf("%d", e.button.y);
				if ((e.button.x > 100) && (e.button.x < 95 + BUTTON_WIDTH))
					switch (e.button.y) {
					case NEW_GAME_LOCATION:
						action = NEW_GAME;
						break;
					case LOAD_GAME_LOCATION:
						action = LOAD_GAME;
						break;
					case CREATE_GAME_LOCATION:
						action = CREATE_GAME;
						break;
					case EDIT_GAME_LOCATION:
						action = EDIT_GAME;
						puts("edit game");
						break;
					case QUIT_LOCATION:
						puts("quit");
						action = QUIT;
						break;
					default:
						break;
					} //end mouse location switch
				break; //event type
			} //end switch

			switch (action) {
			case NEW_GAME:
			case LOAD_GAME:
				puts("new game");
				SDL_Quit();
				if (action == LOAD_GAME) {
					gameNum = worldSelect(LOAD_GAME, 1, -1);
				} else {
					gameNum = 1;
				}
				if (gameNum != BACK) {
					puts("gamenum select succeeded");
					if (humanSelect(CAT) == 1) {
						puts("finished game gathering");
						openGameWindow(gameNum);
						puts("back from game");
					}
				}

				WindowInitMacro
				display = SDL_SetVideoMode(500, 700, 0,
				SDL_HWSURFACE | SDL_DOUBLEBUF);
				nonRecDFS(t, displayWidget);
				//display caption
				displayCaptions(MAIN_MENU);

				action = 0;
				break;
			case EDIT_GAME:
			case CREATE_GAME:

				puts("create game");
				SDL_Quit();

				if (action == EDIT_GAME) {
					gameNum = worldSelect(EDIT_GAME, 1, -1);
				} else {
					gameNum = 0;
				}
				if (gameNum != BACK) {
					openEditorWindow(gameNum);
				}
				WindowInitMacro
				display = SDL_SetVideoMode(500, 700, 0,
				SDL_HWSURFACE | SDL_DOUBLEBUF);
				nonRecDFS(t, displayWidget);
				//display caption
				displayCaptions(MAIN_MENU);

				action = 0;
				break;
			case QUIT:
				SDL_Quit();
				return 1;
				break;
			default:
				break;
			} //end switch
		} //end poll event
	}  //end while
//SDL_Delay(4000);

	return 1;
}

WidgetRef newWidget(int x, int y, int width, int height, int isSelected,
		char* img_filename, char* caption) {
	WidgetRef new;
	new = malloc(sizeof(struct Widget));
	new->x = x;
	new->y = y;
	new->width = width;
	new->height = height;
	new->isSelected = isSelected;
	new->img_filename = img_filename;
	new->caption = caption;

	return new;
}

int main90() {
	//findShortestPaths();
	 game.cat_human =1;
	 game.mouse_human =1;

	openGameWindow(1);
	//openEditorWindow(0);
	//SaveErrorMsgWindow(3);
	//worldSelect(LOAD_GAME, 3, 0);
	//skillSelect(4);

}

void displayCaptions(int action) {
	WidgetRef temp = createWidget(100, 100, CAPTION_X, 0,
	CAPTION_WIDTH, CAPTION_HEIGHT, 0, "Buttons2.bmp", "Caption");
	printf("dispaly action %d\n", action);
	switch (action) {
	case MAIN_MENU:
		SDL_WM_SetCaption("Cat and Mouse", "");
		temp->srcY = 6 * (CAPTION_HEIGHT) + 5 * CAPTION_DELTA;
		break;
	case LOAD_GAME:
		SDL_WM_SetCaption("Load Game", "");
		temp->srcY = 5;
		break;
	case SAVE_GAME:
		SDL_WM_SetCaption("Save Game", "");
		temp->srcY = 9 * CAPTION_HEIGHT + 8 * CAPTION_DELTA;
		break;
	case RECONF_CAT_SKILL:
		SDL_WM_SetCaption("Choose Cat Skill", "");
		temp->srcY = 2 * CAPTION_HEIGHT + CAPTION_DELTA;
		break;
	case RECONF_MOUSE_SKILL:
		SDL_WM_SetCaption("Choose Mouse Skill", "");
		temp->srcY = 4 * CAPTION_HEIGHT + 3 * CAPTION_DELTA;
		break;
	case RECONF_CAT:
		SDL_WM_SetCaption("Choose Your Cat", "");
		temp->srcY = 7 * CAPTION_HEIGHT + 6 * CAPTION_DELTA;
		break;
	case RECONF_MOUSE:
		SDL_WM_SetCaption("Choose Your Mouse", "");
		temp->srcY = 8 * CAPTION_HEIGHT + 7 * CAPTION_DELTA;
		break;
	case EDIT_GAME:
		SDL_WM_SetCaption("Edit Game", "");
		temp->srcY = 10 * CAPTION_HEIGHT + 9 * CAPTION_DELTA;
		break;
	case CREATE_GAME:
		SDL_WM_SetCaption("Create Game", "");
		break;

	}
	displayWidget(temp);
	free(temp);
}

int main(int argc, char* args[]) {

	if (argc > 1) {
		if (!strcmp(args[1], "-console")) {
			consoleMode();
		}
	}

	int quit = 0;
	while (!quit) {
		quit = openMainWindow();
	}
	return 0;
}

TreeRef boardToTree(char** board) {
	TreeRef temp = newTree(
			createWidget(0, 0, 0, 0, 862, 662, 0, "Board.bmp", "Board Panel"));
	int i, j;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			switch (board[j][i]) {
			case 'M':
				mouse = createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH + 3,
				TOP_PANEL_HEIGHT + j * CELL_HEIGHT, MOUSE_LOCATION, 0,
				CELL_WIDTH - 10, CELL_HEIGHT - 4, 0, "Board.bmp", "Mouse");
				insertChild(temp, mouse);

				break;
			case 'P':
				insertChild(temp,
						createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH + 4,
						TOP_PANEL_HEIGHT + j * CELL_HEIGHT + 1, CHEESE_LOCATION,
								0,
								CELL_WIDTH - 10, CELL_HEIGHT - 5, 0,
								"Board.bmp", "Cheese"));
				break;
			case 'W':
				insertChild(temp,
						createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH + 4,
						TOP_PANEL_HEIGHT + j * CELL_HEIGHT, WALL_LOCATION, 0,
						CELL_WIDTH - 10, CELL_HEIGHT - 3, 0, "Board.bmp",
								"Wall"));
				break;
			case 'C':
				cat = createWidget(SIDE_PANEL_WIDTH + i * CELL_WIDTH + 3,
				TOP_PANEL_HEIGHT + j * CELL_HEIGHT, CAT_LOCATION, 0,
				CELL_WIDTH - 10, CELL_HEIGHT - 4, 0, "Board.bmp", "Cat");
				insertChild(temp, cat);
				break;
			}
		}
	}

	return temp;
}

void updateGridAdj(int direction, char newCellContent, char prevCellContent) {

	puts("update");
	WidgetRef temp;
	int imgLoc;

	if (strcmp(turn, "mouse")) {
		temp = mouse;
		imgLoc = MOUSE_LOCATION;
	} else if (strcmp(turn, "cat")) {
		temp = cat;
		imgLoc = CAT_LOCATION;
	}

	//print empty cell in old location

	temp->srcY = TOP_PANEL_HEIGHT;
	temp->srcX = SIDE_PANEL_WIDTH + 3;
	displayWidget(temp);
	//move to new location and print in new location
	temp->srcX = imgLoc;
	temp->srcY = 0;

	switch (direction) {
	case UP:
		temp->y -= CELL_HEIGHT;
		break;
	case DOWN:
		temp->y += CELL_HEIGHT;
		break;
	case LEFT:
		temp->x -= CELL_WIDTH;
		break;
	case RIGHT:
		temp->x += CELL_WIDTH;
		break;
	}
	//display new cell
	displayWidget(temp);

	//fix widths and xs
	switch (newCellContent) {
	case 'C':
	case 'M':
	case 'P':
		temp->width += 7;
		temp->x -= 3;
		break;

	case 'W':
		temp->width += 14;
		temp->x -= 10;
		break;

	}
}

int openGameWindow(int gameNum) {
	int i = 1, action = -1, direction = 0, pause = 0, turnCounter = 1,
			gameOver = 0, prevRow = 0, prevCol = 0;
	WidgetRef temp, gameOverCaption;

	if (game.cat_human == -1 || game.mouse_human == -1) {
		return -1;
	}
	//loading game & initing board
	char** board = loadGame(gameNum);
	//updating global variables
	updateBoardStatus(board);

	WindowInitMacro;
	display = SDL_SetVideoMode(826, 662, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

	TreeRef sidePanel = newTree(
			createWidget(0, TOP_PANEL_HEIGHT, 0, 0, SIDE_PANEL_WIDTH,
			GRID_HEIGHT, 0,
			NULL, "Side Panel"));
	for (i = 0; i < 5; i++) {
		insertChild(sidePanel,
				createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i, 0,
						i * BOARD_BUTTON_HEIGHT,
						BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0,
						"Buttons2.bmp", ""));
	}

	TreeRef frozenSidePanel = newTree(
			createWidget(0, TOP_PANEL_HEIGHT, PAUSED_SIDE_PANEL_LOCATION, 0,
			SIDE_PANEL_WIDTH, GRID_HEIGHT, 0, "Board.bmp", "hgh"));
	for (i = 0; i < 5; i++) {
		insertChild(frozenSidePanel,
				createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i,
				BOARD_BUTTON_WIDTH + 6, i * BOARD_BUTTON_HEIGHT,
				BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0, "Buttons2.bmp",
						"Frozen SidePanel Button"));
	}

	TreeRef topPanel = newTree(
			createWidget(0, 0, 0, 0, 0, 0, 0, NULL, "Top Panel object"));
	for (i = 0; i < 2; i++) {
		insertChild(topPanel,
				createWidget(300, 5 + i * ( STATUS_CAPTION_HEIGHT),
				STATUS_CAPTIONS_X, 0, STATUS_CAPTION_WIDTH,
				STATUS_CAPTION_HEIGHT + 4, 0, "Buttons.bmp", "TopPanelWidget"));
	}
	WidgetRef pauseButton = createWidget(330, SPACE_BUTTON_Y + 2,
	SPACE_BUTTON_X, SPACE_BUTTON_HEIGHT + 9, SPACE_BUTTON_WIDTH,
	SPACE_BUTTON_HEIGHT, 0, "Buttons.bmp", "PauseButton");

	WidgetRef tens = createWidget(300 + MOUSE_1ST_DIGIT_X, 9, 2000, 2000,
	DIGIT_WIDTH,
	STATUS_CAPTION_HEIGHT, 0, "Buttons.bmp", "Tens digit");
	WidgetRef ones = createWidget(300 + MOUSE_1ST_DIGIT_X + DIGIT_WIDTH, 9,
	DIGIT_X, DIGIT_Y, DIGIT_WIDTH,
	STATUS_CAPTION_HEIGHT, 0, "Buttons.bmp", "Ones digit");

	insertChild(topPanel, pauseButton);
	insertChild(topPanel, tens);
	insertChild(topPanel, ones);

	getChildWidget(topPanel, 3)->height = SPACE_BUTTON_HEIGHT;
	TreeRef boardTree = boardToTree(board);

	nonRecDFS(boardTree, displayWidget);
	nonRecDFS(frozenSidePanel, displayWidget);
	SDL_WM_SetCaption("Cat and Mouse", "");
	//SDL_Delay(5000);

	SDL_Event e;
	WidgetRef cur;
	SDL_Rect curPos;
	curPos.w = CELL_WIDTH;
	curPos.h = CELL_HEIGHT;

	while (action != QUIT) {

		if (action != 0 && pause == 0) {
			puts("refreshing");
			printf("turn %d, %d\n", turnCounter,
					(turnCounter / 2 - turnCounter / 2 % 10) / 10);

			action = 0;
			//digit updating
			if (turnCounter > 19 && tens->srcX == 2000) {
				tens->srcX = DIGIT_X;
			}
			tens->srcY = DIGIT_Y
					+ (DIGIT_DELTA)
							* +(((turnCounter / 2 - (turnCounter / 2) % 10))
									/ 10);

			ones->srcY = DIGIT_Y + (DIGIT_DELTA) * ((turnCounter / 2 % 10));
			if (!strcmp(turn, "mouse")) {
				cur = mouse;
				puts("mouse turn");
				tens->x = 300 + MOUSE_1ST_DIGIT_X;
				ones->x = 300 + MOUSE_1ST_DIGIT_X + DIGIT_WIDTH;

				getChildWidget(topPanel, 1)->srcY = 2 + NUM_HEIGHT;

				if (game.mouse_human) {
					puts("human mouse");
					getChildWidget(topPanel, 2)->srcY = 2 + NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 2;
					getChildWidget(topPanel, 3)->srcY = SPACE_BUTTON_HEIGHT + 5;

				} else { //machine mouse
					getChildWidget(topPanel, 2)->srcY = 2 + NUM_HEIGHT
							+ 4 * STATUS_CAPTION_HEIGHT;
					direction = wrapGetBestChild(catRow, catCol, mouseRow,
							mouseCol, &board, game.mouse_skill,
							getChildrenStates, free, evalFunc, 1).index;

					action = MOVE;
				}

			} else {
				cur = cat;
				puts("cat turn");

				tens->x = 300 + CAT_1ST_DIGIT_X;
				ones->x = 300 + CAT_1ST_DIGIT_X + DIGIT_WIDTH;

				getChildWidget(topPanel, 1)->srcY = 2 + NUM_HEIGHT
						+ STATUS_CAPTION_HEIGHT;
				if (game.cat_human) {
					puts("human cat");
					getChildWidget(topPanel, 2)->srcY = 2 + NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 2;

				} else { //machine cat
					direction = wrapGetBestChild(catRow, catCol, mouseRow,
							mouseCol, &board, game.cat_skill, getChildrenStates,
							free, evalFunc, 1).index;

					action = MOVE;
					getChildWidget(topPanel, 2)->srcY = 2 + NUM_HEIGHT
							+ 4 * STATUS_CAPTION_HEIGHT;
				}

			}
			if (!game.cat_human && (!game.mouse_human)) {
				SDL_Delay(1000);
			}
			curPos.x = cur->x;
			curPos.y = cur->y;
			nonRecDFS(topPanel, displayWidget);

		}
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {

				case SDLK_DOWN:
					action = MOVE;
					direction = DOWN;
					break;
				case SDLK_UP:
					action = MOVE;
					direction = UP;
					break;
				case SDLK_LEFT:
					action = MOVE;
					direction = LEFT;
					break;
				case SDLK_RIGHT:
					action = MOVE;
					direction = RIGHT;
					break;
				case SDLK_ESCAPE:
					action = BACK;
					break;
				case (SDLK_SPACE):
					PauseMacro

					break;
				case (SDLK_F1):
					if (pause)
						action = RECONF_MOUSE;
					break;
				case (SDLK_F2):
					if (pause)
						action = RECONF_CAT;
					break;
				case (SDLK_F3):
					if (pause)
						action = RESTART_GAME;
					break;
				case (SDLK_F4):
					if (pause)
						action = BACK;
					break;
				default:
					break;
				} //end switch-keyup
				break; //break keyup

			case (SDL_MOUSEBUTTONUP):
				if ((!strcmp(turn, "mouse") && game.mouse_human)
						|| (!strcmp(turn, "cat") && game.cat_human)) {
					if ((e.button.x > curPos.x + curPos.w)
							&& (e.button.x < curPos.x + 2 * curPos.w)
							&& (e.button.y > curPos.y)
							&& (e.button.y < curPos.y + curPos.h)) {

						action = MOVE;
						direction = RIGHT;
					} else if ((e.button.x > curPos.x - CELL_WIDTH)
							&& (e.button.x < curPos.x)
							&& (e.button.y > curPos.y)
							&& (e.button.y < curPos.y + curPos.h)) {
						action = MOVE;
						direction = LEFT;
					} else if ((e.button.x > curPos.x)
							&& (e.button.x < curPos.x + curPos.w)
							&& (e.button.y > curPos.y - curPos.h)
							&& (e.button.y < curPos.y)) {
						action = MOVE;
						direction = UP;
					} else if ((e.button.x > curPos.x)
							&& (e.button.x < curPos.x + curPos.w)
							&& (e.button.y > curPos.y + curPos.h)
							&& (e.button.y < curPos.y + 2 * curPos.h)) {
						action = MOVE;
						direction = DOWN;
					}
				}
				if ((e.button.x > getChildWidget(topPanel, 3)->x)
						&& (e.button.x
								< getChildWidget(topPanel, 3)->x
										+ SPACE_BUTTON_WIDTH)
						&& (e.button.y > getChildWidget(topPanel, 3)->y)
						&& (e.button.y
								< getChildWidget(topPanel, 3)->y
										+ SPACE_BUTTON_HEIGHT)) {
					PauseMacro;
				} else if (pause
						&& e.button.x > 20&& e.button.x<20+BOARD_BUTTON_WIDTH) {
					puts("board button");
					printf("%d\n", e.button.y);
					switch (e.button.y) {
					case RECONF_MOUSE_LOCATION:
						action = RECONF_MOUSE;
						break;
					case RECONF_CAT_LOCATION:
						action = RECONF_CAT;
						puts("place cat");
						break;
					case RESTART_GAME_LOCATION:
						action = RESTART_GAME;
						puts("place cheese");
						break;
					case GOTO_MAIN_MENU_LOCATION:
						action = BACK;
						break;
					case QUIT_PROG_LOCATION:
						action = QUIT;
						break;

					} //end switch
				}

				break;
			} //end switch e-type

		} //end pollevent
		switch (action) {
		case RECONF_CAT:
		case RECONF_MOUSE:
			SDL_Quit();
			if (action == RECONF_CAT) {
				puts("reconf cat");
				humanSelect(CAT + MID_GAME);
			} else {
				puts("reconf mouse");
				humanSelect(MOUSE + MID_GAME);
			}
			WindowInitMacro

			display = SDL_SetVideoMode(826, 662, 0,
			SDL_HWSURFACE | SDL_DOUBLEBUF);
			SDL_WM_SetCaption("Cat and Mouse", "");
			nonRecDFS(boardTree, displayWidget);
			nonRecDFS(sidePanel, displayWidget);
			nonRecDFS(topPanel, displayWidget);
			action = 0;
			break;
		case QUIT:
		case BACK:
			SDL_Quit();
			destroyTree(sidePanel, free);
			destroyTree(topPanel, free);
			destroyTree(frozenSidePanel, free);
			destroyTree(boardTree, free);
			freeBoard(board);
			if (action == BACK) {
				return 1;
			} else {
				exit(0);
			}
			break;
		case MOVE:

			if (pause) {
				action = 0;
				direction = 0;
				break;
			} else if (i=move(&board, direction)) {
				puts("move");
				puts(turn);
				printf ("%d" ,i);
				updateGridAdj(direction, '#', '#');
				turnCounter++;
			}
			direction = 0;
			break;

		case PAUSE:
			pause = 1;
			if (!gameOver) {
				nonRecDFS(sidePanel, displayWidget);
				if ((game.cat_human && !strcmp(turn, "cat"))
						|| (game.mouse_human && !strcmp(turn, "mouse"))) {
					getChildWidget(topPanel, 2)->srcY = 3 + NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 3;
				} else {
					getChildWidget(topPanel, 2)->srcY = 3 + NUM_HEIGHT
							+ STATUS_CAPTION_HEIGHT * 3; //TODO add machine pause
				}

				getChildWidget(topPanel, 3)->srcY =
						11 + 2 * SPACE_BUTTON_HEIGHT;
				nonRecDFS(topPanel, displayWidget);
			}
			action = 0;
			break;
		case RESTART_GAME:
		case UNPAUSE:
			if (action == RESTART_GAME) {
				board = loadGame(gameNum);
				gameOver = 0;
				turnCounter = 1;
				boardTree = boardToTree(board);
				nonRecDFS(boardTree, displayWidget);
			}
			pause = 0;
			if (!gameOver) {
				nonRecDFS(frozenSidePanel, displayWidget);
				getChildWidget(topPanel, 2)->srcY = NUM_HEIGHT
						+ STATUS_CAPTION_HEIGHT * 2;
				if ((game.cat_human && !strcmp(turn, "cat"))
						|| (game.mouse_human && !strcmp(turn, "mouse"))) {
					getChildWidget(topPanel, 3)->srcY = 8 + SPACE_BUTTON_HEIGHT;
				} else {
					getChildWidget(topPanel, 3)->srcY = 5;
				}
				nonRecDFS(topPanel, displayWidget);
			}

			break;
		default:
			break;
		}

		if (!gameOver) {
			if (turnCounter / 2 > max_turns) {
				puts("tie");
				gameOver = 1;
				//cover up things
				temp = rootData(topPanel);
				temp->width = 882;
				temp->height = TOP_PANEL_HEIGHT - 5;
				displayWidget(temp);
				temp->width = temp->height = 0;

				//display game over caption
				gameOverCaption = createWidget(120, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y + BIG_MSG_HEIGHT * 2,
				BIG_MSG_WIDTH - 10, BIG_MSG_HEIGHT + 5, 0, "Buttons.bmp", "");
				displayWidget(gameOverCaption);
				free(gameOverCaption);

			}
			switch (updateBoardStatus(board)) {
			case 1:
				puts("mouse wins");
				gameOver = 1;

				//cover up things
				temp = rootData(topPanel);
				temp->width = 882;
				temp->height = TOP_PANEL_HEIGHT - 5;
				displayWidget(temp);
				temp->width = temp->height = 0;
				//display caption
				gameOverCaption = createWidget(80, 10,
				STATUS_CAPTIONS_X, GAME_OVER_MOUSE_Y,
				BIG_MSG_WIDTH + 30, BIG_MSG_HEIGHT + 5, 0, "Buttons.bmp", "");
				displayWidget(gameOverCaption);
				free(gameOverCaption);
				break;
			case 2:
				puts("cat wins");
				gameOver = 1;

				//cover up things
				temp = rootData(topPanel);
				temp->width = 882;
				temp->height = TOP_PANEL_HEIGHT - 5;
				displayWidget(temp);
				temp->width = temp->height = 0;
				//display caption
				gameOverCaption = createWidget(120, 10,
				STATUS_CAPTIONS_X, GAME_OVER_CAT_Y,
				BIG_MSG_WIDTH - 10, BIG_MSG_HEIGHT + 5, 0, "Buttons.bmp", "");
				displayWidget(gameOverCaption);
				free(gameOverCaption);
				break;
			}	//end select
			if (gameOver) {
				action = PAUSE;
				pause = 1;
				nonRecDFS(sidePanel, displayWidget);
			}
		} //end gameover if
	} //while action
//SDL_Delay(5000);
	SDL_Quit();

	return 1;
}

int worldSelect(int selectionType, int curVal, int player) {
	int i = 1, action = 0, worldNum = -1, skillNum = -1;
	WidgetRef temp;
	switch (selectionType) {
	case SAVE_GAME:
	case LOAD_GAME:
	case EDIT_GAME:
		worldNum = curVal;
		temp = createWidget(100, 0 * 10 + 200 + 0 * BUTTON_HEIGHT, 6,
				10 + (BUTTON_HEIGHT + BUTTON_H_GAP) * (8 + worldNum),
				BUTTON_WIDTH, BUTTON_HEIGHT, 1, "Buttons.bmp",
				"World # Button");
		break;
	case RECONF_CAT_SKILL:
	case RECONF_MOUSE_SKILL:
		if (selectionType == RECONF_CAT_SKILL) {
			skillNum = (game.cat_skill) ? game.cat_skill : 1;
		} else {
			skillNum = (game.mouse_skill) ? game.mouse_skill : 1;
		}
		temp = createWidget(100, 0 * 10 + 200 + 0 * BUTTON_HEIGHT,
		BUTTON_WIDTH + 6, 9 + (BUTTON_HEIGHT + BUTTON_H_GAP) * (skillNum - 1),
		BUTTON_WIDTH, BUTTON_HEIGHT, 1, "Buttons.bmp", "Skill # Button");
		break;

	}

	puts("worldNum select");
	WindowInitMacro;
	display = SDL_SetVideoMode(480, 500, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

//build and display display-tree
	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 800, 0, NULL, "hgh"));
	insertChild(t2, temp); //insert skill/world selection box
	for (i = 7; i < 9; i++) {
		Widget* b = createWidget(100,
				(i - 6) * 10 + 200 + (i - 6) * BUTTON_HEIGHT, 6,
				9 + i * (BUTTON_HEIGHT + BUTTON_H_GAP), BUTTON_WIDTH,
				BUTTON_HEIGHT + 1, 0, "Buttons.bmp", "");
		insertChild(t2, b);
	}

	nonRecDFS(t2, displayWidget);

//display caption
	displayCaptions(selectionType);

	SDL_Event e;
	WidgetRef cur;
	i = 1;
	while (action != QUIT) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							switch (i) {
							case 1:
								break;
							case 2:
								action = BACK;
								break;
							case 3:
								action = DONE;
								break;
							}

							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					} //end while

					break;
				case SDLK_DOWN:
					if (i == 1)
						action = LEVEL_DOWN;
					break;
				case SDLK_UP:
					if (i == 1)
						action = LEVEL_UP;
					break;
				case SDLK_ESCAPE:
					action = BACK;
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t2, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t2, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					}
					break;
				default:
					break;
				}
				break;
			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > 100) && (e.button.x < 95 + BUTTON_WIDTH)) {
					switch (e.button.y) {

					case S_SELECT_BACK_LOCATION:
						action = BACK;
						break;
					case DONE_LOCATION:
						action = DONE;
						break;
					case UP_ARROW:
						puts("up");
						if (e.button.x > 100 + BUTTON_WIDTH - 32) {
							action = LEVEL_UP;
						}
						break;
					case DOWN_ARROW:
						puts("down");
						if (e.button.x > 100 + BUTTON_WIDTH - 32) {
							action = LEVEL_DOWN;
						}
						break;
					} //end mouse-y button
				}  //end mouse-x button
				break;
			}

		} //end pollevenet
		switch (action) {
		case LEVEL_DOWN:
		case LEVEL_UP:
			cur = getChildWidget(t2, 1);
			if (selectionType == SAVE_GAME || selectionType == LOAD_GAME
					|| selectionType == EDIT_GAME) {
				if (action == LEVEL_DOWN) {
					worldNum--;
					if (worldNum == 0) {
						worldNum = 5;

					}
				} else {
					worldNum++;
					if (worldNum == 6) {
						worldNum = 1;

					}
				}

				cur->srcY = 10
						+ (BUTTON_HEIGHT + BUTTON_H_GAP) * (8 + worldNum);
			} else { //skill congif
				puts("change skill");
				if (action == LEVEL_DOWN) {
					skillNum--;
					if (skillNum == 0) {
						skillNum = 9;

					}
				} else {
					skillNum++;
					if (skillNum == 10) {
						skillNum = 1;
					}
				}

				cur->srcY = 10
						+ (BUTTON_HEIGHT + BUTTON_H_GAP) * (skillNum - 1);
			}
			displayWidget(cur);
			action = 0;
			break;

		case BACK:
			SDL_Quit();
			destroyTree(t2, free);
			return BACK;
			break;
		case DONE:
			SDL_Quit();
			puts("quit");
			if (selectionType == SAVE_GAME || selectionType == LOAD_GAME
					|| selectionType == EDIT_GAME) {
				destroyTree(t2, free);
				return worldNum;
			} else {
				if (player % 10 == MOUSE) {
					game.mouse_skill = skillNum;
					destroyTree(t2, free);
					return 1;
				}
				if (player == CAT + MID_GAME) {
					game.cat_skill = skillNum;
					destroyTree(t2, free);
					return 1;
				}
				if (player == CAT) {
					game.cat_skill = skillNum;
					//open window for mouse
					if (humanSelect(MOUSE) == BACK) {
						puts("back pressed");
						WindowInitMacro
						display = SDL_SetVideoMode(500, 500, 0,
						SDL_HWSURFACE | SDL_DOUBLEBUF);
						nonRecDFS(t2, displayWidget);
						displayCaptions(selectionType);

						action = 0;
					} else { //mouse skill select succeeded
						destroyTree(t2, free);
						return 1;
					}
				}
				destroyTree(t2, free);
				return skillNum;
			}
		}
	}

//SDL_Delay(2000);
	SDL_Quit();
	return -1;
}

int humanSelect(int player) {
//return 1 if human selected correctly , otherwise on fail

	printf("start player %d\n", player);
	int i, action = 0;
	WindowInitMacro;
	display = SDL_SetVideoMode(500, 500, 0,
	SDL_HWSURFACE | SDL_DOUBLEBUF);

	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 800, 0, NULL, "hgh"));

	for (i = 5; i < 8; i++) {
		Widget* b = createWidget(100,
				(i - 5) * 10 + 200 + (i - 5) * BUTTON_HEIGHT, 6,
				10 + i * (BUTTON_HEIGHT + BUTTON_H_GAP), BUTTON_WIDTH,
				BUTTON_HEIGHT, 0, "Buttons.bmp", "");
		insertChild(t2, b);
	}
	getChildWidget(t2, 1)->isSelected = 1;

	nonRecDFS(t2, displayWidget);
	//display caption
	if (player % 10 == CAT) {
		displayCaptions(RECONF_CAT);
	} else {
		displayCaptions(RECONF_MOUSE);
	}

	SDL_Event e;
	WidgetRef cur;
	while (action != QUIT) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = BACK;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					puts("enter");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						if (cur->isSelected == 1) {
							printWidget(cur);
							action = i + 5;
							printf("%d", action);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					} //emd while

					break;
				case SDLK_ESCAPE:
					action = BACK;
					break;
				case SDLK_TAB:
					printf("tab\n");
					i = 1;
					cur = getChildWidget(t2, i);
					while (cur != NULL) {
						printWidget(cur);

						if (cur->isSelected == 1) {
							puts("1");
							cur->isSelected = 0;
							displayWidget(cur);
							i++;
							cur = getChildWidget(t2, i);
							if (cur == NULL) {
								//if list is over, start from beginning
								i = 1;
								cur = getChildWidget(t2, i);
								puts("over");
							}
							cur->isSelected = 1;
							displayWidget(cur);
							break;
						}
						i++;
						cur = getChildWidget(t2, i);
					}
					break;
				default:
					break;
				}
				break;
			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > 100) && (e.button.x < 95 + BUTTON_WIDTH)) {
					switch (e.button.y) {
					case HUMAN_LOCATION:
						action = HUMAN;
						break;
					case MACHINE_LOCATION:
						action = MACHINE;
						break;
					case U_SELECT_BACK_LOCATION:
						action = BACK;
						break;
					}
				}
				break;
			}

		} //end pollevenet

		switch (action) {

		case HUMAN:
			SDL_Quit();

			if (player % 10 == MOUSE) { // player is mouse
				game.mouse_human = 1;
				destroyTree(t2, free);
				return 1;
			}
			if (player == CAT + MID_GAME) {
				printf("player %d", player);
				game.cat_human = 1;
				destroyTree(t2, free);
				return 1;
			}
			puts("try mouse");
			if (player == CAT) { //on game setup, not midgame
				game.cat_human = 1;
				if (humanSelect(MOUSE) == BACK) {
					puts("back pressed");
					WindowInitMacro
					display = SDL_SetVideoMode(500, 500, 0,
					SDL_HWSURFACE | SDL_DOUBLEBUF);
					nonRecDFS(t2, displayWidget);

					//display caption
					displayCaptions(RECONF_CAT);
					action = 0;
				} else { // human select succeeded
					SDL_Quit();
					destroyTree(t2, free);
					return 1;
				}
			}

			break;
		case MACHINE:
			SDL_Quit();
			puts("machine");

			//update gameOptions
			if (player % 10 == CAT) {
				game.cat_human = 0;
				action = RECONF_CAT_SKILL;
			} else {
				game.mouse_human = 0;
				action = RECONF_MOUSE_SKILL;
			}

			//open following windows

			if (worldSelect(action, -1, player) == BACK) { //skill was put back
				WindowInitMacro
				display = SDL_SetVideoMode(500, 500, 0,
				SDL_HWSURFACE | SDL_DOUBLEBUF);
				nonRecDFS(t2, displayWidget);

				//display caption
				if (player % 10 == CAT) {
					displayCaptions(RECONF_CAT);
				} else {
					displayCaptions(RECONF_MOUSE);
				}
				action = 0;
			} else {
				SDL_Quit();
				destroyTree(t2, free);
				return 1;

			}
			break;
		case BACK:
			SDL_Quit();
			destroyTree(t2, free);
			return BACK;
			break;
		} //end switch
	} //end poll

	SDL_Quit();
	return 0;

}

int openEditorWindow(int gameNum) {

	int i = 1, action = -1, direction = 0, selectedCellRow = 0,
			selectedCellCol = 0, prevCol = 0, prevRow = 0, err = 0;

	/*
	 if (game.cat_human == -1 || game.mouse_human == -1) {
	 return -1;
	 }*/

	char** board = loadGame(gameNum);

	sprintf(turn, "editor");
	//$$$ Check including it inside
	updateBoardStatus(board);

	WindowInitMacro;
	display = SDL_SetVideoMode(826, 662, 0,
	SDL_HWSURFACE | SDL_DOUBLEBUF);

	//TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 826, 662, 0, NULL, "hgh"));
	//nonRecDFS(t2, displayWidget);

	TreeRef sidePanel = newTree(createWidget(0, TOP_PANEL_HEIGHT, 0, 0,
	SIDE_PANEL_WIDTH,
	GRID_HEIGHT, 0,
	NULL, "Panel"));
	for (i = 0; i < 5; i++) {
		insertChild(sidePanel,
				createWidget(20, 130 + BOARD_BUTTON_HEIGHT * i + 50 * i, 0,
						(i + 5) * BOARD_BUTTON_HEIGHT,
						BOARD_BUTTON_WIDTH, BOARD_BUTTON_HEIGHT, 0,
						"Buttons2.bmp", "Button"));
	}

	TreeRef topPanel = newTree(
			createWidget(0, 310, 0, 0, 0, 0, 0, NULL, "Panel"));
	for (i = 0; i < 3; i++) {
		insertChild(topPanel, createWidget(10 + i * 300, 50, 0,
		BOARD_BUTTON_HEIGHT * (10 + i),
		BOARD_BUTTON_WIDTH - 1,
		BOARD_BUTTON_HEIGHT + 2, 0, "Buttons2.bmp", "hgh"));
	}
	WidgetRef gameNumCaption = createWidget(350, 10, WORLD_CAPTION_X,
	WORLD_CAPTION_Y + (gameNum - 1) * (BUTTON_HEIGHT + BUTTON_H_GAP),
	WORLD_CAPTION_WIDTH, WORLD_CAPTION_HEIGHT, 0, "Buttons.bmp",
			"World # caption");
	if (!gameNum) {
		cheeseRow = cheeseCol = mouseRow = mouseCol = catRow = catCol = -1;
		gameNumCaption->srcX = 461;
		gameNumCaption->srcY = 64;
		gameNumCaption->img_filename = "Buttons2.bmp";
	}
	insertChild(topPanel, gameNumCaption);

	TreeRef boardTree = boardToTree(board);

	nonRecDFS(boardTree, displayWidget);
	nonRecDFS(sidePanel, displayWidget);
	nonRecDFS(topPanel, displayWidget);
	SDL_WM_SetCaption("World Editor", "");

	//init pink cell
	pink = createWidget(SIDE_PANEL_WIDTH, TOP_PANEL_HEIGHT,
	PINK_CELL_X,
	PINK_CELL_Y,
	CELL_WIDTH, CELL_HEIGHT - 4, 0, "Board.bmp", "");

	displayWidget(pink);

	SDL_Event e;
	while (action != QUIT) {

		if (action != 0) {
			puts("refreshing");

			action = 0;
			direction = 0;
		}

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = QUIT;
				break;
			case (SDL_KEYUP):
				switch (e.key.keysym.sym) {

				case SDLK_DOWN:
					action = MOVE;
					direction = DOWN;
					break;
				case SDLK_UP:
					action = MOVE;
					direction = UP;
					break;
				case SDLK_LEFT:
					action = MOVE;
					direction = LEFT;
					break;
				case SDLK_RIGHT:
					action = MOVE;
					direction = RIGHT;
					break;
				case SDLK_ESCAPE:
					action = BACK;
					break;
				case (SDLK_SPACE):
					action = PLACE_EMPTY;
					break;
				case (SDLK_c):
					action = PLACE_CAT;
					break;
				case (SDLK_m):
					action = PLACE_MOUSE;
					break;
				case (SDLK_p):
					action = PLACE_CHEESE;
					break;
				case (SDLK_w):
					action = PLACE_WALL;
					break;
				case (SDLK_s):
					action = SAVE_GAME;
					break;

				default:
					break;
				} //end switch-keyup
				break; //break keyup

			case (SDL_MOUSEBUTTONUP):

				if ((e.button.x > SIDE_PANEL_WIDTH) && (e.button.x < 862)
						&& (e.button.y > TOP_PANEL_HEIGHT)
						&& (e.button.y < 662)) {
					action = WARP;
					selectedCellRow = (e.button.y - TOP_PANEL_HEIGHT)
							/ CELL_HEIGHT;
					selectedCellCol = (e.button.x - SIDE_PANEL_WIDTH)
							/ CELL_WIDTH;
					printf("row %d, col %d\n", selectedCellRow,
							selectedCellCol);

				} else if (

				/*(e.button.x > 10)
				 && (e.button.x
				 < 10
				 + BOARD_BUTTON_WIDTH)*/
				(e.button.y > getChildWidget(topPanel, 1)->y)
						&& (e.button.y
								< getChildWidget(topPanel, 1)->y
										+ BOARD_BUTTON_HEIGHT)) {
					switch (e.button.x) {
					case SAVE_GAME_LOCATION:
						action = SAVE_GAME;
						break;
					case RETURN_TO_MAIN_EDIT_LOCATION:
						action = BACK;
						break;
					case QUIT_PROG_EDIT_LOCATION:
						action = QUIT;
						break;
					}

				}

				else if (e.button.x > 20
						&& e.button.x < (20 + BOARD_BUTTON_WIDTH)) {
					printf("y: %d\n", e.button.y);
					switch (e.button.y) {

					case PLACE_MOUSE_LOCATION:
						action = PLACE_MOUSE;
						puts("place mouse");
						break;
					case PLACE_CAT_LOCATION:
						puts("place cat");
						action = PLACE_CAT;
						break;
					case PLACE_CHEESE_LOCATION:
						action = PLACE_CHEESE;
						break;
					case PLACE_WALL_LOCATION:
						action = PLACE_WALL;
						break;
					case PLACE_EMPTY_LOCATION:
						action = PLACE_EMPTY;
						break;
					} //end switch
				} //end if

				break; //break mouseup
			} //end switch e-type

		} //end pollevent
		switch (action) {
		case SAVE_GAME:
			SDL_Quit();
			err = checkBoard(board);
			printf("%d\n", err);
			if (err) {

				SaveErrorMsgWindow(err);

			} //end switch checkboard
			else {
				sprintf(turn, "cat");
				max_turns = 20;
				gameNum = worldSelect(SAVE_GAME, 1, -1);
				if (gameNum != BACK) {
					saveGame(board, gameNum);
				}
				sprintf(turn, "editor");
			}
			//reopen editor window
			WindowInitMacro

			display = SDL_SetVideoMode(826, 662, 0,
			SDL_HWSURFACE | SDL_DOUBLEBUF);

			gameNumCaption->srcX = WORLD_CAPTION_X;
			gameNumCaption->srcY = WORLD_CAPTION_Y
					+ (gameNum - 1) * (BUTTON_HEIGHT + BUTTON_H_GAP);
			gameNumCaption->img_filename = "Buttons.bmp";

			puts(gameNumCaption->img_filename);
			//sprintf (gameNumCaption->img_filename, "Buttons.bmp");
			puts("back from save");
			nonRecDFS(boardTree, displayWidget);
			nonRecDFS(sidePanel, displayWidget);
			nonRecDFS(topPanel, displayWidget);
			displayWidget(gameNumCaption);
			SDL_WM_SetCaption("World Editor", "");
			updateGridWarp(board, -1, -1, selectedCellRow, selectedCellCol);
			//displayWidget(pink);

			break;
		case PLACE_CHEESE:
			puts("place cheese");
			if (selectedCellRow == cheeseRow && selectedCellCol == cheeseCol) {
				break;
			}

			switch (board[selectedCellRow][selectedCellCol]) {
			case 'C':
				catCol = -1;
				catRow = -1;
				break;
			case 'M':
				mouseCol = -1;
				mouseRow = -1;
				break;

			}
			board[selectedCellRow][selectedCellCol] = 'P';
			if (cheeseRow > -1 && cheeseCol > -1) {
				printf("cheeseRow %d %d", cheeseRow, cheeseCol);
				board[cheeseRow][cheeseCol] = '#';
			}
			updateGridWarp(board, cheeseRow, cheeseCol, selectedCellRow,
					selectedCellCol);

			cheeseRow = selectedCellRow;
			cheeseCol = selectedCellCol;
			break;

		case PLACE_WALL:
		case PLACE_EMPTY:
			switch (board[selectedCellRow][selectedCellCol]) {
			case 'C':
				catCol = -1;
				catRow = -1;
				break;
			case 'M':
				mouseCol = -1;
				mouseRow = -1;
				break;
			case 'P':
				cheeseCol = -1;
				cheeseRow = -1;
				break;
			}
			if (action == PLACE_EMPTY) {
				board[selectedCellRow][selectedCellCol] = '#';
			} else {
				board[selectedCellRow][selectedCellCol] = 'W';
			}
			updateGridWarp(board, -1, -1, selectedCellRow, selectedCellCol);
			//updateGridAdj(0, 'W', 'W');
			break;

		case PLACE_CAT:
			puts("place cat");
			if (selectedCellRow == catRow && selectedCellCol == catCol) {
				break;
			}

			switch (board[selectedCellRow][selectedCellCol]) {
			case 'P':
				cheeseCol = -1;
				cheeseRow = -1;
				break;
			case 'M':
				mouseCol = -1;
				mouseRow = -1;
				break;
			}
			board[selectedCellRow][selectedCellCol] = 'C';
			if (catRow > -1 && catCol > -1) {
				board[catRow][catCol] = '#';

			}
			updateGridWarp(board, catRow, catCol, selectedCellRow,
					selectedCellCol);

			catRow = selectedCellRow;
			catCol = selectedCellCol;
			break;

		case PLACE_MOUSE:

			if (selectedCellRow == mouseRow && selectedCellCol == mouseCol) {
				break;
			}

			switch (board[selectedCellRow][selectedCellCol]) {
			case 'C':
				catCol = -1;
				catRow = -1;
				break;
			case 'P':
				cheeseCol = -1;
				cheeseRow = -1;
				break;
			}
			board[selectedCellRow][selectedCellCol] = 'M';
			if (mouseRow > -1 && mouseCol > -1) {
				board[mouseRow][mouseCol] = '#';
			}

			updateGridWarp(board, mouseRow, mouseCol, selectedCellRow,
					selectedCellCol);
			mouseRow = selectedCellRow;
			mouseCol = selectedCellCol;
			break;

		case QUIT:
		case BACK:
			SDL_Quit();
			destroyTree(sidePanel, free);
			destroyTree(topPanel, free);
			destroyTree(boardTree, free);
			free(pink);
			freeBoard(board);
			if (action == BACK) {
				return 1;
			} else {
				exit(0);
			}
			break;

		case MOVE:
			if ((pink->x == SIDE_PANEL_WIDTH && direction == LEFT)
					|| (pink->x == SIDE_PANEL_WIDTH + 6 * CELL_WIDTH
							&& direction == RIGHT)
					|| (pink->y == TOP_PANEL_HEIGHT && direction == UP)
					|| (pink->y == TOP_PANEL_HEIGHT + 6 * CELL_HEIGHT
							&& direction == DOWN)) {
				break;
			} else { // move is possible
				puts("change");

				switch (direction) {
				case RIGHT:
					selectedCellCol++;
					break;
				case LEFT:
					selectedCellCol--;
					break;
				case UP:
					selectedCellRow--;
					break;
				case DOWN:
					selectedCellRow++;
					break;
				}
				printf("%c", board[selectedCellRow][selectedCellCol]);
				updateGridWarp(board, prevRow, prevCol, selectedCellRow,
						selectedCellCol);
				prevCol = selectedCellCol;
				prevRow = selectedCellRow;
			}
			break;
		case WARP:
			updateGridWarp(board, prevRow, prevCol, selectedCellRow,
					selectedCellCol);
			prevCol = selectedCellCol;
			prevRow = selectedCellRow;
			break;
		} //end switch

	} //while action
	SDL_Quit();

	return 1;
}

void updateGridWarp(char** board, int prevRow, int prevCol, int newRow,
		int newCol) {
	//puts("updateGridWarp");
	//printBoard(board);
	WidgetRef temp = pink;

//draw prevCol

	printf("newrow %d newcol %d %c \n ", newRow, newCol, board[newRow][newCol]);

	if (prevCol > -1 && prevRow > -1) {

		//set prevLocation

		pink->y = TOP_PANEL_HEIGHT + prevRow * CELL_HEIGHT;
		pink->x = SIDE_PANEL_WIDTH + prevCol * CELL_WIDTH;

		//draw empty first
		temp->srcY = TOP_PANEL_HEIGHT;
		temp->srcX = SIDE_PANEL_WIDTH;
		displayWidget(temp);
		//draw actual

		temp->width -= 14;
		switch (board[prevRow][prevCol]) {
		case 'C':
			temp->x += 3;
			temp->srcX = CAT_LOCATION - 2;
			temp->srcY = 0;
			displayWidget(temp);
			temp->x -= 3;
			break;
		case 'M':
			temp->x += 3;
			temp->srcX = MOUSE_LOCATION;
			temp->srcY = 0;
			displayWidget(temp);
			temp->x -= 3;
			break;
		case 'P':
			temp->x += 3;
			temp->srcX = CHEESE_LOCATION;
			temp->srcY = 0;
			displayWidget(temp);
			temp->x -= 3;
			break;
		case 'W':
			temp->x += 4;
			temp->srcX = WALL_LOCATION;
			temp->srcY = 0;
			displayWidget(temp);
			temp->x -= 4;
			break;

		} //end switch prevcell content
		temp->width += 14;
	}
//warp to new location
	pink->y = TOP_PANEL_HEIGHT + newRow * CELL_HEIGHT;
	pink->x = SIDE_PANEL_WIDTH + newCol * CELL_WIDTH;

// print new cell
	/*
	 printf ("row %d col %d %c \n " , newRow, newCol, board[newRow][newCol] );
	 */
	temp->srcY = TOP_PANEL_HEIGHT;
	temp->srcX = SIDE_PANEL_WIDTH;
	displayWidget(temp);

	temp->srcY = YELLOW_Y;
	switch (board[newRow][newCol]) {
	case 'C':
		temp->width -= 5;
		temp->x += 3;
		temp->srcX = CAT_LOCATION - 2;
		break;
	case 'M':
		temp->width -= 5;
		temp->x += 3;
		temp->srcX = MOUSE_LOCATION;
		break;
	case 'P':
		temp->width -= 5;
		temp->x += 3;
		temp->srcX = CHEESE_LOCATION;
		break;
	case 'W':
		temp->width -= 4;
		temp->x += 4;
		temp->srcX = WALL_LOCATION;
		break;
	default:
		temp->srcX = PINK_CELL_X;
		temp->srcY = PINK_CELL_Y;
		break;
	} //end switch newcell content

	displayWidget(temp);

//fix widths and xs

	switch (board[newRow][newCol]) {
	case 'C':
	case 'M':
	case 'P':
		temp->width += 5;
		temp->x -= 3;
		break;
	case 'W':
		temp->width += 4;
		temp->x -= 4;
		break;
	}
}

int SaveErrorMsgWindow(int err) {
	int action;
	SDL_Event e;
	WindowInitMacro;
	display = SDL_SetVideoMode(500, 300, 0,
	SDL_HWSURFACE | SDL_DOUBLEBUF);
	TreeRef t2 = newTree(createWidget(0, 0, 0, 0, 600, 800, 0, NULL, "hgh"));
	insertChild(t2, createWidget(110, 50,
	CAPTION_LOCATION_X, 23 + (CAPTION_HEIGHT - 2) * 7 - 8,
	CAPTION_WIDTH - 50, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));
	insertChild(t2, createWidget(110, 120,
	CAPTION_LOCATION_X, 23 + (CAPTION_HEIGHT - 2) * (7 + err) - 8,
	CAPTION_WIDTH - 50, CAPTION_HEIGHT, 0, "Buttons2.bmp", ""));

	insertChild(t2, createWidget(100, 200, 4, BUTTON_HEIGHT * 7 - 8,
	BUTTON_WIDTH, BUTTON_HEIGHT, 1, "Buttons.bmp", ""));

	nonRecDFS(t2, displayWidget);
	while (action != QUIT) {

		if (action != 0) {
			puts("refreshing");

			action = 0;
		}

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT):
				action = BACK;
				break;
			case (SDL_KEYUP):
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					action = BACK;
				}
				break; //break keyup
			case (SDL_MOUSEBUTTONUP):
				if (e.button.x > 100&& e.button.x < 100 + BUTTON_WIDTH
				&& e.button.y > 200
				&& e.button.y < 200 + BOARD_BUTTON_HEIGHT) {
					action = BACK;
				} //end if
				break; //break mouseup
			} //end switch e-type
		} //end pollevent
		if (action == BACK) {
			SDL_Quit();
			return 1;
			break;
		} //end if

	} //while action
	return 0;
}
