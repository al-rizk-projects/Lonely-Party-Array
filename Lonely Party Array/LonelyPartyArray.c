//Alex Rizk
//NID: al998687
#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
    int i, j;
    //Dynamically allocate space for new lonelyPartyArray
    LonelyPartyArray *party = malloc(sizeof(LonelyPartyArray));

    if (num_fragments < 1 || fragment_length < 1)
    {
        return NULL;
    }

    if(party == NULL)
    {
        return NULL;
    }

    party -> num_active_fragments = 0;
    party -> size = 0;
    party -> num_fragments = num_fragments;
    party -> fragment_length = fragment_length;

    //Dynamically allocate the fragments array to be an array of num_fragmetns
    party -> fragments = malloc(sizeof(int *) * num_fragments);

    if (party -> fragments == NULL)
    {
        free(party);
        return NULL;
    }

    //Initialize all of the pointers to NULL
    for (i = 0; i < num_fragments; i++)
    {
        party -> fragments[i] = NULL;
    }

    //Dynamically allocate the fragment sizes array
    party -> fragment_sizes = malloc(sizeof(int)* num_fragments);

    if (party -> fragment_sizes == NULL)
    {
        free(party);
        free(party ->fragment_sizes);
        return NULL;
    }

    //Initialize all values to zero
    for(j = 0; j < num_fragments; j++)
    {
        party -> fragment_sizes[j] = 0;
    }

    if (party -> fragments <= 0 && party -> fragment_sizes <= 0)
    {
        free(party -> fragments);
        free(party -> fragment_sizes);
        free(party);
        return NULL;


    } else {
        printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", num_fragments*fragment_length, num_fragments);
        //return a pointer to the new LonelyPartyArray
        return party;

    }
}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
    int i;

    if (party == NULL)
    {
        return NULL;
    }else{
        for(i = 0; i < party -> num_fragments; i++)
        {
          free(party -> fragments[i]);
        }
        free(party -> fragments);
        free(party -> fragment_sizes);
        free(party);
        printf("-> The LonelyPartyArray has returned to the void.\n");
        return NULL;
    }
}

int set(LonelyPartyArray *party, int index, int key)
{
    //If statements checking for NULL and invalid index
    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in set().\n");
        return LPA_FAILURE;
    }

    int i;
    //Finds the fragment and cell of the index
    int frag = index / party -> fragment_length;
    int cell = index % party -> fragment_length;

    if(index < 0 || index > ((party->num_fragments*party->fragment_length) - 1) )
    {
        printf( "-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, frag, cell);
        return LPA_FAILURE;
    }

    //If statement checking if fragment is null and dynamically allocates array
    if(party -> fragments[frag] == NULL)
    {
        party -> fragments[frag] = malloc(sizeof(int) * party -> fragment_length);

        for(i = 0; i < party -> fragment_length; i++)
        {
            party -> fragments[frag][i] = UNUSED;
        }

        //adds key to the newly allocated array and increments size and num_active_fragments
        party -> fragments[frag][cell] = key;
        party -> size++;
        party -> num_active_fragments++;

        party -> fragment_sizes[frag]++;
        printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", frag, party -> fragment_length, party -> fragment_length * frag, party -> fragment_length * frag + party -> fragment_length -1);
        return LPA_SUCCESS;

    }else{

        if(party -> fragments[frag][cell] != UNUSED)
        {

          party -> fragments[frag][cell] = key;
          return LPA_SUCCESS;
        }
        party -> fragment_sizes[frag]++;
        party -> fragments[frag][cell] = key;
        party -> size++;
        return LPA_SUCCESS;
    }

    return 0;
}

int get(LonelyPartyArray *party, int index)
{

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    int frag = index / party -> fragment_length;
    int cell = index % party -> fragment_length;

    if(index < 0 || index > ((party->num_fragments*party->fragment_length) - 1) )
    {
        printf( "-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, frag, cell);
        return LPA_FAILURE;
    }

    if(party -> fragments[frag] == NULL)
    {
      return UNUSED;
    }

    if(party->fragments[frag][cell] == UNUSED)
    {
        return UNUSED;
    }

    return party -> fragments[frag][cell];



    return 0;
}

int delete(LonelyPartyArray *party, int index)
{
    int i;

    //NULL CHECK
    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
    }

    //Finds the specific fragment and index
    int frag = index / party -> fragment_length;
    int cell = index % party -> fragment_length;

    //Checks if index is valid
    if(index < 0 || index > ((party->num_fragments*party->fragment_length) - 1) )
    {
        printf( "-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, frag, cell);
        return LPA_FAILURE;
    }

    //Checks if the fragment itself is NULL and returns LPA FAILURE
    if(party -> fragments[frag] == NULL)
    {
      //party -> num_active_fragments--;
      free(party -> fragments[frag]);
      return LPA_FAILURE;
    }

    //Checks if the cell is UNUSED and returns LPA failure
    if(party -> fragments[frag][cell] == UNUSED)
    {
      return LPA_FAILURE;
    }

    //removes specific index from array, decrements size, and sets
    //the decrements fragment in fragment sizes
    party -> fragments[frag][cell] = UNUSED;
    party -> size--;
    party -> fragment_sizes[frag]--;

    //Deallocates the array id
    if(party -> fragment_sizes[frag] == 0)
    {
        printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", frag, party -> fragment_length, party -> fragment_length * frag, (party -> fragment_length * frag + party -> fragment_length) - 1);
        free(party -> fragments[frag]);
        party -> fragments[frag] = NULL;
        party -> num_active_fragments--;
    }
    return LPA_SUCCESS;
    return 0;
}

int printIfValid(LonelyPartyArray *party, int index)
{
    if (party == NULL)
    {
        return LPA_FAILURE;
    }

    int frag = index / party -> fragment_length;
    int cell = index % party -> fragment_length;

    if(index < 0 || index > ((party->num_fragments*party->fragment_length) - 1) )
    {
        return LPA_FAILURE;
    }

    //Checks if the fragment itself is NULL and returns LPA FAILURE
    if(party -> fragments[frag] == NULL)
    {
      //party -> num_active_fragments--;
      return LPA_FAILURE;
    }

    //Checks if the cell is UNUSED and returns LPA failure
    if(party -> fragments[frag][cell] == UNUSED)
    {
      return LPA_FAILURE;
    }

    printf("%d\n", party -> fragments[frag][cell]);
    return LPA_SUCCESS;

    return 0;
}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{

    int i, j, k;

    //free all fragments of the fragments array
    for (i = 0; i < party -> num_fragments; i++)
    {
        free(party -> fragments[i]);
    }


    //Initialize all of the pointers to NULL
    for (j = 0; j < party -> num_fragments; j++)
    {
        party -> fragments[j] = NULL;
    }

    //Initialize all values to zero
    for(k = 0; k < party -> num_fragments; k++)
    {
        party -> fragment_sizes[k] = 0;
    }

    //resets num_active_fragments to 0
    party -> num_active_fragments = 0;
    party -> size = 0;
    printf( "-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n",party -> fragment_length * party -> num_fragments, party -> num_fragments);
    return party;
}

int getSize(LonelyPartyArray *party)
{
    if(party == NULL)
    {
        return -1;
    }else{
        return party -> size;
    }

}

int getCapacity(LonelyPartyArray *party)
{
    if (party == NULL)
    {
        return -1;
    }else{
        return party -> num_fragments * party -> fragment_length;
    }
    return 0;
}

int getAllocatedCellCount(LonelyPartyArray *party)
{
    if (party == NULL)
    {
        return -1;
    }else{
        return party -> num_active_fragments * party -> fragment_length;
    }
    return 0;
}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
    if(party == NULL)
    {
      return 0;
    }else{
        return getCapacity(party) * sizeof(int);
    }
}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
    long long unsigned int retval = 0;
    retval = sizeof(LonelyPartyArray*) + sizeof(LonelyPartyArray);
    retval += sizeof(int *) * (long long unsigned int) party -> num_fragments + sizeof(int) * party -> num_fragments;
    retval +=  sizeof(int) * party -> num_active_fragments * party -> fragment_length;
    return retval;
}



double difficultyRating(void)
{
    return 3.0;
}

double hoursSpent(void)
{
    return 28.0;
}
