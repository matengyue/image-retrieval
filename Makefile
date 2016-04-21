#CFLAGS = -std=c++11
CFLAGS = -std=c++11 -fopenmp

LIBS =  -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videostab -lopencv_calib3d -lopencv_contrib

#INCPATH = -I/usr/local/Cellar/opencv/2.4.11_1/include
#LIBPATH = -L/usr/local/Cellar/opencv/2.4.11_1/lib

INCPATH = -I/usr/local/include -I/home/matengyue/image-retrieval-1.0/cppsugar
#INCPATH = -I/usr/local/include -I/Users/willard/Public/github/image-retrieval/bag-of-words-stable-version/cppsugar
LIBPATH = -L/usr/local/lib

all:    main_ search scale HSV GETHSV Kmeans Kmeans_y getKmeans getPct
main_:    ./src/main_.cpp ./src/BoWBuilder.h
	g++ $(CFLAGS) $(INCPATH) $(LIBPATH) -o ./bin/index ./src/main_.cpp $(LIBS)
search:    ./src/search.cpp ./src/BoWBuilder.h
	g++ $(CFLAGS) $(INCPATH) $(LIBPATH) `pkg-config --cflags opencv` -o ./bin/search ./src/search.cpp `pkg-config --libs opencv` $(LIBS)
scale:    ./src/scale.cpp ./src/BoWBuilder.h
	g++ $(CFLAGS) $(INCPATH) $(LIBPATH) -o ./bin/scale ./src/scale.cpp $(LIBS)
HSV:    ./src/HSV.cpp
	g++ $(CFLAGS) `pkg-config --cflags opencv` -o ./bin/hsv ./src/HSV.cpp `pkg-config --libs opencv`
GETHSV:    ./src/getHSV.cpp
	g++ $(CFLAGS) `pkg-config --cflags opencv` -o ./bin/gethsv ./src/getHSV.cpp `pkg-config --libs opencv`
Kmeans:    ./src/Kmeans.cpp
	g++ $(CFLAGS) `pkg-config --cflags opencv` -o ./bin/kmeans ./src/Kmeans.cpp `pkg-config --libs opencv`
Kmeans_y:    ./src/Kmeans_y.cpp
	g++ $(CFLAGS) `pkg-config --cflags opencv` -o ./bin/kmeans_y ./src/Kmeans_y.cpp `pkg-config --libs opencv`
getKmeans:    ./src/getKmeans.cpp
	g++ $(CFLAGS) `pkg-config --cflags opencv` -o ./bin/getkmeans ./src/getKmeans.cpp `pkg-config --libs opencv`
getPct:	   ./src/getPct.cpp
	g++ -o ./bin/getpct ./src/getPct.cpp
clean:
	rm -f index search scale hsv gethsv kmeans kmeans_y getkmeans getpct
