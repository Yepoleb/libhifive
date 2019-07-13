#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/times.h>

#include "uart.h"

#define SYS_STUB errno = ENOSYS; return -1;

void _system_init(void)
{
    uart_init(UART0, UART_8N1, 139);
}

ssize_t _write(int file, const void* ptr, size_t len)
{
    if (file != STDOUT_FILENO && file != STDERR_FILENO) {
        errno = ENOSYS;
        return -1;
    }

    uart_write_blocking(UART0, ptr, len);
    return 0;
}

int _getpid()
{
    return 1;
}

int _access(const char* file, int mode) {SYS_STUB}
int _chdir(const char* path) {SYS_STUB}
int _chmod(const char* path, mode_t mode) {SYS_STUB}
int _chown(const char* path, uid_t owner, gid_t group) {SYS_STUB}
int _close(int file) {SYS_STUB}
int _execve(const char *name, char *const argv[], char *const env[]) {SYS_STUB}
//~ int _exit(int exit_status) {SYS_STUB}
int _faccessat(int dirfd, const char *file, int mode, int flags) {SYS_STUB}
int _fork() {SYS_STUB}
int _fstat(int file, struct stat *st) {SYS_STUB}
int _fstatat(int dirfd, const char *file, struct stat *st, int flags) {SYS_STUB}
int _ftime(struct timeb *tp) {SYS_STUB}
char* _getcwd(char *buf, size_t size) {errno = ENOSYS; return NULL;}
int _gettimeofday(struct timeval *tp, void *tzp) {SYS_STUB}
int _isatty(int file) {SYS_STUB}
int _kill(int pid, int sig) {SYS_STUB}
int _link(const char *old_name, const char *new_name) {SYS_STUB}
int _lseek(int file, off_t ptr, int dir) {SYS_STUB}
int _lstat(const char *file, struct stat *st) {SYS_STUB}
int _open(const char *name, int flags, int mode) {SYS_STUB}
int _openat(int dirfd, const char *name, int flags, int mode) {SYS_STUB}
ssize_t _read(int file, void *ptr, size_t len) {SYS_STUB}
int _stat(const char *file, struct stat *st) {SYS_STUB}
long _sysconf(int name) {SYS_STUB}
clock_t _times(struct tms *buf) {SYS_STUB}
int _unlink(const char *name) {SYS_STUB}
struct utimbuf;
int _utime(const char *path, const struct utimbuf *times) {SYS_STUB}
int _wait(int *status) {SYS_STUB}

/* brk, sbrk */

extern char segment_heap_target_start;
extern char segment_heap_target_end;
static char *brk = &segment_heap_target_start;

int _brk(void *addr)
{
    if ((char*)addr < &segment_heap_target_end) {
        brk = addr;
        return 0;
    } else {
        return 1;
    }
}

char* _sbrk(ptrdiff_t incr)
{
    char *old = brk;

    /* If __heap_size == 0, we can't allocate memory on the heap */
    if(&segment_heap_target_start == &segment_heap_target_end) {
        return NULL;
    }

    /* Don't move the break past the end of the heap */
    if ((brk + incr) < &segment_heap_target_end) {
        brk += incr;
    } else {
        brk = &segment_heap_target_end;
    }

    return old;
}
