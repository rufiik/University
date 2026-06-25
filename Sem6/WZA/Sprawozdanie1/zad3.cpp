#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

using Point2D = pair<int, int>;
using Point3D = tuple<int, int, int>;

struct Point4D {
    int x[4];
    
    bool operator<(const Point4D& other) const {
        for (int i = 0; i < 4; ++i) {
            if (x[i] != other.x[i]) return x[i] < other.x[i];
        }
        return false;
    }
    
    bool operator==(const Point4D& other) const {
        for (int i = 0; i < 4; ++i) {
            if (x[i] != other.x[i]) return false;
        }
        return true;
    }
};

bool leq(const Point2D& p, const Point2D& q) {
    return p.first <= q.first && p.second <= q.second;
}

bool leq(const Point3D& p, const Point3D& q) {
    return get<0>(p) <= get<0>(q) && 
           get<1>(p) <= get<1>(q) && 
           get<2>(p) <= get<2>(q);
}

bool leq(const Point4D& p, const Point4D& q) {
    for (int i = 0; i < 4; ++i) {
        if (p.x[i] > q.x[i]) return false;
    }
    return true;
}

template<typename T>
vector<T> findMinimals(const vector<T>& points) {
    vector<T> result;
    for (size_t i = 0; i < points.size(); ++i) {
        bool isMinimal = true;
        for (size_t j = 0; j < points.size(); ++j) {
            if (i != j && leq(points[j], points[i])) {
                isMinimal = false;
                break;
            }
        }
        if (isMinimal) {
            result.push_back(points[i]);
        }
    }
    return result;
}

vector<Point2D> generateSetA(int a, int b, bool includeZero = true) {
    vector<Point2D> A;
    int start = includeZero ? 0 : 1;
    int end = 15;
    
    for (int x = start; x <= end; ++x) {
        for (int y = start; y <= end; ++y) {
            int dx = x - a;
            int dy = y - b;
            if (dx*dx + dy*dy < 5) {
                A.push_back({x, y});
            }
        }
    }
    return A;
}

bool inSetB(const Point4D& p, const int c[4]) {
    int sum = 0;
    for (int i = 0; i < 4; ++i) {
        sum += (p.x[i] - c[i]) * (p.x[i] - c[i]);
    }
    return sum > 224;
}

vector<Point4D> generateSetB(int c1, int c2, int c3, int c4) {
    int c[4] = {c1, c2, c3, c4};
    vector<Point4D> pointsInB;
    
    vector<vector<int>> allowedValues(4);
    for (int i = 0; i < 4; ++i) {
        allowedValues[i].push_back(0); 
        for (int val = c[i] + 1; val <= c[i] + 15; ++val) {
            allowedValues[i].push_back(val);
        }
        for (int val = 1; val <= c[i]; ++val) {
            if (val != 0) allowedValues[i].push_back(val);
        }
        sort(allowedValues[i].begin(), allowedValues[i].end());
        allowedValues[i].erase(unique(allowedValues[i].begin(), allowedValues[i].end()), allowedValues[i].end());
    }
    
    for (int x1 : allowedValues[0]) {
        for (int x2 : allowedValues[1]) {
            for (int x3 : allowedValues[2]) {
                for (int x4 : allowedValues[3]) {
                    Point4D p = {x1, x2, x3, x4};
                    if (inSetB(p, c)) {
                        pointsInB.push_back(p);
                    }
                }
            }
        }
    }
    
    return pointsInB;
}

void printPoint2D(const Point2D& p) {
    cout << "(" << p.first << ", " << p.second << ")";
}

void printPoint3D(const Point3D& p) {
    cout << "(" << get<0>(p) << ", " << get<1>(p) << ", " << get<2>(p) << ")";
}

void printPoint4D(const Point4D& p) {
    cout << "(" << p.x[0] << ", " << p.x[1] << ", " 
         << p.x[2] << ", " << p.x[3] << ")";
}

template<typename T>
void printResults(const vector<T>& results, const string& label) {
    cout << label << ":\n";
    if (results.empty()) {
        cout << "  brak\n";
    } else {
        for (const auto& p : results) {
            cout << "  ";
            if constexpr (is_same_v<T, Point2D>) printPoint2D(p);
            else if constexpr (is_same_v<T, Point3D>) printPoint3D(p);
            else if constexpr (is_same_v<T, Point4D>) printPoint4D(p);
            cout << "\n";
        }
    }
    cout << "\n";
}

int main() {
    int a = 2, b = 7, c = 9, d = 7, e = 5, f = 2;
    
    cout << "=== ZADANIE 3 ===\n";
    cout << "Numer indeksu: 279752\n";
    cout << "a=" << a << ", b=" << b << ", c=" << c << ", d=" << d << ", e=" << e << ", f=" << f << "\n\n";
    
    cout << "--- Podpunkt a) Pary: (a,b), (c,d), (e,f) ---\n";
    vector<Point2D> pairs = {
        {a, b},  
        {c, d},  
        {e, f}   
    };
    
    cout << "Zbior: ";
    for (const auto& p : pairs) {
        printPoint2D(p);
        cout << " ";
    }
    cout << "\n";
    
    vector<Point2D> minimalsPairs = findMinimals(pairs);
    printResults(minimalsPairs, "Elementy minimalne");
    
    cout << "--- Podpunkt b) Trojki: (a,c,e), (b,d,f) ---\n";
    vector<Point3D> triples = {
        {a, c, e},  
        {b, d, f}   
    };
    
    cout << "Zbior: ";
    for (const auto& p : triples) {
        printPoint3D(p);
        cout << " ";
    }
    cout << "\n";
    
    vector<Point3D> minimalsTriples = findMinimals(triples);
    printResults(minimalsTriples, "Elementy minimalne");
    
    cout << "--- Podpunkt c) Zbior A = {(x,y) in N^2 : (x-2)^2 + (y-7)^2 < 5} ---\n";
    
    cout << "Zalozony N = {0,1,2,...}:\n";
    vector<Point2D> setA_withZero = generateSetA(a, b, true);
    cout << "Wygenerowano " << setA_withZero.size() << " punktow\n";
    vector<Point2D> minimalsA_withZero = findMinimals(setA_withZero);
    printResults(minimalsA_withZero, "Elementy minimalne");
    
    cout << "Zalozony N = {1,2,3,...}:\n";
    vector<Point2D> setA_withoutZero = generateSetA(a, b, false);
    cout << "Wygenerowano " << setA_withoutZero.size() << " punktow\n";
    vector<Point2D> minimalsA_withoutZero = findMinimals(setA_withoutZero);
    printResults(minimalsA_withoutZero, "Elementy minimalne");
    
    cout << "--- Podpunkt d) Zbior B = {(x1,x2,x3,x4) in N^4 : suma(xi - ci)^2 > 224} ---\n";
    cout << "Generowanie z optymalizacja (tylko potencjalne minima)...\n";
    
    vector<Point4D> setB = generateSetB(c, d, e, f);
    cout << "Wygenerowano " << setB.size() << " punktow-kandydatow\n";
    
    vector<Point4D> minimalsB = findMinimals(setB);
    cout << "Znaleziono " << minimalsB.size() << " elementow minimalnych:\n";
    for (const auto& p : minimalsB) {
        cout << "  ";
        printPoint4D(p);
        cout << "\n";
    }
    
    return 0;
}