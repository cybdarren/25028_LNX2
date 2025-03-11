all: video_receiver.cpp
	$(CXX) $(CXXFLAGS) `pkg-config libegt --cflags --libs` -Og video_receiver.cpp -g -o video_receiver.bin

clean:
	rm -rf video_receiver.bin
	