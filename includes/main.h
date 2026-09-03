#pragma once
#include <raylib.h>
#include <math.h>
#include <raymath.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <bit>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define MAP_WIDTH 50
#define MAP_HEIGHT 50

enum Game_state {
    LOADING,
    PLAYING,
    PAUSED,
    GAME_OVER,
    PLACE_ROOM,
    CRAFT_MENU,
    PLACE_STAFF,
};

enum Tiletypes {
    FLOOR,
    GRASS,
    TREE,
    WALL,
    DOOR,
    ROCK,
    STUMP, // chopped-down tree
    MINE_TILE,  // damages enemies on collision
    STAFF_TILE, // attacks nearby enemies
};

enum Enemytypes {
    SKELEMAN,
    VAMPIRE,
    ZOMBIE,
    MOTHMAN,
};

enum Tools {
    BASIC_ATTACK,
    AXE,
    MINE,
    GUARD_STAFF,
};

typedef struct Animated_sprite {
    float x;
    float y;
    int frames;
    int current_frame;
    int width;
    int height;
    float fps;
    float time_remaining;
    Texture2D* texture;
    bool play_once;

    Rectangle make_frame() {
        float frame_width =  (float)(this->current_frame % this->texture->width) * this->width;
        float frame_height = (float)(this->current_frame / this->texture->height) * this->height;

        return Rectangle{frame_width, frame_height, (float)this->width, (float)this->height};
    }

    Rectangle get_hitbox() {
        return Rectangle {this->x     + this->width/10.f,        this->y      + this->height/10.f, 
                   (float)this->width - this->width/10.f, (float)this->height - this->height/10.f};
    }

    void update() {
        this->time_remaining -= GetFrameTime();
        if (this->time_remaining <= 0.f) {
            //std::cout << "frame switching";
            this->time_remaining = 1.f/this->fps;
            //std::cout << "new time remaining: " << this->time_remaining << "\n";
            this->current_frame = (this->current_frame+1)%this->frames;
        }
    }

    // don't use getter and setter to centre the position
    Vector2 get_pos() {
        return Vector2 {this->x,this->y};
        //return Vector2 {this->x + (this->width/2), this->y + (this->height/2)};
    }

    void set_pos(Vector2 pos) {
        this->x = pos.x;// - (this->width/2);
        this->y = pos.y;// - (this->height/2);
    }
} Animated_sprite; // TODO sort memory leak when sprites unload- resource mgmt

typedef struct Projectile {
    Vector2 direction;
    int damage;
    int speed;
    Animated_sprite sprite;
    bool dead = {0};

    void update() {
        this->sprite.update();
        this->sprite.x += direction.x * speed * GetFrameTime();
        this->sprite.y += direction.y * speed * GetFrameTime();
    }

    Rectangle get_hitbox() {
        return Rectangle {this->sprite.x, this->sprite.y, (float)this->sprite.width, (float)this->sprite.height};
    }
} Projectile;

typedef struct Enemy {
    int hp;
    int damage;
    int type;
    float atk_cooldown;
    float atk_cooldown_max;
    Animated_sprite sprite;
} Enemy;

typedef struct Guard_staff {
    Vector2 pos;
    float cooldown;
} Guard_staff;

//Color floor_basic = {r,g,b,a};
Color menu_transl = {160, 155, 140, 100};

int game_state = Game_state(LOADING);
int player_facing = 2; // NORTH 0 EAST 1 SOUTH 2 WEST 3
Texture2D player_tex;
Animated_sprite player;
float player_attack_cooldown = {0.f};
float atk_cooldown_max = {1.f};
int atk_damage = {2};
double wave_end_time = {0.};
int wave = {0};

int current_enemy_type;

Camera2D camera = { 0 };
int player_hp;

bool blocked_left;
bool blocked_right;
bool blocked_up;
bool blocked_down;

int** dijkstra_map;
std::vector<Vector2> walls;
int tile_map[MAP_WIDTH][MAP_HEIGHT] = {0};
Texture2D* tile_display[MAP_WIDTH][MAP_HEIGHT];

Rectangle health_bar { 20, 20, 300, 50 };

Vector2 mousetile;
Vector2 nearest_wall_to_mouse;
std::vector<Vector2> selected_walls;
int x_offset = 0; int y_offset = 0;
int player_x_offset = 0; int player_y_offset = 0;
bool paused_this_turn = 1;
int num_tools = {4};
int tool_equipped = {0};
double last_grumble_time;

int wood = {0};
int rock = {0};
int mana = {0};
//int iron;
int mines = {0};
int guard_staffs = {0};
float global_staff_cooldown_max = {1.f};

std::vector<Enemy> enemies;

std::vector<Projectile> projectiles;

Sound crafting;
Sound wood_chops[3];
Sound hit_noise;
Sound fireball_fire;
Sound fireball_impact;
Sound mine_explode;
Sound wizard_grunts[10];
Sound wizard_hit[3];
Sound zombie_grunts[4];
Sound vampire_grunts[9];

Image wizard_img;
//Image warrior_img = LoadImage("assets/warrior.png");
//Image ranger_img = LoadImage("assets/ranger.png");

Image skeleman_img;
Image vampire_img;
Image zombie_img;
Image mothman_img;
Texture2D skeleman_tex;
Texture2D vampire_tex;
Texture2D zombie_tex;
Texture2D mothman_tex;

Image fireball_img;
Texture2D fireball_tex;
Image axe_img;
Image fireball_hud_img;
Texture2D fireball_hud_sprite;
Texture2D axe_hud_sprite;
Texture2D mine_hud_sprite;
Texture2D staff_tower_hud_sprite;

Texture2D dirt_tiles[1];
Texture2D grass_tiles[2];
Texture2D tree_tiles[1];
Texture2D stump_tile;
Texture2D wall_tile;
Texture2D door_tile;
Texture2D rock_tile;
Texture2D null_tile;
Texture2D mine_tile;
Texture2D staff_tower_tile;

bool staff_upgraded = {0};
Rectangle staff_upgrade_button_bounds = {180, 300, 200, 400};
Texture2D staff_upgrade_button_up;
Texture2D staff_upgrade_button_down;
Texture2D staff_upgrade_button_hover;
Texture2D staff_upgrade_button_disabled;
Texture2D* current_staff_upgrade_button_state;

Rectangle mine_button_bounds = {400, 300, 200, 400};
Texture2D mine_button_up;
Texture2D mine_button_down;
Texture2D mine_button_hover;
Texture2D* current_mine_button_state;

std::vector<Vector2> active_mines;

Rectangle staff_tower_button_bounds = {620, 300, 200, 400};
Texture2D staff_tower_button_up;
Texture2D staff_tower_button_down;
Texture2D staff_tower_button_hover;
Texture2D* current_staff_tower_button_state;

std::vector<Guard_staff> guardian_staffs;

void ProcessAudio(void *buffer, unsigned int frames);
void load_textures();
void load_sounds();
static Vector2 get_tile_at_mousepos(Vector2 pos, Camera2D cam, int tile_map[MAP_WIDTH][MAP_HEIGHT]);
static std::vector<Vector2> get_neighbouring_tiles(Vector2 pos, int tile_map[MAP_WIDTH][MAP_HEIGHT]);
static int** generate_dijkstra_map(std::vector<Vector2> current_tiles, int tile_map[MAP_WIDTH][MAP_HEIGHT]);
static Vector2 find_nearest_wall(Vector2 tile, int** dijkstra_map);
static std::vector<Vector2> find_neighbouring_walls(Vector2 tile, int** dijkstra_map, int tile_map[MAP_WIDTH][MAP_HEIGHT], int max_length);
Texture2D* get_tile_sprite(int idx);
void select_walls_from_mouse(Camera2D cam);
void create_room(int min_size, int max_size, std::vector<Vector2> selected_tiles);
void generate_level();
Vector2 worldpos_to_tilespace(Vector2 world_pos);
int get_tile_type_at_worldpos(Vector2 world_pos);
void damage_player(int dmg);
void spawn_enemy_wave(int num_enemies, int hp);
