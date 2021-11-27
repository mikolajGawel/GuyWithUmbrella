#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
//color pallete
#define C_BLACK (Color){0,0,0,255}
#define C_WHITE (Color){255,255,255,255}
#define C_BROWN (Color){161, 104, 60,255}
#define C_CYAN (Color){106, 190, 198,255}
#define C_DARK_GRAY (Color){98, 98, 98,255}
#define C_GRAY (Color){137, 137, 137,255}
#define C_LIGHT_GRAY (Color){173, 173, 173,255}
#define C_RED (Color){159, 79, 68,255}
#define C_PINK (Color){203, 126, 117,255}
#define C_DARK_BROWN (Color){109, 85, 18,255}
#define C_YELLOW (Color){200, 212, 135,255}
#define C_LIGHT_GREEN (Color){154, 226, 155 ,255}
#define C_GREEN (Color){92, 171, 95,255}
#define C_LIGHT_PURPLE (Color){136, 126, 203,255}
#define C_BLUE (Color){80, 69, 155,255}
#define C_PURPLE (Color){160, 87, 163,255}
//screen size
#define WIDTH 512
#define HEIGHT 512
//time 
#define SECOND 50
enum game_statuses
{
    IN_GAME,
    GAME_OVER,
    IN_MENU
};
struct player
{
    int x,y;
    int w,h;
    Texture t;
};
struct block
{
    int x,y;
    int w,h;
    Texture t;
};

void init_player(struct player* player,int x,int y,int w,int h,const char* texture_path)
{
    player->x = x;
    player->y = y;
    player->w = w;
    player->h = h;
    Image i = LoadImage(texture_path);
    player->t = LoadTextureFromImage(i);
    UnloadImage(i);
    player->t.width = w;
    player->t.height = h;
}
void player_input(struct player* p)
{
    if(IsKeyDown(KEY_LEFT))
    {p->x -= 4;}
    if(IsKeyDown(KEY_RIGHT))
    {p->x += 4;}
}
void check_player(struct player* player)
{
    int xr = player->x%4;//rounded x
    if(xr != 0)player->x -= xr;

    int yr = player->y%4;//rounded y
    if(yr != 0)player->y -= yr; 


}
void init_block(struct block* block,int x,int y,int w ,int h,const char* texture_path)
{
    block->x = x;
    block->y = y;
    block->w = w;
    block->h = h;
    Image i = LoadImage(texture_path);
    block->t = LoadTextureFromImage(i);
    UnloadImage(i);
    block->t.width = w;
    block->t.height = h;
}
void init_blocks_row(struct block blocks[10],int x,int y,int w,int h,const char* texture_path)
{
    for(int i = 0;i < 10;i++)
    {
        init_block(&blocks[i],x,i*h,w,h,texture_path);
    }
}
void draw_blocks_row(struct block blocks[10])
{
    for(int i=0;i < 10;i++)
    {
        DrawTexture(blocks[i].t,blocks[i].x,blocks[i].y,C_WHITE);
    }
}
void update_blocks_row(struct block blocks[10])
{
    for(int i = 0;i < 10;i++)
    {
        blocks[i].y-=4;
        if(blocks[i].y <-blocks[i].h)blocks[i].y = HEIGHT+blocks[i].h-1;
    }
}
void destroy_blocks_row(struct block blocks[10])
{
    for(int i = 0;i<10;i++)
    {
        UnloadTexture(blocks[i].t);
    }
}
struct warning
{
    Texture t;
    int x,y;
    int w,h;
};
int game_state = IN_GAME;
int main()
{
    
    InitWindow(WIDTH,HEIGHT,"Guy with Umbrella");
    SetTargetFPS(50);
    
    struct player player;
    init_player(&player,(512-11*4)/2,32*4,11*4,18*4,"rsc/guy.png");
    struct block left[10];
    init_blocks_row(left,0,0,16*4,16*4,"rsc/rock.png");
    struct block right[10];
    init_blocks_row(right,WIDTH-4*16,0,4*16,4*16,"rsc/rock.png");

    while(!WindowShouldClose())
    {
        if(game_state == IN_GAME){
            player_input(&player);
            check_player(&player);
            update_blocks_row(left);
            update_blocks_row(right);
            BeginDrawing();
                ClearBackground(C_CYAN);

                draw_blocks_row(left);
                draw_blocks_row(right);
                DrawTexture(player.t,player.x,player.y,C_WHITE);

            EndDrawing();
            WaitTime(1);
        }else if(game_state == GAME_OVER)
        {
            
        }
    }
    UnloadTexture(player.t);
    destroy_blocks_row(left);
    destroy_blocks_row(right);
  
    CloseWindow();
    return 0;
}