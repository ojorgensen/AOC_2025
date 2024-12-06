#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1010

// Structure for each key-value pair
typedef struct HashNode {
    int key;
    int value;
    struct HashNode* next;
} HashNode;

// Structure for the hash table
typedef struct {
    HashNode* table[TABLE_SIZE];
} HashMap;

// Hash function
unsigned int hash(int key) {
    // Convert the signed integer to an unsigned integer
    // to ensure arithmetic wraps correctly.
    unsigned int ukey = (unsigned int)key;

    // A simple approach: directly take the remainder when divided by TABLE_SIZE
    return ukey % TABLE_SIZE;
}

// Create a new hash map
HashMap* createHashMap() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (map == NULL) return NULL;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }
    return map;
}

// Insert a key-value pair
void insert(HashMap* map, int key, int value) {
    unsigned int index = hash(key);
    
    // Create new node
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;  // Make a copy of the key
    newNode->value = value;
    newNode->next = NULL;
    
    // If bucket is empty
    if (map->table[index] == NULL) {
        map->table[index] = newNode;
        return;
    }
    
    // Handle collision by adding to front of list
    newNode->next = map->table[index];
    map->table[index] = newNode;
}

// Get value for a key
int get(HashMap* map, int key) {
    unsigned int index = hash(key);
    HashNode* current = map->table[index];
    
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return 0;  // Key not found
}

// Increment value for a key (or create with value 1 if doesn't exist)
void increment(HashMap* map, int key) {
    unsigned int index = hash(key);
    HashNode* current = map->table[index];
    
    // Look for existing key
    while (current != NULL) {
        if (current->key == key) {
            current->value++;
            return;
        }
        current = current->next;
    }
    
    // Key not found, create new entry with value 1
    insert(map, key, 1);
}

// Free the hash map
void freeHashMap(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = map->table[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(map);
}


// Structure to hold the result of reading the file
typedef struct {
    int* list1;
    int* list2;
    int count;
} NumberLists;



NumberLists readNumbersFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    NumberLists result = {NULL, NULL, 0};
    if (file == NULL) {
        printf("Error opening file\n");
        return result;
    }

    // Count lines first
    int lineCount = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        lineCount++;
    }
    printf("%d\n", lineCount);

    // Allocate memory for both lists
    result.list1 = (int*)malloc(lineCount * sizeof(int));
    result.list2 = (int*)malloc(lineCount * sizeof(int));
    
    if (result.list1 == NULL || result.list2 == NULL) {
        printf("Memory allocation failed\n");
        if (result.list1) free(result.list1);
        if (result.list2) free(result.list2);
        result.list1 = NULL;
        result.list2 = NULL;
        fclose(file);
        return result;
    }

    // Reset file pointer to beginning
    rewind(file);

    // Read numbers into arrays
    int num1, num2;
    int index = 0;
    while (fscanf(file, "%d %d", &num1, &num2) == 2) {
        result.list1[index] = num1;
        result.list2[index] = num2;
        index++;
    }

    result.count = index + 1;
    fclose(file);
    return result;
}

// Function to free the allocated memory
void freeNumberLists(NumberLists lists) {
    if (lists.list1) free(lists.list1);
    if (lists.list2) free(lists.list2);
}


void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temporary arrays
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    
    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temporary arrays back into arr[left..right]
    i = 0;  // Initial index of first subarray
    j = 0;  // Initial index of second subarray
    k = left;  // Initial index of merged subarray
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    // Free the temporary arrays
    free(L);
    free(R);
}

// Main merge sort function
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        
        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Utility function to print an array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


int sum_diffs(int* list1, int* list2, int size) {
    int total = 0;
    int count = 0;
    for (int i = 0; i < size; i++) {
        count = count + 1;
        total = total + abs(list1[i] -list2[i]);
    }
    printf("%d\n", count);
    return total;
}


int main_part_1() {
    NumberLists number_lists = readNumbersFromFile("src/day_1/input.txt");
    mergeSort(number_lists.list1, 0, number_lists.count);
    mergeSort(number_lists.list2, 0, number_lists.count);
    // int length = number_lists.count;
    int length = number_lists.count;
    // int length = 3;
    printArray(number_lists.list1, length);
    printArray(number_lists.list2, length);
    int final = sum_diffs(number_lists.list1, number_lists.list2, length);
    printf("%d\n", final);
    return final;
}

HashMap* rightListCounts(NumberLists number_lists) {
    int size = number_lists.count;
    HashMap* rightlistcounts = createHashMap();
    for (int i = 0; i < size; i++) {
        increment(rightlistcounts, number_lists.list2[i]);
    }
    return rightlistcounts;
}

int computeSimilarityScore(HashMap* counts, int* list, int size) {
    int sim_score = 0;
    for (int i = 0; i < size; i++) {
        int count = get(counts, list[i]);
        sim_score = sim_score + count * list[i];
    }
    return sim_score;
}


int main() {
    NumberLists number_lists = readNumbersFromFile("src/day_1/input.txt");
    HashMap* rightlistcounts = rightListCounts(number_lists);
    int sim_score = computeSimilarityScore(rightlistcounts, number_lists.list1, number_lists.count);
    printf("%d\n", sim_score);
    return sim_score;
}