#ifndef _TOYFDTD1_H_
#define _TOYFDTD1_H_

#include "gm/Thread.h"


typedef struct fdtdres {
	char*** data;
	long size;
} fdtdres;

class ToyFDTD : public gm::Thread {

public:
	ToyFDTD();
	~ToyFDTD();
	void run();
    template<typename Func>
    void setPythonCallback (Func func) {
        python_callback = func;
    }
private:
	std::function <int (int first, int second)>python_callback;

};


#ifdef __cplusplus
extern "C" {
#endif

void* runfdtd(int(*fn)(int, int));
void join(void* thread);
//int reduce(int(*fn)(int, int), int* lst, int len);

#ifdef __cplusplus
}
#endif

#endif //_TOYFDTD1_H_
