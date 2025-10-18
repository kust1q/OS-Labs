#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

#include "thread.h"

struct Point {
    float x;
    float y;
    int32_t cluster;

    Point(float x = 0.0, float y = 0.0, int32_t cluster = -1): x(x), y(y), cluster(cluster) {}
};

struct Cluster {
    Point centroid;
    Cluster(const Point& point): centroid(point) {} 
};

struct ThreadData {
    int32_t begin;
    int32_t end;
    int32_t id;
};

double distance(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

const int32_t MAXITERATIONS = 1000;
const double EPS = 1e-6;

int32_t threadsCount;
int32_t clustersCount;

std::vector <Point> allPoints;
std::vector <Cluster> allClusters;

void generatePointsAndClusters(int32_t count = 10000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_x(-10000, 10000);
    std::uniform_real_distribution<> dis_y(-10000, 10000);

    allPoints.clear();
    for (size_t i = 0; i < count; ++i) {
        allPoints.emplace_back(dis_x(gen), dis_y(gen), -1);
    }

    if (clustersCount > allPoints.size()) {
        std::cerr << "Error: count points must be greater than clusters count" << std::endl;
        exit(1);
    }

    allClusters.clear();
    for (size_t i = 0; i < clustersCount; ++i) {
        Cluster cl = Cluster(Point(allPoints[i].x, allPoints[i].y, i));
        allClusters.push_back(cl);
    }
}

void* updateClusters(void* threadData) {
    ThreadData* td = static_cast<ThreadData*>(threadData);

    if (td == nullptr) {
        std::cerr << "Error: thead data mustn't be nullptr." << std::endl;
        return nullptr;
    }

    for (size_t i = td->begin; i < td->end; ++i) {
        double baseDist = std::numeric_limits<double>::max();
        int cluster = 0;
        for (size_t j = 0; j < allClusters.size(); ++j) {
            double dist = distance(allPoints[i], allClusters[j].centroid);
            if (dist < baseDist) {
                baseDist = dist;
                cluster = j;
            }
        }
        allPoints[i].cluster = cluster;
    }
    return nullptr;
}

void updateCentroids() {
    std::vector<int32_t> count(clustersCount, 0);
    std::vector<double> xSums(clustersCount, 0);
    std::vector<double> ySums(clustersCount, 0);

    for (const Point& p: allPoints) {
        xSums[p.cluster] += p.x;
        ySums[p.cluster] += p.y;
        ++count[p.cluster];
    }

    for (size_t i = 0; i < allClusters.size(); ++i) {
        if (count[i] > 0) {
            allClusters[i].centroid.x =  xSums[i] / count[i];
            allClusters[i].centroid.y =  ySums[i] / count[i];
        }
    }
}

void kMeans() {
    generatePointsAndClusters();

    for (size_t j = 0; j < MAXITERATIONS; ++j) {
        std::vector<thread::Thread> threads;
        threads.reserve(threadsCount);
        std::vector<ThreadData> threadsData(threadsCount);
        
        int32_t chunkSize = allPoints.size() / threadsCount;

        for (size_t i = 0; i < threadsCount; ++i) {

            threadsData[i].begin = i * chunkSize;
            threadsData[i].end = (i == threadsCount - 1) ? allPoints.size() : (i + 1) * chunkSize;
            threadsData[i].id = i;

            try {
                threads.emplace_back(updateClusters);
                threads[i].Run(&threadsData[i]);
            } catch (const exceptions::CreateThreadException& e) {
                std::cerr << e.what() << std::endl;
                for (size_t i = 0; i < threads.size(); ++i) {
                    threads[i].~Thread();
                }
                exit(1);
            }
        }

        try {
            for (size_t i = 0; i < threadsCount; ++i) {
                threads[i].Join();
            }
        } catch (const exceptions::WaitThreadException& e) {
            std::cerr << e.what() << std::endl;
            for (size_t i = 0; i < threads.size(); ++i) {
                threads[i].~Thread();
            }
            exit(1);
        }

        std::vector<Cluster> oldClasters = allClusters;
        updateCentroids();

        bool changed = false;
        for (size_t i = 0; i < clustersCount; ++i) {
            if (distance(oldClasters[i].centroid, allClusters[i].centroid) > EPS) {
                changed = true;
                break;
            }
        }
        if (!changed) {
            std::cout << "Algorithm ended on " << j + 1 << " iterations" << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Enter the clusters count: " << std::endl;
    std::cin >> clustersCount;

    if (argc < 2) {
        std::cerr << "Must be min 2 args" << std::endl;
        return 1;
    }

    try {
        threadsCount = std::stoul(argv[1]);
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: incorrect number format: not a number" << std::endl;
    } catch (const std::out_of_range&) {
        std::cerr << "Error: incorrect number format: too long." << std::endl;
    }

    auto start = std::chrono::steady_clock::now();
    kMeans();
    auto end = std::chrono::steady_clock::now();

    std::cout << "Algorithm was done in: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    return 0;
}


