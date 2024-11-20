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
#include <queue>

using namespace std;

bool ends_with_cpp(const std::string &str)
{
    return str.size() >= 4 && str.compare(str.size() - 4, 4, ".cpp") == 0;
}

long long maxLove = 0;

long long Minimum_Build_Time(vector<tuple<string, vector<string>, int>> &makefile)
{
    map<string, tuple<vector<string>, int>> makefile_copy;
    map<string, int> in_degree;
    queue<string> q;
    for (const auto &obj : makefile)
    {
        makefile_copy[get<0>(obj)] = {get<1>(obj), get<2>(obj)};
        if (get<0>(obj).find(".exe") != string::npos)
        {
            if (makefile_copy.find("root") != makefile_copy.end())
            {
                get<0>(makefile_copy["root"]).push_back(get<0>(obj));
                in_degree["root"]++;
            }
            else
            {
                makefile_copy["root"] = make_tuple(vector<string>{get<0>(obj)}, 0);
                in_degree["root"] = 0;
            }
        }
        in_degree[get<0>(obj)] = get<1>(obj).size();
        for (const auto &z : get<1>(obj))
        {
            if (ends_with_cpp(z))
            {
            q.push(z);
            }
        }
    }

    vector<string> topo_order;
    string u ="";
    /*while (q.size()!=makefile_copy.size())
    {
       if(!q.empty()){ 
        u = q.front();
        q.pop();
        topo_order.push_back(u);
    }
        for (const auto &dep : get<0>(makefile_copy[u]))
        {
            if (--in_degree[dep] == 0)
            {
                q.push(dep);
            }
        }
    }*/
   while(!q.empty()){
    
   }

    map<string, long long> ans;
    for (auto it = topo_order.rbegin(); it != topo_order.rend(); ++it)
    {
        const string &s = *it;
        if (ends_with_cpp(s))
        {
            ans[s] = 0;
        }
        else
        {
            ans[s] = get<1>(makefile_copy[s]);
            long long mx = 0 ;
            for (const auto &z : get<0>(makefile_copy[s]))
            {
                mx = max(mx, ans[z]);
            }
            ans[s] += mx;
        }
    }


    return ans["root"];
}
