/**
 * Description: For two polys $f,g$ computes $q,r$ such that $f=qg+r,$ $\text{deg}(r)<\text{deg}(g)$
 * Time: $O(N\log N)$
 */

#include "PolyInv.h"

template<class T> pair<vector<T>,vector<T>> divi(const vector<T>& f, const vector<T>& g) { 
	if (sz(f) < sz(g)) return {{},f};
	auto q = mult(inv(rev(g),sz(f)-sz(g)+1),rev(f));
	q.rsz(sz(f)-sz(g)+1); q = rev(q);
	auto r = f-mult(q,g); r.rsz(sz(g)-1);
	return {q,r};
}
