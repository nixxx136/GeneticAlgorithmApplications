#include<iostream>
#include<fstream>
#include<unistd.h>
#include <sfml.h>

using namespace std;

static const int COLS = 100;
static const int ROWS = 100;
static const int RGBS = 3;
static const int POP_SIZE = 1;
static const int TIME_DELAY = 2;

unsigned int data[COLS*ROWS*RGBS];
ofstream image;

void init_pop();
void init_pop0();
void writePPM(unsigned int data[COLS*ROWS*RGBS]);

int main() {

    init_pop();
    writePPM(data);
    for (int i=0; i<10; i++) {
        usleep(TIME_DELAY * 1000000);
        if (i%2==0) init_pop();
        else init_pop0();
        writePPM(data);
    }
}

void init_pop() {
    srand( time(NULL) );
    for (int i=0; i<POP_SIZE; i++) {
        for (int j=0; j<COLS*ROWS*RGBS; j++) {
            data[j] = rand() % 256;
        }
    }
}

void init_pop0() {
    srand( time(NULL) );
    for (int i=0; i<POP_SIZE; i++) {
        for (int j=0; j<COLS*ROWS*RGBS; j++) {
            data[j] = 0;
        }
    }
}
void writePPM(unsigned int data[COLS*ROWS*RGBS]) {
    // cout << (int)data[0] << endl;
    image.open("test.ppm");
    image << "P6" << "\n" << COLS << " " << ROWS << "\n" <<"255 \n";
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS*RGBS; j++) {
            // cout <<  data[i*COLS*RGBS + j] << " ";
            image << data[i*COLS*RGBS + j] << " ";
        }
        image << "\n";
        // cout << endl;
    }
    // image << "\n";
    image.close();

}
