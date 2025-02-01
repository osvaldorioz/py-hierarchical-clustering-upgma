#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>
#include <limits>
#include <cmath>

//g++ -O2 -Wall -shared -std=c++20 -fPIC `python3.12 -m pybind11 --includes` pugma.cpp -o hierarchical_clustering_upgma`python3.12-config --extension-suffix`
//g++ -O3 -Wall -shared -std=c++20 -fPIC $(python3.12 -m pybind11 --includes) pugma.cpp -o hierarchical_clustering_upgma$(python3.12-config --extension-suffix)

namespace py = pybind11;
using Matrix = std::vector<std::vector<double>>;

// Compute the Euclidean distance between two points
double euclidean_distance(const std::vector<double>& a, const std::vector<double>& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(sum);
}

// Compute the average (unweighted pairwise mean) distance between two clusters
double upgma_linkage(const std::vector<int>& cluster_a, const std::vector<int>& cluster_b, const Matrix& distances) {
    double sum_distance = 0.0;
    int count = 0;
    for (int i : cluster_a) {
        for (int j : cluster_b) {
            sum_distance += distances[i][j];
            count++;
        }
    }
    return sum_distance / count;
}

// Hierarchical clustering using UPGMA
py::tuple hierarchical_clustering_upgma(py::array_t<double> input_data) {
    py::buffer_info buf = input_data.request();
    if (buf.ndim != 2) {
        throw std::runtime_error("Input must be a 2D array");
    }

    size_t num_points = buf.shape[0];
    size_t dimensions = buf.shape[1];
    
    // Convert input data to C++ vector
    std::vector<std::vector<double>> data(num_points, std::vector<double>(dimensions));
    double* ptr = static_cast<double*>(buf.ptr);
    for (size_t i = 0; i < num_points; ++i) {
        for (size_t j = 0; j < dimensions; ++j) {
            data[i][j] = ptr[i * dimensions + j];
        }
    }
    
    // Compute initial distance matrix
    Matrix distances(num_points, std::vector<double>(num_points, 0.0));
    for (size_t i = 0; i < num_points; ++i) {
        for (size_t j = i + 1; j < num_points; ++j) {
            distances[i][j] = distances[j][i] = euclidean_distance(data[i], data[j]);
        }
    }

    // Initialize clusters
    std::vector<std::vector<int>> clusters(num_points);
    for (size_t i = 0; i < num_points; ++i) {
        clusters[i] = {static_cast<int>(i)};
    }

    py::list merges;

    // Agglomerative clustering using UPGMA
    while (clusters.size() > 1) {
        double min_dist = std::numeric_limits<double>::max();
        int cluster_a = -1, cluster_b = -1;

        for (size_t i = 0; i < clusters.size(); ++i) {
            for (size_t j = i + 1; j < clusters.size(); ++j) {
                double dist = upgma_linkage(clusters[i], clusters[j], distances);
                if (dist < min_dist) {
                    min_dist = dist;
                    cluster_a = i;
                    cluster_b = j;
                }
            }
        }

        // Merge the clusters
        clusters[cluster_a].insert(clusters[cluster_a].end(), clusters[cluster_b].begin(), clusters[cluster_b].end());
        clusters.erase(clusters.begin() + cluster_b);

        // Store the merge as a Python tuple
        merges.append(py::make_tuple(cluster_a, cluster_b, min_dist));
    }

    return py::make_tuple(merges, num_points);
}

PYBIND11_MODULE(hierarchical_clustering_upgma, m) {
    m.def("hierarchical_clustering_upgma", &hierarchical_clustering_upgma, "Perform hierarchical clustering using UPGMA (Unweighted Pairwise Mean Linkage)");
}
