#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char pattern_v[BUFFER_MAX];
  FILE *file_name;
  options opt = {0};
  if (argc > 3) {
    int pattern_c = scan_opt(argc, argv, &opt, pattern_v);
    if (pattern_c != -1) {
      int fc = optind;
      int file_counter = counter_of_file(argc, argv);
      while (fc < argc) {
        file_name = fopen(argv[fc], "r");
        if (file_name == NULL) {
          if (!opt.s)
            fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                    argv[optind]);
        } else {
          search_match(argv, &opt, file_name, pattern_c, pattern_v,
                       file_counter, fc);
          fclose(file_name);
        }
        fc++;
      }
    }
  }
  return 0;
}

int scan_opt(int argc, char **argv, options *opt, char *pattern_v) {
  int option, flag = 1;
  char file_w_pattern[BUFFER_MAX], buffer[BUFFER_MAX];
  while ((option = getopt_long(argc, argv, short_options, long_options,
                               NULL)) != -1) {
    switch (option) {
      case 'e':
        opt->e = 1;
        snprintf(buffer, BUFFER_MAX, "%s", optarg);
        break;
      case 'i':
        opt->i = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'h':
        opt->h = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'f':
        opt->f = 1;
        snprintf(file_w_pattern, BUFFER_MAX, "%s", optarg);
        break;
      case 'o':
        opt->o = 1;
        break;
      case '?':
        printf("usage: grep [-e:ivclnhsf:o][file ...]\n");
        flag = -1;
        break;
    }
  }
  flag = pattern_creator(argv, opt, pattern_v, file_w_pattern, buffer);
  return flag;
}

void search_match(char **argv, options *opt, FILE *file_name, int pattern_c,
                  char *pattern_v, int file_counter, int fc) {
  char file_buffer[BUFFER_MAX] = {0}, test[BUFFER_MAX] = {0};
  regex_t reg;
  regmatch_t match[1];
  int string_match_counter = 0, match_l = 0, line_numbers = 1, md = 0;
  for (int i = 0; i < pattern_c; i++) test[i] = pattern_v[i];
  regcomp(&reg, test, opt->i ? REG_ICASE : REG_EXTENDED);
  int v_flag = opt->v ? 1 : 0, current_file = 0;
  while (fgets(file_buffer, BUFFER_MAX, file_name)) {
    int match_done = regexec(&reg, file_buffer, 1, match, REG_NOTEOL);
    if (match_done == v_flag && opt->c) string_match_counter++;
    if (!opt->l && !opt->c) {
      if (match_done == v_flag && !opt->h && file_counter > 1)
        printf("%s:", argv[fc]);
      if (match_done == v_flag &&
          (opt->e || opt->f || opt->h || opt->v || opt->i))
        md = 1;
      if (match_done == v_flag && opt->n) {
        printf("%d:", line_numbers++);
        md = 1;
      } else
        line_numbers++;
      if (match_done == 0 && opt->o && !opt->v) {
        for (int i = match[0].rm_so; i < match[0].rm_eo; i++) {
          printf("%c", file_buffer[i]);
          md = 0;
        }
        printf("\n");
      }
      if (md) {
        fprintf(stdout, "%s", file_buffer);
        last_char(file_buffer);
        md = 0;
      }
    } else {
      if (match_done == v_flag && opt->l) match_l = 1;
    }
    if (!opt->c && !opt->l && md == 0) {
    }
  }
  regfree(&reg);
  current_file++;
  if (opt->c) {
    if (opt->l && string_match_counter != 0) string_match_counter = 1;
    if (file_counter > 1 && !opt->h) {
      printf("%s:", argv[fc]);
      printf("%d\n", string_match_counter);
    } else
      printf("%d\n", string_match_counter);
  }
  if (opt->l && match_l) printf("%s\n", argv[fc]);
}

int pattern_creator(char **argv, options *opt, char *pattern_v,
                    char *file_w_pattern, char *buffer) {
  int i = 0;
  char test[BUFFER_MAX];
  if (!opt->e && !opt->f) {
    snprintf(buffer, BUFFER_MAX, "%s", argv[optind]);
    optind += 1;
    for (int k = 0; buffer[k] != '\0'; k++) pattern_v[i++] = buffer[k];
    pattern_v[i++] = '|';
  }
  if (opt->e && !opt->f) {
    for (int k = 0; buffer[k] != '\0'; k++) pattern_v[i++] = buffer[k];
    pattern_v[i++] = '|';
  }
  if (!opt->e && opt->f) {
    FILE *pattern_file;
    pattern_file = fopen(file_w_pattern, "r");
    if (pattern_file != NULL) {
      while (fgets(test, BUFFER_MAX, pattern_file)) {
        for (int k = 0; test[k] != '\0'; k++) {
          if (test[k] != 10 && test[k] != 13) pattern_v[i++] = test[k];
          if (test[k] == 10 || test[k] == 13) pattern_v[i++] = '|';
        }
      }
      fclose(pattern_file);
    } else {
      if (!opt->s)
        fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                file_w_pattern);
      i = -1;
    }
  }
  if (pattern_v[i - 1] == '|') pattern_v[i - 1] = '\0';
  return i;
}

int counter_of_file(int argc, char **argv) {
  int fc = optind, file_counter = 0;
  FILE *file_name;
  while (fc < argc) {
    file_name = fopen(argv[fc], "r");
    if (file_name != NULL) file_counter++;
    fc++;
    fclose(file_name);
  }
  return file_counter;
}

void last_char(char *file_buffer) {
  int lenght = strlen(file_buffer);
  char last_char = file_buffer[lenght];
  if (last_char != '\0' && last_char != '\n') printf("\n");
}