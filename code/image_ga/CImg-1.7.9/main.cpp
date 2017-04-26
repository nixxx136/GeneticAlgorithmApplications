#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "CImg.h"
#include <fstream>
#include <unistd.h>
#include <vector>
#include <time.h>

using namespace cimg_library;
 
// static const CImg<unsigned int> TARGET("bulldog.png");
static const CImg<unsigned int> TARGET("ml.png");
const int WIDTH = TARGET.width();
const int HEIGHT = TARGET.height();
const int SIZE = WIDTH * HEIGHT * 3;
CImg<unsigned int> BEST(WIDTH, HEIGHT, 1, 3);

static const int POP_SIZE = 5;
static const int TOURNAMENT_SIZE = 2;

bool BINGO = false;
double MUTATION_RATE = 0.01;

std::vector< CImg<unsigned int> > pop;
// cimg_library::CImg<unsigned int> img;

void init_pop();
long long int fitness(CImg<unsigned int> &);
void selection(std::vector< CImg<unsigned int> > pop, CImg<unsigned int> & img1, CImg<unsigned int> & img2);
void crossover(CImg<unsigned int> & img1, CImg<unsigned int> & img2, CImg<unsigned int> & child);
void mutation(CImg<unsigned int> & img);
void operations();

int main() {
    clock_t tStart = clock();
    system("exec rm bulldogs/*");
    int i=0, j=0;
    init_pop();
    CImgDisplay main_disp(WIDTH, HEIGHT, "TEST", 0);

    while (!main_disp.is_closed()) {
    // while (!BINGO) {
        // main_disp.display(TARGET);
        main_disp.display(BEST);
        // if ((int)(clock() - tStart)/CLOCKS_PER_SEC > i*5) {
        if (j >= i*50) {
            i+=1;
            std::string name = "bulldogs/bulldog";

            std::stringstream ss;
            ss << std::setw(6) << std::setfill('0') << i;
            std::string s = ss.str();

            // name += std::to_string(i);
            name += s;
            name += ".png";
            BEST.save_png(name.c_str());
        }
        j+=1;
        operations();
        std::cout << " " << fitness(BEST) << std::endl;
    }
    return 0;
}

void init_pop() {
    srand( time(NULL) );
    for (int i=0; i<POP_SIZE; i++) {
        CImg<unsigned int> img(WIDTH, HEIGHT, 1, 3);
        for (int j=0; j<SIZE; j++) {
            img[j] = rand() % 256;
        }
        if (BEST == NULL || fitness(BEST) > fitness(img)) BEST.assign(img);
        pop.push_back(img);
    }
}

long long int fitness(CImg<unsigned int> & img) {
    long long int res = 0;
    for (int i=0; i<SIZE; i++){
        res += abs(TARGET[i] - img[i]);
    }
    return res;
}

// tournament selection
void selection(std::vector< CImg<unsigned int> > pop, CImg<unsigned int> & img1, CImg<unsigned int> & img2) {
    for (int i=0; i<TOURNAMENT_SIZE; i++) {
        int idx1 = rand()%POP_SIZE, idx2 = rand()%POP_SIZE;
        // cout << idx1  << " " << idx2 << endl;

        if (img1 == NULL || fitness(pop[idx1]) < fitness(img1))
            img1.assign(pop[idx1]);

        if (img2 == NULL || fitness(pop[idx2]) < fitness(img2))
            img2.assign(pop[idx2]);
    }
}


// two points crossover
void crossover(CImg<unsigned int> & img1, CImg<unsigned int> & img2, CImg<unsigned int> & child) {
    int cross_index1 = rand() % SIZE;
    int cross_index2 = rand() % SIZE;

    if (cross_index1 < cross_index2) {
        for (int i=0; i<SIZE; i++) {
            if (i<cross_index1 || i>cross_index2) {
                child[i] = img1[i];
            } else {
                child[i] = img2[i];
            }
        }
    } else {
        for (int i=0; i<SIZE; i++) {
            if (i<cross_index2 || i>cross_index1) {
                child[i] = img1[i];
            } else {
                child[i] = img2[i];
            }
        }
    }
}

// flip bit
void mutation(CImg<unsigned int> & img){
    for (int i=0; i<SIZE; i++) {
        double comp = rand()/ (RAND_MAX + 1.);
        // cout << comp << endl;
        if (comp < MUTATION_RATE) {
            // img[i] = rand() % 256;
            img[i] = (unsigned int)TARGET[i];
        }
    }
}

// GA operations
void operations() {
    std::vector< CImg<unsigned int> > new_pop;
    while (new_pop.size() < POP_SIZE) {
        CImg<unsigned int> parent1(WIDTH, HEIGHT, 1, 3), parent2(WIDTH, HEIGHT, 1, 3), child(WIDTH, HEIGHT, 1, 3);
        // std::cout << "selection" << std::endl;
        selection(pop, parent1, parent2);
        // std::cout << "crossover" << std::endl;
        crossover(parent1, parent2, child);
        // std::cout << "mutation" << std::endl;
        mutation(child);
        // cout << child << " " << fitness(child) << endl;
        if (fitness(child)<WIDTH*HEIGHT) BINGO = true;

        if (find(new_pop.begin(), new_pop.end(), child) == new_pop.end() ) {
            if (BEST==NULL || fitness(BEST) > fitness(child)) BEST.assign(child);
            new_pop.push_back(child);
        }
        // std :: cout << new_pop.size() << std :: endl;
        // new_pop.push_back(child);
    }
    pop = new_pop;
}

// end
