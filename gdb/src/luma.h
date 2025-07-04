#ifndef LUMA_H_LOADED
#define LUMA_H_LOADED

#define DIR_DATA "../data/"
#define DIR_MELODY "../data/melody"
#define FILE_NOTES "../data/notes"
#define FILE_COLORS "../data/colors"

struct note {
    char letter;
    int octave;
    int wave;
    int color;
};
typedef struct note note;

struct color {
    char letter;
    int code;
};
typedef struct color color;

int run(int, char **);  // Точка входа (нужна для тестирования)
int check_melody_file(char *); // Проверить, существует ли файл с мелодией

int read_notes(note ** notes);  // Считать конфигурацию нот
int note_line_to_struct(char *, note *);  // Преобразовать строку в структуру

int me_here = 17;

int read_colors(color ** colors);
int get_wave_by_note(note **, note *);

// Получить имя файла с музыкой
int get_melody_from_file(char *);
int check_if_melody_exists(int, char **);
// Считать мелодию в память
// Воспроизвести музыку и вывести цвет одновременно


#endif
