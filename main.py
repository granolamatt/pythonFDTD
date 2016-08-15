import time
import spam

def python_callback():
    print("python callback called")
    def python_thread():
        print("python thread started")
        print("python thread ended")
    # `threading` might implicitly call PyEval_InitThreads()
    import threading
    threading.Thread(target=python_thread).start()

# there should be no GIL yet (we have only one main thread)
spam.spawn_non_python_thread(python_callback)

while True:
    print "This prints once a second."
    time.sleep(1)  # Delay for 1 minute (60 seconds)

# if there is no KeyError in threading module; add time.sleep() to reproduce it:
####import time; time.sleep(1e-3)
#NOTE: if there is no new thread in python_callback or if there are
#multiple Python threads before calling the extension module then
#there is no error
print("exit main thread")
