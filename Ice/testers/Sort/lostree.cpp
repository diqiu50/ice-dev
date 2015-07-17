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
	int merge_data[20] = {0};
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
			else if (buff[lstree[p_idx]]== buff[win])
			{
				if (buff[win] != min_val && buff[win] != max_val)
				{
					if (sds[win].pos<sds[win].num)
					{
						merge_data[buff[win]] ++;
						//buff[win] = sds[win].data[sds[win].pos];
						sds[win].pos++;
						//buff[win] = sds[win].data[sds[win].pos++];
					}
					else
					{
						buff[win] = max_val;
					}
					p_idx = (i+sds.size())/2;
					continue;
				}
			}
			p_idx = p_idx/2;
		}
		lstree[0] = win;
	}

	
	int total = 5*6;
	vector<int> v;
	do {
		int idx = lstree[0];

		a:
		if (buff[idx]==max_val) break;
		int value = buff[idx];

		if (sds[idx].pos<sds[idx].num)
			buff[idx] = sds[idx].data[sds[idx].pos++];
		else
			buff[idx] = max_val;

		if (value != buff[idx])
		{
			v.push_back(value);
		}
		else
		{
			merge_data[buff[idx]] ++;
			goto a;
		}

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
			else if (buff[lstree[p_idx]]== buff[win])
			{
				if (buff[win] != min_val && buff[win] != max_val)
				{
					if (sds[win].pos<sds[win].num)
					{
						merge_data[buff[win]] += buff[lstree[p_idx]];
						//buff[win] = sds[win].data[sds[win].pos];
						sds[win].pos++;
						//buff[win] = sds[win].data[sds[win].pos++];
					}
					else
						buff[win] = max_val;
					p_idx = (idx+sds.size())/2;
					continue;
				}
			}
			p_idx = p_idx/2;
		}
		lstree[0] = win;
	}while(1);

	for (int i=0; i<v.size(); i++)
		cout << "key : " << v[i] << " # value : " << merge_data[v[i]] << ". "<<  endl;
}
