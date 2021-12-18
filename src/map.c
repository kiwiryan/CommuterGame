#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <math.h>

#define MAX_ROWS 10
#define MAX_COLUMNS 10

#define PATH ' '
#define BASE '~'
#define BOUNDARY '#'
#define PLAYER 'P'
#define WORK 'W'

#define BOUNDARY_C 1
#define ROAD_C 2
#define UI_TIME 3
#define ENEMIES 4


struct Point origin, bounds, playerLocation, workLocation, playerCoordinates, workCoordinates;

struct Point nodeCoordinates[MAX_ROWS][MAX_COLUMNS];

int spacing, mp_border, mp_block, mp_max_x, mp_max_y, mp_rows, mp_cols, mp_road;

/*
    Earl Kwofie:    

    Generates a the gridded map for the user, and calculates its size
        block_width - size of square grid modules (blocks)
        road_width - space between blocks
        blocks_row - amount of blocks in a row
        blocks_column - amount of blocks in a column    
        
*/

void setMap(int blockWidth, int roadWidth, int blocks_row, int blocks_col)
{ 
    setNodeCoordinates(blocks_row, blocks_col);
    start_color();
    init_pair(BOUNDARY_C, COLOR_WHITE, COLOR_WHITE);  
    init_pair(ROAD_C, COLOR_WHITE, COLOR_BLUE);
    init_pair(UI_TIME, COLOR_BLACK, COLOR_RED);  
    init_pair(ENEMIES, COLOR_RED, COLOR_GREEN);    

    //determines intermediate space between the center points of each block
    int btwn_top = round(blockWidth/2) + roadWidth;
    
    //assigning map values to external variables within the file
    mp_border = btwn_top;
    mp_block = blockWidth;
    mp_rows = blocks_row;
    mp_cols = blocks_col;
    spacing = roadWidth + blockWidth;
    mp_road = roadWidth;
    
    //determines the outermost spawn points for the user
    bounds.x = btwn_top + ((mp_cols - 1) * spacing);
    bounds.y = btwn_top + ((mp_rows - 1) * spacing); 
    origin.x = btwn_top; 
    origin.y = btwn_top;
   
     
    //determines the outermost boundary of play space
    mp_max_x = (spacing * blocks_col) + roadWidth;
    mp_max_y = (spacing * blocks_row) + roadWidth;
     
    /*
        Creates a series of blocks representing the city blocks
        Also determines at random where the player and the destination that the player and the player goal is located.     
    */
    
    //Establish player spawn and player goal
    setPlayer();

    attron(COLOR_PAIR(ROAD_C));
    for(int i = 0; i <= mp_max_x; i++)
    {
        mvvline(0,i,PATH,mp_max_y);   
    }
    attroff(COLOR_PAIR(ROAD_C));
   
    for(int i = mp_border; i <= bounds.x; i+=spacing)
    {
        for(int j = mp_border; j <= bounds.y; j+=spacing)
        {   
         /*if( i == playerCoordinates.y && j == playerCoordinates.x )
           { 
                for(int k = 0; k < mp_block; k++)
                { 
                  attron(COLOR_PAIR(ROAD_C));
                  mvvline(i-round(mp_block/2),j-round(mp_block/2)+k, PATH ,mp_block);
                  attroff(COLOR_PAIR(ROAD_C)); 
                }
           }*/
           if(i == workCoordinates.x && j == workCoordinates.y)
           {
                for(int k = 0; k < mp_block; k++)
                {
                    
                    attron(COLOR_PAIR(BOUNDARY_C)); 
                    mvvline(j-round(mp_block/2),i-round(mp_block/2)+k, PATH  ,mp_block);
                    attroff(COLOR_PAIR(BOUNDARY_C));
                    
                }
           }
           
           else
           {
                for(int k = 0; k < mp_block; k++)
                {
                    attron(COLOR_PAIR(BOUNDARY_C));
                    mvvline(i-round(mp_block/2),j-round(mp_block/2)+k,BOUNDARY,mp_block);
                    attroff(COLOR_PAIR(BOUNDARY_C));
                }
           }            
        }
    }  
    
    mvaddch(getNodeCoordinate(workLocation.y), getNodeCoordinate(workLocation.x), WORK);     
}

/*
    Earl Kwofie
    
    Determines where and how many enemies will spawn depending on the time the player has and the difficulty they set
    
    The lower the variable dif, the higher the difficulty
*/

void setEnemies(int dif, int time)
{
    int e_origin = mp_block + round(1.3 * mp_road);
    int e_terminal = mp_max_y - e_origin;
    int time_start = time % dif; 

    for(int i = e_origin; i <= e_terminal; i += spacing)
    {
        for(int j = time_start; j <= mp_max_x; j += dif)
        { 
             if(time_start == 0 && (j + dif - 1) <= mp_max_x)
             {
                  attron(COLOR_PAIR(ROAD_C));
                  mvaddch(i,j+(dif-1),PATH);
                  attroff(COLOR_PAIR(ROAD_C)); 

                  attron(COLOR_PAIR(ENEMIES));
                  mvaddch(i, j, '0');
                  attroff(COLOR_PAIR(ENEMIES));
             } 
             else
             {
                  attron(COLOR_PAIR(ROAD_C));
                  mvaddch(i, j-1, PATH);
                  attroff(COLOR_PAIR(ROAD_C));

                  attron(COLOR_PAIR(ENEMIES));
                  mvaddch(i, j, '0');
                  attroff(COLOR_PAIR(ENEMIES));
             }        
        
        }
    }
}

bool isDead(int y, int x)
{
    char test = mvinch(y,x);
    return (test & A_CHARTEXT) == '0'; 
}



/*
    Earl Kwofie:
    
    Determines a random location to place the player on the map
    Also determines the user destination, or the place the player works
        
    PROBLEM FIXED
        NOTE: Currently, there seems to be an issue with the way that the coordinates of the player location and the work location are setup.
        - it seems to effect the way that the placement of characters are set up,
        - maybe pointers should be used instead to be more precise about where things are on the map?
        - maybe theres a problem in the draw map function.
*/
void setPlayer()
{
    
   /* playerLocation.y = rand() % mp_rows;
      playerLocation.x = rand() % mp_cols;
    
    playerCoordinates.x = getNodeCoordinate(playerLocation.x);
    playerCoordinates.y = getNodeCoordinate(playerLocation.y); */
    
       /* do
        { */

            workLocation.x = rand() % mp_cols;
            workLocation.y = rand() % mp_rows;

       /* }while(playerLocation.y == workLocation.y && playerLocation.x == workLocation.x); */
        
    workCoordinates.x = getNodeCoordinate(workLocation.x);
    workCoordinates.y= getNodeCoordinate(workLocation.y);
}

/*
    Earl Kwofie:
    
    Determines Whether the player has made it to the destination or not 
*/
bool atWork(int x, int y)
{
    return (mvinch(y,x) == 'W');
}

int getNodeCoordinate(int coordinate)
{
    return (mp_border + (coordinate * spacing)); 
}

/*
    Earl Kwofie:    

    Returns the location of the player, primarily for the initial position
*/
int getPlayerStartX()
{
    return getNodeCoordinate(playerLocation.x);
}

/*
    Earl Kwofie:
    
    Returns the location of the player, primarily for the initial position
*/
int getPlayerStartY()
{
    return getNodeCoordinate(playerLocation.y);
}

/*
    Earl Kwofie:     

    Checks if the player can move on to the space given
*/
bool isAvailable(int x, int y) 
{   
   int test;   
 
   test = mvinch(y,x);
   return ((test & A_CHARTEXT) == PATH) || ((test & A_CHARTEXT) == BASE) || ((test & A_CHARTEXT) == WORK) || ((test & A_CHARTEXT) == '0');
}

/*
    Earl Kwofie:    

    Assigns coordinates to the possible spawn nodes
*/
void setNodeCoordinates(int rows, int columns)
{       
    if( rows <= 10 && columns <= 10)
    {
       for(int i = 0; i < rows; i++)
       {
            for(int j = 0; j < columns; j++)
            {
                nodeCoordinates[i][j].x = getNodeCoordinate(j);
                nodeCoordinates[i][j].y = getNodeCoordinate(i);
            }
       }
    }
    else
    {
    }
      
}

/*
   Earl Kwofie:    

   Gives you the X maximum boundary  
*/
int getBoundaryX()
{
    return mp_max_x;
}

/*
    Earl Kwofie:    

    Gives you the Y maximum boundary
*/
int getBoundaryY()
{
    return mp_max_y;
}
