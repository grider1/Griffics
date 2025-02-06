TARGET := main
SRC := main.cpp Draw.cpp Object3D.cpp Matrix.cpp

$(TARGET): $(SRC)
	clang++ -std=c++17 -I/opt/homebrew/Cellar/sdl2/2.30.11/include -L/opt/homebrew/Cellar/sdl2/2.30.11/lib -lSDL2 -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
	
