#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

int16_t ECOCALLMETHOD CEcoLab1_Subtraction(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b);

// Функция QueryInterface для интерфейса IEcoLab1
int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Включение */
    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    /* Агрегирование-догадка, возможно не так */
    else if (pCMe->m_pIUnkOuter != 0){
        return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
    }
    else {
        *ppv = 0;
        return -1;
    }

    /* Агрегирование */
    // /* Передача вызова внешнему компоненту при запросе интерфейса IX */
    //return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);

    return 0;
}



void ECOCALLMETHOD deleteCEcoLab1_IEcoCalculatorX(/* in */  IEcoCalculatorX* pIX) {
    CEcoLab1* pCMe = (CEcoLab1*)pIX;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIX != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_pIEcoCalculatorX != 0 ) {
            pCMe->m_pIEcoCalculatorX->pVTbl->Release(pCMe->m_pIEcoCalculatorX);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}


// Функция AddRef для интерфейса IEcoLab1
uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

// Функция Release для интерфейса IEcoLab1
uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

// Функция copyBytes, копирающая байты памяти из src в dst
void copyBytes(char *src, char *dst, size_t count) {
    char *end = src + count;
    while (src < end) {
        *(dst++) = *(src++);
    }
}

// Функция swapBytes, меняющая местами байты из elem1 и elem2
void swapBytes(char* elem1, char* elem2, size_t type_size) {
    size_t i = 0;
    char tmp;
    for (; i < type_size; ++i) {
        tmp = elem1[i];
        elem1[i] = elem2[i];
        elem2[i] = tmp;
    }
}

int16_t getMinRun(size_t n) {
    int16_t r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

// Один из компонентов timsort
// Реализация сортировки вставками для интерфейса IEcoLab1
int16_t insertionSortBytes(
    struct IEcoLab1* me, 
    char* start_ptr,
    size_t arr_size,
    size_t elem_size,
    size_t left_border,
    size_t right_border,
    int (__cdecl *comp)(const void *, const void*)
) {

    size_t i;
    size_t previous;
    size_t j;

    if (arr_size == 0) return;

    for (i = left_border; i < right_border; i++) {
        j = i + 1;
        previous = i;
        while (comp(start_ptr + previous * elem_size, start_ptr + j * elem_size) == 1 && previous >= 0 && j > left_border) {
            swapBytes(start_ptr + previous * elem_size, start_ptr + j * elem_size, elem_size);
            j--;
            if (previous == 0) break;
            previous--;
        }
    }

    return 0;
}

// Один из компонентов timsort
// Функция слияния для интерфейса IEcoLab1
int16_t mergeBytes(
    struct IEcoLab1* me, 
    char* start_ptr, 
    size_t arr_size, 
    size_t elem_size, 
    size_t left, 
    size_t mid, 
    size_t right,
    int (__cdecl *comp)(const void *, const void*)
) {

    CEcoLab1* pCMe = (CEcoLab1*)me;

    size_t left_count = mid - left + 1;
    size_t right_count = right - mid;

    char* leftPart = (char*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, left_count * elem_size);
    char* rightPart = (char*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, right_count * elem_size);

    size_t i;
    size_t j;
    size_t k;

    for (i = 0; i < left_count; i++) {
        copyBytes(start_ptr + ((left + i) * elem_size), leftPart + i * elem_size, elem_size);
    }

    for (i = 0; i < right_count; i++) {
        copyBytes(start_ptr + ((mid + 1 + i) * elem_size), rightPart + i * elem_size, elem_size);
    }

    i = 0;
    j = 0;
    k = left;

    while (i < left_count && j < right_count) {
        int comp_result = comp(leftPart + i * elem_size, rightPart + j * elem_size);
        if (comp_result <= 0) {
            copyBytes(leftPart + i * elem_size, start_ptr + k * elem_size, elem_size);
            i++;
        }
        else {
            copyBytes(rightPart + j * elem_size, start_ptr + k * elem_size, elem_size);
            j++;
        }
        k++;
    }

    while (i < left_count) {
        copyBytes(leftPart + i * elem_size, start_ptr + k * elem_size, elem_size);
        k++;
        i++;
    }

    while (j < right_count) {
        copyBytes(rightPart + j * elem_size, start_ptr + k * elem_size, elem_size);
        k++;
        j++;
    }

    return 0;
}

// Реализация сортировки timsort
int16_t timSort(
    struct IEcoLab1* me,
    char* start_ptr,
    size_t arr_size,
    size_t elem_size,
    size_t run,
    int (__cdecl *comp)(const void *, const void*)
) {

    size_t i;
    size_t left;
    size_t right;
    size_t mid;

    for (i = 0; i < arr_size; i += run) {
        insertionSortBytes(
            me,
            start_ptr,
            arr_size,
            elem_size,
            i,
            min((i + run - 1), (arr_size - 1)),
            comp
        );

    }

    for (i = run; i < arr_size; i = 2 * i) {
        for (left = 0; left < arr_size; left += 2 * i) {

            mid = left + i - 1;
            right = min((left + 2 * i - 1), (arr_size - 1));

            if (mid < right) {
                mergeBytes(
                    me,
                    start_ptr,
                    arr_size,
                    elem_size,
                    left,
                    mid,
                    right,
                    comp
                );
            }
        }
    }
    return 0;
}


// Функция qsort для интерфейса IEcoLab1
int16_t ECOCALLMETHOD CEcoLab1_qsort(
    struct IEcoLab1* me,
    char* start_ptr,
    size_t arr_size,
    size_t elem_size, 
    int (__cdecl *comp)(const void *, const void*)
) {

    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t index = 0;
    int16_t a = 4;
    int16_t b = 16;
    int test = 0;

    //printf("Addition %d\n", pCMe->m_pVTblIEcoCalculatorX->Addition((IEcoCalculatorX*)pCMe, a++, b++));
    //printf("Substruction result: %d\n", CEcoLab1_Subtraction(pCMe->m_pIEcoCalculatorX, a, b)); 

    if (me == 0 || start_ptr == 0 || comp == 0) 
    {
        printf("Pointer error!\n");
        return -1;
    }

    timSort(me, start_ptr, arr_size, elem_size, getMinRun(arr_size), comp);
    return 0;
}


// Функция Init для интерфейса IEcoLab1
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIEcoCalculatorX);
    if (result != 0 || pCMe->m_pIEcoCalculatorX == 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIEcoCalculatorX);
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIEcoCalculatorY);
    if (result != 0 || pCMe->m_pIEcoCalculatorX == 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIEcoCalculatorY);
    }

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);
    
    return result;
}

int16_t ECOCALLMETHOD CEcoLab1_QueryInterface_IEcoCalculatorX(/* in */ struct IEcoCalculatorX* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoCalculatorX->AddRef((IEcoCalculatorX*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoCalculatorY->AddRef((IEcoCalculatorX*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoCalculatorY->AddRef((IEcoCalculatorX*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoCalculatorX
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRef_IEcoCalculatorX(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoCalculatorX
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_Release_IEcoCalculatorX(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1_IEcoCalculatorX((IEcoCalculatorX*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}



/*
 *
 * <сводка>
 *   Функция Addition
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int32_t ECOCALLMETHOD CEcoLab1_Addition(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    //CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    /* Проверка указателя включаемого компонента и вызов метода */
    if (pCMe->m_pIEcoCalculatorX != 0) {
        //result = me->pVTbl->Addition(pCMe->m_pIEcoCalculatorX, a, b); // РАБОТАЕТ
        //result = pCMe->m_pVTblIEcoCalculatorX->Addition(pCMe->m_pIEcoCalculatorX, a, b); // РАБОТАЕТ
        result = pCMe->m_pIEcoCalculatorX->pVTbl->Addition(pCMe->m_pIEcoCalculatorX, a, b); // НЕ РАБОТАЕТ 
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция Subtraction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_Subtraction(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    /* Проверка указателя включаемого компонента и вызов метода */
    if (pCMe->m_pIEcoCalculatorX != 0) {
        result = pCMe->m_pIEcoCalculatorX->pVTbl->Subtraction(pCMe->m_pIEcoCalculatorX, a, b);
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoCalculatorY
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoCalculatorY_QueryInterface(/* in */ struct IEcoCalculatorY* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoCalculatorX->AddRef((IEcoCalculatorX*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoCalculatorY->AddRef((IEcoCalculatorX*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoCalculatorY->AddRef((IEcoCalculatorX*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoCalculatorY
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoCalculatorY_AddRef(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoCalculatorY
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoCalculatorY_Release(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1_IEcoCalculatorX((IEcoCalculatorX*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Multiplication
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int32_t ECOCALLMETHOD CEcoLab1_Multiplication(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int32_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    result = a * b;

    return result;
}

/*
 *
 * <сводка>
 *   Функция Division
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */

int16_t ECOCALLMETHOD CEcoLab1_Division(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    result = a / b;

    return result;
}



/* Create Virtual Table */
IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTblD = {
    CEcoLab1_QueryInterface_IEcoCalculatorX,
    CEcoLab1_AddRef_IEcoCalculatorX,
    CEcoLab1_Release_IEcoCalculatorX,
    CEcoLab1_Addition,
    CEcoLab1_Subtraction
};

IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTblD = {
    CEcoLab1_IEcoCalculatorY_QueryInterface,
    CEcoLab1_IEcoCalculatorY_AddRef,
    CEcoLab1_IEcoCalculatorY_Release,
    CEcoLab1_Multiplication,
    CEcoLab1_Division
};

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_qsort
};


// Функция Create для интерфейса IEcoLab1
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
    
    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Если не агрегируется, использовать неделегирующий интерфейс IEcoUnknown */
    if (pIUnkOuter != 0) {
    pCMe->m_pIUnkOuter = pIUnkOuter;
    }

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;
    pCMe->m_pVTblIEcoCalculatorX = &g_x9322111622484742AE0682819447843DVTblD;
    pCMe->m_pVTblIEcoCalculatorY = &g_xBD6414C29096423EA90C04D77AFD1CADVTblD;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}



// Функция Delete для интерфейса IEcoLab1
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

