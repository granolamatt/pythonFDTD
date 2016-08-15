#include "Python.h" // should be before any standard headers
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

// spam example from python and threading from https://gist.github.com/liuyu81/3473376

static void*
non_python_thread(void *python_callback) {
  //XXX without PyEval_InitThreads() it produces:
  // Fatal Python error: PyEval_SaveThread: NULL tstate
  //XXX with PyEval_InitThreads() it deadlocks!!
  //XXX without PyGILState_*() (with/without PyEval_InitThreads()) it produces:
  // Exception KeyError: KeyError(139794956588800,)
  // in <module 'threading' from '/usr/lib/python2.7/threading.pyc'> ignored
    usleep(10000000);
  PyGILState_STATE state = PyGILState_Ensure();
  
  // print something
  PyObject* stdout = PySys_GetObject("stdout");
  if (stdout == NULL) {
    PyErr_SetString(PyExc_RuntimeError,  "no stdout");
    goto done;
  }
  Py_INCREF(stdout);
  if (PyFile_WriteString("non-python thread\n", stdout) != 0) {
    // a write error
    Py_DECREF(stdout);
    goto done;
  }
  else {
#if PY_MAJOR_VERSION < 3
      PyFile_SoftSpace(stdout, 0); // softspace = False after a newline
#endif
    Py_DECREF(stdout);
  }

  // call python callback
  PyObject* ignored = PyObject_CallFunctionObjArgs(python_callback, NULL);
  Py_XDECREF(ignored); // `ignored` may be NULL
  // fall through
 done:
  PyGILState_Release(state);
  return NULL; //NOTE: it doesn't propagate exceptions
}

static PyObject *
spawn_non_python_thread(PyObject *self, PyObject *args) {
 /* it should be safe to call PyEval_InitThreads(): current function holds
    GIL or it is started from the main thread */
  //XXX see the above comments near PyGILState_Ensure
  
  // I would suggest calling this in the module initializer, rather than on a
  // per-function basis. It is a matter of efficiency, not safety, though.
  // -- by LIU Yu <liuyu@openjudge.net>
  //// PyEval_InitThreads();
  
  PyObject* ret = NULL;
  PyObject* python_callback = NULL;
  if (!PyArg_ParseTuple(args, "O:spawn_non_python_thread", &python_callback))
    return NULL; // propagate exception
  Py_INCREF(python_callback); // hold on to it until the thread is finished
  
  // Give up GIL when doing C thread creation, this prevents the deadlock
  // caused by double-locking the GIL.
  // -- by LIU Yu <liuyu@openjudge.net>
  Py_BEGIN_ALLOW_THREADS
  
  pthread_t tid;
  int s = 0;
  // start a single non-python thread
  if ((s = pthread_create(&tid, NULL, non_python_thread, python_callback))!= 0) {
    errno = s; // errno!!
    // Get back GIL so that we can safely call Python API's
    // -- by LIU Yu <liuyu@openjudge.net>
    Py_BLOCK_THREADS
    PyErr_SetFromErrno(PyExc_OSError); goto done;
  }
  // join it
//  else if ((s = pthread_join(tid, NULL)) != 0) {
//    errno = s; // errno!!
//    Py_BLOCK_THREADS
//    PyErr_SetFromErrno(PyExc_OSError); goto done;
//  }
  // check whether non_python_thread raised any Python exceptions
  else {
    Py_BLOCK_THREADS
    if (PyErr_Occurred() == NULL) {  // no error
      ret = Py_None; Py_INCREF(ret);
    }
  }
  // fall through
 done:
  Py_UNBLOCK_THREADS
  Py_END_ALLOW_THREADS
  Py_DECREF(python_callback);
  return ret;
}

static PyMethodDef
module_functions[] = {
    { "spawn_non_python_thread", spawn_non_python_thread,
      METH_VARARGS, "func docstring" },
    { NULL }
};

// http://python3porting.com/cextensions.html
#if PY_MAJOR_VERSION >= 3
  #define MOD_ERROR_VAL NULL
  #define MOD_SUCCESS_VAL(val) val
  #define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
  #define MOD_DEF(ob, name, doc, methods) \
          static struct PyModuleDef moduledef = { \
            PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
          ob = PyModule_Create(&moduledef);
#else
  #define MOD_ERROR_VAL
  #define MOD_SUCCESS_VAL(val)
  #define MOD_INIT(name) void init##name(void)
  #define MOD_DEF(ob, name, doc, methods) \
          ob = Py_InitModule3(name, methods, doc);
#endif

MOD_INIT(spam)
{
    PyObject *m = NULL;

    MOD_DEF(m, "spam", "module docstring",  module_functions)

    if (m == NULL)
        return MOD_ERROR_VAL;
    
    // The decref will not destroy the imported module because another reference
    // reference is held by sys.module until the interpreter exits. But it seems
    // PY3K is planning to add module unload support. I have no idea what will
    // happen by that time.
    // -- by LIU Yu <liuyu@openjudge.net>
    PyObject * mod = PyImport_ImportModule("threading");
    Py_XDECREF(mod);
    
    PyEval_InitThreads();
    
    return MOD_SUCCESS_VAL(m);
}

