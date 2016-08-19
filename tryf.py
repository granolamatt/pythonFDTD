import time
from cffi import FFI

ffi = FFI()
ffi.cdef("""
    typedef struct fdtdres {
      char*** data;
      long size;
    } fdtdres;
    void* runfdtd(int(*)(int, int));
    void join(void* thread);
""")

#int reduce(int(*)(int, int), int* lst, int len);

C = ffi.dlopen("./spam.so")

@ffi.callback("int(int, int)")
def add(x, y):
   print "made it to function"
   return x + y

toyfdtd = C.runfdtd(add)
C.join(toyfdtd)
#lst = [1, 2, 3]
#print C.reduce(add, lst, len(lst))
