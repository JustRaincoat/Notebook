#include <bits/stdc++.h>
#define endl '\n'
#define int long long
using std::cin;
using std::cout;
constexpr int maxn = 1e5+7;
int n;
std::string s;
std::vector<std::pair<int,int>> gE[maxn];
std::pair<int,int> edges[maxn];

struct HLDT {
    struct Node {
        int id, faid, dep, siz, dfn, w;
        Node *fa, *hs, *top;
        std::vector<Node*> s;
        Node(auto& m) { 
            id = faid = 1, dep = 0, fa = nullptr, hs = nullptr, 
            m[id] = top = this, siz = 1, w = 0; 
        }
        Node(Node* _fa, int _id, int _w, auto& m) {
            fa = _fa, w = _w, id = _id, faid = fa->id, 
            dep = fa->dep + 1, siz = 1, hs = nullptr;
            m[id] = this, fa->s.push_back(this);
        }
    };
    std::unordered_map<int, Node*> f_id, f_dfn;
    Node* root;
    
    void dfs_build(Node* u) {
        int heavy = 0;
        for (auto [v, w] : gE[u->id]) {
            if (v == u->faid) continue;
            Node* vp = new Node(u, v, w, f_id);
            dfs_build(vp);
            u->siz += vp->siz;
            if (vp->siz > heavy) heavy = vp->siz, u->hs = vp;
        }
    }
    int timer = 0;
    void dfs_link(Node* u) {
        u->dfn = ++timer; 
        f_dfn[u->dfn] = u;
        if (!u->hs) return;
        u->hs->top = u->top; 
        dfs_link(u->hs);
        for (auto v : u->s) {
            if (v == u->hs) continue;
            v->top = v; 
            dfs_link(v);
        }
    }
    struct SGT {
        struct Rge {
            int l, r, mid, len;
            Rge() {};
            Rge(int _l, int _r) {
                if (_l > _r) std::swap(_l, _r);
                l = _l, r = _r, mid = (l + r) >> 1, len = (r - l + 1);
            }
            friend bool operator <= (const Rge& a, const Rge& b) {
                return b.l <= a.l && a.r <= b.r;
            }
            std::pair<Rge, Rge> div() { 
                return {Rge(l, mid), Rge(mid + 1, r)}; 
            }
        } inrg;
        struct Node {
            Node *ls, *rs;
            Rge rg;
            int maxx, add_val;
            bool has_cv;
            int cv_val;
            Node(Rge _rg) { 
                ls = rs = nullptr, rg = _rg, maxx = -1e18, 
                add_val = 0, cv_val = 0, has_cv = false; 
            }
            void update_to(int v) {
                has_cv = true, cv_val = v, add_val = 0, maxx = v;
            }
            void update_by(int v) {
                if (has_cv) cv_val += v;
                else add_val += v;
                maxx += v;
            }
            void pushup() { 
                if (ls && rs) maxx = std::max(ls->maxx, rs->maxx); 
            }
            void pushdown() {
                if (!ls || !rs) return;
                if (has_cv) {
                    ls->update_to(cv_val);
                    rs->update_to(cv_val);
                    has_cv = false; 
                    cv_val = 0;
                }
                if (add_val != 0) {
                    ls->update_by(add_val);
                    rs->update_by(add_val);
                    add_val = 0;
                }
            }
        };
        Node* root;
        void build(Node* u, auto &m) {
            if (u->rg.len == 1) {
                auto it = m.find(u->rg.l);
                if (it != m.end()) {
                    u->maxx = it->second->w;
                } else {
                    u->maxx = 0;
                }
                return;
            }
            auto p = u->rg.div();
            build(u->ls = new Node(p.first), m);
            build(u->rs = new Node(p.second), m);
            u->pushup();
        }
        
        int query(Node *u) {
            if (u->rg <= inrg) return u->maxx;
            u->pushdown();
            int res = -1e18;
            if (inrg.l <= u->rg.mid) res = std::max(res, query(u->ls));
            if (inrg.r > u->rg.mid) res = std::max(res, query(u->rs));
            return res;
        }
        
        void modify(Node *u, std::function<void(Node*)> factor) {
            if (u->rg <= inrg) { 
                factor(u); 
                return; 
            }
            u->pushdown();
            if (inrg.l <= u->rg.mid) modify(u->ls, factor);
            if (inrg.r > u->rg.mid) modify(u->rs, factor);
            u->pushup();
        }
    } sgt;
    
    HLDT() {
        root = new Node(f_id);
        dfs_build(root);
        root->top = root;
        dfs_link(root);
        sgt.build(sgt.root = new SGT::Node(SGT::Rge(1, n)), f_dfn);
    }
};

void Max(int _u, int _v, HLDT &Tree) {
    auto &t = Tree;
    auto u = t.f_id[_u], v = t.f_id[_v];
    int maxx = -1e18;
    
    while (u->top != v->top) {
        if (u->top->dep < v->top->dep) std::swap(u, v);
        t.sgt.inrg = HLDT::SGT::Rge(u->top->dfn, u->dfn);
        maxx = std::max(maxx, t.sgt.query(t.sgt.root));
        u = u->top->fa;
    }
    
    if (u != v) {
        if (u->dep < v->dep) std::swap(u, v);
        t.sgt.inrg = HLDT::SGT::Rge(v->dfn + 1, u->dfn);
        maxx = std::max(maxx, t.sgt.query(t.sgt.root));
    }
    
    cout << (maxx <= -1e17 ? 0 : maxx) << endl;
}

void operate_on_path(int _u, int _v, std::function<void(HLDT&)> factor, HLDT &t) {
    auto u = t.f_id[_u], v = t.f_id[_v];
    
    while (u->top != v->top) {
        if (u->top->dep < v->top->dep) std::swap(u, v);
        t.sgt.inrg = HLDT::SGT::Rge(u->top->dfn, u->dfn);
        factor(t);
        u = u->top->fa;
    }
    
    if (u != v) {
        if (u->dep < v->dep) std::swap(u, v);
        t.sgt.inrg = HLDT::SGT::Rge(v->dfn + 1, u->dfn);
        factor(t);
    }
}

void Cover(int _u, int _v, int w, HLDT &t) {
    operate_on_path(_u, _v, [w](HLDT &tree) {
        tree.sgt.modify(tree.sgt.root, [w](auto node) { 
            node->update_to(w); 
        });
    }, t);
}

void Add(int _u, int _v, int w, HLDT &t) {
    operate_on_path(_u, _v, [w](HLDT &tree) {
        tree.sgt.modify(tree.sgt.root, [w](auto node) { 
            node->update_by(w); 
        });
    }, t);
}

void Change(int k, int w, HLDT &t) {
    auto [_u, _v] = edges[k];
    auto u = t.f_id[_u], v = t.f_id[_v];
    int target_dfn = (u->dep > v->dep ? u->dfn : v->dfn);
    t.sgt.inrg = HLDT::SGT::Rge(target_dfn, target_dfn);
    t.sgt.modify(t.sgt.root, [w](auto node) { 
        node->update_to(w); 
    });
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);
    if(!(cin >> n))return 0;
    for (int i = 1, u, v, w; i <= n - 1; i++) {
        cin >> u >> v >> w;
        gE[u].push_back({v, w});
        gE[v].push_back({u, w});
        edges[i] = {u, v};
    }
    HLDT Tree;
    while (cin >> s && s != "Stop") {
        int u, v, w, k;
        if (s == "Max") {
            cin >> u >> v; 
            Max(u, v, Tree);
        } else if (s == "Change") {
            cin >> k >> w; 
            Change(k, w, Tree);
        } else if (s == "Cover") {
            cin >> u >> v >> w; 
            Cover(u, v, w, Tree);
        } else if (s == "Add") {
            cin >> u >> v >> w; 
            Add(u, v, w, Tree);
        }
    }
    return 0;
}