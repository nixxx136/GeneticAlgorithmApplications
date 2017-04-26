#include<iostream>
#include<vector>
#include <unistd.h>

using namespace std;

string TARGET;
int CHRO_LENGTH;

// GA variables
static const int POPSIZE = 100;
static const int TOURNAMENT_SIZE = 4;
static const double MUTATION_RATE = 0.01;
static const double TIME_DELAY = 0.05; // second
bool BINGO = false;
string best;

vector<string> pop;

// GA functions
void init_pop();
int fitness(string&);
void selection(vector<string>&, string& parent1, string& parent2); // tournament selection
void crossover(string& parent1, string& parent2, string& child); // single point crossover
void mutation(string& chro); // flip bit
void operations();
void solve();


int main() {
    srand (time(NULL));
    cout << "Please enter your target string: \n";
    getline(cin, TARGET);
    CHRO_LENGTH = TARGET.length();
    init_pop();
    // cout << pop[0] << " " << fitness(pop[0]) << endl;
    solve();
}

void init_pop() {
    for (int i=0; i<POPSIZE; i++) {
        string chro = "";
        for (int j=0; j<CHRO_LENGTH; j++) {
            char ch = rand()%128;
            chro += ch;
        }
        if (best.empty() || fitness(best) > fitness(chro)) best = chro;
        pop.push_back(chro);
    }
}

int fitness(string& chro) {
    int res = 0;
    for (int i=0; i<chro.length(); i++) {
        // hamming distance
        res+= abs(chro[i]-TARGET[i]);
    }
    return res;
}

// tournament selection
void selection(vector<string>& pop, string& parent1, string& parent2) {
    for (int i=0; i<TOURNAMENT_SIZE; i++) {
        int idx1 = rand()%POPSIZE, idx2 = rand()%POPSIZE;
        // cout << idx1  << " " << idx2 << endl;

        if (parent1.empty() || fitness(pop[idx1]) < fitness(parent1))
            parent1 = pop[idx1];

        if (parent2.empty() || fitness(pop[idx2]) < fitness(parent2))
            parent2 = pop[idx2];
    }
}

// single point crossover
void crossover(string &parent1, string &parent2, string &child) {
    int cross_index = rand() % parent1.length();
    child = parent1.substr(0, cross_index) +
        parent2.substr(cross_index, parent1.length() - cross_index);
}

// flip bit
void mutation(string& chro){
    for (int i=0; i<chro.length(); i++) {
        double comp = rand()/ (RAND_MAX + 1.);
        // cout << comp << endl;
        if (comp < MUTATION_RATE) {
            chro[i] = rand()%128;
        }
    }
}

// GA operations
void operations() {
    vector<string> new_pop;
    while (new_pop.size() < POPSIZE) {
        string parent1, parent2, child;
        selection(pop, parent1, parent2);
        crossover(parent1, parent2, child);
        mutation(child);
        // cout << child << " " << fitness(child) << endl;
        if (fitness(child)==0) BINGO = true;

        if (find(new_pop.begin(), new_pop.end(), child) == new_pop.end() ) {
            if (best.empty() || fitness(best) > fitness(child)) best = child;
            new_pop.push_back(child);
        }
        // new_pop.push_back(child);
    }
    pop = new_pop;
}

// solve the problem
void solve() {
    while (!BINGO) {
        usleep(TIME_DELAY * 1000000);
        operations();
        cout << best << " " << fitness(best) << endl;
    }
}

// end
