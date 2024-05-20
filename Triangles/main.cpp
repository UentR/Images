#include <iostream>
#include <fstream> 
#include <math.h>

using namespace std;


char* envNBLoop = getenv("NBLoop");
bool EmptyNBLoop = envNBLoop == NULL;
const int NBLoop = (EmptyNBLoop) ? 10000 : atoi((envNBLoop));

char* envSIZE = getenv("SIZE");
bool EmptySIZE = envSIZE == NULL;
const int SIZE = (EmptySIZE) ? 1024 : atoi((envSIZE));

char* envNBCote = getenv("NBCote");
bool EmptyNBCote = envNBCote == NULL;
const int NBCote = (EmptyNBCote) ? 6 : atoi((envNBCote));

char* envI = getenv("I");
bool EmptyI = envI == NULL;
const int I = (EmptyI) ? 1 : atoi((envI));

struct Point {
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
    Point() : Point(0, 0) {}
};

Point RacineUnite(int NbCote, int i, int size) {
    double angle = 2 * M_PI * i / NbCote;
    return Point(cos(angle)*size*0.9 + size, sin(angle)*size*0.9 + size);
}

ostream& operator<<(ostream& out, const Point& p) {
    return out << "(" << p.x << ", " << p.y << ")";
}

Point operator+(Point a, Point b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator/(const Point& a, int b) {
    return Point(a.x / b, a.y / b);
}

bool isInside(Point a, Point *Poly, int size) {
    bool inside = false;
    float x1 = Poly[0].x;
    float y1 = Poly[0].y;
    float x2, y2, x;
    for (int i=0; i<size+1; i++) {
        x2 = Poly[i % size].x;
        y2 = Poly[i % size].y;
        if (a.y > min(y1, y2)) {
            if (a.y <= max(y1, y2)) {
                if (a.x <= max(x1, x2)) {
                    if (y1 != y2) {
                        x = (a.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                    }
                    if (y1 == y2 || a.x <= x) {
                        inside = !inside;
                    }
                }
            }
        }
        x1 = x2;
        y1 = y2;
    }
    return inside;
}


struct Grid {
    bool* points;
    int size;
    int NbCote;
    Point* Cote;
    Grid(int size, int NbCote) : size(size), NbCote(NbCote) {
        points = new bool[size*size]();
        Cote = new Point[NbCote];
        for (int i=0; i<NbCote; i++) {
            Cote[i] = RacineUnite(NbCote, i, size/2);
        }
    }

    Point RandomCote() {
        int Nb = rand() % NbCote;
        int D = (rand()%2)*2 - 1;
        int NbP = (Nb+D)%NbCote;
        if (NbP < 0) {
            NbP += NbCote;
        }
        return Cote[Nb] + Cote[NbP];
    }

    bool operator[](int i) const {
        return points[i];
    }

    void set(int x, int y) {
        for (int j=-I; j<=I; j++) {
            for (int k=-I; k<=I; k++) {
                if (x >= 0 && x < size && y >= 0 && y < size) {
                    // cout << (x+j)*size + y+k << endl;
                    points[(x+j)*size + y+k] = true;
                }
            }
        }
        // cout << endl;

    }
};

void ToPPM(const std::string& filename, const Grid points) {
    std::ofstream file(filename);
    file << "P3\n" << points.size << " " << points.size << "\n255\n";
    for (int i=0; i<points.size; i++) {
        for (int j=0; j<points.size; j++) {
            if (points[i*points.size + j]) {
                file << "255 0 255 ";
            } else {
                file << "0 0 0 ";
            }
        }
    }
}

Point RandomGrid(Grid *points) {
    float x = rand() % points->size;
    float y = rand() % points->size;
    Point Coord{x, y};
    while (!isInside(Coord, points->Cote, points->NbCote)) {
        x = rand() % points->size;
        y = rand() % points->size;
        Coord = Point{x, y};
    }
    return Coord;
}

void ShowCote(Grid* points) {
    for (int i=0; i<points->NbCote; i++) {
        cout << points->Cote[i] << endl;
    }
}

void loop(Grid *points) {
    Point p = RandomGrid(points);
    points->set(p.x, p.y);
    for (int i=0; i<NBLoop; i++) {
        Point cote = points->RandomCote();
        p = (p + cote) / 3;
        points->set(p.x, p.y);
    }
}

int main() {
    srand(time(0));
    Grid points(SIZE, NBCote);
    loop(&points);
    ToPPM("PPM/Cote-" + to_string(NBCote) + "|Size-" + to_string(SIZE) + "|Points-" + to_string(NBLoop) + ".ppm", points);
    return 0;
}