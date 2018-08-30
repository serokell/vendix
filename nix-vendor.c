#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/limits.h>
#include <sys/stat.h>

#include <errno.h>

int (*real_access)(const char *, int);
int (*real_chdir)(const char *);
void *(*real_dlopen)(const char *, int);
int (*real_eaccess)(const char *, int);
int (*real_execvp)(const char *, char *const[]);
int (*real_euidaccess)(const char *, int);
int (*real_faccessat)(int, const char *, int, int);
FILE *(*real_fopen)(const char *, const char *);
FILE *(*real_freopen)(const char *, const char *, FILE *);
int (*real_fstatat)(int, const char *, struct stat *, int);
int (*real_open)(const char *, int, ...);
int (*real_openat)(int, const char *, int, ...);
DIR *(*real_opendir)(const char *);
long (*real_pathconf)(const char *, int);
char *(*real_realpath)(const char *, char *);
int (*real_stat)(const char *, struct stat *);
int (*real_system)(const char *);

void init() __attribute__((constructor)) {
  real_access = dlsym(RTLD_NEXT, "access");
  real_chdir = dlsym(RTLD_NEXT, "chdir");
  real_dlopen = dlsym(RTLD_NEXT, "dlopen");
  real_eaccess = dlsym(RTLD_NEXT, "eaccess");
  real_execvp = dlsym(RTLD_NEXT, "execvp");
  real_euidaccess = dlsym(RTLD_NEXT, "euidaccess");
  real_faccessat = dlsym(RTLD_NEXT, "faccessat");
  real_fopen = dlsym(RTLD_NEXT, "fopen");
  real_freopen = dlsym(RTLD_NEXT, "freopen");
  real_fstatat = dlsym(RTLD_NEXT, "fstatat");
  real_open = dlsym(RTLD_NEXT, "open");
  real_openat = dlsym(RTLD_NEXT, "openat");
  real_opendir = dlsym(RTLD_NEXT, "opendir");
  real_pathconf = dlsym(RTLD_NEXT, "pathconf");
  real_realpath = dlsym(RTLD_NEXT, "realpath");
  real_stat = dlsym(RTLD_NEXT, "stat");
  real_system = dlsym(RTLD_NEXT, "system");
}

#define NIX_PATH "/nix"

int access(const char *path, int mode) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_access(path_override, mode);

  if (path_override != path)
    free(path_override);

  return ret;
}

int chdir(const char *path) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_chdir(path_override);

  if (path_override != path)
    free(path_override);

  return ret;
}

void *dlopen(const char *path, int flags) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  void *ret = real_dlopen(path_override, flags);

  if (path_override != path)
    free(path_override);

  return ret;
}

int execvp(const char *path, char *const argv[]) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_execvp(path_override, argv);

  if (path_override != path)
    free(path_override);

  return ret;
}

int eaccess(const char *path, int mode) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_eaccess(path_override, mode);

  if (path_override != path)
    free(path_override);

  return ret;
}

int euidaccess(const char *path, int mode) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_euidaccess(path_override, mode);

  if (path_override != path)
    free(path_override);

  return ret;
}

int faccessat(int fd, const char *path, int mode, int flags) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_faccessat(fd, path_override, mode, flags);

  if (path_override != path)
    free(path_override);

  return ret;
}

FILE *fopen(const char *path, const char *mode) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  FILE *ret = real_fopen(path_override, mode);

  if (path_override != path)
    free(path_override);

  return ret;
}

FILE *freopen(const char *path, const char *mode, FILE *stream) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  FILE *ret = real_freopen(path_override, mode, stream);

  if (path_override != path)
    free(path_override);

  return ret;
}

int fstatat(int fd, const char *path, struct stat *buf, int flag) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = real_fstatat(fd, path_override, buf, flag);

  if (path_override != path)
    free(path_override);

  return ret;
}

int lstat(const char *path, struct stat *buf) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = lstat64(path_override, (struct stat64 *)buf);

  if (path_override != path)
    free(path_override);

  return ret;
}

int open(const char *path, int oflag, ...) {
  va_list args;
  va_start(args, oflag);

  char *path_override = (char *)path;
  int mode = 0;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  if (oflag & O_CREAT)
    mode = va_arg(args, int);

  va_end(args);

  int ret = real_open(path_override, oflag, mode);

  if (path_override != path)
    free(path_override);

  return ret;
}

int openat(int fd, const char *path, int oflag, ...) {
  va_list args;
  va_start(args, oflag);

  char *path_override = (char *)path;

  int mode = 0;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  if (oflag & O_CREAT)
    mode = va_arg(args, int);

  va_end(args);

  int ret = real_openat(fd, path_override, oflag, mode);

  if (path_override != path)
    free(path_override);

  return ret;
}

DIR *opendir(const char *path) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  DIR *ret = real_opendir(path_override);

  if (path_override != path)
    free(path_override);

  return ret;
}

long pathconf(const char *path, int name) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  long ret = real_pathconf(path_override, name);

  if (path_override != path)
    free(path_override);

  return ret;
}

char *realpath(const char *path, char *resolved_path) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  char *ret = real_realpath(path_override, resolved_path);

  if (path_override != path)
    free(path_override);

  return ret;
}

int stat(const char *path, struct stat *buf) {
  char *path_override = (char *)path;

  if (strncmp(NIX_PATH, path, strlen(NIX_PATH)) == 0)
    asprintf(&path_override, "%s%s", getenv("BUNDLE_ROOT"), path);

  int ret = stat64(path_override, (struct stat64 *)buf);

  if (path_override != path)
    free(path_override);

  return ret;
}

int system(const char *command) {
  char *command_override = (char *)command;

  if (strncmp(NIX_PATH, command, strlen(NIX_PATH)) == 0)
    asprintf(&command_override, "%s%s", getenv("BUNDLE_ROOT"), command);

  int ret = system(command_override);

  if (command_override != command)
    free(command_override);

  return ret;
}
