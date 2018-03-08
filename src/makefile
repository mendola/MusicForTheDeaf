CC=g++
CFLAGS=-I../kiss_fft130 -I../kiss_fft130/tools -I.
CFLAGS+=-DFIXED_POINT=16
#CFlags+= /home/alex/kiss_fft130/kiss_fft.c /home/alex/kiss_fft130/tools/kiss_fftr.c
KISSFFTDEPS= ../kiss_fft130/kiss_fft.c ../kiss_fft130/tools/kiss_fftr.c
DEPS= readWav.cpp runTestReadWav.cpp
REALTIME=-DREALTIME=true

readWav: $(DEPS)
	$(CC) $(DEPS) $(KISSFFTDEPS) $(CFLAGS) -o readWav

readWavRealTime: $(DEPS)
	$(CC) $(DEPS) $(KISSFFTDEPS) $(CFLAGS) $(REALTIME) -o readWavRealTime
#readWav: readWav.cpp runTestReadWav.cpp

clean:
	rm *.o ./readWav ./readWavRealTime

