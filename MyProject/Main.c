#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "./constants.h"




//TODO 
// ADD ERROR HANDLING
// ADD SOME FORM OF MEMORY MANAGEMENT







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
int snake_width = SNAKE_SIZE; // controls how wide from the top left the snake is
int snake_height = SNAKE_SIZE; // controls how tall the snake is from the top left
int food_counter = 0; // counter for how often the snakes eaten food
int direction = 0; // decides direction of movement of snake defaults to left
int colission = FALSE;

typedef struct color
{
    int red;
    int green;
    int blue;
}color;
typedef struct box // Main struct for everything
{
    float x;
    float y;
    float width;
    float height;
    struct color color;
}box;

 box background; // box that represents the background for the entire window
 box play_area; // box that describes the play area casted within the window
 box unit; // snake unit
 box food; // Food for snake





typedef struct node
{
    struct node* next; // points to next snake node
    box snake_body;// contains the structure for the snakes body part at that node
    int direction; // contains the direction that snake node is facing

}node;


// step 1 create a pointer to our node
node* snake_node = NULL;

//struct body snake_body[SNAKE_SIZE_CAP]; // we create a snake body comprised of body parts which are themselves a simple snake unit

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
    food.color.red = 255;
    food.color.green = 255;
    food.color.blue = 255;



    //TODO:
    // controls the setting of our background fields 
    background.x = 0;
    background.y = 0;
    background.width = WINDOW_WIDTH;
    background.height = WINDOW_HEIGHT;
    // play area
    play_area.x = (float)origin_background_x;
    play_area.y = (float)origin_background_y;
    play_area.width = GAME_WINDOW_WIDTH;
    play_area.height = GAME_WINDOW_HEIGHT;

    unit.x = WINDOW_WIDTH / 2; // snake head will start at midpoint of window width
    unit.y = WINDOW_HEIGHT / 2; // snake head will start at midpoint of window height
    unit.width = snake_width;
    unit.height = snake_height;
    unit.color.red = food.color.red;
    unit.color.green = food.color.green;
    unit.color.blue = food.color.blue;

    // food will spawn in a random location
    food.x = (rand() % (int)(GAME_WINDOW_WIDTH / 2)) + origin_background_x;
    food.y = (rand() % (int)(GAME_WINDOW_HEIGHT / 2)) + origin_background_y;
    food.width = FOOD_SIZE;
    food.height = FOOD_SIZE;






    ////create a temp node to get our first address
    node *temp = malloc(sizeof(node));

    if (temp == NULL) // error handling to ensure we get memory for our temporary node
    {
        fprintf(stderr, "Error allocating memory");
        return(1);
    }

    temp->next = NULL;

    snake_node = temp;

    //now need to free temp

    //Defines the beginning of the snake body by defining the head first





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
            break;
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
    if (colission == TRUE)
    {
        game_is_running = FALSE;
    }


   // step 1 is to move the snake head.
    node flummy;
    node dummy;

    flummy = *snake_node;

    snake_node->snake_body.x = unit.x;
    snake_node->snake_body.y = unit.y;
    snake_node->snake_body.width = unit.width;
    snake_node->snake_body.height = unit.height;

    snake_node->direction = direction;

    
    node* body = snake_node->next;

    while (body != NULL) // handles the middle
    {
        dummy.snake_body.x = body->snake_body.x;
        dummy.snake_body.y = body->snake_body.y;
        dummy.direction = body->direction;
        body->snake_body.x = flummy.snake_body.x;
        body->snake_body.y = flummy.snake_body.y;
        body->direction = flummy.direction;
        flummy.snake_body.x = dummy.snake_body.x;
        flummy.snake_body.y = dummy.snake_body.y;
        flummy.direction = dummy.direction;
        body = body->next;

    }
    if (body != NULL && body->next == NULL) // handles the last cell
    {
        body->snake_body = flummy.snake_body;
        body->direction = flummy.direction;
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


    // Logic to keep a fixed timestamp
    {
        last_frame_time = SDL_GetTicks();
        // the below handles the motion of the main unit
        if (direction == 0)
        {
            unit.x += (SPEED * delta_time);
        }
        else if (direction == 1)
        {
            unit.x -= (SPEED * delta_time);
        }
        else if (direction == 2)
        {
            unit.y -= (SPEED * delta_time); // we subtract in up direction since pixels are drawn from top left to bottom right ie 3 in y is higher than 111 in y
        }
        else if (direction == 3)
        {
            unit.y += (SPEED * delta_time);
        }
    }


    // Map Wrap around
    {
        if (unit.x < origin_background_x)
            unit.x = WINDOW_WIDTH - (origin_background_x + snake_width);
        if (unit.x > WINDOW_WIDTH - origin_background_x - snake_width)
            unit.x = origin_background_x;
        if (unit.y < origin_background_y)
            unit.y = WINDOW_HEIGHT - (origin_background_y + snake_height);
        if (unit.y > WINDOW_HEIGHT - origin_background_y - snake_height)
            unit.y = origin_background_y;
    }



    //TODO:
            // swap out flummy_x for unit.x and flummy_y for unit.y to make food disappear instantly
    //TODO:


    // now we move the snake node body
       // // handles creating a new snake body
        if ((unit.x > food.x - food.width / 1.5 && unit.x < food.x + food.width / 1.5) && (unit.y > food.y - food.height / 1.5 && unit.y < food.y + food.height / 1.5))
        {
            food.x = (rand() % (int)(GAME_WINDOW_WIDTH / 2)) + origin_background_x;
            food.y = (rand() % (int)(GAME_WINDOW_HEIGHT / 2)) + origin_background_y;
            food_counter++;


            node* temp = malloc(sizeof(node));

            if (temp == NULL)
            {
                // need to actually add real error handling

                game_is_running = FALSE;
            }

            temp->next = snake_node;
            temp->snake_body = unit;
            temp->direction = direction;
            temp->snake_body.color = food.color;


            food.color.red = rand() % 255;
            food.color.green = rand() % 255;
            food.color.blue = rand() % 255;

            snake_node = temp;


        }
    //// handles collisions
        if (food_counter > 3)
        {
            node* m = snake_node->next->next;
            while (m != NULL)
            {
                if (snake_node->snake_body.x > m->snake_body.x - (float) SNAKE_SIZE / 10 && snake_node->snake_body.x < m->snake_body.x + (float) SNAKE_SIZE / 10 && snake_node->snake_body.y > m->snake_body.y - (float) SNAKE_SIZE / 10 && snake_node->snake_body.y < m->snake_body.y + (float) SNAKE_SIZE / 10)
                {
                    colission = TRUE;
                }
                m = m->next;
            }
        }

}


void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect background_rect =
    {
        (int)background.x,
        (int)background.y,
        (int)background.width,
        (int)background.height
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, (&background_rect));

    // below creates background for main play area
    SDL_Rect play_area_background_rect =
    {
        (int)play_area.x,
        (int)play_area.y,
        (int)play_area.width,
        (int)play_area.height
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, (&play_area_background_rect));

    // here we can start the drawing of our objects
    //render snake head first
    //SDL_Rect snake_body_rect =
    //{
    //    (int)snake_node->snake_body.x,
    //    (int)snake_node->snake_body.y,
    //    (int)snake_node->snake_body.width,
    //    (int)snake_node->snake_body.height
    //};
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(renderer, (&snake_body_rect));



    //SDL_Rect snake_body_rect =
    //{
    //    (int)snake_node->snake_body.x,
    //    (int)snake_node->snake_body.y,
    //    (int)snake_node->snake_body.width,
    //    (int)snake_node->snake_body.height
    //};
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(renderer, (&snake_body_rect));


    node* flow = snake_node;



    while (flow != NULL)
    {
        SDL_Rect snake_body_flow_rect =
        {
            (int)flow->snake_body.x,
            (int)flow->snake_body.y,
            (int)flow->snake_body.width,
            (int)flow->snake_body.height
        };
        SDL_SetRenderDrawColor(renderer, snake_node->snake_body.color.red, snake_node->snake_body.color.green, snake_node->snake_body.color.green, 255);
        SDL_RenderFillRect(renderer, (&snake_body_flow_rect));
        flow = flow->next;
    }

    // below creates the food 
    SDL_Rect food_rect =
    {
        (int)food.x,
        (int)food.y,
        (int)food.width,
        (int)food.height
    };
    SDL_SetRenderDrawColor(renderer, food.color.red, food.color.green, food.color.blue, 255);
    SDL_RenderFillRect(renderer, (&food_rect));


    // IMPORTANT
    //// render the rest of the snake

    //int k = 1;
    //while (snake_body[k].exists == TRUE)
    //{
    //    SDL_Rect snake_body_rect =
    //    {
    //        (int)snake_body[k].body_part.x,
    //        (int)snake_body[k].body_part.y,
    //        (int)snake_body[k].body_part.width,
    //        (int)snake_body[k].body_part.height
    //    };
    //    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //    SDL_RenderFillRect(renderer, (&snake_body_rect));
    //    k++;
    //}
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

    while (snake_node->next != NULL)
    {
    node* destroyer = snake_node;
    snake_node = snake_node->next;
    free(destroyer);
    }
    free(snake_node);



    return 0;
}