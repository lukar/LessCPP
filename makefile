less: less.cc block_shape.cc
	g++ less.cc block_shape.cc -o less `pkg-config --libs --cflags sfml-graphics`
