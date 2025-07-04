#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "luma.h"
#include <fcntl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char ** argv) {

   run(argc, argv);

   return EXIT_SUCCESS;

}

int run(int argc, char ** argv) {
    char * file = NULL;
    if (argc > 1) {
        file = argv[1];
        check_melody_file(file);
    } else exit(EXIT_FAILURE);

    //printf("%s\n", file);


    note ** notes = malloc(sizeof(note *));
    notes[0] = NULL;
    read_notes(notes);
    printf("%d\n", notes[4]->wave);

    note n = {'c', 4, 0, 0};
    int r = get_wave_by_note(notes, &n);
    printf("%d\n", r);

    color ** colors = malloc(sizeof(color *));
    colors[0] = NULL;
    read_colors(colors);


    FILE * mf = fopen(file, "r");

    size_t buffer = 0;
    ssize_t len;
    char * line = malloc(sizeof(char)*100);

    while((len = getline(&line, &buffer, mf)) != -1) {
  //  printf("%ld %s\n", len, line);
    int q = 0;
    double l = 0;

    sscanf(line, "%d:%lf", &q, &l);

//    printf("Q: %d\t L: %.2lf\n", q, l);

    char * sound = malloc(sizeof(char) * 100);

    sprintf(sound, "play -n synth %.2lf pluck %d 2>/dev/null", l, q);

    system(sound);

    }

    return 0;
}

int check_melody_file(char * file) {
    struct stat * file_stat = malloc(sizeof(struct stat));
    int file_exists = stat(file, file_stat);
    if (file_exists != -1) printf("File exists.\n");
    return 0;
}


int read_notes(note ** notes) {
    size_t buffer = 0;
    ssize_t len;
    char * line = malloc(sizeof(char)*100);

    FILE * nf = fopen(FILE_NOTES, "r");
    int i = 0;
 
    while((len = getline(&line, &buffer, nf)) != -1) {
      notes = realloc(notes, sizeof(void)* (i + 1));
        notes[i] = malloc(sizeof(note));
        sscanf(line, "%c%d:%d:%d", &(notes[i]->letter), &(notes[i]->octave), &(notes[i]->wave), &(notes[i]->color));
        // printf("%c-%d-%d-%d\n", notes[i]->letter, notes[i]->octave, notes[i]->wave, notes[i]->color);
      i++;
    }
    notes[i] = NULL;

    return 0;
}

int get_wave_by_note(note ** notes, note * n) {
  int result = 0;
/*  
    note * p = notes[0];
    while(p != NULL) {
        if ((p->letter == n->letter) && (p->octave == n->octave)) {
            result = p->wave;
            break;
        }
        p++;
    }
*/
    return result;
}

int read_colors(color ** colors) {
    size_t buffer = 0;
    ssize_t len;
    char * line = malloc(sizeof(char)*100);

    FILE * cf = fopen(FILE_COLORS, "r");
    int i = 0;
 
    while((len = getline(&line, &buffer, cf)) != -1) {
      colors = realloc(colors, sizeof(void)* (i + 1));
        colors[i] = malloc(sizeof(color));
        sscanf(line, "%c:%d", &(colors[i]->letter), &(colors[i]->code));
      i++;
    }
    colors[i] = NULL;

    return 0;
}

