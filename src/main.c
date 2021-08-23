// Main Class 

#include <jo/jo.h>
#include "alex.h"
#include "enemy.h"
#include "pickup.h"
#include "title.h"
#include "pause.h"
#include "block.h"

//**********************************************************************************************
//** Variables **

jo_sidescroller_physics_params physics;
static int map_pos_x = WORLD_DEFAULT_X;
static int map_pos_y = WORLD_DEFAULT_Y;

// Game state manager
enum gameStateManger {GSM_MENU, GSM_PLAYING, GSM_PAUSE};
enum gameStateManger gameState;

static jo_sound jump;

int punchTimer = 0;
int pauseTimer = 0;

//int font_sprite_id = -1;
//**********************************************************************************************


// Create an enemy
void create_enemy(int number, int x, int y, int distance){
	enemies[number].x = x;
	enemies[number].y = y;
	enemies[number].start_x = enemies[number].x;
	enemies[number].distance = distance;
    enemies[number].direction = 1;
	enemies[number].alive = true;
	enemies[number].recently_died = false;
}

// Create the array of enemies
void create_enemies(void){
	create_enemy(0, 75, 365, 220);
	create_enemy(1, 185, 462, 55);
	create_enemy(2, 120, 585, 25);
	create_enemy(3, 75, 655, 215);
	create_enemy(4, 120, 800, 70);
	create_enemy(5, 55, 862, 265);
	create_enemy(6, 105, 945, 215);
	create_enemy(7, 85, 1085, 185);
	create_enemy(8, 55, 1252, 255);
	create_enemy(9, 55, 1380, 50);
	create_enemy(10, 55, 1550, 50);
	create_enemy(11, 85, 1590, 210);
}





// Create a pickups
void create_pickup(int number, int type, bool visible, int x, int y){
	pickups[number].x = x;
	pickups[number].y = y;
	pickups[number].visible = visible;
	pickups[number].collected = false;
	pickups[number].type = type;
}

// Create the array of pickups
void create_pickups(void){
	create_pickup(0, 1, false, 275, 131);
	create_pickup(1, 0, false, 275, 243);
	create_pickup(2, 1, false, 37, 196);
	create_pickup(3, 2, false, 37, 212);
}





// Create a block
void create_block(int number, int block_type, bool drop_item, int drop_item_number, int x, int y){
	blocks[number].x = x;
	blocks[number].y = y;
	blocks[number].type = block_type;
	blocks[number].broken = false;
	blocks[number].broken_spread_distance = 0;
	blocks[number].drop_item = drop_item;
	blocks[number].drop_item_number = drop_item_number;
}


// Create the array of blocks
void create_blocks(void){
	create_block( 0, 0, true,  0, 276, 131);
	create_block( 1, 4, false, 0, 258, 243);
	create_block( 2, 1, true,  1, 275, 243);
	
	create_block( 3, 0, true,  2,  37, 196);
	create_block( 4, 0, true,  3,  37, 212);
	create_block( 5, 4, false, 0,  37, 228);
	
	create_block( 6, 4, false, 0, 165, 275);
	create_block( 7, 4, false, 0, 165, 291);
	create_block( 8, 4, false, 0, 148, 388);
	create_block( 9, 4, false, 0, 148, 404);
	create_block(10, 0, false, 0, 276, 388);
	create_block(11, 0, false, 0, 276, 404);
	create_block(12, 4, false, 0, 244, 404);
	create_block(13, 4, false, 0, 260, 420);
	create_block(14, 4, false, 0, 276, 436);
	create_block(15, 0, false, 0, 212, 451);
	create_block(16, 0, false, 0,   5, 404);
	create_block(17, 4, false, 0,   5, 420);
	create_block(18, 4, false, 0,  21, 420);
	create_block(19, 1, false, 0, 196, 563);
	
	create_block(20, 4, false, 0, 116, 547);
	create_block(21, 4, false, 0, 116, 563);
	create_block(22, 4, false, 0, 100, 547);
	create_block(23, 0, false, 0, 116, 579);
	create_block(24, 0, false, 0, 132, 579);
	create_block(25, 0, false, 0, 116, 595);
}





// Reset the game
void reset_demo(void){

	// Load the audio files
	//jo_audio_load_pcm("PICKUP.PCM", JoSoundMono16Bit, &jump);

	// Set the map position
    map_pos_x = WORLD_DEFAULT_X;
    map_pos_y = WORLD_DEFAULT_Y;
	
	// Set the player values
	physics.jump_speed_y = -5.0f;
	physics.gravity = 0.22f;
	physics.max_speed_y = 10.0f;
	
	player.lives = 3;
	player.score = 0;
	player.money = 0;
	player.alive = true;
    player.x = 50;
    player.y = 150;
    player.flip_alex = false;
    player.can_jump = true;
	player.is_ducking = false;
	player.is_punching = false;
	player.in_water = false;
	
	
	
	// Load the title screen
	gameState = GSM_PLAYING;
	load_title();
	
	// Load the map
	jo_map_free(WORLD_MAP_ID);
	load_map();
	
	// Create the enemies
	create_enemies();
	
	
	
	create_pickups();
	
	create_blocks();
	
	
	
	
	// Load the font sprite
	//font_sprite_id = jo_font_load("UI","FONT.TGA", JO_COLOR_Blue, 8, 8, 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!?c");	

}



// Respawn the player
void make_alex_respawn(void){
	player.alive = true;
    player.x = player.dead_x;
    player.y = player.dead_y;
	player.invincible = true;
	player.invincible_start_time = jo_get_ticks();
    player.flip_alex = false;
    player.can_jump = true;
	player.is_ducking = false;
	player.is_punching = false;
	player.in_water = false;
	physics.speed = 0.0f;
	physics.speed_y < 0.0f;
}


// Load the map
void load_map(void){
	jo_sprite_add_image_pack("WORLD", "WORLD.TEX", JO_COLOR_Blue);
	jo_map_load_from_file(WORLD_MAP_ID, 500, "MAP", "LEVEL1.MAP");
	jo_map_load_from_file(WORLD_BACKGROUND_ID, 501, "MAP", "CLOUD.MAP");
}


// Camera handler
inline void camera_handling(int prev_y){
    int delta;
    delta = JO_ABS(player.y - prev_y);
	
	if (map_pos_y < 1680){
		// Camera follow player down screen
		if (player.y > 150){
			map_pos_y += delta;
			player.y -= delta;
		}
	}
}


// Make the player jump
inline void make_alex_jump(void){
    player.can_jump = false;
    jo_physics_jump(&physics);
	//jo_audio_play_sound_on_channel(&jump, 0);
}




// Make the player punch
void make_alex_punch(void){
    player.is_punching = true;
	punchTimer = jo_get_ticks();

	// Check for collisions between the player's fist and the blocks
	int next_pixel = jo_physics_is_going_on_the_right(&physics) ? player.x + 2 : jo_physics_is_going_on_the_left(&physics) ? player.x - 2 : player.x;			 
	int i;
	int block_distance ;
	for (i = 0; i < BLOCK_NUMBER; i++) {
		
		if (!player.flip_alex){block_distance = jo_hitbox_detection_custom_boundaries (get_alex_sprite_id(), map_pos_x + next_pixel + ALEX_PUNCH_DISTANCE, player.y + map_pos_y, blocks[i].x + map_pos_x, blocks[i].y, 16, 16);}
		else {block_distance = jo_hitbox_detection_custom_boundaries (get_alex_sprite_id(), map_pos_x + next_pixel - ALEX_PUNCH_DISTANCE, player.y + map_pos_y, blocks[i].x + map_pos_x, blocks[i].y, 16, 16);}
		
		if (block_distance == 1){blocks[i].broken = true;}
	}
}




// This handles the pickup items
void handle_pickup_item(int item_number){
   
   pickups[item_number].collected = true;
   
   switch(pickups[item_number].type) {
		case 0 :
			break;
		case 1 :
			player.money += 100;
			break;
		case 2 :
			player.money += 50;
			break;
		case 3 :
			player.lives += 1;
			break;
		case 4 :

			break;
   }
							
}






//**********************************************************************************************
//** Collision Detection **

// Detect vertical collisions between the player and the map
inline bool has_vertical_collision(void){
	
    player.can_jump = false;
	
	// If the player is currently in the air
    if (physics.speed_y < 0.0f){
        physics.is_in_air = true;
		
		// Check for vertical collisions whilst in the air
		if (player.flip_alex) {
			if (jo_map_hitbox_detection_custom_boundaries(WORLD_MAP_ID, map_pos_x + player.x +8, map_pos_y + player.y, ALEX_WIDTH, ALEX_HEIGHT) != JO_MAP_NO_COLLISION){
				return true;
			}
		}
		else {
			if (jo_map_hitbox_detection_custom_boundaries(WORLD_MAP_ID, map_pos_x + player.x -8, map_pos_y + player.y, ALEX_WIDTH, ALEX_HEIGHT) != JO_MAP_NO_COLLISION){
				return true;
			}
		}
		
		return false;
    }

	int ground_distance = jo_map_per_pixel_vertical_collision(WORLD_MAP_ID, map_pos_x + player.x + ALEX_WIDTH, map_pos_y + player.y + ALEX_HEIGHT + 1, 1);
	if (ground_distance == JO_MAP_NO_COLLISION || ground_distance > 0){
        if (ground_distance != JO_MAP_NO_COLLISION && ground_distance < ALEX_JUMP_PER_PIXEL_TOLERANCE){
			player.can_jump = true;
		}
            
        physics.is_in_air = true;
        return false;
   }
	
	// If the player is on the ground
    if (ground_distance < 0 && jo_is_float_equals_zero(physics.speed_y)){
		player.y += ground_distance;
	}
        
    player.can_jump = true;
    physics.is_in_air = false;
	return true;
}







// Check for vertical collisions between the player and the blocks
bool has_vertical_collision_with_block(void){
	
	int i;
	for (i = 0; i < BLOCK_NUMBER; i++) {
		int block_distance = jo_hitbox_detection_custom_boundaries (get_alex_sprite_id(), map_pos_x + player.x, player.y - 4 + map_pos_y, blocks[i].x + map_pos_x, blocks[i].y, 16, 16);
	
		if (block_distance == 1){
			player.can_jump = true;
			physics.is_in_air = false;
			return true;
		}
	}
	
	return false;
}











// Detect horizontal collisions
inline bool has_horizontal_collision(void){
	
	// Block player on the right side of the screen
	if (map_pos_y < 1550){
		if (player.x >= 301){
			player.x = 300;
			return true;
		}
	}
	
	// Block player on the left side of the screen
	if (player.x <= -3){
		player.x = -2;
		return true;
	}
	
	// Check for horizontal collisions between the player and the blocks
	int next_pixel;
	if (!has_vertical_collision_with_block()){
		next_pixel = jo_physics_is_going_on_the_right(&physics) ? player.x + 2 : jo_physics_is_going_on_the_left(&physics) ? player.x - 2 : player.x;
					 
		int i;
		for (i = 0; i < BLOCK_NUMBER; i++) {
			int block_distance = jo_hitbox_detection_custom_boundaries (get_alex_sprite_id(), map_pos_x + next_pixel, player.y + map_pos_y, blocks[i].x + map_pos_x, blocks[i].y, 16, 16);
			if (block_distance == 1){
				return true;
			}
		}
	}
	
	// Check for horizontal collisions between the player and the map
    next_pixel = jo_physics_is_going_on_the_right(&physics) ? player.x + 8 : jo_physics_is_going_on_the_left(&physics) ? player.x - 8 : player.x;
			 
    int side_distance = jo_map_hitbox_detection_custom_boundaries(WORLD_MAP_ID, map_pos_x + next_pixel + ALEX_WIDTH_HALF, map_pos_y + player.y, 4, 20);
    if (side_distance == JO_MAP_NO_COLLISION){return false;}
    if (side_distance == 1){return true;}
        
    return true;
}





// Collision handler
inline void alex_collision_handling(void){
	
	// Vertical collision
    if (has_vertical_collision()){
		physics.speed_y = 0.0f;
	}  
    else {
        jo_physics_apply_gravity(&physics);
        player.y += physics.speed_y;
    }
	
	
	
	if (has_vertical_collision_with_block()){
		physics.speed_y = 0.0f;
	}
	
	
	
	// Horizontal collision
    if (has_horizontal_collision()){
		physics.speed = 0.0f;
	}  
    else if (physics.speed > 0.0f){

		// If above the water area, scrolling moves the player sprite. Once in the water area, scrolling moves the map
		//if (map_pos_y < 1600){
			player.x += physics.speed < 1.0f ? 1.0f : physics.speed;
		//}
		//else {
			//map_pos_x += physics.speed < 1.0f ? 1.0f : physics.speed;
		//}
	} 
    else if (physics.speed < 0.0f){
		
		// If above the water area, scrolling moves the player sprite. Once in the water area, scrolling moves the map
		//if (map_pos_y < 1600){
			player.x += physics.speed > -0.1f ? -0.1f : physics.speed;
		//}
		//else {
			//map_pos_x += physics.speed < -1.0f ? -1.0f : physics.speed;
		//}
	} 
}

//**********************************************************************************************





















//**********************************************************************************************
// Update the game
inline void update_game(void){
	switch (gameState) {
		case GSM_MENU :{	
		}
		break;
		case GSM_PLAYING :{	
		
			int i;
			int prev_y;
			prev_y = player.y;

			// If the pickup item has not been collected and it is visible, check for collisions and handle the pickup item
			if (player.alive){
				for (i = 0; i < PICKUP_NUMBER; i++) {
					if (!pickups[i].collected && pickups[i].visible) {
						if (collision_with_pickup(get_alex_sprite_id(), player.x + map_pos_x, player.y + map_pos_y, i)){handle_pickup_item(i);}
					}
				}
			}
	
			// Update the enemies
			for (i = 0; i < ENEMY_NUMBER; i++) {
				if (enemies[i].alive){
					update_enemy(i, map_pos_x, map_pos_y);

					// Check for collisions between alex and the enemy sprite
					if (player.alive){
						if (collision_with_enemy(get_alex_sprite_id(), player.x + map_pos_x, player.y + map_pos_y, i)){

							// If the player is punching, kill the enemy, otherwise kill the player (unless he is invincible)
							if (player.is_punching){
								enemies[i].alive = false;
								enemies[i].recently_died = true;
							}
							else {
								if (!player.invincible){
									player.lives -=1;
									player.alive = false;
									player.dead_x = player.x;
									player.dead_y = player.y;
								}
							}
						}
					}
				}
			}
		
			// Update the player
			if (player.alive){
				
				// Update the player invincible timer check (invincible for 3 seconds after respawn)
				if (player.invincible){
					if (jo_get_ticks() > player.invincible_start_time +3000){player.invincible = false;}
				}
				
				// Detect player collisions with the map
				alex_collision_handling();

				// Update the camera
				camera_handling(prev_y);
				
				// Update player animations
				if (!physics.is_in_air){alex_running_animation_handler();}
				else {alex_jump_animation_handler();}
			}
			else {
				// Update the player death sequence 
				if (player.y + ALEX_HEIGHT*2 > 0){player.y--;}
				else {make_alex_respawn();}
			}
		}
		break;
		case GSM_PAUSE :{
		}
		break;
	}
}
//**********************************************************************************************




//**********************************************************************************************
// Draw the game
inline void draw_game(void){

	switch (gameState) {
		
		case GSM_MENU :{	
			jo_clear_background(JO_COLOR_RGB(253,253,10));
			display_title();
		}
		break;
		
		case GSM_PLAYING :{	

			jo_clear_background(JO_COLOR_Blue);

			// Draw the map
			jo_map_draw(WORLD_MAP_ID, map_pos_x, map_pos_y);
			jo_map_draw(1, map_pos_x, map_pos_y);
			
			
			// If the pickup item has not been collected, draw the pickup item sprite
			int i;
			for (i = 0; i < PICKUP_NUMBER; i++) {
				if (!pickups[i].collected && pickups[i].visible) {display_pickup(i, map_pos_x, map_pos_y);}
			}

			// If the block has not been broken, draw the block sprite
			for (i = 0; i < BLOCK_NUMBER; i++) {
				//if (!blocks[i].broken) {display_block(i, map_pos_x, map_pos_y);}
				display_block(i, map_pos_x, map_pos_y);
			}
			
			
			// If the enemy is alive, draw the enemy sprite
			for (i = 0; i < ENEMY_NUMBER; i++) {
				if (enemies[i].alive){display_enemy(i, map_pos_x, map_pos_y);}
				if (enemies[i].recently_died){display_enemy_death(i, map_pos_x, map_pos_y);}
			}
			
			// If the player is alive, draw the player sprite, otherwise draw the player death sprite
			if (player.alive){display_alex(jo_get_ticks());} else {display_alex_death();}

			// Print game info
			//jo_font_print(font_sprite_id, 220, 0, 1.0, "JO ALEX 0.1");
			//jo_printf_with_color(0, 0, JO_COLOR_INDEX_Yellow, "Lives: %d  ", player.lives);
			
			//jo_printf_with_color(0, 4, JO_COLOR_INDEX_Yellow, "Player Y: %d  ", player.y);
			//jo_printf_with_color(0, 5, JO_COLOR_INDEX_Yellow, "MapPos Y: %d  ", map_pos_y);
			
			//jo_printf_with_color(0, 25, JO_COLOR_INDEX_Yellow, "FPS: %d/60", 60 / JO_FRAMERATE);
			//jo_printf_with_color(0, 26, JO_COLOR_INDEX_Yellow, "Dynamic Mem Usage: %d%%  ", jo_memory_usage_percent());
			//jo_printf_with_color(0, 27, JO_COLOR_INDEX_Yellow, "Sprite Mem Usage: %d%%  ", jo_sprite_usage_percent());
			//jo_printf_with_color(0, 28, JO_COLOR_INDEX_Yellow, "Audio Mem Usage: %d%%  ", jo_audio_usage_percent());
			//jo_printf_with_color(0, 29, JO_COLOR_INDEX_Yellow, "Engine Version: %f", JO_ENGINE_VERSION);
		}
		break;
		
		case GSM_PAUSE :{
			jo_clear_screen();
			jo_clear_background(JO_COLOR_Black);
			display_pause(player.score, player.lives, player.money);
		}
		break;
	}	
}
//**********************************************************************************************



//**********************************************************************************************
// Handle user input
inline void input_handler(void){
	
	switch (gameState) {
		
		case GSM_MENU :{	
		
			// Start button starts the game
			if (game_started()){
				if (jo_is_pad1_key_pressed(JO_KEY_START)){
					unload_title();
					pauseTimer = jo_get_ticks();
					gameState = GSM_PLAYING;
					//jo_audio_play_cd_track(2, 2, 1);
				}
			}
		}
		break;
		
		case GSM_PLAYING :{		

			// If the player is in the air
			if (physics.is_in_air){
				if (jo_is_pad1_key_pressed(JO_KEY_LEFT)){
					player.flip_alex = true;
					jo_physics_accelerate_left(&physics);
				}
				else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT)){
					player.flip_alex = false;
					jo_physics_accelerate_right(&physics);
				}
			}
			// If the player is not in the air
			else {
				if (jo_is_pad1_key_pressed(JO_KEY_LEFT)){
					
					player.flip_alex = true;
					if (jo_physics_is_going_on_the_right(&physics) || jo_physics_should_brake(&physics)){
						jo_physics_decelerate_left(&physics);
					}    
					else{
						
						// If the player is not ducking or punching, this moves the plaer left
						if (!player.is_ducking && !player.is_punching){
							jo_physics_accelerate_left(&physics);
						}
					}     
				}
				else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT)){
					
					player.flip_alex = false;
					if (jo_physics_is_going_on_the_left(&physics) || jo_physics_should_brake(&physics)){
						jo_physics_decelerate_right(&physics);
					}  
					else{
						
						// If the player is not ducking or punching, this moves the plaer right
						if (!player.is_ducking && !player.is_punching){
							jo_physics_accelerate_right(&physics);
						}
					}  
				}
				else{
					// If no directional button is pressed, apply friction
					jo_physics_apply_friction(&physics);
				}    
			}
			
			// Down button makes Alex duck
			if (jo_is_pad1_key_down(JO_KEY_DOWN) && !player.is_ducking){
				player.is_ducking = true;
			}
			
			// Up button makes Alex stand
			if (jo_is_pad1_key_down(JO_KEY_UP) && player.is_ducking){
				player.is_ducking = false;
			}
	
			// B button makes Alex punch
			if (jo_is_pad1_key_pressed(JO_KEY_B) && !player.is_punching){
				make_alex_punch();
			}
			
			// Stop Alex from punching after a short period
			if (jo_get_ticks() > punchTimer +150) {
				player.is_punching = false;
			}
			
			// Shoulder buttons restart the game
			if (jo_is_pad1_key_down(JO_KEY_L) && jo_is_pad1_key_down(JO_KEY_R)){reset_demo();}
			   
			// A button makes the character jump
			if (!player.is_ducking && !player.is_punching && player.can_jump && jo_is_pad1_key_down(JO_KEY_A)){make_alex_jump();}   

			
			// Start button pauses the game
			if (jo_is_pad1_key_pressed(JO_KEY_START)){
				
				if (jo_get_ticks() > pauseTimer +100) {
					pauseTimer = jo_get_ticks();
					jo_map_free(WORLD_MAP_ID);
					unload_alex();
					unload_enemy();
					load_pause();
					gameState = GSM_PAUSE;
				}
			}	
			
			
		}
		break;
		
		case GSM_PAUSE :{
			if (jo_is_pad1_key_pressed(JO_KEY_START)){
				
				if (jo_get_ticks() > pauseTimer +100) {
					pauseTimer = jo_get_ticks();
					jo_clear_screen();
					unload_pause();
					load_map();
					load_alex();
					gameState = GSM_PLAYING;
					//jo_audio_play_cd_track(2, 2, 1);
				}
			}
		}
		break;
	}	
}
//**********************************************************************************************



// Main
void jo_main(void){
	
	jo_core_init(JO_COLOR_Black);
	load_alex();
	load_map();
	load_enemy();
	
	load_pickup();
	load_block();
	
    jo_physics_init_for_sonic(&physics);
    reset_demo();
	
	
	
	jo_core_add_callback(draw_game);
	jo_core_add_callback(input_handler);
	jo_core_add_callback(update_game);
	jo_core_run();
}

// END OF FILE