#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc > 3) {
    grep_parser(argc, argv);
  }
  return 0;
}

void grep_parser(int argc, char *argv[]) {
  grep_options flags = {0};
  int opt = 0;
  char buffer[BUFFER], pattern[BUFFER], file_name_buffer[225];
  size_t buf_size = BUFFER * sizeof(char);
  if ((buf_size >= sizeof(pattern)) && (buf_size >= sizeof(buffer)) &&
      (buf_size >= sizeof(file_name_buffer))) {
    memset(pattern, 0, sizeof(pattern));
    memset(buffer, 0, sizeof(buffer));
    memset(file_name_buffer, 0, sizeof(file_name_buffer));
    while ((opt = getopt_long(argc, argv, short_opt, long_opt, 0)) != -1) {
      switch_case(opt, &flags, buffer, file_name_buffer, argv);
      pattern_cr_e(argv, buffer, pattern, opt);
      memset(buffer, 0, sizeof(buffer));
    }
    int size = strlen(pattern);
    if (pattern[size - 1] == '|') pattern[size - 1] = '\0';
    other_pattern_cr(argc, argv, pattern, &flags, file_name_buffer);
  }
}

void switch_case(int opt, grep_options *flags, char *buffer, char *f_buffer,
                 char *argv[]) {
  switch (opt) {
    case 'e':
      flags->e = 1;
      snprintf(buffer, BUFFER, "%s", optarg);
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'i':
      flags->i = 1;
      break;
    case 'f':
      flags->f = 1;
      snprintf(f_buffer, 225, "%s", optarg);
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'o':
      flags->o = 1;
      break;
    case '?':
      printf("%s: illegal option", argv[0]);
      break;
  }
}

void pattern_cr_e(char *argv[], char *buffer, char *pattern, int options) {
  if (options == 'e') {
    int lb = strlen(buffer);
    if (pattern[0] != 1) {
      strncat(pattern, buffer, lb);
    } else
      memcpy(pattern, buffer, lb);
    char *test2 = "|";
    strncat(pattern, test2, 1);
  }
  if (options != 'e' && options != 'f' && argv[optind] != NULL) {
    char test[BUFFER];
    snprintf(test, BUFFER, "%s", argv[optind]);
    if (test[0] != '-') {
      int lb = strlen(argv[optind]);
      if (pattern[0] != 1) {
        strncat(pattern, argv[optind], lb);
      } else
        memcpy(pattern, argv[optind], lb);

      char *test2 = "|";
      strncat(pattern, test2, 1);
    }
  }
}

void other_pattern_cr(int argc, char *argv[], char *pattern,
                      grep_options *flags, char *file_name_buffer) {
  if (flags->f) {
    FILE *file = fopen(file_name_buffer, "r");
    if (file != NULL) {
      int lp = strlen(pattern);
      if (pattern[lp] == '\0') pattern[lp] = '|';
      char buffer[BUFFER];
      while (fgets(buffer, BUFFER, file)) {
        int lb = strlen(buffer);
        if (buffer[lb - 1] == 10 || buffer[lb - 1] == 13) buffer[lb - 1] = '|';
        if (pattern[0] != 1) {
          strncat(pattern, buffer, lb);
        } else {
          memcpy(pattern, buffer, lb);
        }
      }
      fclose(file);
    } else {
      if (!flags->s)
        printf("%s: %s: No such file or directory", argv[0], file_name_buffer);
    }
  }
  if (!flags->f && !flags->e) {
    int lb = strlen(argv[optind]);
    if (pattern[0] != '\0') {
      char test2[] = "|";
      strcat(pattern, test2);
      strncat(pattern, argv[optind], lb);
    } else
      memcpy(pattern, argv[optind], lb);
    optind += 1;
  }
  int size = strlen(pattern);
  if (pattern[size - 1] == '|') pattern[size - 1] = '\0';
  grep_reader(argc, argv, pattern, flags);
}

void grep_reader(int argc, char *argv[], char *pattern, grep_options *flags) {
  if (flags->l || flags->c) {
    flags->e = 0;
    flags->n = 0;
    flags->f = 0;
    flags->o = 0;
  }
  int current_file = optind;
  FILE *file_name;
  while (current_file < argc) {
    file_name = fopen(argv[current_file], "r");
    if (file_name != NULL) {
      grep_s21(argc, argv, flags, file_name, pattern, current_file);
      fclose(file_name);
    } else {
      if (!flags->s)
        printf("%s: %s: No such file or directory", argv[0],
               argv[current_file]);
    }
    current_file++;
  }
}

void grep_s21(int argc, char *argv[], grep_options *flags, FILE *file_name,
              char *pattern, int current_file) {
  char buffer[BUFFER];
  int v_match = flags->v ? 1 : 0, print = 0, number_string = 1,
      count_string = 0, match_l = 0, file_count = argc - optind;
  regex_t reg;
  regmatch_t pmatch[1];
  if (regcomp(&reg, pattern,
              flags->i ? REG_ICASE | REG_EXTENDED : REG_EXTENDED) == 0) {
    while (fgets(buffer, BUFFER, file_name)) {
      int match = regexec(&reg, buffer, 1, pmatch, REG_NOTEOL);
      if (match == v_match) print = 1;
      if (!flags->c && !flags->l) {
        if (!flags->h && match == v_match && file_count > 1)
          printf("%s:", argv[current_file]);
        if (flags->n && match == v_match) {
          printf("%d:", number_string++);
        } else
          number_string++;
      }
      if (flags->c && match == v_match) {
        count_string++;
        print = 0;
      }
      if (flags->l && match == v_match) {
        match_l = 1;
        count_string = 1;
        print = 0;
      }
      if (print) grep_print(flags, buffer, reg);
      print = 0;
    }
    if (flags->l || flags->c) {
      if (flags->c) {
        if (file_count > 1 && !flags->h) {
          printf("%s:", argv[current_file]);
          printf("%d\n", count_string);
        } else
          printf("%d\n", count_string);
      }
      if (flags->l && match_l) printf("%s\n", argv[current_file]);
    }
    regfree(&reg);
  }
}

void grep_print(grep_options *flags, char *buffer, regex_t reg) {
  int lenght = strlen(buffer), pos = 0;
  if (flags->o && !flags->v) {
    regmatch_t matches[1];
    while (regexec(&reg, buffer + pos, 1, matches, 0) == 0) {
      int start = pos + matches[0].rm_so;
      int end = pos + matches[0].rm_eo;
      printf("%.*s\n", end - start, buffer + start);
      pos += matches[0].rm_eo;
    }
  } else {
    for (int i = 0; i < lenght; i++) {
      if (buffer[i] != 0 && buffer[i] != 10) putchar(buffer[i]);
    }
    putchar('\n');
  }
}