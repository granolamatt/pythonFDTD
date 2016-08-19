#ifndef _TOYFDTD1_H_
#define _TOYFDTD1_H_

#include "gm/Thread.h"


typedef struct fdtdres {
	int* data;
	int x;
	int y;
	int z;
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
	std::function <int (fdtdres* res)>python_callback;

};


#ifdef __cplusplus
extern "C" {
#endif

void* runfdtd(int(*fn)(fdtdres*));
void join(void* thread);
//int reduce(int(*fn)(int, int), int* lst, int len);

#ifdef __cplusplus
}
#endif

#endif //_TOYFDTD1_H_
