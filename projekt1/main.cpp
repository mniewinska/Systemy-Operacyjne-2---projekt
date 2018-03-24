#include <ncurses.h> 
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>
#include <mutex> 

using std::thread;
using std::vector;
using std::string;
using std::mutex;

enum Direction

{
	UP,
	DOWN,
	LEFT,
	RIGHT
};



struct Coord
{
public:
	int X;
	int Y;
};

class Snake
{
private:
	Direction snakeDirection;
	bool canGoUp();
	bool canGoDown();
	bool canGoLeft();
	bool canGoRight();
	void moveDirection(Direction direction);
public:
	vector<Coord> AllPoints;
	int Min_X;
	int Max_X;
	int Min_Y;
	int Max_Y;
	Snake(int snakeSize);
	void Move();
};

bool Snake::canGoUp()
{
	for (Coord point : AllPoints)
	{
		if (point.Y - 1 < Min_Y)
		{
			return false;
		}
	}
	return true;
}

bool Snake::canGoDown()
{
	for (Coord point : AllPoints)
	{
		if(point.Y + 1 > Max_Y)
		{
		return false;
		}
	}
	return true;
}

bool Snake::canGoRight()
{
	for(Coord point : AllPoints)
	{
		if(point.X + 1 > Max_X)
		{
		return false;
		}
	}
	return true;
}

bool Snake::canGoLeft()
{
	for(Coord point : AllPoints)
	{
		if(point.X - 1 <Min_Y)
		{
		return false;
		}
	}
	return true;
}
void Snake::moveDirection(Direction direction)
{
	Coord point = AllPoints.at(0);

	switch (direction)
	{
	case UP:
		point.Y--;
		break;
	case DOWN:
		point.Y++;
		break;
	case LEFT:
		point.X--;
		break;
	case RIGHT:
		point.X++;
		break;
	}
	printf("move...");
	AllPoints.emplace(AllPoints.cbegin(), point);
	AllPoints.erase(AllPoints.cend());
	printf("move...end");
}

Snake::Snake(int snakeSize)
{
	AllPoints = vector<Coord>();
	snakeDirection = RIGHT;

	for (int i = 0; i < snakeSize; i++)
	{
		Coord tempPoint{};
		tempPoint.X = i;
		tempPoint.Y = 1;
		AllPoints.push_back(tempPoint);
	}
}

void Snake::Move()
{
	switch (snakeDirection)
	{
	case UP:
		if (canGoUp())
		{
			moveDirection(UP);
		}
		else
		{
			if (canGoRight())
			{
				moveDirection(RIGHT);
			}
			else
			{
				moveDirection(LEFT);
			}
		}
		break;

	case DOWN: 
		if (canGoDown())
		{
			moveDirection(DOWN);
		}
		else
		{
			if (canGoRight())
			{
				moveDirection(RIGHT);
			}
			else
			{
				moveDirection(LEFT);
			}
		}
		break;

	case LEFT: 
		if (canGoLeft())
		{
			moveDirection(LEFT);
		}
		else
		{
			if (canGoUp())
			{
				moveDirection(UP);
			}
			else
			{
				moveDirection(DOWN);
			}
		}
		break;

	case RIGHT: 
		if (canGoRight())
		{
			moveDirection(LEFT);
		}
		else
		{
			if (canGoUp())
			{
				moveDirection(UP);
			}
			else
			{
				moveDirection(DOWN);
			}
		}
		break;
	default: ;
	}
}


void draw_snake(WINDOW *screen);
void draw_borders(WINDOW *screen);
Snake* create_snake(WINDOW *screen);
void draw_single_snake(WINDOW *screen, Snake* snake);
void start_snake_movement(WINDOW *screen, const char* screenNumber);

mutex mtx;

int main(int argc, char *argv[]) {
	int parent_x, parent_y; 
	
	initscr(); 
	noecho(); 
	curs_set(FALSE); // wyłączenie widoczności kursora
	getmaxyx(stdscr, parent_y, parent_x); // pobranie wymiarów okna
	WINDOW *half_one = newwin(parent_y/2, parent_x / 2, 0, 0);
	WINDOW *half_two = newwin(parent_y/2, parent_x / 2, 0, (parent_x/2));
	WINDOW *half_three = newwin(parent_y/2,parent_x/2, (parent_y/2),0);
	WINDOW *half_four = newwin(parent_y/2, parent_x/2, (parent_y/2), (parent_x/2));
 	
	thread sneak_1(start_snake_movement, half_one, "1");
	
	thread sneak_2(start_snake_movement, half_two, "2");
	
	thread sneak_3(start_snake_movement, half_three, "3");
	
	thread sneak_4(start_snake_movement, half_four, "4");
	
	sneak_1.join();sneak_2.join();sneak_3.join();sneak_4.join();
	getch();
	//endwin();
	
	return 0; 
}

void draw_borders(WINDOW *screen) {
	int x, y, i;
	getmaxyx(screen, y, x); // 4 corners
	mvwprintw(screen, 0, 0, "+");
	mvwprintw(screen, y - 1, 0, "+"); 
	mvwprintw(screen, 0, x - 1, "+"); 
	mvwprintw(screen, y - 1, x - 1, "+"); // sides 
	for (i = 1; i < (y - 1); i++) { 
		mvwprintw(screen, i, 0, "|");
		mvwprintw(screen, i, x - 1, "|"); 
	} // top and bottom 
	for (i = 1; i < (x - 1); i++) { 
		mvwprintw(screen, 0, i, "-"); 
		mvwprintw(screen, y - 1, i, "-"); 
	}
}

Snake* create_snake(WINDOW *screen)
{
	int x, y;
        getmaxyx(screen, y, x);
	int snakeSize = 3;

	Snake* snake = new Snake(snakeSize);

	snake->Min_Y = 1;
	snake->Max_Y = ((y/2)-1);
	snake->Min_X = 1;
	snake->Max_X = ((x/2)-1);

	return snake;
}

void draw_single_snake(WINDOW *screen, Snake* snake) 
{	
	for(Coord point : snake->AllPoints)
	{
		mvwprintw(screen, (point.Y + 3) , (point.X + 3), "o");	
	}
}

void start_snake_movement(WINDOW *screen, const char* screenNumber)
{
	Snake* snake =  create_snake(screen);
	
	while(1) {
		mtx.lock();
		draw_borders(screen);
		clear();
		mvwprintw(screen, 1, 1, screenNumber); 
		draw_single_snake(screen, snake);
		wrefresh(screen); 
		mtx.unlock();
		usleep(300000);
		snake->Move();
		}
	delwin(screen); 
}
