#include <iostream>
#include <string.h>
#include <vector>
#include <math.h>   
#include "../headers/bucket.h"

void Bucket::add(item* image)
{
    images.push_back(image);
}

unsigned int mod(int a, int m) 
{
    if(a%m < 0) return (unsigned int ) a % m + m;
    return (unsigned int) a % m;
}

int Calculate_hp(int* a_i, infoptr info)
{
    int sum = 0;
    int m = pow(2,(32-3));
    int M = pow(2,int(32/info->k));
    int temp;

    for(int i=1; i<=info->dimensions;i++)
    {
        temp = ((a_i[info->dimensions-i] * pow(m,i-1)));
        sum += mod(temp,M);
    }

    sum = mod(sum,M);

    return int(sum);
}

void Calculate_g_Values(infoptr info,unsigned int** g_i)
{
    for(int image=0;image<info->Num_of_Images;image++)
    {
        for(int i=0;i<info->L;i++)
        {
            int h_p[info->k];
            for(int j=0;j<info->k;j++)
            {
                int a_i[info->dimensions];

                for(int z=0;z<info->dimensions;z++)
                {
                    a_i[z] = floor((info->Images_Array[image][z] - info->s_i[i*info->k+j][z])/info->W);
                }
                h_p[j] = Calculate_hp(a_i,info);

                g_i[image][i] |= (h_p[j] << (j*8));                
                g_i[image][i] = g_i[image][i]%(info->Num_of_Images/16);
            }
        }
    }
}

int Calculate_g_Values(infoptr info,unsigned int** g_i)
{
    for(int image=0;image<info->Num_of_Images;image++)
    {
        for(int i=0;i<info->L;i++)
        {
            int h_p[info->k];
            for(int j=0;j<info->k;j++)
            {
                int a_i[info->dimensions];

                for(int z=0;z<info->dimensions;z++)
                {
                    a_i[z] = floor((info->Images_Array[image][z] - info->s_i[i*info->k+j][z])/info->W);
                }
                h_p[j] = Calculate_hp(a_i,info);

                g_i[image][i] |= (h_p[j] << (j*8));                
                g_i[image][i] = g_i[image][i]%(info->Num_of_Images/16);
            }
        }
    }
}

void Insert_Images_To_Buckets(infoptr info,Bucket*** Hash_Tables)
{
    //Allocate memory so as to store temporarily g_i values...
    unsigned int** g_i = new unsigned int*[info->Num_of_Images];
    for(int i=0;i<info->Num_of_Images;i++)  
        g_i[i] = new unsigned int[info->L];
    
    //Call function so as to compute all g_i values...
    Calculate_g_Values(info,g_i);
    
    // // Printing...
    // for(int i=0;i<100;i++)
    // {
    //     for(int j=0;j<info->L;j++)
    //     {
    //         cout << g_i[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    
    //Fill buckets of L Hash_Tables...
    for(int i=0;i<info->Num_of_Images;i++)
    {
        for(int j=0;j<info->L;j++)
        {
            if(Hash_Tables[j][g_i[i][j]]==NULL)  Hash_Tables[j][g_i[i][j]] = new Bucket();
            Hash_Tables[j][g_i[i][j]]->add(info->Images_Array[i]);    
        }
    }

    //Deallocation of memory...
    for(int i=0;i<info->Num_of_Images;i++)  
        delete [] g_i[i];
    delete [] g_i;
}