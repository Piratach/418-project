APP_NAME=midi-test

OBJS=Binasc.o Options.o MidiMessage.o MidiEvent.o MidiEventList.o MidiFile.o

CXX = g++ -m64 -std=c++11
CXXFLAGS = -I. -O3 -Wall

default: solver-test

midi-test: midi-test.cpp chord.cpp note.cpp voicing.cpp midi.cpp
	g++ -o midi-test midifile/Binasc.cpp midifile/Options.cpp midifile/MidiMessage.cpp midifile/MidiEvent.cpp midifile/MidiEventList.cpp midifile/MidiFile.cpp midi-test.cpp chord.cpp note.cpp voicing.cpp midi.cpp

solver-test: main.cpp chord.cpp note.cpp voicing.cpp midi.cpp solver.cpp
	g++ -o solver-test midifile/Binasc.cpp midifile/Options.cpp midifile/MidiMessage.cpp midifile/MidiEvent.cpp midifile/MidiEventList.cpp midifile/MidiFile.cpp main.cpp chord.cpp note.cpp voicing.cpp midi.cpp solver.cpp

clean:
	/bin/rm -rf *.o
	/bin/rm solver-test
	/bin/rm midi-test

# default: $(APP_NAME)

# $(APP_NAME): midifile/$(OBJS)
	# $(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# %.o: %.cpp
	# $(CXX) $< $(CXXFLAGS) -c -o $@

# clean:
	# /bin/rm -rf *~ *.o $(APP_NAME)
