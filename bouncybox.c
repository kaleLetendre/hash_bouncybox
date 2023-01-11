#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>
#include <unistd.h>
#define height 50
#define width  105
char black[] = "\033[0;30m#";
char white[] = "\033[0;37m#";
char box_colors[6][11] = {"\033[0;37m#","\033[0;31m#","\033[0;32m#","\033[0;33m#","\033[0;35m#","\033[0;36m#"};
// Do NOT touch this function signature
void setImage(int image[height][width]) {
        for(int i = 0; i < height; i++){
                for (int j = 0; j< width; j++){
                        image[i][j] = 0;
                }
        }
}
void pixelOn(int x, int y, int image[height][width]){image[y][x] = 1;}
void pixelOff(int x, int y, int image[height][width]){image[y][x] = 0;}

void draw(int image[height][width],int color_choice){
	char screen[height*width*11] = {'\0'};
	for(int i = 0; i < height; i++){
                for (int j = 0; j < width; j++){
			if(image[i][j]){strcat(screen,box_colors[color_choice]);}
			else{strcat(screen,black);}
                }
        }
        printf("%s\n", screen);
}
void box(int x, int y, int w, int h,int image[height][width]){
	for (int i = x; i < x+w;i++){
		for (int j = y; j < y+h; j++){
			pixelOn(i,j,image);
		}
	}

}
/*
   ###
 #######
 #######
#########
######### looks better on hash graphics format
#########
 #######
 #######
   ###

*/
int is_key_pressed() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
int main(){
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
    	newt = oldt;
    	newt.c_lflag &= ~(ICANON | ECHO);
    	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	int image1[height][width];
   	int image2[height][width];

    	setImage(image1);
    	setImage(image2);

	int (*current_image)[width] = image1;
	int (*new_image)[width] = image2;

    	int xpos = 52;
    	int ypos = 0;
	int size = 4;
    	int running = 1;
	int gravity = 1;
	int bounce_loss = 1;
	int deltaX = 0;
	int deltaY = 0;
	int ground_time = 0;
	int friction = 1;
	int key_presses = 0;
	int color_choice = 0;
	int colorslen = sizeof(box_colors)/sizeof(box_colors[0]);
    	while(running){
		if (is_key_pressed()) {
			key_presses +=1;
            		char c = getchar();
            		if (c == 'w') {deltaY-=9;} else if (c == 'a'){deltaX-=5;}else if(c == 's'){deltaY+=9;}else if(c == 'd'){deltaX+=5;}
			if(key_presses > 3){tcflush(STDIN_FILENO, TCIFLUSH);}
        	}
        	if(xpos>=width-size){//RIGHT WALL
			xpos = width-size-1;
			deltaX = (-deltaX+bounce_loss/2);
			if(deltaX > 0){deltaX = 0;}
			color_choice +=1;
		}
        	if(xpos <= 0){//LEFT WALL
			xpos = 1;
        		deltaX = (-deltaX-bounce_loss/2);
        		if (deltaX < 0){deltaX = 0;}
			color_choice +=1;
        	}
		if(ypos>=height-size){//GROUND
			ypos = height-size;
			deltaY = -deltaY+bounce_loss;//bounce but add 1 to stop infinite bounce (energy loss)
			ground_time +=1; //to check if friction (aka x speed loss) should occur
			if(ground_time==1){
				color_choice +=1;
			}
		}
		if(ypos <= 0){//ROOF
			ypos = 1;
			deltaY = gravity;
			color_choice +=1;
		}
		if(ypos<height-size){
			ground_time=0;//reset ground time
		}
		if(ground_time >= 2 && deltaX != 0){
			int sign = (deltaX>0)?1:((deltaX<0)?-1:0); //if num>0 sign = 1 if num < 0 sign = -1 else sign = 0
			deltaX -= sign*friction;
			ground_time=0;
		}

		if(color_choice >= colorslen){color_choice = 0;};
		//printf("%d\t%d\n",deltaX,ground_time);
		box(xpos,ypos,size,size,new_image);//calculate for next frame
		draw(current_image,color_choice);//show previouse calulated frame

		deltaY+=gravity;//accelerate due to gravity every frame
		xpos+=deltaX;
		ypos+=deltaY;

        	usleep(30000);//wait 1/15 seconds

		current_image = new_image;//set calculated frame to be shown next
		setImage(new_image);//reset to be calulated
    	}
    	return 0;
}

