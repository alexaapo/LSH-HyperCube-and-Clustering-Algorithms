#include "../headers/kmeans.h"

int RA_HyperCube::get_dimensions()
{
    return dimensions;
}

int RA_HyperCube::get_M_boundary()
{
    return M_boundary;
}

int RA_HyperCube::get_k()
{
    return k;
}

int RA_HyperCube::get_M()
{
    return M;
}

int RA_HyperCube::get_W()
{
    return W;
}

int RA_HyperCube::get_HashTableSize()
{
    return HashTableSize;
}

int RA_HyperCube::get_Num_of_Images()
{
    return Num_of_Images;
}

int* RA_HyperCube::get_modulars()
{
    return modulars;
}

int** RA_HyperCube::get_s_i()
{
    return s_i;
}

map<unsigned int,unsigned int>* RA_HyperCube::get_f_i_map()
{
    return f_i_map;
}

item** RA_HyperCube::get_Images_Array()
{
    return Images_Array;
}

Bucket** RA_HyperCube::get_Hash_Table()
{
    return Hash_Table;
}

int RA_HyperCube::hammingDistance(int n1, int n2) 
{ 
    int x = n1 ^ n2; 
    int setBits = 0; 
    
    while (x > 0) { 
        setBits += x & 1; 
        x >>= 1; 
    }

    return setBits; 
} 

void RA_HyperCube::Initialize_Values()
{
    K = kmeansptr->get_K();
    dimensions = kmeansptr->get_dimensions();
    k = kmeansptr->get_HC_k();
    M_boundary = kmeansptr->get_HC_M();
    probes = kmeansptr->get_probes();
    Num_of_Images = kmeansptr->get_number_of_images();
    Images_Array = kmeansptr->get_Images_Array();
    HashTableSize = pow(2,k);
}

void RA_HyperCube::Deallocation_of_Memory()
{
    delete [] f_i_map;

    //Deallocation of memory of s_i...
    for(int i=0;i<k;i++)    delete [] s_i[i];
    delete [] s_i;        

    //Deallocation of memory of Hash_Tables...
    for(int j=0;j<(HashTableSize);j++)   
        if(Hash_Table[j]!=NULL)
            delete Hash_Table[j];
    delete [] Hash_Table;

    //Deallocation of memory of modulars...
    delete [] modulars;
}

void RA_HyperCube::Init_RA_HyperCube()
{
    f_i_map = new map<unsigned int, unsigned int>[k];

    //Declaration of hash tables...
    Hash_Table = new Bucket*[HashTableSize];
    for(int i=0;i<HashTableSize;i++)    Hash_Table[i] = NULL;

    //Initialization of m,M...
    M = pow(2,floor((double)32/(double)k));
    m = 423255;
    W = 40000;

    //Calculation of m^d-1modM array...
    modulars = new int[dimensions];
    for(int i=0;i<dimensions;i++)   modulars[i]=mod_expo(m,i,M);

    //Initialization of uniform_int_distribution class...
    default_random_engine generator;   
    uniform_int_distribution<int> distribution(0,W);
    
    //Initialization of k*d(imensions) random s_i integers...
    s_i = new int*[k];
    for(int i=0;i<k;i++)
    {
        s_i[i] = new int[dimensions];
        for(int j=0;j<dimensions;j++)   
            s_i[i][j] = distribution(generator);        
    }
        
    //Fill Hash Table...
    //YOU HAVE TO SOLVE THIS...
    // Insert_Images_To_Buckets_HyperCube(this);
}

// void HyperCube::Approximate_Hypercube()
// {
//     unsigned int fi_query_values[Num_of_Queries];
//     for(int i=0;i<Num_of_Queries;i++)  fi_query_values[i] = 0;
    
//     fi_values_of_query(this, fi_query_values);

//     for(int i=0;i<Num_of_Queries;i++)
//     {
//         auto start = chrono::high_resolution_clock::now();
//         int HyperCube_nns[N],HyperCube_Distances[N], count_hamming=1,count_images=0, count_probes=0; 
//         priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > distances; 

//         Bucket* temp = Hash_Table[fi_query_values[i]];

//         if(temp != NULL)
//         {
//             vector<pair<item*,unsigned int>>::iterator it;

//             for(it=temp->images.begin();it!=temp->images.end();it++)    
//             {
//                 distances.push(make_pair(ManhattanDistance(Queries_Array[i],it->first,dimensions), (it->first[dimensions])));
//                 count_images++;
//                 // cout << count_images << " ";
//                 if(count_images == M_boundary)  break;
//             }
//             // cout << endl;
//         }
//         if((temp == NULL) || (count_images < M_boundary))
//         {
//             while((count_probes < probes) && (count_images < M_boundary))
//             {
//                 for(int j=0;j<HashTableSize;j++)
//                 {
//                     Bucket* hash_cell = Hash_Table[j];

//                     if(hash_cell != NULL)
//                     {
//                         int hamming_distance = hammingDistance(fi_query_values[i],j);
//                         if(hamming_distance == count_hamming)
//                         {
//                             // cout << endl << "query_bucket= " << fi_query_values[i]  << " bucket=" << j << " hamming=" << count_hamming << " " << count_images << "<" << M_boundary << " " << count_probes << "<" << probes << endl;
//                             vector<pair<item*,unsigned int>>::iterator it;

//                             for(it=hash_cell->images.begin();it!=hash_cell->images.end();it++)    
//                             {
//                                 distances.push(make_pair(ManhattanDistance(Queries_Array[i],it->first, dimensions), (it->first[dimensions])));
//                                 count_images++;
//                                 if(count_images == M_boundary)  break;
//                             }
//                             count_probes++;
//                         }
//                     }
//                     if((count_images == M_boundary) || (count_probes == probes))  break;
//                     // if((count_images == M_boundary))  break;
//                 }
//                 count_hamming++;
//             }
//         }

//         auto end = chrono::high_resolution_clock::now(); 

//         file << endl << "--------------------------------------------" << endl;
//         file << "Query: " << Queries_Array[i][dimensions] << endl;

//         for(int k=0;k<N;k++)
//         {
//             HyperCube_Distances[k] = distances.top().first;
//             HyperCube_nns[k] = distances.top().second;
//             distances.pop();
//             file << "Nearest neighbor-" << k+1 << ": " << HyperCube_nns[k] << endl;
//             file << "distanceHyperCube: " << HyperCube_Distances[k] << endl;
//             file << "distanceTrue: " << True_Distances[i][k] << endl << endl;
//             dist_AF += (double)(HyperCube_Distances[k])/(double)True_Distances[i][k];
//         }

//         tHypercube[i] = chrono::duration_cast<chrono::microseconds>(end - start).count();  
//         file << "tHypercube: " << tHypercube[i] << "μs" << endl << "tTrue: " << tTrue[i] << "μs";
//         time_error += tHypercube[i]/tTrue[i];

//         Approximate_Range_Search(i,fi_query_values[i]);
//     }

//     file << endl << "HyperCube's Mean Distance Error: " << dist_AF/(double)(Num_of_Queries*N) << endl;
//     file << endl << "tHyperCube/tTrue: " << time_error/(double)(Num_of_Queries) << endl;

//     file << endl;
//     //Print Buckets...
//     int counter=0;
//     for(int j=0;j<HashTableSize;j++)
//         if(Hash_Table[j]!=NULL)
//             counter++;                
//     file << "HashTable: " << counter << " out of " << HashTableSize << endl;
// }

// void HyperCube::Approximate_Range_Search(int query_index,unsigned int fi_query_value)
// {
//     // auto start = chrono::high_resolution_clock::now();
//     int HyperCube_nns[N], count_hamming=1,count_images=0, count_probes=0; 
//     priority_queue<int, vector<int>, greater<int>> neighboors;

//     Bucket* temp = Hash_Table[fi_query_value];

//     if(temp != NULL)
//     {
//         vector<pair<item*,unsigned int>>::iterator it;

//         for(it=temp->images.begin();it!=temp->images.end();it++)
//         {
//             if(ManhattanDistance(Queries_Array[query_index],it->first, dimensions) < R)
//                 neighboors.push(it->first[dimensions]);
//             count_images++;
//             // cout << count_images << " ";
//             if(count_images == M_boundary)  break;
//         }
//         // cout << endl;
//     }
//     if((temp == NULL) || (count_images < M_boundary))
//     {
//         while((count_probes < probes) && (count_images < M_boundary))
//         {
//             for(int j=0;j<HashTableSize;j++)
//             {
//                 Bucket* hash_cell = Hash_Table[j];
                
//                 if(hash_cell != NULL)
//                 {
//                     int hamming_distance = hammingDistance(fi_query_value,j);
//                     if(hamming_distance == count_hamming)
//                     {
//                         vector<pair<item*,unsigned int>>::iterator it;

//                         // cout << endl << "bucket=" << j << " hamming=" << count_hamming << " " << count_images << "<" << M_boundary << " " << count_probes << "<" << probes << endl;
//                         for(it=hash_cell->images.begin();it!=hash_cell->images.end();it++)   
//                         {
//                             if(ManhattanDistance(Queries_Array[query_index],it->first, dimensions) < R)
//                                 neighboors.push(it->first[dimensions]);
                            
//                             count_images++;
//                             if(count_images == M_boundary)  break;
//                         }
//                         count_probes++;
//                     }
//                 }
//                 if(count_images == M_boundary)  break;
//                 if(count_probes == probes)  break;
//             }
//             count_hamming++;
//         }
//     }

//     // auto end = chrono::high_resolution_clock::now(); 

//     file << endl << "R-near neighbors:" << endl;
//     if(neighboors.empty())  file << "None" << endl;
//     else
//     {
//         for(int k=0;k<neighboors.size();k++)
//         { 
//             file << neighboors.top() << endl;
//             neighboors.pop();
//         }    
//     }  
// }
