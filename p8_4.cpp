#include "p8_4.h"
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

map<string, long long> nodeHeight;

bool ends_with_cpp(const std::string &str)
{
    return str.size() >= 4 && str.compare(str.size() - 4, 4, ".cpp") == 0;
}

long long maxLove = 0;

void computeHeight(const map<string, tuple<vector<string>, int>> &mf, string name, long long ih)
{
    if (nodeHeight.find(name) != nodeHeight.end())
    {
        nodeHeight[name] = nodeHeight[name] > ih ? nodeHeight[name] : ih;
    }
    else
    {
        nodeHeight[name] = ih;
    }

    if (ends_with_cpp(name))
    {
        maxLove = max(maxLove, nodeHeight[name]);
    }
    else
    {
        for (const auto &dep : get<0>(mf.at(name)))
        {
            computeHeight(mf, dep, ih + get<1>(mf.at(name)));
        }
    }
}

long long Minimum_Build_Time(vector<tuple<string, vector<string>, int>> &makefile)
{
    nodeHeight["root"] = 0;
    map<string, tuple<vector<string>, int>> makefile_copy;
    for (const auto &obj : makefile)
    {
        makefile_copy[get<0>(obj)] = {get<1>(obj), get<2>(obj)};
        if (get<0>(obj).find(".exe") != string::npos)
        {
            if (makefile_copy.find("root") != makefile_copy.end())
            {
                get<0>(makefile_copy["root"]).push_back(get<0>(obj));
            }
            else
            {
                makefile_copy["root"] = make_tuple(vector<string>{get<0>(obj)}, 0);
            }
        }
    }

    computeHeight(makefile_copy, "root", 0);

    return maxLove;
}
