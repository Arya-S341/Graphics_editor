// Function to draw a line using a 2D character array
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 20

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

void print_canvas(char canvas[HEIGHT][WIDTH]) {
    // Print X-axis labels
    printf("   ");
    for (int x = 0; x < WIDTH; x++) {
        putchar('0' + (x / 10));
    }
    putchar('\n');

    printf("   ");
    for (int x = 0; x < WIDTH; x++) {
        putchar('0' + (x % 10));
    }
    putchar('\n');

    for (int y = 0; y < HEIGHT; y++) {
        printf("%2d ", y);
        for (int x = 0; x < WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
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

    // Draw a triangle using three line segments.
    // These coordinates create a large isosceles triangle that is easy to visualize.
    draw_line(canvas, 8, 16, 32, 16);  // base
    draw_line(canvas, 8, 16, 20, 4);   // left side
    draw_line(canvas, 32, 16, 20, 4);  // right side

    print_canvas(canvas);
    return 0;
}
