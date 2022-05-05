APP_NAME=midi-test

OBJS=Binasc.o Options.o MidiMessage.o MidiEvent.o MidiEventList.o MidiFile.o

CXX = g++ -m64 -std=c++11
CXXFLAGS = -I. -O3 -Wall

default: parallel

midi-test: midi-test.cpp chord.cpp note.cpp voicing.cpp midi.cpp
	g++ -o midi-test midifile/Binasc.cpp midifile/Options.cpp midifile/MidiMessage.cpp midifile/MidiEvent.cpp midifile/MidiEventList.cpp midifile/MidiFile.cpp midi-test.cpp chord.cpp note.cpp voicing.cpp midi.cpp

sequential: main.cpp chord.cpp note.cpp voicing.cpp midi.cpp solver-sequential.cpp
	g++ -o sequential -fopenmp midifile/Binasc.cpp midifile/Options.cpp midifile/MidiMessage.cpp midifile/MidiEvent.cpp midifile/MidiEventList.cpp midifile/MidiFile.cpp main.cpp chord.cpp note.cpp voicing.cpp midi.cpp solver-sequential.cpp

parallel: main.cpp chord.cpp note.cpp voicing.cpp midi.cpp solver-parallel.cpp
	g++ -o parallel -fopenmp midifile/Binasc.cpp midifile/Options.cpp midifile/MidiMessage.cpp midifile/MidiEvent.cpp midifile/MidiEventList.cpp midifile/MidiFile.cpp main.cpp chord.cpp note.cpp voicing.cpp midi.cpp solver-parallel.cpp

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
