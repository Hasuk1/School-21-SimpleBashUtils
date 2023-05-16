#include "s21_cat.h"

int main(int argc, char *argv[]) {
  FILE *file_name;
  options opt = {0};
  if (argc > 1) {
    if (scan_opt(argc, argv, &opt)) {
      int fc = optind;
      while (fc < argc) {
        file_name = fopen(argv[fc], "r");
        if (file_name == NULL) {
          fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                  argv[fc]);
        } else
          fclose(cat_option(file_name, &opt));
        fc++;
      }
    }
  }
  return 0;
}

int scan_opt(int argc, char **argv, options *opt) {
  int option, option_index = -1, flag = 1;
  while ((option = getopt_long(argc, argv, short_options, long_options,
                               &option_index)) != -1) {
    switch (option) {
      case 'b':
        opt->b = 1;
        break;
      case 'e':
        opt->e = 1;
        break;
      case 'E':
        opt->ee = 1;
        break;
      case 'n':
        opt->n = opt->b ? 0 : 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 't':
        opt->t = 1;
        break;
      case 'T':
        opt->tt = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case '?':
        printf("usage: cat [-benstuv] [file ...]\n");
        flag = 0;
    }
  }
  return flag;
}

FILE *cat_option(FILE *file_name, options *opt) {
  char file_buffer[FILE_BUFFER_MAX];
  int number_lines = 1, next_blank_line = 0;
  while (fgets(file_buffer, FILE_BUFFER_MAX, file_name)) {
    int lenght = strlen(file_buffer);
    if (opt->s) {
      int current_blank_line = (lenght == 1) ? 1 : 0;
      if (current_blank_line && next_blank_line) continue;
      next_blank_line = current_blank_line;
    }
    if (opt->b && lenght > 1) printf("%6d\t", number_lines++);
    if (opt->n && !opt->b) printf("%6d\t", number_lines++);

    for (int i = 0; file_buffer[i] != '\0'; i++) {
      unsigned char ch = file_buffer[i];
      char_option_cat(ch, opt);
    }
  }
  return file_name;
}

void char_option_cat(unsigned char ch, options *opt) {
  if (opt->e || opt->t || opt->v) {
    if (ch >= 32) {
      if (ch == 127) {
        ch = '?';
        printf("^");
      }
      if (ch > 127) {
        ch = '-';
        printf("M");
        if (ch >= 128 + 32) {
          if (ch < 128 + 127) {
            ch = ch - 128;
          } else {
            ch = '?';
            printf("^");
          }
        } else {
          ch = ch - 128 + 64;
          printf("^");
        }
      }
    } else if (ch == '\t') {
      if (opt->t) {
        ch += 64;
        printf("^");
      }
    } else if (ch == '\n') {
      if (opt->e) {
        ch = '$';
        putchar(ch);
        printf("\n");
        opt->print_ch_done = 1;
      }
    } else {
      ch += 64;
      printf("^");
    }
  }
  if (opt->ee && ch == 10) {
    ch = '$';
    putchar(ch);
    opt->print_ch_done = 1;
    printf("\n");
  }
  if (opt->tt && ch == 9) {
    ch += 64;
    printf("^");
  }
  if (!opt->print_ch_done) putchar(ch);
  opt->print_ch_done = 0;
}