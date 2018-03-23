#include <ncurses.h> 
#include <unistd.h>
#include <thread>
#include <vector>

using std::thread;
using std::vector;

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

	AllPoints.emplace(AllPoints.cbegin(), point);
	AllPoints.erase(AllPoints.cend());
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
 	// rysowanie ramki 
	draw_borders(half_one); 
	draw_borders(half_two); 
	draw_borders(half_three);
	draw_borders(half_four);

	draw_snake(half_one);
	draw_snake(half_two);
	draw_snake(half_three);
	draw_snake(half_four);


	while(1) { // draw to our windows 
		clear();
		mvwprintw(half_one, 1, 1, "1"); 
		mvwprintw(half_two, 1, 1, "2"); // refresh each window 
		mvwprintw(half_three, 1, 1, "3");
		mvwprintw(half_four, 1, 1, "4");
		//mvwprintw(half_one, a+1, b+1, "o");
		//mvwprintw(half_two, a+1, b+4, "o");
		//mvwprintw(half_three, a+1, b+2, "o");
                //mvwprintw(half_four, a+1, b+3, "o");



		wrefresh(half_one); 
		wrefresh(half_two); 
		wrefresh(half_three);
		wrefresh(half_four);

		clear();

		usleep(300000);

	} // clean up 
	delwin(half_one); 
	delwin(half_two); 
	delwin(half_three);
	delwin(half_four);
	endwin();
	
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

void draw_snake(WINDOW *screen) {

	int x, y;
        getmaxyx(screen, y, x);

	int snakeSize = 3;
	Snake* snake1 = new Snake(snakeSize);
	Snake* snake2 = new Snake(snakeSize);
	Snake* snake3 = new Snake(snakeSize);
	Snake* snake4 = new Snake(snakeSize);
	

	snake1->Min_Y = 1;
	snake1->Max_Y = ((y/2)-1);
	snake1->Min_X = 1;
	snake1->Max_X = ((x/2)-1);

        snake2->Min_Y = 1;
        snake2->Max_Y = ((y/2)-1);
        snake2->Min_X = ((x/2)+1);
        snake2->Max_X = x-1;

        snake3->Min_Y = ((y/2)+1);
        snake3->Max_Y = y-1;
        snake3->Min_X = 1;
        snake3->Max_X = ((x/2)-1);

        snake1->Min_Y = ((y/2)+1);
        snake1->Max_Y = y-1;
        snake1->Min_X = ((x/2)+1);
        snake1->Max_X = x-1;

	
		for(Coord point : snake1->AllPoints)
		{
			mvwprintw(screen, (point.Y + 3) , (point.X + 3), "o");	

		}
		move(1,1);



}

