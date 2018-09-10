#define _GNU_SOURCE

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dirent.h>   // opendir
#include <dlfcn.h>    // dlopen
#include <fcntl.h>    // open, openat
#include <fts.h>      // fts_open, fts_open_b
#include <ftw.h>      // ftw, nftw
#include <sys/stat.h> // stat, lstat, fstatat

#define NIX_HOME "/nix"

// TODO: Call getenv("DYLD_ROOT_PATH") only once
static char *expand_store(const char *source) {
  char *target = NULL;

  if (strncmp(NIX_HOME, source, strlen(NIX_HOME)) == 0)
    asprintf(&target, "%s%s", getenv("DYLD_ROOT_PATH"), source);

  return target;
}

static void freep(void *p) { free(*(void **)p); }
#define raii __attribute__((cleanup(freep)))

#define CONCAT_HELPER(x, y) x##y
#define CONCAT(x, y) CONCAT_HELPER(x, y)

#define OR(x, y) x ? x : y

#define EXPAND_STORE_TO(source, target)                                        \
  raii char *target = expand_store(source);                                    \
  source = OR(target, source)

#define EXPAND_STORE(source)                                                   \
  EXPAND_STORE_TO(source, CONCAT(store_target, __COUNTER__))

int access_wrapper(const char *path, int mode) {
  EXPAND_STORE(path);
  return access(path, mode);
}

int chdir_wrapper(const char *path) {
  EXPAND_STORE(path);
  return chdir(path);
}

void *dlopen_wrapper(const char *path, int mode) {
  EXPAND_STORE(path);
  return dlopen(path, mode);
}

int execl_wrapper(const char *path, const char *arg0, ...) {
  va_list args;
  va_start(args, arg0);

  raii const char **argv = malloc(sizeof(char *));

  size_t i = 0;
  argv[i] = arg0;

  while (argv[i] != NULL) {
    argv = realloc(argv, (++i + 1) * sizeof(char *));
    argv[i] = va_arg(args, const char *);
  }

  va_end(args);

  EXPAND_STORE(path);
  return execvp(path, argv);
}

int execle_wrapper(const char *path, const char *arg0, ...) {
  return -1; // TODO
}

int execlp_wrapper(const char *path, const char *arg0, ...) {
  return -1; // TODO
}

int execv_wrapper(const char *path, char *const argv[]) {
  EXPAND_STORE(path);
  return execv(path, argv);
}

int execve_wrapper(const char *path, char *const argv[], char *const envp[]) {
  EXPAND_STORE(path);
  return execve(path, argv, envp);
}

int execvp_wrapper(const char *path, char *const argv[]) {
  EXPAND_STORE(path);
  return execvp(path, argv);
}

int execvP_wrapper(const char *path, const char *searched_path,
                   char *const argv[]) {
  EXPAND_STORE(path);
  return execvP(path, searched_path, argv);
}

FILE *fopen_wrapper(const char *path, const char *mode) {
  EXPAND_STORE(path);
  return fopen(path, mode);
}

FILE *freopen_wrapper(const char *path, const char *mode, FILE *stream) {
  EXPAND_STORE(path);
  return freopen(path, mode, stream);
}

int fstatat_wrapper(int fd, const char *path, struct stat *buf, int flag) {
  EXPAND_STORE(path);
  return fstatat(fd, path, buf, flag);
}

void **copy_array(const void *const *array) {
  void *const *ptr = (void *const)array;
  size_t len = 0;

  while (ptr[len++] != NULL)
    ;

  void **array_copy = malloc(len * sizeof(void *));
  memcpy(array_copy, array, len * sizeof(void *));

  return array_copy;
}

FTS *fts_open_wrapper(char *const *path_argv, int options,
                      int (*compar)(const FTSENT **, const FTSENT **)) {
  raii char **path_argv_copy =
      (char **)copy_array((const void *const *)path_argv);

  EXPAND_STORE(path_argv_copy[0]);
  return fts_open(path_argv_copy, options, compar);
}

FTS *fts_open_b_wrapper(char *const *path_argv, int options,
                        int (^compar)(const FTSENT **, const FTSENT **)) {
  raii char **path_argv_copy =
      (char **)copy_array((const void *const *)path_argv);

  EXPAND_STORE(path_argv_copy[0]);
  return fts_open_b(path_argv, options, compar);
}

int ftw_wrapper(const char *path,
                int (*fn)(const char *, const struct stat *ptr, int flag),
                int depth) {
  EXPAND_STORE(path);
  return ftw(path, fn, depth);
}

int lstat_wrapper(const char *path, struct stat *buf) {
  EXPAND_STORE(path);
  return lstat(path, buf);
}

int nftw_wrapper(const char *path,
                 int (*fn)(const char *, const struct stat *ptr, int flag,
                           struct FTW *),
                 int depth, int flags) {
  EXPAND_STORE(path);
  return nftw(path, fn, depth, flags);
}

int open_wrapper(const char *path, int oflag, ...) {
  va_list args;
  va_start(args, oflag);

  int mode = 0;

  if (oflag & O_CREAT)
    mode = va_arg(args, int);

  va_end(args);

  EXPAND_STORE(path);
  return open(path, oflag, mode);
}

int openat_wrapper(int fd, const char *path, int oflag, ...) {
  va_list args;
  va_start(args, oflag);

  int mode = 0;

  if (oflag & O_CREAT)
    mode = va_arg(args, int);

  va_end(args);

  EXPAND_STORE(path);
  return openat(fd, path, oflag, mode);
}

DIR *opendir_wrapper(const char *path) {
  EXPAND_STORE(path);
  return opendir(path);
}

long pathconf_wrapper(const char *path, int name) {
  EXPAND_STORE(path);
  return pathconf(path, name);
}

char *realpath_wrapper(const char *path, char *resolved_path) {
  EXPAND_STORE(path);
  return realpath(path, resolved_path);
}

int stat_wrapper(const char *path, struct stat *buf) {
  EXPAND_STORE(path);
  return stat(path, buf);
}

// https://opensource.apple.com/source/dyld/dyld-210.2.3/include/mach-o/dyld-interposing.h
#define DYLD_INTERPOSE(_replacement, _replacee)                                \
  __attribute__((used)) static struct {                                        \
    const void *replacement;                                                   \
    const void *replacee;                                                      \
  } _interpose_##_replacee __attribute__((section("__DATA,__interpose"))) = {  \
      (const void *)(unsigned long)&_replacement,                              \
      (const void *)(unsigned long)&_replacee};

#define WRAP(f) DYLD_INTERPOSE(f##_wrapper, f)

WRAP(access);
WRAP(chdir);
WRAP(dlopen);
WRAP(execl);
WRAP(execv);
WRAP(execve);
WRAP(execvp);
WRAP(execvP);
WRAP(fopen);
WRAP(freopen);
WRAP(fstatat);
WRAP(fts_open);
WRAP(fts_open_b);
WRAP(ftw);
WRAP(lstat);
WRAP(nftw);
WRAP(open);
WRAP(openat);
WRAP(opendir);
WRAP(pathconf);
WRAP(realpath);
WRAP(stat);
