#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 23

// RecordType
struct RecordType
{
    int     id; // consider the ID as a key
    char    name;
    int     order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType* recordPtr;
    struct HashType* next;
};

struct hashTable
{
    struct HashType* buckets[TABLESIZE];
};

// Compute the hash function
int hash(int x)
{
    return x % TABLESIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray[], int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d: ", i);
        struct HashType *current = pHashArray[i];
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->recordPtr->id, current->recordPtr->name, current->recordPtr->order);
            current = current->next;
        }
        printf("\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    struct HashType *hashTable[TABLESIZE] = {NULL};

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    for (int i = 0; i < recordSz; ++i)
    {
        struct RecordType *record = &pRecords[i];
        int index = hash(record->id);

        struct HashType *newNode = (struct HashType*)malloc(sizeof(struct HashType));
        newNode->recordPtr = record;
        newNode->next = NULL;

        if (hashTable[index] == NULL)
        {
            hashTable[index] = newNode;
        }
        else
        {
            // collision handling, add to the end of the linked list
            struct HashType *current = hashTable[index];
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    displayRecordsInHash(hashTable, TABLESIZE);

    free(pRecords);
    return 0;
}
