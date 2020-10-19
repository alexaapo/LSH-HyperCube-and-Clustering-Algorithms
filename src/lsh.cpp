#include <iostream>
#include <queue> 
#include <chrono> 
#include "../headers/lsh.h"

using namespace std;
using namespace std::chrono;

void Approximate_LSH(infoptr info)
{   
    for(int i=0;i<info->Num_of_Queries;i++)
    {
        int N_NN_Range_Search[info->N];

        int LSH_nns[info->N],LSH_Distances[info->N]; 
        auto start = chrono::high_resolution_clock::now(); 
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > distances; 

        unsigned int gi_query_values[info->L];
        
        gi_values_of_query(info, gi_query_values, i);

        for(int j=0;j<info->L;j++)
            if(info->Hash_Tables[j][gi_query_values[j]] != NULL)
                for(int p=0;p<info->Hash_Tables[j][gi_query_values[j]]->images.size();p++)
                    distances.push(make_pair(ManhattanDistance(info->Queries_Array[i],info->Hash_Tables[j][gi_query_values[j]]->images[p], info->dimensions), (info->Hash_Tables[j][gi_query_values[j]]->images[p][info->dimensions])));

        auto end = chrono::high_resolution_clock::now(); 
        
        cout << endl << "--------------------------------------------" << endl;
        cout << "Query: " << info->Queries_Array[i][info->dimensions] << endl;
        
        for(int k=0;k<info->N;k++)
        {
            LSH_Distances[k] = distances.top().first;
            LSH_nns[k] = distances.top().second;
            distances.pop();
            cout << "Nearest neighbor-" << k+1 << ": " << LSH_nns[k] << endl;
            cout << "distanceLSH: " << LSH_Distances[k] << endl;
            cout << "distanceTrue: " << info->True_Distances[i][k] << endl << endl;
        }
        info->tLSH[i] = chrono::duration_cast<chrono::milliseconds>(end - start).count();  
        cout << "tLSH: " << info->tLSH[i] << "ms" << endl << "tTrue: " << info->tTrue[i] << "ms";

        Approximate_Range_Search(info,N_NN_Range_Search, i);

        cout << endl;
        cout << "R-near neighbors:" << endl;

        for(int n=0;n<info->N;n++)
        { 
            if(N_NN_Range_Search[n] != -1) cout << N_NN_Range_Search[n] << endl;
            else cout << "None\n";
        }

    }
    cout << endl;
}

void Approximate_Range_Search(infoptr info,int* N_NN_Range_Search, int query_index)
{   
    priority_queue<int, vector<int>, greater<int>> neighboors; 

    unsigned int gi_query_values[info->L];

    gi_values_of_query(info, gi_query_values, query_index);

    for(int j=0;j<info->L;j++)
    {
        if(info->Hash_Tables[j][gi_query_values[j]] != NULL)
        {
            int images_in_bucket = info->Hash_Tables[j][gi_query_values[j]]->images.size();
            
            for(int p=0; p<images_in_bucket;p++)
            {
                if(ManhattanDistance(info->Queries_Array[query_index],info->Hash_Tables[j][gi_query_values[j]]->images[p], info->dimensions) < info->R)
                    neighboors.push(info->Hash_Tables[j][gi_query_values[j]]->images[p][info->dimensions]);
            }
        }
    }

    for(int k=0;k<info->N;k++)
    {
        if(!neighboors.empty())
        {
            N_NN_Range_Search[k] = neighboors.top();
            neighboors.pop();
        }
        else
        {
            N_NN_Range_Search[k] = -1;
        }
    }
}
