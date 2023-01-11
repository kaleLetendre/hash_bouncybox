# README

This is a simple program that displays a bouncing box on the terminal using the WASD keys for movement. The box is displayed using the Monospace 12pt font at a width of 2 and a height of 1. The box will bounce around the terminal and change color when it hits the edges of the terminal.

To run this program, simply compile the code and run the executable. The code is written in C and uses the following libraries:
- `termios.h`
- `unistd.h`
- `stdlib.h`
- `string.h`
- `stdio.h`

The following functions are defined in the code:
- `setImage(int image[height][width])`: Initializes the image array with all pixels turned off.
- `pixelOn(int x, int y, int image[height][width])`: Turns on a specific pixel in the image.
- `pixelOff(int x, int y, int image[height][width])`: Turns off a specific pixel in the image.
- `draw(int image[height][width])`: Displays the image on the terminal.
- `box(int x, int y, int w, int h, int image[height][width])`: Draws a box at the specified position and dimensions on the image.
- `is_key_pressed()`: A helper function for detecting keyboard input.

The main function uses these functions to display the bouncing box on the terminal. The position and velocity of the box are tracked using the `xpos`, `ypos`, `deltaX`, and `deltaY` variables. The terminal is set to non-canonical mode to allow for real-time keyboard input detection. The program runs until the user terminates it.

Feel free to use this code as a starting point for your own projects or to learn more about displaying graphics on the terminal.
