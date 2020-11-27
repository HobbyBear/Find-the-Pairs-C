#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define ELIMINATED 'X'
#define START_CHAR 'A'
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER '\r'
#define RANDOM_SEED 200

typedef struct {
    char element;
    int index;
}map_element_t;

void pause(char *s){
    printf("%s\nPress any key to continue.", s);
    getch();
}

map_element_t* generate_map(int size){
    char candidtes[size];

    for (int i=0; i<size; i++){
        candidtes[i] = START_CHAR + i;
    }

    map_element_t *map = (map_element_t *) malloc(sizeof(map_element_t) * size * size);

    int paired[size * size];

    for (int i=0; i<size*size/2; i++){
        map[i].element = candidtes[rand() % size];
        map[i].index = i;
        paired[i] = 0;
    }
    for (int i=size*size/2; i<size*size; i++){
        int index;
        while (1){
            index = rand() % (size * size/2);
            if (!paired[index]){
                paired[index] = 1;
                break;
            }
        }
        map[i].element = map[index].element;
        map[i].index = i;
    }

    return map;
}

void print_map(map_element_t *map, int size, int pointer, map_element_t *selected, int moved){
    for (int row=0; row<size; row++){
        for (int col=0; col<size; col++){
            int index = row * size + col;
            if (index == pointer){
                printf("%4c<-", map[index].element);
            }else{
                printf("%5c ", map[index].element);
            }
            if (selected){
                if (index == selected->index){
                    printf("*");
                }
            }
        }
        printf("\n");
    }
    printf("you have moved %d steps.\n", moved);
    printf("arrow keys: move the pointer\n");
    printf("*: current selected\n");
    printf("X: eliminated\n");
    printf("q: quit\n");
}

void main_game(int size){
    srand(RANDOM_SEED);

    system("cls");

    int const max_index = size * size - 1;
    int count = size * size;
    int pointer = 0;
    int moved = 0;
    map_element_t *selected = NULL;
    map_element_t *map = generate_map(size);
    print_map(map, size, pointer, selected, moved);

    while (1){
        switch(getch()) {
            case KEY_UP:
                pointer = pointer < size ? pointer : pointer-size;
                break;
            case KEY_DOWN:
                pointer = pointer > max_index-size ? pointer : pointer+size;
                break;
            case KEY_RIGHT:
                pointer = pointer < max_index ? pointer+1 : pointer;
                break;
            case KEY_LEFT:
                pointer = pointer > 0 ? pointer-1 : pointer;
                break;
            case KEY_ENTER:
                if (selected){
                    if (pointer == selected->index){
                        /* cancel selection */
                        selected = NULL;
                    }else if (map[pointer].element == selected->element){
                        /* eliminated */
                        map[pointer].element = ELIMINATED;
                        selected->element = ELIMINATED;
                        count -= 2;
                        selected = NULL;
                    }
                }else{
                    if (map[pointer].element != ELIMINATED){
                        selected = &map[pointer];
                    }
                }
                /* enter does not count to movement */
                moved--;
                break;
            case 'q':
                return;
        }
        system("cls");
        moved++;
        if (count <= 0){
            break;
        }
        print_map(map, size, pointer, selected, moved);

    }

    pause("Winner!");
    free(map);

}


int main(int argv, char *args[]){

    int size = 8;

    main_game(size);

    return 0;
}
