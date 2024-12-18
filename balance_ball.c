#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>

#define SURF_WIDTH 900
#define SURF_HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_GRAY 0xf1f1f1f1
#define COLOR_BLACK 0x00000000
#define COLOR_RED 0xffa500
#define GRAVITY 0.2

struct Ball {
  double x;
  double y;
  double radius;
  double v_x;
  double v_y;

};

int draw_point(SDL_Surface* surface, int x, int y, uint32_t COLOR){
  SDL_Rect point = (SDL_Rect) {x, y, 1, 1};
  SDL_FillRect(surface, &point, COLOR);
  
}

int create_ball(SDL_Surface* surface, struct Ball ball){
  double x_low = ball.x - ball.radius;
  double y_low = ball.y - ball.radius;
  double x_high = ball.x + ball.radius;
  double y_high = ball.y + ball.radius;
  double radius_square = ball.radius * ball.radius;
  for(double x = x_low; x < x_high; x++) {
    for(double y = y_low; y < y_high; y++) { 
      if (abs(sqrt((x - ball.x)*(x - ball.x)  +( y - ball.y)*(y - ball.y))) < ball.radius) {
	draw_point(surface, x, y, COLOR_WHITE);
      }
    }
  }
}

int draw_trajectory(SDL_Surface* surface, struct Ball trajectory[], int trajectory_points_num){
  for (int i = 0; i < trajectory_points_num; i++) {
    if (i > 0) {
      double x_low = trajectory[i].x - trajectory[i].radius;
      double y_low = trajectory[i].y - trajectory[i].radius;
      double x_high = trajectory[i].x + trajectory[i].radius;
      double y_high = trajectory[i].y + trajectory[i].radius;
      //double radius_square = ball.radius * ball.radius;
      for(double x = x_low; x < x_high; x++) {
	for(double y = y_low; y < y_high; y++) { 
	  if (abs(sqrt((x - trajectory[i].x)*(x - trajectory[i].x)  +( y - trajectory[i].y)*(y - trajectory[i].y))) < trajectory[i].radius) {
	    draw_point(surface, x, y, COLOR_RED);
	  }
	}
      }
    }      
  }
}


int move_ball(struct Ball* ball){
  ball->x += ball->v_x;
  ball->y += ball->v_y;
  ball->v_y += GRAVITY;
  printf("v_x = %f, v_y = %f\n", ball->v_x, ball->v_y);
  
  
  if ((ball->x + ball->radius) > SURF_WIDTH) {
    ball->x = SURF_WIDTH - ball->radius;
    ball->v_x = -ball->v_x;
  }
  if ((ball->y + ball->radius) > SURF_HEIGHT) {
    ball->y = SURF_HEIGHT - ball->radius;
    ball->v_y = -ball->v_y;
  }
  if((ball->x - ball->radius) <= 0) {
    ball->x = ball->radius;
    ball->v_x =  -ball->v_x;
  }
  if ((ball->y - ball->radius) <= 0) {
    ball->y = 0 + ball->radius;
    ball->v_y = -ball->v_y;
  }

  
  
  
}

int draw1_trajectory(SDL_Surface* surface, struct Ball trajectory[], int trajectory_points_num) {

  for (int i = 0; i <= trajectory_points_num; i++) {
    if ( i > 6) {
      double x_val = trajectory[i].x;
      double y_val = trajectory[i].y;
      SDL_Rect point = (SDL_Rect) {x_val, y_val, 20, 20};
      SDL_FillRect(surface, &point, COLOR_GRAY); 
    }
    
  }
}

int create_trajectory(struct Ball trajectory[], struct Ball ball, int trajectory_points_num){

  trajectory[0] = ball;
  
  for (int i = trajectory_points_num - 1; i > 0; i--) {
    trajectory[i].x = trajectory[i-1].x;
    trajectory[i].y = trajectory[i-1].y;
    trajectory[i].radius = trajectory[i-1].radius - 2;
    //printf("t x = %f, t y = %f\n", trajectory[i].x, trajectory[i].y);
    
  }
  trajectory[0].x = ball.x;
  trajectory[0].y = ball.y;
  trajectory[0].radius = ball.radius;
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Bauncy Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SURF_WIDTH, SURF_HEIGHT, 0);
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  int trajectory_points_num = 25;
  struct Ball ball = {450, 300, 20, 10, 0};
  struct Ball trajectory[trajectory_points_num];
  SDL_Event event;
  int running = 1;
  while (running) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
	running = 0;
      }
    }
    SDL_FillRect(surface, NULL, 0x00000000);
    draw_trajectory(surface, trajectory, trajectory_points_num);
    create_ball(surface, ball);
    move_ball(&ball);
    create_trajectory(trajectory, ball, trajectory_points_num);
    
    SDL_UpdateWindowSurface(window);
    SDL_Delay(50);
  }



}
