CC = gcc
CFLAGS = -O3 -fopenmp

# Kaynak dosyaları
SRC = matrix.c
EXEC = matrix

# Varsayılan hedef (all)
all: $(EXEC)
	./$(EXEC) matrix_a.txt matrix_b.txt sonuclar.txt

# Derleme kuralı
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

# Temizlik hedefi
clean:
	rm -f $(EXEC)