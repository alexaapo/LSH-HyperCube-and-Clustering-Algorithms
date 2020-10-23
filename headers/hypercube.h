#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "../headers/exhausting.h"

class HyperCube
{
    public:
        int Num_of_Images,Num_of_Queries,k,m,M,M_boundary,N,probes,R,dimensions,W,HashTableSize;
        int* modulars, *f_i;
        double* tHypercube,*tTrue,dist_error,time_error;
        int **s_i,**True_Distances;
        item **Images_Array,**Queries_Array;
        Bucket** Hash_Table;
        string input_file,query_file;
        map<unsigned int,unsigned int>* f_i_map;

        HyperCube(string input_file_,string query_file_,int N_,int k_,int R_,int M_boundary_,int probes_)
        : k(k_),M_boundary(M_boundary_),probes(probes_),R(R_),input_file(input_file_),query_file(query_file_),N(N_),dist_error(0.0),time_error(0.0)
        {            
            InitHyperCube();
        }

        int hammingDistance(int , int );
        void InitHyperCube();
        void Approximate_Hypercube();
        void Approximate_Range_Search(int ,unsigned int );

};
#endif