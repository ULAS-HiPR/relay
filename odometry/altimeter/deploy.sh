g++ altimeter.cpp ../../lib/altimeter/BMP390.cpp ../../lib/altimeter/bmp3.c -o altimeter -lpigpio -lrt -lpthread
sudo ./altimeter