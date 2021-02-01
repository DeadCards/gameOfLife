#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>

enum cell_state{
    DEAD,
    ALIVE
};

struct grid{
    int width;
    int height;
    enum cell_state **cells;
};

struct grid *make_grid(int width,int height){
    struct grid *new_grid = calloc(1, sizeof(struct grid));
    if(!new_grid){ exit(2);}

    new_grid->width = width;
    new_grid->height = height;
    new_grid->cells = calloc(height, sizeof(enum cell_state *));

    if(!new_grid->cells){exit(2);}
    for(int i=0; i< height; i++){
        new_grid->cells[i] = calloc(width, sizeof(enum cell_state));
        if(!new_grid->cells[i]){exit(2);}
    }
    
    return new_grid;
}

void print_grid(struct grid *grid){
    for(int i =0; i < grid->height; i++){
        for(int j=0;j < grid->width; j++){
            
            printf("%c",grid->cells[i][j] == DEAD ? '.':'#');
        }
        printf("\n");
    }
}
void free_grid(struct grid *grid){
    for(int i = 0; i<grid->height;i++){
        free(grid->cells[i]);
    }
    free(grid->cells);
    free(grid);
}
int count_neighbors(struct grid *grid, int i, int j){
    
    return grid->cells[i][j - 1] + grid->cells[i][j + 1] + grid->cells[i - 1][j - 1]+ grid->cells[i - 1][j] + grid->cells[i - 1][j + 1] + grid->cells[i + 1][j - 1] + grid->cells[i+1][j] + grid->cells[i + 1][j + 1];
}

//grid will be printed 10 times
struct grid *envole_grid(struct grid *grid){
    struct grid *new_grid = make_grid(grid->width, grid->height);

    for(int i = 1; i < grid->height - 1; i++){
        for(int j = 1;j < grid->width - 1; j++){

            switch(count_neighbors(grid, i, j)){
                case 2:
                     new_grid->cells[i][j] = grid->cells[i][j];
                     break;
                 case 3:
                    new_grid->cells[i][j] = ALIVE;
                    break;
                 default:
                    new_grid->cells[i][j] = DEAD;
                    break;
            }
        }
    }

    free_grid(grid);

    return new_grid;
}

int main(int argc, char **argv){
    struct grid *grid = make_grid(40, 20);

    char* endpointer = NULL;
    if(argc < 3){
        printf("Use with ./game [Iteration count] [file name]\n");
        return 1;
    }
    
    char *endptr = NULL;
    errno = 0;
    long iterations = strtol(argv[1], &endptr, 10);
    if(*argv[1] == '\0' || *endptr != '\0'){
        fprintf(stderr, "Not a proper number given\n");
        return 1;
    }
    if(errno){
        perror("given number");
        return 1;
    }
    



    for(int i = 0; i < iterations; i++) {
        printf("\033[2J");
        printf("Iteration: %d\n", i);
        print_grid(grid);
        grid = envole_grid(grid);
        usleep(100000);
    }
    size_t a = -1;
    printf("%lu", a);
    free_grid(grid);
}
