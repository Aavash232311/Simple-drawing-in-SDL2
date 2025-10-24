#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct
{
    int x;
    int y;
} Vector;

typedef struct
{
    int x1;
    int y1;
    int x2;
    int y2;
} LineSegmant;
// Bresenham's Circle Drawing Algorithm
void draw_circle(SDL_Renderer *ren, int xc, int yc, int r)
{
    int point_dim = 4;
    int x = 0;
    int y = r;

    int d = 3 - (2 * r); // formula
    SDL_Rect dot = {x + xc, r + yc, point_dim, point_dim};
    SDL_RenderFillRect(ren, &dot);
    int c = 0;

    Vector collision;

    while (x <= y)
    { // at the completion of one Octet both x and y value will be the same
        if (d < 0)
        {
            d += (4 * x) + 6;
        }
        else
        {
            d += (4 * (x - y)) + 10;
            y--; // okay based on the algorithm we did it
        }
        x++;
        // if found confisuing visualize in desmos, we are using symmetrical property of circle
        // figuring out point across all 8 cotects
        // original
        SDL_Rect curr_dot_orginal = {x + xc, y + yc, point_dim, point_dim};
        // reflected across Y
        SDL_Rect curr_dot_across_y_x = {xc + y, x + yc, point_dim, point_dim};
        // reflect across y-axis
        SDL_Rect curr_dot_across_y = {xc - x, yc + y, point_dim, point_dim};
        // reflect across y-axis then x = y
        SDL_Rect curr_dot_across_y_then_y_x = {xc - y, yc + x, point_dim, point_dim};
        // reflect across both axes
        SDL_Rect curr_dot_both_axes = {xc - x, yc - y, point_dim, point_dim};
        // reflect across both then x= y
        SDL_Rect curr_dot_both_then_x_y = {xc - y, yc - x, point_dim, point_dim};
        // reflect across x axis
        SDL_Rect curr_dot_across_x = {xc + x, yc - y, point_dim, point_dim};
        // reflect across x-axis and then x = y
        SDL_Rect curr_dot_across_x_x_y = {xc + y, yc - x, point_dim, point_dim};
        SDL_RenderFillRect(ren, &curr_dot_orginal);
        SDL_RenderFillRect(ren, &curr_dot_across_y_x);
        SDL_RenderFillRect(ren, &curr_dot_across_y);
        SDL_RenderFillRect(ren, &curr_dot_across_y_then_y_x);
        SDL_RenderFillRect(ren, &curr_dot_both_axes);
        SDL_RenderFillRect(ren, &curr_dot_both_then_x_y);
        SDL_RenderFillRect(ren, &curr_dot_across_x);
        SDL_RenderFillRect(ren, &curr_dot_across_x_x_y);
        c++;
    }

    collision.x = r + xc; // right and side we need to draw a hidden line to check for collision logic
    collision.y = yc;

    LineSegmant collision_line_1;
    collision_line_1.x1 = collision.x;
    collision_line_1.x2 = collision.y + r;
    collision_line_1.y1 = collision.x;
    collision_line_1.y2 = collision.y - r;

    LineSegmant collision_line_2;

    SDL_SetRenderDrawColor(ren, 252, 186, 3, 0);

    SDL_RenderDrawLine(ren, collision_line_1.x1, collision_line_1.x2, collision_line_1.y1, collision_line_1.y2); // collision border 1

    collision_line_2.x1 = (collision.x - 2*r);
    collision_line_2.y1 = collision.y + r;

    collision_line_2.x2 = (collision.x - 2*r);
    collision_line_2.y2 = collision.y - r;

    SDL_RenderDrawLine(ren, collision_line_2.x1, collision_line_2.y1, collision_line_2.x2, collision_line_2.y2);
    /*
        Note: Circle's are symmetrcial

    /*
        Derived from the question of circle at origin 0 = x^2 + y^2 - r^2
       f(x,y) = x^2 + y^2 - r^2 -- eq (i)
       f(x,y) < 0 point is inside the circle
       f(x,y) = 0 point is exactly on the curve
       f(x,y) > 0 then the point is outiside of the curve

       initial decision formula d0 = 3 - 2R

       if f(x, y) < 0 then next coordinate of (x + 1, y); d1 = d0 + 4x + 6
       if f(x, y) > 0 then (x + 1, y - 1) d2 = d1 + 4(x-y) + 10

       continue till x = y, it's for first quarter we can find of all quarters.

    */
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    { // initilizes library
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // creates a window, the other ags 500, 100 is position of window on screen.
    // 400x400 is height and width of window in pixel
    int width_frame = 800;
    int height_frame = 400;

    SDL_Window *win = SDL_CreateWindow("Game.c", 500, 200, width_frame, height_frame, SDL_WINDOW_SHOWN);
    if (!win)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    // SDL_RENDERER_ACCELERATED: use GPU for fast drawing, SDL_RENDERER_PRESENTVSYNC Sync rendering to the monitor refresh rate
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren)
    {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int quit = 0;

    Uint32 frame_start;
    Uint32 frame_time;
    float current_fps = 0.0f;
    int height_of_board = 180;
    int width_of_board = 20;

    float board_2_y = 0.0f;

    int init_circle_x = 400;
    int init_circle_y = 200;
    int r = 15;

    const float travel_speed = 1.8f;

    while (!quit)
    {
        frame_start = SDL_GetTicks(); // this starts timing the frame

        // set's background color
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // set's color to the red square
        SDL_SetRenderDrawColor(ren, 102, 0, 102, 0);

        SDL_Rect board_1 = {0, 0, width_of_board, height_of_board}; // x, y, width, height
        SDL_RenderFillRect(ren, &board_1);                          // draw filled square

        /* In our game we wan't the board 1 to be fixed, and board 2 to move,
        that casting to (int) is very important here */

        SDL_Rect board_2 = {width_frame - width_of_board, (int)board_2_y, width_of_board, height_of_board};
        SDL_RenderFillRect(ren, &board_2);

        draw_circle(ren, init_circle_x, init_circle_y, r);

        // Here we have made the circle to increment in x-axis by 1.8 and once it reaches the bounce point we need to define it go bounce back ofc
        // init_circle_x += travel_speed;

        SDL_RenderPresent(ren);

        frame_time = SDL_GetTicks() - frame_start; // time taken for same, i.e to render stuff above

        if (frame_time > 0)
        {
            current_fps = 1000.0f / frame_time;
        }
        else
        {
            current_fps = 9999.0f; // handeling division by 0
        }

        int inc_limit = height_frame - height_of_board;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            { // User clicked the close button
                quit = 1;
            }
            // Here we can listen to the key press event
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (board_2_y >= 0)
                    {
                        board_2_y -= 15.0f;
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (!(board_2_y >= inc_limit))
                    {
                        board_2_y += 15.0f;
                    }
                }
            }
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

/*
    F(C) = x^2 + Y^2 - r^2 ---- i)

    initial Point P(0, r)

    for function of circle at N
    F(N) = (x k+1)^2 + (yk)^2 - r^2 positive
    F(S) = (x k+1)^2 + (y k+1)^2 - r^2 negative inside the circle

    for decistion paramter of the dircle we can say that
    Dk = F(N) + F(S) ---- ii) it's the net distance from the curver, if the net distance is closer to 0 then what we can have is point being on the arc

    DK = 2(x k+1)^2 + (yk-1)^2 + (yk)^2 - 2r^2

    We know we need to go to the next column, x + 1, but we need to decide weather or not we need to change y or not so why is unknown

    D (k+1) = 2(xk + 2)^2 + (y k+1)^2 + (y (k + 1) - 1)^2 - 2r^2 basically we don't know the entire y thing

    By subracting two eqn we can eliminate constanct like r^2 because floating
    point calculation get's heavy
    D (k+1) = DK + 4xk + 2(y k + 1)^2 - 2yk+1 - 2(yk)^2 -2yk + 6 nvm if it get gross here basically we have the idea

    if DK < 0; put P(x k+1, yk) because it's inside the arc
    Dk+1 = DK + 4xk + 6

    if DK > 0; outside the arc, put P(x k + 1, y k -1)
    D k +1 = DK + 4(xk-yk)  + 10

    for initial decistion in DK, put P(0, r)
    D0 = 3-2r
*/