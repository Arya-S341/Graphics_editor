//function to draw a line using 2d charcter array
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 20
#define HEIGHT 15
void draw_line(char canvas[HEIGHT][WIDTH], int x1, int y1, int x2, int y2) {
    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        canvas[y1][x1] = '*'; // Mark the point on the canvas

        if (x1 == x2 && y1 == y2) break; // Line is complete

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
int main() {
    char canvas[HEIGHT][WIDTH];
    
    // Initialize the canvas with underscores
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = '_';
        }
    }

    // Draw a line from (2, 3) to (15, 10)
    draw_line(canvas, 2, 3, 15, 10);

    // Print the canvas
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(canvas[i][j]);
        }
        putchar('\n');
    }

    return 0;
}
