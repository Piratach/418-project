APP_NAME=midi-test

OBJS=Binasc.o Options.o MidiMessage.o MidiEvent.o MidiEventList.o MidiFile.o

CXX = g++ -m64 -std=c++11
CXXFLAGS = -I. -O3 -Wall

default: midi-test

midi-test: midi-test.cpp midifile/Binasc.cpp midifile/Options.cpp
	g++ -o midi-test midi-test.cpp midifile/Binasc.cpp midifile/Options.cpp midifile/MidiMessage.cpp midifile/MidiEvent.cpp midifile/MidiEventList.cpp midifile/MidiFile.cpp

# default: $(APP_NAME)

# $(APP_NAME): midifile/$(OBJS)
	# $(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# %.o: %.cpp
	# $(CXX) $< $(CXXFLAGS) -c -o $@

# clean:
	# /bin/rm -rf *~ *.o $(APP_NAME)
