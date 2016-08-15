This is a quick test of using python c to expose my c and c++ functions to a web gui in chrome.

The project does a simple FDTD simulation based on ToyFDTD http://dougneubauer.com/wp-content/uploads/wdata/toyfdtd/toyfdtd.html

ToyFDTD uses AnimaBob https://sourceforge.net/projects/animabob to display the results, but I want to push the results to chrome and use THREE.js http://threejs.org/ instead.

I am a c c++ programmer of math and always plan to be.  I have experimented with Java swing and web guis to display my results.  A few weeks ago I worked on a python project at work and like how easy it is to script up.  This project is to test the idea of building my math in c++ and then pushing that to python as a python c module.  One of the things then that I want to determine is the difference between c modules and just interfacing to a library with cffi.  This project is not about FDTDs or guis, it is to learn another route to get information from a model to a human.  FDTD is a good example of that model and I am trying python and THREE.js as the interface.
