#ifndef MMAN_H
#define MMAN_H

#include <Win32_Interop/win32fixes.h>

#include <unistd.h>

static void *mmap(void *addr,
                  size_t len,
                  int prot,
                  int flags,
                  int fd,
                  off_t off) {
  void *result = (void *) (-1);
  if (!addr && prot == MAP_SHARED) {
    /* HACK: we're assuming handle sizes can't exceed 32 bits, which is wrong...
     * but works for now. */
    void *ptr = MapViewOfFile((HANDLE)(intptr_t) fd, FILE_MAP_ALL_ACCESS,
                              (DWORD)(off >> (CHAR_BIT * sizeof(DWORD))),
                              (DWORD) off, (SIZE_T) len);
    if (ptr) {
      result = ptr;
    }
  }
  return result;
}
static int munmap(void *addr, size_t length) {
  (void) length;
  return UnmapViewOfFile(addr) ? 0 : -1;
}

#endif /* MMAN_H */
