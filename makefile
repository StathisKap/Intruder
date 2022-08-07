CC = g++
CFLAGS = -ggdb -std=c++11
CFLAGS += `pkg-config --cflags --libs opencv4`
PICS_DIR = ./IntruderPics

Intruder: Intruder.cpp $(PICS_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(PICS_DIR):
	@mkdir $@
