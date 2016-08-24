#include "subsetsum4.h"
#include "config.h"
#include <cstdio>
#include <cmath>
#include <climits>
#include <algorithm>
#include <cassert>

subsetsum4::subsetsum4(const vector<PI> &s, const vector<PI> &t)
	: source(s), target(t)
{
}

int subsetsum4::solve()
{
	rescale();
	init(source, table1, ubound1);
	fill(source, table1, ubound1);
	init(target, table2, ubound2);
	fill(target, table2, ubound2);
	optimize();
	return 0;
}

int subsetsum4::rescale()
{
	int s1 = 0, s2 = 0;
	for(int i = 0; i < source.size(); i++) s1 += source[i].first;
	for(int i = 0; i < target.size(); i++) s2 += target[i].first;

	int ubound = (s1 > s2) ? s1 : s2;
	if(ubound > 1000) ubound = 1000;

	double r1 = ubound * 1.0 / s1;
	double r2 = ubound * 1.0 / s2;

	for(int i = 0; i < source.size(); i++) source[i].first = (int)(source[i].first * r1);
	for(int i = 0; i < target.size(); i++) target[i].first = (int)(target[i].first * r2);

	s1 = 0, s2 = 0;
	for(int i = 0; i < source.size(); i++) s1 += source[i].first;
	for(int i = 0; i < target.size(); i++) s2 += target[i].first;

	ubound1 = s1 - 1;
	ubound2 = s2 - 1;

	sort(source.begin(), source.end());
	sort(target.begin(), target.end());

	return 0;
}

int subsetsum4::init(const vector<PI> &vv, vector< vector<int> > &table, int ubound)
{
	table.resize(vv.size() + 1);
	for(int i = 0; i < table.size(); i++)
	{
		table[i].assign(ubound + 1, -1);
	}

	for(int i = 0; i <= vv.size(); i++)
	{
		table[i][0] = 0;
	}

	for(int j = 1; j <= ubound; j++)
	{
		table[0][j] = -1;
	}
	return 0;
}

int subsetsum4::fill(const vector<PI> &vv, vector< vector<int> > &table, int ubound)
{
	for(int j = 1; j <= ubound; j++)
	{
		for(int i = 1; i <= vv.size(); i++)
		{
			int s = vv[i - 1].first;
			if(j >= s && table[i - 1][j - s] >= 0)
			{
				table[i][j] = i;
			}
			
			if(table[i - 1][j] >= 0)
			{
				table[i][j] = table[i - 1][j];
			}
		}
	}
	return 0;
}

int subsetsum4::backtrace(int t, const vector<PI> &vv, const vector< vector<int> > &table, vector<int> &ss)
{
	ss.clear();
	if(table.size() <= 0) return -1;
	if(t <= 0 || t > table[0].size()) return -1;
	int n = vv.size();
	if(table[n][t] == -1) return -1;

	//ss.push_back(vv[vi].second);

	int x = t;
	int s = table[n][t];
	while(x >= 1 && s >= 1)
	{
		assert(table[s][x] >= 0);
		ss.push_back(vv[s - 1].second);

		x -= vv[s - 1].first;
		s = table[s - 1][x];
	}
	return 0;
}

int subsetsum4::optimize()
{
	vector<PI> v;
	int n1 = source.size();
	int n2 = target.size();
	for(int i = 1; i <= ubound1; i++)
	{
		if(table1[n1][i] < 0) continue;
		v.push_back(PI(i, 1));
	}
	for(int i = 1; i <= ubound2; i++)
	{
		if(table2[n2][i] < 0) continue;
		v.push_back(PI(i, 2));
	}

	sort(v.begin(), v.end());

	int d = INT_MAX;
	int k = -1;
	for(int i = 0; i < v.size() - 1; i++)
	{
		if(v[i + 1].first - v[i].first >= d) continue;
		d = v[i + 1].first - v[i].first;
		k = i;
	}

	assert(k != -1);

	eqn.e = d;
	if(v[k].second == 1) backtrace(v[k].first, source, table1, eqn.s);
	else  backtrace(v[k].first, target, table2, eqn.s);

	if(v[k + 1].second == 1) backtrace(v[k + 1].first, source, table1, eqn.t);
	else  backtrace(v[k + 1].first, target, table2, eqn.t);

	return 0;

}

int subsetsum4::print()
{
	printf("source: ");
	for(int i = 0; i < source.size(); i++) printf("%d:%d, ", source[i].second, source[i].first);
	printf("\n");
	printf("target: ");
	for(int i = 0; i < target.size(); i++) printf("%d:%d, ", target[i].second, target[i].first);
	printf("\n");

	printf("table 1\n");
	printf("   ");
	for(int i = 0; i < table1[0].size(); i++) printf("%3d", i);
	printf("\n");

	for(int i = 0; i < table1.size(); i++)
	{
		printf("%3d", i);
		for(int j = 0; j < table1[i].size(); j++)
		{
			printf("%3d", table1[i][j]);
		}
		printf("\n");
	}

	printf("table 2\n");
	printf("   ");
	for(int i = 0; i < table2[0].size(); i++) printf("%3d", i);
	printf("\n");

	for(int i = 0; i < table2.size(); i++)
	{
		printf("%3d", i);
		for(int j = 0; j < table2[i].size(); j++)
		{
			printf("%3d", table2[i][j]);
		}
		printf("\n");
	}

	eqn.print(99);

	/*
	vector<int> v;
	for(int i = 0; i <= ubound1; i++)
	{
		backtrace(i, source, table1, v);
		printf("backtrace %d: ", i);
		printv(v);
		printf("\n");
	}
	*/

	return 0;
}

int subsetsum4::test()
{
	vector<PI> v;
	v.push_back(PI(5, 1));
	v.push_back(PI(6, 2));
	v.push_back(PI(8, 3));
	v.push_back(PI(3, 4));

	vector<PI> t;
	t.push_back(PI(3, 1));
	t.push_back(PI(17, 3));
	t.push_back(PI(9, 4));
	t.push_back(PI(16, 5));

	subsetsum4 sss(v, t);
	sss.solve();
	sss.print();

	return 0;
}
