#include <stdio.h>
#include <stdlib.h>

#define WIDTH 50
#define HEIGHT 25
#define MAX_OBJECTS 100
#define EMPTY '_'
#define DRAW '*'

typedef enum {
    LINE = 1,
    RECTANGLE,
    TRIANGLE,
    CIRCLE
} ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Object;

void clear_canvas(char canvas[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = EMPTY;
        }
    }
}

void set_pixel(char canvas[HEIGHT][WIDTH], int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = DRAW;
    }
}

void display_canvas(char canvas[HEIGHT][WIDTH]) {
    printf("\nCanvas (%d x %d)\n\n", WIDTH, HEIGHT);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
    }
}

void draw_line(char canvas[HEIGHT][WIDTH], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        set_pixel(canvas, x1, y1);

        if (x1 == x2 && y1 == y2) {
            break;
        }

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

void draw_rectangle(char canvas[HEIGHT][WIDTH], int x1, int y1, int x2, int y2) {
    int left = (x1 < x2) ? x1 : x2;
    int right = (x1 > x2) ? x1 : x2;
    int top = (y1 < y2) ? y1 : y2;
    int bottom = (y1 > y2) ? y1 : y2;

    for (int x = left; x <= right; x++) {
        set_pixel(canvas, x, top);
        set_pixel(canvas, x, bottom);
    }
    for (int y = top; y <= bottom; y++) {
        set_pixel(canvas, left, y);
        set_pixel(canvas, right, y);
    }
}

void draw_triangle(char canvas[HEIGHT][WIDTH], int x1, int y1,
                   int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}

void draw_circle(char canvas[HEIGHT][WIDTH], int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        set_pixel(canvas, xc + x, yc + y);
        set_pixel(canvas, xc - x, yc + y);
        set_pixel(canvas, xc + x, yc - y);
        set_pixel(canvas, xc - x, yc - y);
        set_pixel(canvas, xc + y, yc + x);
        set_pixel(canvas, xc - y, yc + x);
        set_pixel(canvas, xc + y, yc - x);
        set_pixel(canvas, xc - y, yc - x);

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void draw_object(char canvas[HEIGHT][WIDTH], Object object) {
    switch (object.type) {
        case LINE:
            draw_line(canvas, object.x1, object.y1, object.x2, object.y2);
            break;
        case RECTANGLE:
            draw_rectangle(canvas, object.x1, object.y1, object.x2, object.y2);
            break;
        case TRIANGLE:
            draw_triangle(canvas, object.x1, object.y1, object.x2, object.y2,
                          object.x3, object.y3);
            break;
        case CIRCLE:
            draw_circle(canvas, object.x1, object.y1, object.radius);
            break;
    }
}

void redraw_canvas(char canvas[HEIGHT][WIDTH], Object objects[], int count) {
    clear_canvas(canvas);
    for (int i = 0; i < count; i++) {
        draw_object(canvas, objects[i]);
    }
}

const char *shape_name(ShapeType type) {
    switch (type) {
        case LINE:
            return "Line";
        case RECTANGLE:
            return "Rectangle";
        case TRIANGLE:
            return "Triangle";
        case CIRCLE:
            return "Circle";
        default:
            return "Unknown";
    }
}

void list_objects(Object objects[], int count) {
    if (count == 0) {
        printf("\nNo objects in the picture.\n");
        return;
    }

    printf("\nObjects:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s", i + 1, shape_name(objects[i].type));
        if (objects[i].type == CIRCLE) {
            printf(" center=(%d,%d), radius=%d", objects[i].x1, objects[i].y1,
                   objects[i].radius);
        } else if (objects[i].type == TRIANGLE) {
            printf(" points=(%d,%d), (%d,%d), (%d,%d)", objects[i].x1,
                   objects[i].y1, objects[i].x2, objects[i].y2, objects[i].x3,
                   objects[i].y3);
        } else {
            printf(" points=(%d,%d), (%d,%d)", objects[i].x1, objects[i].y1,
                   objects[i].x2, objects[i].y2);
        }
        putchar('\n');
    }
}

int read_shape_type(ShapeType *type) {
    int choice;

    printf("\n1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Triangle\n");
    printf("4. Circle\n");
    printf("Choose object type: ");

    if (scanf("%d", &choice) != 1 || choice < LINE || choice > CIRCLE) {
        printf("Invalid object type.\n");
        return 0;
    }

    *type = (ShapeType)choice;
    return 1;
}

int read_object(Object *object) {
    if (!read_shape_type(&object->type)) {
        return 0;
    }

    object->x1 = object->y1 = object->x2 = object->y2 = 0;
    object->x3 = object->y3 = object->radius = 0;

    switch (object->type) {
        case LINE:
            printf("Enter x1 y1 x2 y2: ");
            return scanf("%d %d %d %d", &object->x1, &object->y1, &object->x2,
                         &object->y2) == 4;
        case RECTANGLE:
            printf("Enter opposite corners x1 y1 x2 y2: ");
            return scanf("%d %d %d %d", &object->x1, &object->y1, &object->x2,
                         &object->y2) == 4;
        case TRIANGLE:
            printf("Enter three points x1 y1 x2 y2 x3 y3: ");
            return scanf("%d %d %d %d %d %d", &object->x1, &object->y1,
                         &object->x2, &object->y2, &object->x3,
                         &object->y3) == 6;
        case CIRCLE:
            printf("Enter center x y and radius: ");
            return scanf("%d %d %d", &object->x1, &object->y1,
                         &object->radius) == 3 && object->radius >= 0;
    }

    return 0;
}

void add_object(Object objects[], int *count) {
    if (*count >= MAX_OBJECTS) {
        printf("Object list is full.\n");
        return;
    }

    if (read_object(&objects[*count])) {
        (*count)++;
        printf("Object added.\n");
    } else {
        printf("Invalid input. Object was not added.\n");
    }
}

void delete_object(Object objects[], int *count) {
    int index;

    list_objects(objects, *count);
    if (*count == 0) {
        return;
    }

    printf("Enter object number to delete: ");
    if (scanf("%d", &index) != 1 || index < 1 || index > *count) {
        printf("Invalid object number.\n");
        return;
    }

    for (int i = index - 1; i < *count - 1; i++) {
        objects[i] = objects[i + 1];
    }
    (*count)--;
    printf("Object deleted.\n");
}

void modify_object(Object objects[], int count) {
    int index;
    Object updated;

    list_objects(objects, count);
    if (count == 0) {
        return;
    }

    printf("Enter object number to modify: ");
    if (scanf("%d", &index) != 1 || index < 1 || index > count) {
        printf("Invalid object number.\n");
        return;
    }

    printf("Enter the new object details.\n");
    if (read_object(&updated)) {
        objects[index - 1] = updated;
        printf("Object modified.\n");
    } else {
        printf("Invalid input. Object was not modified.\n");
    }
}

void print_menu(void) {
    printf("\n2D Graphics Editor\n");
    printf("1. Add object\n");
    printf("2. Delete object\n");
    printf("3. Modify object\n");
    printf("4. Display picture\n");
    printf("5. List objects\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    char canvas[HEIGHT][WIDTH];
    Object objects[MAX_OBJECTS];
    int object_count = 0;
    int choice;

    clear_canvas(canvas);

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                add_object(objects, &object_count);
                redraw_canvas(canvas, objects, object_count);
                break;
            case 2:
                delete_object(objects, &object_count);
                redraw_canvas(canvas, objects, object_count);
                break;
            case 3:
                modify_object(objects, object_count);
                redraw_canvas(canvas, objects, object_count);
                break;
            case 4:
                redraw_canvas(canvas, objects, object_count);
                display_canvas(canvas);
                break;
            case 5:
                list_objects(objects, object_count);
                break;
            case 0:
                printf("Goodbye.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
