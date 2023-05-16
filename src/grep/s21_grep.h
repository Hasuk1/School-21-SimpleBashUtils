#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_MAX 8192

const char *short_options = ":e:ivclnhsf:o";

const struct option long_options[] = {
    {"regexp", required_argument, NULL, 'e'},
    {"ignore-case", no_argument, NULL, 'i'},
    {"invert-match", no_argument, NULL, 'v'},
    {"count", no_argument, NULL, 'c'},
    {"files-with-matches", no_argument, NULL, 'l'},
    {"line-number", no_argument, NULL, 'n'},
    {"no-filename", no_argument, NULL, 'h'},
    {"no-messages", no_argument, NULL, 's'},
    {"file", required_argument, NULL, 'f'},
    {"only-matching", no_argument, NULL, 'o'},
    {NULL, 0, NULL, 0}};

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} options;

int scan_opt(int argc, char **argv, options *opt, char *pattern_v);
void search_match(char **argv, options *opt, FILE *file_name, int pattern_c,
                  char *pattern_v, int file_counter, int fc);
int pattern_creator(char **argv, options *opt, char *pattern_v,
                    char *file_w_pattern, char *buffer);
int counter_of_file(int argc, char **argv);
void last_char(char *file_buffer);

#endif