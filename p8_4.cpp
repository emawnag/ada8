#include "p8_4.h"
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

struct edgeD
{
    string fromV;
    string toV;
    int weight;
};

struct node
{
    bool isLeaf;
    int height;
};

long long Minimum_Build_Time(vector<
                             tuple<string, vector<string>, int>> &makefile)
{

    vector<edgeD> graph;
    map<string, node> vertex;
    map<string, int> mf;
    vector<edgeD> tmp_h_eq_1;

    for (auto &i : makefile)
    {
        mf[get<0>(i)] = get<2>(i);
        for (auto &j : get<1>(i))
        {
            edgeD temp;
            temp.toV = get<0>(i);
            temp.fromV = j;
            temp.weight = get<2>(i);
            graph.push_back(temp);

            if (j.size() >= 4 && j.substr(j.size() - 4) == ".cpp")
            {
                vertex[j] = {true, -1};
            }
            else if (j.size() >= 4 && j.substr(j.size() - 4) == ".exe")
            {
                vertex[j] = {false, 1};
                tmp_h_eq_1.push_back(temp);
            }
            else
            {
                vertex[j] = {false, -1};
            }
        }
    }

    for (auto &i : tmp_h_eq_1)
    {
        edgeD temp;
        temp.fromV = i.toV;
        temp.toV = "start";
        temp.weight = 0;
        graph.push_back(temp);
    }
    vertex["start"] = {false, 0};

    for (const auto &e : graph)
    {
        cout << "Edge from " << e.fromV << " to " << e.toV << " with weight " << e.weight << endl;
    }

    function<int(const string &)> calculate_height = [&](const string &v) -> int
    {
        if (vertex[v].height != -1)
            return vertex[v].height;

        int max_height = 0;
        for (const auto &e : graph)
        {
            if (e.toV == v)
            {
                max_height = max(max_height, calculate_height(e.fromV) + 1);
            }
        }
        vertex[v].height = max_height;
        return max_height;
    };

    vector<int> pre_ans;
    pre_ans.push_back(0);
    for (auto &v : vertex)
    {
        if (v.second.height == -1)
        {
            int c = calculate_height(v.first);
            /*if (c>pre_ans.size()){

            }*/
        }
    }

for (const auto &v : vertex)
{
    cout << "Vertex: " << v.first << ", IsLeaf: " << v.second.isLeaf << ", Height: " << v.second.height << endl;
}

    return 0;
}
