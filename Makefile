default:
	emcc -std=17 -s main.c -o web #gcc main.c -o Game -L lib/ -lraylib -lm -ldl -lpthread -lrt -lX11 && ./Game