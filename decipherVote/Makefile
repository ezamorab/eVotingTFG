HELIB = /home/espe/Escritorio/HElib-master/src
CPPFLAGS=-Wall -DNDEBUG -g2 -I$(HELIB)
LIBS=$(HELIB)/fhe.a -lntl -lgmp -lm -pthread
TARGET= decryptVote

.PHONY: all clean

all: $(TARGET)

$(TARGET): decrypt.cpp
	g++ $(CPPFLAGS) $< -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

