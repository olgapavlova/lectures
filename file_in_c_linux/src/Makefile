.PHONY: all clean
.SILENT:

CC = gcc
CFLAGS = -Wall -Werror -Wextra
BUILD = ../build/

all: read_txt_line_by_line big_file read_text_on_low_level file_type

r: file_desc

read_text_line_by_line: read_txt_line_by_line.c
	$(CC) $(CFLAGS) $^ -o $(BUILD)$@

big_file: big_file.c
	$(CC) $(CFLAGS) $^ -o $(BUILD)$@

read_text_on_low_level: read_text_on_low_level.c
	$(CC) $(CFLAGS) $^ -o $(BUILD)$@

file_desc: file_desc.c
	$(CC) $(CFLAGS) $^ -o $(BUILD)$@
