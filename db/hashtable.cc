#include "hashtable.h"
#include <math.h>
#include <string>
#include "../net/Ionet.h"
#include "../util/Logger.h"
using namespace std;

namespace ionet
{
    HashTable::HashTable(int power)
    {
        currentSize=pow(2,power);
        elementTotal=0;
        std::vector<HashNode*> tmp(currentSize,NULL);
        buckets.swap(tmp);
    }
    HashTable::~HashTable()
    {

    }
    void HashTable::insert(int hashkey, HashNode *data)
    {
        data->next=buckets[hashkey%currentSize];
        buckets[hashkey%currentSize]=data;
    }
    string HashTable::find(int hashkey,string& key)
    {
        HashNode* tmpNode=buckets[hashkey%currentSize];
        while(tmpNode!=NULL && tmpNode->key!=key)
        {
            tmpNode=tmpNode->next;
        }
        if(tmpNode==NULL)
        {
            return string();
        }
        else
        {
            return tmpNode->value;
        }
    }
    int HashTable::getAll(vector<string> &keyAll)
    {
        for(int i=0;i<currentSize;i++)
        {
            HashNode *tmp=buckets[i];
            while(tmp!=NULL)
            {
                keyAll.push_back(tmp->key);
                tmp=tmp->next;
            }
        }
        return 1;
    }
    HashNode** HashTable::findPointer(int index,std::string &key)
    {
        HashNode** ptr=&buckets[index];
        while(*ptr!=NULL && key!=(*ptr)->key)
        {
            ptr=&(*ptr)->next;
        }
        return ptr;
    }
    int HashTable::del(int hashkey,string& key)
    {
        int index=hashkey%currentSize;
        HashNode **ptr=findPointer(index, key);
        if(*ptr==NULL)
        {
            return -1;
        }
        else
        {
            HashNode *result=*ptr;
            if(result!=NULL)
            {
                *ptr=result->next;
            }
            return 0;
        }
    }
}
