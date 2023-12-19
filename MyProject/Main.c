#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "./constants.h"



int game_is_running = FALSE; // initially game isn't actually running, similar to flag first down
SDL_Window* window = NULL; // assumes address of our window is set to null, made global such that main can access it
SDL_Renderer* renderer = NULL; // assumes address of our renderer is set to null, made global such that main can access it


int last_frame_time = 0;
double origin_background_x = (WINDOW_WIDTH / 2) - GAME_WINDOW_WIDTH / 2; // controls background origin in x
double origin_background_y = (WINDOW_HEIGHT / 2) - GAME_WINDOW_HEIGHT / 2; // controls backgroun origin in y
int right_motion = 0; // controls motion in right direction
int left_motion = 0; // controls motion in left direction
int up_motion = 0; // controls motion in up direction
int down_motion = 0; // controls motion in the down direction
int snake_width = 15; // controls how wide from the top left the snake is
int snake_height = 15; // controls how tall the snake is from the top left
int food_counter = 0; // counter for how often the snakes eaten food
int direction = 0; // decides direction of movement of snake defaults to left

int Collision = FALSE; // variable for detecting collisions currently does nothing



struct unit //stuct that describes our basic snake head and snake body units
{
    float x;
    float y;
    float width;
    float height;

} unit;

struct body //struct that will contain a body
{
    float exists; // this field determines if that part of the snakes body actually exists
    struct unit body_part;
    int direction_body;

} body;


struct food // struct that is for our food units 
{
    float x;
    float y;
    float width;
    float height;
}food;

struct background // background struct
{
    float x;
    float y;
    float width;
    float height;
}background;


struct body snake_body[SNAKE_SIZE_CAP]; // we create a snake body comprised of body parts which are themselves a simple snake unit


int initialize_window(void) // don't need a function declaration as we are calling a function before main
{
    // initalizes everything
    if ((SDL_Init(SDL_INIT_EVERYTHING)) != 0) // handles the case where initializes fails
    {
        fprintf(stderr, "Error Initializing SDL. \n");
        return FALSE; // if we ever fail just lets us know that its false ie failure
    }
    window = SDL_CreateWindow(
        NULL, // title set it to null so we have a somewhat borderless window
        SDL_WINDOWPOS_CENTERED, //x position of window using SDL parameter that centers by x axis
        SDL_WINDOWPOS_CENTERED, // y position of window using SDL parameter that centers by y axis
        WINDOW_WIDTH,// width of window
        WINDOW_HEIGHT,// height of window
        SDL_WINDOW_BORDERLESS // sets our window to be truly borderless
    );


    // window might fail so need to error handle

    if (!window)
    {
        fprintf(stderr, "Error creating SDL window\n");
        return FALSE;
    }
    // next up is actually rendering the window
    renderer = SDL_CreateRenderer
    (
        window, // renderer first needs to know which window we are rendering, we pass the address of the window stored in window
        -1, // next we need to tell sdl what kind of driver will render our window, -1 just says use the default display driver downloaded
        0 //we don't have any special flags ie no special way of renderering
    );
    // renderer might fail so need to error handle
    if (!window)
    {
        fprintf(stderr, "Error rendering SDL window\n");
        return FALSE;
    }
    return TRUE;
}

void setup() {
    srand(time((NULL))); // used to set the seed for rand based on current time


    for (int i = 0; i < 50; i++)
    {
        snake_body[i].exists = 0;
    }
    //TODO:
    // controls the setting of our background fields 
    background.x = (float)origin_background_x;
    background.y = (float)origin_background_y;
    background.width = GAME_WINDOW_WIDTH;
    background.height = GAME_WINDOW_HEIGHT;

    unit.x = WINDOW_WIDTH / 2; // snake head will start at midpoint of window width
    unit.y = WINDOW_HEIGHT / 2; // snake head will start at midpoint of window height
    unit.width = snake_width;
    unit.height = snake_height;

    // food will spawn in a random location
    food.x = (rand() % (int)(GAME_WINDOW_WIDTH / 2)) + origin_background_x;
    food.y = (rand() % (int)(GAME_WINDOW_HEIGHT / 2)) + origin_background_y;


    food.width = 15;
    food.height = 15;


}

void process_input()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT: // Refers to the event corresponding to clicking the exit button
        game_is_running = FALSE; // changes game to false kicking us out of the game loop
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_RIGHT && direction != 1) // if they press right arrow and we  aren't moving left
        {
            // sets motion direction to right which I've encoded as 0
            right_motion += 1;
            direction = 0;
        }
        if (event.key.keysym.sym == SDLK_LEFT && direction != 0) // moves left if left arrow pressed and we aren't moving right 
        {
            left_motion += 1;
            direction = 1;
            break;
        }
        if (event.key.keysym.sym == SDLK_UP && direction != 3) // moves up if up arrow pressed and we aren't moving down 
        {
            up_motion += 1;
            direction = 2;
            break;
        }
        if (event.key.keysym.sym == SDLK_DOWN && direction != 2) // moves down if down arrow pressed and we aren't moving down
        {
            down_motion += 1;
            direction = 3;
            break;
        }
        if (event.key.keysym.sym == SDLK_ESCAPE) // refers to the event corresponding to clicking the escape key
        {
            game_is_running = FALSE; // changes game to false kicking us out of the game loop
            break;
        }
    }
}


void update()
{
    // below are some dummy variables for controlling how snake bodies are generated and their positions are moved
    int dummy_x = 0;
    int dummy_y = 0;
    int dummy_direction = 0;
    int flummy_x = 0;
    int flummy_y = 0;
    int flummy_direction = 0;

    for (int i = 0; i < food_counter; i++)
    {
        if (snake_body[i].exists == FALSE) // goes through entire snake body array and sets its exists field to false
        {
            break;
        }
        if (((snake_body[i].body_part.x > unit.x - snake_width) && (snake_body[i].body_part.x < unit.x + snake_width)) && ((snake_body[i].body_part.y > unit.y - snake_height) && (snake_body[i].body_part.y < unit.x + snake_height)))
        {
            //unit.x > food.x - food.width && unit.x < food.x + food.width) && (unit.y > food.y - food.height && unit.y < food.y + food.height
            Collision = TRUE;
        }
    }
    //: waste some time/ sleep until we reach the frame target
    // what the below does is use the SDL_Ticks_Passed function to compare two time stamps.
    // first time stamp is the current time, second one is the time since the last update ran(or first if its the first update)
    // plus the target time. 
    // SDL_TICKS_PASSED will check if we've waited long enough and if we have will continue the loop
    // but what we actually want is for the loop to continue so long as the time hasn't been reached so we use a not operator.
    // Loop will continue until they match.
    //  while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    //while loop cpu heavy, so we use SDL_Delay

    int time_to_wait = (FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time));

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }


    //Issue: Currently motion of objects is in reference to pixels/frame but we want to represent it as pixels/second

    // what the below does is create the delta_time variable by taking current time, subtracting previous update time
    // since it returns as milliseconds we divide by 1000 as a float to get back seconds
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    //originally motion was determined by the frame rate,
    //if we had 120 frames we would move 2 pixels in each frame therefore 240 pixels in 1 second
    //versus if we had 60 frames we would be moving 2 pixels for every single 60 frames so in 1 second you would move 120 pixels

    // before we added delta_time as a factor objects moved a greater distance over the same time when we had higher frames
    // Once we've added delta_time factor, objects move the same distance regardless of the framerate


    // 
    //TODO: Logic to keep a fixed timestamp



    last_frame_time = SDL_GetTicks();
    // the below handles the motion of the main unit
    if (direction == 0)
    {
        unit.x += (SPEED)*delta_time;
    }
    else if (direction == 1)
    {
        unit.x -= (SPEED)*delta_time;
    }
    else if (direction == 2)
    {
        unit.y -= (SPEED)*delta_time; // we subtract in up direction since pixels are drawn from top left to bottom right ie 3 in y is higher than 111 in y
    }
    else if (direction == 3)
    {
        unit.y += (SPEED)*delta_time;
    }


    if (food_counter == 0) // base case at the beginning
    {
        snake_body[0].exists == 1; // sets the first part to exist

        if (direction == 0)
        {
            snake_body[0].body_part.x = unit.x - snake_width;
            snake_body[0].body_part.y = unit.y;

        }
        else if (direction == 1)
        {
            snake_body[0].body_part.x = unit.x + snake_width;
            snake_body[0].body_part.y = unit.y;
        }
        else if (direction == 2)
        {
            snake_body[0].body_part.x = unit.x;
            snake_body[0].body_part.y = unit.y - snake_height;
        }
        else
        {
            snake_body[0].body_part.x = unit.x;
            snake_body[0].body_part.y = unit.y + snake_height;
        }
        snake_body[0].direction_body = direction;
        dummy_x = snake_body[1].body_part.x;
        dummy_y = snake_body[1].body_part.y;
    }



    else if (food_counter > 0)
    {
        if (direction == 0)
        {
            snake_body[food_counter].body_part.x = unit.x - snake_width;
            snake_body[food_counter].body_part.y = unit.y;
        }
        else if (direction == 1)
        {
            snake_body[food_counter].body_part.x = unit.x + snake_width;
            snake_body[food_counter].body_part.y = unit.y;

        }
        else if (direction == 2)
        {
            snake_body[food_counter].body_part.x = unit.x;
            snake_body[food_counter].body_part.y = unit.y + snake_height;
        }
        else
        {
            snake_body[food_counter].body_part.x = unit.x;
            snake_body[food_counter].body_part.y = unit.y - snake_height;
        }
        snake_body[food_counter].direction_body = direction;

        dummy_x = snake_body[food_counter].body_part.x;
        dummy_y = snake_body[food_counter].body_part.y;
        dummy_direction = snake_body[food_counter].direction_body;
    }
    else
    {

    }


    for (int i = food_counter; i > 0; i--)
    {
        if (snake_body[i].exists == 1)
        {

            flummy_x = snake_body[i].body_part.x;
            flummy_y = snake_body[i].body_part.y;
            flummy_direction = snake_body[i].direction_body;

            snake_body[i].direction_body = dummy_direction;
            snake_body[i].body_part.x = dummy_x;
            snake_body[i].body_part.y = dummy_y;

            dummy_x = flummy_x;
            dummy_y = flummy_y;
            dummy_direction = flummy_direction;
        }
        else
        {
            break;
        }
    }


    // allows for the map to wrap around
    if (unit.x < origin_background_x)
        unit.x = WINDOW_WIDTH - (origin_background_x + snake_width);
    if (unit.x > WINDOW_WIDTH - origin_background_x - snake_width)
        unit.x = origin_background_x;
    if (unit.y < origin_background_y)
        unit.y = WINDOW_HEIGHT - (origin_background_y + snake_height);
    if (unit.y > WINDOW_HEIGHT - origin_background_y - snake_height)
        unit.y = origin_background_y;




    // handles creating a new snake body
    if ((unit.x > food.x - food.width && unit.x < food.x + food.width) && (unit.y > food.y - food.height && unit.y < food.y + food.height))
    {
        food.x = (rand() % (int)(GAME_WINDOW_WIDTH / 2)) + origin_background_x;
        food.y = (rand() % (int)(GAME_WINDOW_HEIGHT / 2)) + origin_background_y;
        food_counter++;
        snake_body[food_counter].exists = 1;
        snake_body[food_counter].body_part.x = unit.x;
        snake_body[food_counter].body_part.y = unit.y;
        snake_body[food_counter].body_part.width = unit.width;
        snake_body[food_counter].body_part.height = unit.height;
    }




}
void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // below creates background for main play area
    SDL_Rect background_rect =
    {
        (int)background.x,
        (int)background.y,
        (int)background.width,
        (int)background.height
    };
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, (&background_rect));

    // here we can start the drawing of our objects
    // below creates the food 
    SDL_Rect food_rect =
    {
        (int)food.x,
        (int)food.y,
        (int)food.width,
        (int)food.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, (&food_rect));

    // below creates the snake head
    SDL_Rect unit_rect =
    {
        (int)unit.x,
        (int)unit.y,
        (int)unit.width,
        (int)unit.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, (&unit_rect));


    //below renders the rest of the snake
    for (int i = food_counter; i > 0; i--)
    {
        if (snake_body[i].exists == 1 && i == food_counter)
        {
            SDL_Rect(snake_body_rect) =
            {
                (int)snake_body[i].body_part.x,
                (int)snake_body[i].body_part.y,
                (int)snake_body[i].body_part.width,
                (int)snake_body[i].body_part.height
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, (&snake_body_rect));
        }
        if (snake_body[i].exists == 1 && i != food_counter)
        {
            SDL_Rect(snake_body_rect) =
            {
                (int)snake_body[i].body_part.x,
                (int)snake_body[i].body_part.y,
                (int)snake_body[i].body_part.width,
                (int)snake_body[i].body_part.height
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, (&snake_body_rect));
        }
    }


    SDL_RenderPresent(renderer); // setups buffer for displaying frames
}


void destroy_window()
{
    SDL_DestroyRenderer(renderer); // simply put in the thing we want to destroy
    SDL_DestroyWindow(window);
    SDL_Quit();
    // *** NOTE WE DESTORY IN THE REVERSE ORDER WE CREATED, Don't want to orphan any pointers
}


int main(void)
{
    game_is_running = initialize_window();
    setup();
    // reason we use game_is_running is that process/update/render will run again the initialize window function and if it fails game will stop
    while (game_is_running)
    {
        process_input();
        update();
        render();
    }

    // if we can no longer finish the game we must get rid of the memory associated with the window

    destroy_window();
    return 0;
}