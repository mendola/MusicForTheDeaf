This build takes takes a .wav audio file and performs a real-optimized FFT (from the kissFFT library) for sequential time chunks of the audio file. For each time chunk, it finds a peak frequency in the Bass, Mid, and Treble bands.

To compile the code in linux with GCC compiler
	*  Change line 20 of readWav.h to reflect the path to your music library (the directory where your .wav files are stored)
	*  type "make readWavRealTime" to build the real-time software. This outputs the peak frequencies as they occur in the time domain of the original file. Run with: './readWav <song.wav>' where <song.wav> is contained in the path you inserted into the makefile
	*	type "make readWav" to build the quick display version. When run, this outputs the peak frequencies of the whole song immediately without waiting for their respective timings in the audio file. Run with: './readWav <song.wav>