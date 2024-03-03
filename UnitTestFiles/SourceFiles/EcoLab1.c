#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <stdio.h>
#include <stdlib.h>

// Компараторы
int __cdecl compInts(const void* left, const void* right) {
    const int l = *(int*)left;
    const int r = *(int*)right;
    if (l == r) return 0;
    else if (l < r) return -1;
    else return 1;
}

int __cdecl compInts16_t(const void* left, const void* right) {
    const int16_t l = *(int16_t*)left;
    const int16_t r = *(int16_t*)right;
    if (l == r) return 0;
    else if (l < r) return -1;
    else return 1;
}

int __cdecl compInts32_t(const void* left, const void* right) {
    const int32_t l = *(int32_t*)left;
    const int32_t r = *(int32_t*)right;
    if (l == r) return 0;
    else if (l < r) return -1;
    else return 1;
}

int __cdecl compDoubles(const void* left, const void* right) {
    const double l = *(double*)left;
    const double r = *(double*)right;
    if (l == r) return 0;
    else if (l < r) return -1;
    else return 1;
}


int __cdecl compFloats(const void* left, const void* right) {
    const float l = *(float*)left;
    const float r = *(float*)right;
    if (l == r) return 0;
    else if (l < r) return -1;
    else return 1;
}

// Функции вывода массива
void printIntArray(int* arr, size_t arr_size) {
    size_t i;
    printf("Array: [");
	for (i = 0; i < arr_size; i++) {
		printf("%d ", arr[i]);
	}
	printf("]\n");
}

void printFloatArray(float* arr, size_t arr_size) {
    size_t i;
    printf("Array: [");
	for (i = 0; i < arr_size; i++) {
		printf("%f ", arr[i]);
	}
	printf("]\n");
}

void printDoubleArray(double* arr, size_t arr_size) {
    size_t i;
    printf("Array: [");
	for (i = 0; i < arr_size; i++) {
		printf("%lf ", arr[i]);
	}
	printf("]\n");
}

// Функции создания заполненного массива
int* createIntArray(IEcoMemoryAllocator1 *pIMem, size_t arr_size) {
	size_t i;
    int *arr = (int *) pIMem->pVTbl->Alloc(pIMem, arr_size * sizeof(int));
    for (i = 0; i < arr_size; i++) {
        arr[i] = rand() %20003 - 10000;
    }
    return arr;
}

float* createFloatArray(IEcoMemoryAllocator1 *pIMem, size_t arr_size)  {
    size_t i;
    float *arr = (float *) pIMem->pVTbl->Alloc(pIMem, arr_size * sizeof(float));
    for (i = 0; i < arr_size; i++) {
        arr[i] = ((float) (rand() %20003 - 10000)) / ((float)(rand() % 1000 + 1)) ;
    }
    return arr;
}

double* createDoubleArray(IEcoMemoryAllocator1 *pIMem, size_t arr_size) {
    size_t i;
    double *arr = (double *) pIMem->pVTbl->Alloc(pIMem, arr_size * sizeof(double));
    for (i = 0; i < arr_size; i++) {
        arr[i] = ((double) (rand() %20003 - 10000)) / ((float)(rand() % 1000 + 1)) ;
    }
    return arr;
}

// Функция копирования массива
void *createCopyArray(IEcoMemoryAllocator1 *pIMem, void *src, uint32_t byte_count) {
    void *copy_array = pIMem->pVTbl->Alloc(pIMem, byte_count);
    pIMem->pVTbl->Copy(pIMem, copy_array, src, byte_count);
    return copy_array;
}


int16_t EcoMain(IEcoUnknown* pIUnk) {

	
    IEcoSystem1* pISys = 0;				/* Указатель на системный интерфейс */
    IEcoInterfaceBus1* pIBus = 0;		/* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoMemoryAllocator1* pIMem = 0;	/* Указатель на интерфейс работы с памятью */
	IEcoLab1* pIEcoLab1 = 0;			/* Указатель на тестируемый интерфейс */

	int16_t result = 0;

	// Тестируемые массивы
    int* arr_int_for_timsort;
    int* arr_int_for_qsort;
    float* arr_float_for_timsort;
    float* arr_float_for_qsort;
    double* arr_double_for_timsort;
    double* arr_double_for_qsort;

	// Таймер
    clock_t before;
    clock_t after;
    
	// Счетчики времени
    double tim_sort_time_int;
    double qsort_time_int;
    double tim_sort_time_float;
    double qsort_time_float;
    double tim_sort_time_double;
    double qsort_time_double;

	size_t i = 0;
	size_t arr_size = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            goto Release; /* Освобождение системного интерфейса в случае ошибки */
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release; /* Освобождение в случае ошибки */
    }

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        goto Release; /* Освобождение системного интерфейса в случае ошибки */
    }

	/* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        goto Release; /* Освобождение интерфейсов в случае ошибки */
    }

	printf("Input array size -> ");
	scanf_s("%d", &arr_size);


	arr_int_for_timsort = createIntArray(pIMem, arr_size);
    arr_float_for_timsort = createFloatArray(pIMem, arr_size);
    arr_double_for_timsort = createDoubleArray(pIMem, arr_size);

    arr_int_for_qsort = createCopyArray(pIMem, arr_int_for_timsort, arr_size * sizeof(int));
    arr_float_for_qsort = createCopyArray(pIMem, arr_float_for_timsort, arr_size * sizeof(float));
    arr_double_for_qsort = createCopyArray(pIMem, arr_double_for_timsort, arr_size * sizeof(double));

	printf("Unsorted arrays\n");
    printIntArray(arr_int_for_timsort, arr_size);
    printFloatArray(arr_float_for_timsort, arr_size);
    printDoubleArray(arr_double_for_timsort, arr_size);
    
	before = clock();
	result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, (char*)arr_int_for_timsort, arr_size, sizeof(int),compInts);
	after = clock();
	tim_sort_time_int = (double)(after - before); 

    before = clock();
    result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, (char*)arr_float_for_timsort, arr_size, sizeof(float), compFloats);
    after = clock();
	tim_sort_time_float = (double)(after - before); 

    before = clock();
    result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, (char*)arr_double_for_timsort, arr_size, sizeof(double), compDoubles);
    after = clock();
    tim_sort_time_double = (double)(after - before); 

    before = clock();
    qsort(arr_int_for_qsort, arr_size, sizeof(int), compInts);
    after = clock();
    qsort_time_int = (double)(after - before); 

    before = clock();
    qsort(arr_float_for_qsort, arr_size, sizeof(float), compFloats);
    after = clock();
    qsort_time_float = (double)(after - before); 

    before = clock();
    qsort(arr_double_for_qsort, arr_size, sizeof(double), compDoubles);
    after = clock();
    qsort_time_double = (double)(after - before); 

	printf("Timsort ints = %lfn\n", tim_sort_time_int);
    printf("Timsort floats = %lf\n", tim_sort_time_float);
    printf("Timsort doubles = %lf\n", tim_sort_time_double);
    printf("Qsort ints = %lfn\n", qsort_time_int);
    printf("Qsort floats = %lf\n", qsort_time_float);
    printf("Qsort doubles = %lf\n", qsort_time_double);

	printf("Sorted arrays\n");
	printIntArray(arr_int_for_timsort, arr_size);
    printFloatArray(arr_float_for_timsort, arr_size);
    printDoubleArray(arr_double_for_timsort, arr_size);

    pIMem->pVTbl->Free(pIMem, arr_int_for_timsort);
    pIMem->pVTbl->Free(pIMem, arr_float_for_timsort);
    pIMem->pVTbl->Free(pIMem, arr_double_for_timsort);
    pIMem->pVTbl->Free(pIMem, arr_int_for_qsort);
    pIMem->pVTbl->Free(pIMem, arr_float_for_qsort);
    pIMem->pVTbl->Free(pIMem, arr_double_for_qsort);


Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
    return result;
}
