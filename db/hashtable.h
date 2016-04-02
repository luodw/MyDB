#ifndef _HASHTABLE_
#define _HASHTABLE_
#include "../net/Ionet.h"
#include <string>
#include <vector>
namespace ionet
{
    const int MaxSize=64;
    const int ratio=1;
    class HashTable
    {
    public:
        HashTable()=default;
        ~HashTable();
        HashTable(int power);
        void insert(int hashkey,HashNode *data);
        std::string find(int hashkey,std::string& key);
        int del(int hashkey,std::string& key);
        int getAll(std::vector<std::string> &);
    private:
        HashNode **findPointer(int index,std::string &key);
        int currentSize;
        int elementTotal;
        std::vector<HashNode*> buckets;
    };
}


#endif
