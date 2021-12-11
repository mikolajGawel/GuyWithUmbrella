#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
#define SECOND 40
//default textures
#define WARNING_TEXTURE "rsc/warning.png"
#define ROCKET_TEXTURE_1 "rsc/rocket_1.png"
#define ROCKET_TEXTURE_2 "rsc/rocket_2.png"
enum game_statuses
{
    IN_GAME,
    GAME_OVER,
    IN_MENU
};
int game_state = IN_GAME;
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
    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {p->x -= 8 ;}
    if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {p->x += 8;}
}
void check_player(struct player* player)
{
    int xr = player->x%4;//rounded x
    if(xr != 0)player->x -= xr;

    int yr = player->y%4;//rounded y
    if(yr != 0)player->y -= yr; 

    if(player->x < 64 || player->x+player->w > (WIDTH-64))
    {
        game_state = GAME_OVER;
    }

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
struct rocket
{
    int x,y;
    int w,h;
    Texture t;
};
void init_rocket(struct rocket* rocket,int w,int h,int y)
{
    rocket->x =(rand()%(WIDTH-64*2))+64;
    rocket->y = HEIGHT+y;
    Image i = LoadImage(ROCKET_TEXTURE_1);
    rocket->t = LoadTextureFromImage(i);
    rocket->t.width = w;
    rocket->t.height = h;
    rocket->w = w;
    rocket->h = h;
    UnloadImage(i);
}
void new_rocket(struct rocket* rocket)
{
    rocket->x = (rand()%(WIDTH-64*2-rocket->w))+64;
    rocket->y = HEIGHT;
}
void update_rocket(struct rocket* rocket)
{
    if(rocket->y + rocket->h< 0)
        new_rocket(rocket);
    rocket->y-=8;
}
Sound ex[2];//explosion_sounds
void check_rocket_collision(struct rocket* rocket,struct player* player)
{
    if(CheckCollisionRecs((Rectangle){rocket->x,rocket->y,rocket->w,rocket->h},(Rectangle){player->x+3*4,player->y+4*3,player->w-3*4,player->h-3*4}))
    {
        game_state = GAME_OVER;
        PlaySound(ex[rand()%2]);
        //printf(TextFormat("Player Was Killed By Rocket Number:%i",rocket->num));
    }
}
void draw_rocket(struct rocket* rocket)
{
    DrawTexture(rocket->t,rocket->x,rocket->y,C_WHITE);
}
int temp  = 0;
int rocketFrame = 0;
int main()
{
    
    srand(time(NULL));
    InitWindow(WIDTH,HEIGHT,"Guy with Umbrella");
    InitAudioDevice();
    SetTargetFPS(40);
    Sound start_sound = LoadSound("start.wav");
    ex[0]= LoadSound("rsc/explosion_1.wav");
    ex[1]= LoadSound("rsc/explosion_2.wav");
    struct player player;
    init_player(&player,(512-11*4)/2,32*4,11*4,18*4,"rsc/guy.png");
    struct block left[10];
    init_blocks_row(left,0,0,16*4,16*4,"rsc/rock.png");
    struct block right[10];
    init_blocks_row(right,WIDTH-4*16,0,4*16,4*16,"rsc/rock.png");
    struct rocket rockets[10];
    for(int i = 0;i < 10;i++)
    {
        init_rocket(&rockets[i],8*4,20*4,i*128);
    }
    Image i = LoadImage("rsc/game_over.png");
    Texture Game_Over_Texture = LoadTextureFromImage(i);
    Game_Over_Texture.width = 42*4;
    Game_Over_Texture.height = 8 *4;
    UnloadImage(i);
    
    i = LoadImage("rsc/title.png");
    Texture title = LoadTextureFromImage(i);
    title.width *= 4;
    title.height *= 4;
    UnloadImage(i);
    i = LoadImage("rsc/title_info.png");
    Texture title_info = LoadTextureFromImage(i);
    title_info.width *= 4;
    title_info.height *= 4;
    UnloadImage(i);
    Texture rocket_textures[2];
    i = LoadImage(ROCKET_TEXTURE_1);
    rocket_textures[0] = LoadTextureFromImage(i);
    rocket_textures[0].width *=4;
    rocket_textures[0].height *=4;
    UnloadImage(i);
    i = LoadImage(ROCKET_TEXTURE_2);
    rocket_textures[1] = LoadTextureFromImage(i);
    rocket_textures[1].width *=4;
    rocket_textures[1].height *=4;
    UnloadImage(i);
    game_state = IN_MENU;
    int is_start = 0;
    while(!WindowShouldClose())
    {
        if(game_state == IN_GAME){
            if(is_start == 0)
            {
                PlaySound(start_sound);
                is_start = 1; 
            }
            if(rocketFrame < 20)
            {
                for(int i = 0;i < 10;i++)
                {
                    rockets[i].t = rocket_textures[0];
                }
            }else if(rocketFrame > 20)
            {
                for(int i = 0;i < 10;i++)
                {
                    rockets[i].t = rocket_textures[1];
                }
            }
            if(rocketFrame > 40)rocketFrame = 0;
            rocketFrame++;
            player_input(&player);
            check_player(&player);
            update_blocks_row(left);
            update_blocks_row(right);
            
            for(int i = 0;i < 10;i++)
            {
                update_rocket(&rockets[i]);
            }
            for(int i = 0;i < 10;i++)
            {
                check_rocket_collision(&rockets[i],&player);

            }

            BeginDrawing();
                ClearBackground(C_CYAN);

                draw_blocks_row(left);
                draw_blocks_row(right);
                for(int i = 0;i < 10;i++)
                {
                    draw_rocket(&rockets[i]);
                }
                DrawTexture(player.t,player.x,player.y,C_WHITE);

            EndDrawing();
            WaitTime(1);
        }else if(game_state == GAME_OVER)
        {
            BeginDrawing();
                ClearBackground(C_BLACK);
                DrawTexture(Game_Over_Texture,32*4,((HEIGHT-Game_Over_Texture.height)/2) - (HEIGHT-Game_Over_Texture.height/2)%4,C_WHITE);
            EndDrawing();
            temp++;
            if(temp == SECOND*2)
            {
                temp = 0;
                game_state = IN_MENU;
                  init_player(&player,(512-11*4)/2,32*4,11*4,18*4,"rsc/guy.png");
            init_blocks_row(left,0,0,16*4,16*4,"rsc/rock.png");
            init_blocks_row(right,WIDTH-4*16,0,4*16,4*16,"rsc/rock.png");
            }
          
        }else if(game_state == IN_MENU)
        {
            BeginDrawing();
                ClearBackground(C_CYAN);
            update_blocks_row(left);

            update_blocks_row(right);
            draw_blocks_row(left);
            draw_blocks_row(right);
            DrawTexture(title,0,0,C_WHITE);
            DrawTexture(title_info,0,HEIGHT-32*4,C_WHITE);
            DrawTexture(player.t,player.x,player.y,C_WHITE);
             if(IsKeyDown(KEY_SPACE))
             {

                init_player(&player,(512-11*4)/2,32*4,11*4,18*4,"rsc/guy.png");
                init_blocks_row(left,0,0,16*4,16*4,"rsc/rock.png");
                init_blocks_row(right,WIDTH-4*16,0,4*16,4*16,"rsc/rock.png");
                for(int i = 0;i < 10;i++)
                {
                    init_rocket(&rockets[i],8*4,20*4,i*128);
                }
                is_start = 0;
                game_state = IN_GAME;
            }
            EndDrawing();
        }
    }
    for(int i = 0;i< 10;i++)
    {
        UnloadTexture(rockets[i].t);
    }
    UnloadSound(start_sound);
    UnloadTexture(rocket_textures[0]);
    UnloadTexture(rocket_textures[1]);
    UnloadTexture(title_info);
    UnloadTexture(title);
    UnloadTexture(player.t);
    destroy_blocks_row(left);
    destroy_blocks_row(right);
    UnloadTexture(Game_Over_Texture);
    UnloadSound(ex[0]);
    UnloadSound(ex[1]);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}