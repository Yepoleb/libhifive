#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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

int _isatty(int file) {SYS_STUB}
int _close(int file) {SYS_STUB}
int _lseek(int file, off_t ptr, int dir) {SYS_STUB}
ssize_t _read(int file, void *ptr, size_t len) {SYS_STUB}
int _fstat(int file, struct stat *st) {SYS_STUB}

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
