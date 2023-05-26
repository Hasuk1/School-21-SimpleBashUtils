#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_BUFFER_MAX 4096

const char *short_options = "+beEnstTv";

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"show-ends", no_argument, NULL, 'E'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}};

typedef struct {
  int b;
  int e;
  int ee;
  int n;
  int s;
  int t;
  int tt;
  int v;
  int print_ch_done;
} options;

int scan_opt(int argc, char **argv, options *opt);
FILE *cat_option(FILE *file_name, options *opt);
void char_option_cat(unsigned char ch, options *opt);

#endif