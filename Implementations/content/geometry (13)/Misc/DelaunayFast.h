/**
 * Description: Delaunay Triangulation, concyclic points are OK (but not all collinear)
 * Time: O(N\log N)
 * Source: KACTL
 * Verification: CCO 2008 P6 Landing
 */

#include "../Primitives/Point.h"

typedef ll T;

typedef struct Quad* Q;
typedef __int128_t lll; // (can be ll if coords are < 2e4)
P arb(LLONG_MAX,LLONG_MAX); // not equal to any other point

struct Quad {
	bool mark; Q o, rot; P p;
	P F() { return r()->p; }
	Q r() { return rot->rot; }
	Q prev() { return rot->o->rot; }
	Q next() { return r()->prev(); }
};

// test if p is in the circumcircle
bool circ(P p, P a, P b, P c) { 
	ll ar = cross(a,b,c); assert(ar); if (ar < 0) swap(a,b);
	lll p2 = norm(p), A = norm(a)-p2,
		B = norm(b)-p2, C = norm(c)-p2;
	return cross(p,a,b)*C+cross(p,b,c)*A+cross(p,c,a)*B > 0;
}

Q makeEdge(P orig, P dest) {
	Q q[] = {new Quad{0,0,0,orig}, new Quad{0,0,0,arb},
			 new Quad{0,0,0,dest}, new Quad{0,0,0,arb}};
	F0R(i,4) q[i]->o = q[-i & 3], q[i]->rot = q[(i+1) & 3];
	return *q;
}
void splice(Q a, Q b) {
	swap(a->o->rot->o, b->o->rot->o); swap(a->o, b->o);
}
Q connect(Q a, Q b) {
	Q q = makeEdge(a->F(), b->p);
	splice(q, a->next());
	splice(q->r(), b);
	return q;
}

pair<Q,Q> rec(const vector<P>& s) {
	if (sz(s) <= 3) {
		Q a = makeEdge(s[0], s[1]), b = makeEdge(s[1], s.back());
		if (sz(s) == 2) return { a, a->r() };
		splice(a->r(), b);
		auto side = cross(s[0], s[1], s[2]);
		Q c = side ? connect(b, a) : 0;
		return {side < 0 ? c->r() : a, side < 0 ? c : b->r() };
	}

#define H(e) e->F(), e->p
#define valid(e) (cross(e->F(),H(base)) > 0)
	Q A, B, ra, rb;
	int half = sz(s) / 2;
	tie(ra, A) = rec({all(s)-half});
	tie(B, rb) = rec({sz(s)-half+all(s)});
	while ((cross(B->p,H(A)) < 0 && (A = A->next())) ||
		   (cross(A->p,H(B)) > 0 && (B = B->r()->o)));
	Q base = connect(B->r(), A);
	if (A->p == ra->p) ra = base->r();
	if (B->p == rb->p) rb = base;

#define DEL(e, init, dir) Q e = init->dir; if (valid(e)) \
		while (circ(e->dir->F(), H(base), e->F())) { \
			Q t = e->dir; \
			splice(e, e->prev()); \
			splice(e->r(), e->r()->prev()); \
			e = t; \
		}
	for (;;) {
		DEL(LC, base->r(), o);	DEL(RC, base, prev());
		if (!valid(LC) && !valid(RC)) break;
		if (!valid(LC) || (valid(RC) && circ(H(RC), H(LC))))
			base = connect(RC, base->r());
		else
			base = connect(base->r(), LC->r());
	}
	return {ra, rb};
}

vector<array<P,3>> triangulate(vector<P> pts) {
	sort(all(pts)); assert(unique(all(pts)) == pts.end());
	if (sz(pts) < 2) return {};

	Q e = rec(pts).f; vector<Q> q = {e};
	int qi = 0;
	while (cross(e->o->F(), e->F(), e->p) < 0) e = e->o;
#define ADD { Q c = e; do { c->mark = 1; pts.push_back(c->p); \
	q.push_back(c->r()); c = c->next(); } while (c != e); }
	ADD; pts.clear();
	while (qi < sz(q)) if (!(e = q[qi++])->mark) ADD;
	
	vector<array<P,3>> ret;
	F0R(i,sz(pts)/3) ret.pb({pts[3*i],pts[3*i+1],pts[3*i+2]});
	return ret;
}