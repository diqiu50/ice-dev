#include <iostream>
#include <vector>

using namespace std;

const int min_val = -1;
const int max_val = 100;

struct sort_data{
	int *data;
	int num;
	int pos;
};

int main(int argc, char** argv)
{
	int f1[5] = {3, 7, 7, 14, 16};
	int f2[5] = {2, 4, 10, 12, 19};
	int f3[5] = {5, 6, 7, 8, 11};
	int f4[5] = {0, 3, 4, 8, 12};

	vector <sort_data> sds;
	sort_data sd;
	sd.pos = 0;
	sd.data = f1;
	sd.num = 5;
	sds.push_back(sd);

	sd.pos = 0;
	sd.data = f2;
	sd.num = 5;
	sds.push_back(sd);

	sd.pos = 0;
	sd.data = f3;
	sd.num = 5;
	sds.push_back(sd);

	sd.pos = 0;
	sd.data = f4;
	sd.num = 5;
	sds.push_back(sd);

	int buff[5];
	buff[0] = sds[0].data[sds[0].pos++];
	buff[1] = sds[1].data[sds[1].pos++];
	buff[2] = sds[2].data[sds[2].pos++];
	buff[3] = sds[3].data[sds[3].pos++];

	buff[4] = min_val;

	int lstree[sds.size()];
	
	for (int i=sds.size()-1; i>=0; i--)
	{
		lstree[i] = 4;
	}

	for (int i=sds.size()-1; i>=0; i--)
	{
		int win = i;
		int p_idx = (i+sds.size())/2;
		while(p_idx>0)
		{
			if (buff[lstree[p_idx]]< buff[win])
			{
				int tmp = win;
				win = lstree[p_idx];
				lstree[p_idx] = tmp;
			}
			p_idx = p_idx/2;
		}
		lstree[0] = win;
	}

	
	int total = 5*4;
	vector<int> v;
	do {
		int idx = lstree[0];
		v.push_back(buff[idx]);

		if (sds[idx].pos<sds[idx].num)
			buff[idx] = sds[idx].data[sds[idx].pos++];
		else
			buff[idx] = max_val;

		int win = idx;
		int p_idx = (idx+sds.size())/2;
		while(p_idx>0)
		{
			if (buff[lstree[p_idx]]<buff[win])
			{
				int tmp = win;
				win = lstree[p_idx];
				lstree[p_idx] = tmp;
			}
			p_idx = p_idx/2;
		}
		lstree[0] = win;
	}while(--total);

	for (int i=0; i<v.size(); i++)
		cout << v[i] << endl;
}
