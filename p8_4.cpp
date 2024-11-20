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

#define WLOG(X)                                                          \
    string tmp = "wget -q --spider http://127.0.0.1:55125/dot?f=" + (X); \
    system(tmp.c_str());
#define NL "@NL@"
#define AR "@AR@"
#define AL "@AL@"

map<string, long long> nodeHeight;

string nodeHtml()
{
    string ans = "";
    for (const auto &obj : nodeHeight)
    {
        ans += "\"" + obj.first + "\" [label=" + AL + AL + "TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\"" + AR + AL + "TR" + AR + AL + "TD" + AR + obj.first + AL + "/TD" + AR + AL + "/TR" + AR + AL + "TR" + AR + AL + "TD" + AR + to_string(obj.second) + AL + "/TD" + AR + AL + "/TR" + AR + AL + "/TABLE" + AR + AR + "];" + NL;
    }
    return ans;
}

string Generate_Graphviz(const map<string, tuple<vector<string>, int>> &makefile)
{
    ostringstream outfile;
    outfile << "digraph G {" << NL;

    for (const auto &obj : makefile)
    {
        string target = obj.first;
        const vector<string> &dependencies = get<0>(obj.second);

        for (const auto &dep : dependencies)
        {
            outfile << "    \"" << dep << "\" -" << AR << " \"" << target << "\"" << "[label=\"" << to_string(get<1>(obj.second)) << "\"]" << ";" << NL;
        }
    }

    outfile << nodeHtml() << "}" << NL;

    return outfile.str();
}

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+-";

std::string base64_encode(const std::string &input)
{
    std::string encoded;
    unsigned char const *bytes_to_encode = reinterpret_cast<const unsigned char *>(input.c_str());
    size_t in_len = input.length();
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
            {
                encoded += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
        {
            encoded += base64_chars[char_array_4[j]];
        }

        while ((i++ < 3))
        {
            encoded += '=';
        }
    }

    return encoded;
}

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

    map<string, int> in_degree;
    for (const auto &obj : makefile_copy)
    {
        in_degree[obj.first] = 0;
    }

    for (const auto &obj : makefile_copy)
    {
        for (const auto &dep : get<0>(obj.second))
        {
            in_degree[dep]++;
        }
    }

    queue<string> q;
    for (const auto &obj : in_degree)
    {
        if (obj.second == 0)
        {
            q.push(obj.first);
            cout << "\t\t\t\t\t\t\tProcessing node with zero in-degree: " << obj.first << endl;
        }
    }

    vector<string> topo_order;
    while (!q.empty())
    {
        string u = q.front();
        q.pop();
        topo_order.push_back(u);

        for (const auto &dep : get<0>(makefile_copy[u]))
        {
            if (--in_degree[dep] == 0)
            {
                q.push(dep);
            }
        }
    }

    if (topo_order.size() != makefile_copy.size())
    {
        cerr << "There exists a cycle in the graph" << endl;
        exit(EXIT_FAILURE);
    }

    computeHeight(makefile_copy, "root", 0);

    string visual = base64_encode(Generate_Graphviz(makefile_copy));
    WLOG(visual);

    for (const auto &node : topo_order)
    {
        cout << node << " ";
    }
    cout << endl;

    return maxLove;
}
