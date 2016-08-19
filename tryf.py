import time
from cffi import FFI

ffi = FFI()
ffi.cdef("""
    typedef struct fdtdres {
      char*** data;
      long size;
    } fdtdres;

    void* runfdtd(int(*)(fdtdres*));
    void join(void* thread);
""")

#int reduce(int(*)(int, int), int* lst, int len);

C = ffi.dlopen("./spam.so")

@ffi.callback("int(fdtdres*)")
def add(x):
   print "Now update the webpage with this data"
   #time.sleep(5)
   return 0

toyfdtd = C.runfdtd(add)
C.join(toyfdtd)
#lst = [1, 2, 3]
#print C.reduce(add, lst, len(lst))
