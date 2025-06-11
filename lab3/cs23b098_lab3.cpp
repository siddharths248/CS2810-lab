#include <bits/stdc++.h>
using namespace std;

class Graph{
    private:
        int n;
        int m;
        set<int> vertices;

        bool dfs(int u, int v, vector<bool> & visited){
            if (u==v) return true;
            visited[u] = true;

            for (int i : vert_adj[u]){
                if (!visited[i]){
                    if (dfs(i,v,visited)){
                        return true;
                    }
                }
            }
            return false;
        }
        
    public:
        vector<set<int>> vert_adj;
    
        Graph(){}

        Graph(int n,int m): n(n), m(m) {
            for (int i=0; i<n; i++){
                vertices.insert(i);
            }
        }

        int getN(){
            return n;
        }

        int getM(){
            return m;
        }

        void addEdge(int u, int v){
            if (u<n && v<n){
                vert_adj[u].insert(v);
                vert_adj[v].insert(u);
            }
        }

        void removeEdge(int u, int v){
            if (u<n && v<n){
                vert_adj[u].erase(v);
                vert_adj[v].erase(u);
            }
        }

        void setParams(int n,int m){
            this->n=n; this->m=m;
            for (int i=0; i<n; i++){
                vertices.insert(i);
                set<int> s;
                vert_adj.push_back(s);
            }

        }

        void operator +(Graph g2){
            int n2 = g2.getN();
            if (n2 > n){
                vert_adj.resize(n2);
                for (int i=n; i<n2; i++){
                    vertices.insert(i);
                }
                n = n2;
            }
            for (int u=0; u<n2; u++){
                for (int v : g2.vert_adj[u]){
                    vert_adj[u].insert(v);
                    vert_adj[v].insert(u);
                }
            }
        }

        void operator -(Graph g2){
            int n2 = g2.getN();
            if (n2 > n){
                vert_adj.resize(n2);
                for (int i=n; i<n2; i++){
                    vertices.insert(i);
                }
                n=n2;
            }
            for (int u=0; u<n2; u++){
                for (auto it= vert_adj[u].begin(); it!= vert_adj[u].end();){
                    if (g2.vert_adj[u].find(*it) == g2.vert_adj[u].end()){
                        it = vert_adj[u].erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
            if (n>n2){
                for (int u=n2; u<n; u++){
                    vert_adj[u].clear();
                }
            }
        }

        void operator !(){
            for (int u = 0; u<n; u++){
                for (int v=0; v<n; v++){
                    if (v==u){
                        continue;
                    }
                    else {
                        if (vert_adj[u].find(v) == vert_adj[u].end()){
                            vert_adj[u].insert(v);
                        }
                        else {
                            vert_adj[u].erase(v);
                        }
                    }
                }
            }
        }

        bool isReachable(int u, int v){
            if (u < n && v < n){
                vector<bool> visited(n,false);
                return dfs(u,v,visited);
            }
            else return false;
        }

};

istream & operator >> (istream & stream, Graph & g){
    int n,m;
    stream >> n >> m;
    g.setParams(n,m);
    for (int i=0; i<m; i++){
        int u,v;
        stream >> u >> v;
        g.addEdge(u,v);
    }
    return stream;
}

ostream & operator << (ostream & stream, Graph & g){
    for (int i=0; i<g.getN(); i++){
        stream << "Vertex " << i << ": ";
        for (int j : g.vert_adj[i]){
            stream << j << " ";
        }
        stream << endl;
    }
    return stream;
}

int main(){
    int f=0;
    Graph g;
    string cmd;
    while (cin >> cmd){
        if (cmd == "end"){
            break;
        }
        else if (cmd == "Graph"){
            cin >> g;
        }
        else if (cmd == "union"){
            Graph g2;
            string cg;
            cin >> cg;
            if (cg == "Graph"){
                cin >> g2;
            }
            g + g2;
        }
        else if (cmd == "intersection"){
            Graph g2;
            string cg;
            cin >> cg;
            if (cg == "Graph"){
                cin >> g2;
            }
            g - g2;
        }
        else if (cmd == "complement"){
            !g;
        }
        else if (cmd == "isReachable"){
            int u,v;
            cin >> u >> v;
            if (g.isReachable(u,v)){
                cout << "Yes" << endl;
            }
            else {
                cout << "No" << endl;
            }
        }
        else if (cmd == "add_edge"){
            int u,v;
            cin >> u >> v;
            g.addEdge(u,v);
        }
        else if (cmd == "remove_edge"){
            int u,v;
            cin >> u >> v;
            g.removeEdge(u,v);
        }
        else if (cmd == "printGraph"){
            cout << g;
        }
    }
}




