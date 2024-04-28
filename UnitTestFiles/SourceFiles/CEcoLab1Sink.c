#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"

int16_t SMALL_PAUSE = 200;
int16_t BIG_PAUSE = 400;

int16_t ECOCALLMETHOD CEcoLab1Sink_QueryInterface(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }

    *ppv = 0;

    return -1;
}

uint32_t ECOCALLMETHOD CEcoLab1Sink_AddRef(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    pCMe->m_cRef++;
    return pCMe->m_cRef;
}

uint32_t ECOCALLMETHOD CEcoLab1Sink_Release(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1Sink((IEcoLab1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

void CEcoLab1Sink_printIntArray(const void *array, size_t size) {
    int* arr = (int*)array;
    size_t i;
    printf("[");
    for (i = 0; i < size - 1; ++i) {
        printf("%d ", arr[i]);
    }
    printf("%d]\n", arr[size-1]);
}

void CEcoLab1Sink_printFirstHalfOfArray(const void *array, size_t size) {
    int* arr = (int*)array;
    size_t i;
    printf("[");
    for (i = 0; i < size/2 - 1; ++i) {
        printf("%d ", arr[i]);
    }
    printf("%d]\n", arr[size/2 - 1]);
}

void CEcoLab1Sink_printSecondHalfOfArray(const void *array, size_t size) {
    int* arr = (int*)array;
    size_t i;
    printf("[");
    for (i = size/2; i < size - 1; ++i) {
        printf("%d ", arr[i]);
    }
    printf("%d]\n", arr[size-1]);
}

void CEcoLab1Sink_printInsertionSortIteration(const void *array, size_t size, int leftIdx, int rightIdx) {
    int* arr = (int*)array;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    size_t i;

    printf("[");
    for (i = 0; i < size - 1; ++i) {
        if (i == leftIdx) {
            // White Text & Blue Background
            SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("%d", arr[i]);
            // Gray Text & Black Background
            SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf(" ");
        }
        else if (i == rightIdx) {
            // White Text & Red Background
            SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("%d", arr[i]);
            // Gray Text & Black Background
            SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf(" ");
        }
        else {
            printf("%d ", arr[i]);
        }
    }
    if (size -1 == rightIdx) {
        // White Text & Red Background
        SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("%d", arr[size-1]);
        // Gray Text & Black Background
        SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("]\n");
    }
    else {
        printf("%d]\n", arr[size-1]);
    }
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnInsertionSortCalled(struct IEcoLab1Events* me, const void *startPtr, size_t elem_count) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("Before Insertion: ");
    CEcoLab1Sink_printIntArray(startPtr, elem_count);
    Sleep(SMALL_PAUSE);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnInsertionSortEnded(struct IEcoLab1Events* me, const void *startPtr, size_t elem_count) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("After Insertion: ");
    CEcoLab1Sink_printIntArray(startPtr, elem_count);
    Sleep(SMALL_PAUSE);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnInsertionSortIteration(struct IEcoLab1Events* me, const void *startPtr, size_t elem_count, int leftIdx, int rightIdx) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("Iteration Step: ");
    CEcoLab1Sink_printInsertionSortIteration(startPtr, elem_count, leftIdx, rightIdx);
    Sleep(SMALL_PAUSE);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnMergeSortCalled(struct IEcoLab1Events* me, const void *startPtr, size_t elem_count) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (me == 0 ) {
        return -1;
    }

    printf("Before Merge: ");
    // Black Text & White Background
    SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
    CEcoLab1Sink_printIntArray(startPtr, elem_count);
    // Gray Text & Black Background
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n");
    Sleep(BIG_PAUSE);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnMergeSortEnded(struct IEcoLab1Events* me, const void *startPtr, size_t elem_count) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (me == 0 ) {
        return -1;
    }

    printf("First half:");
    // White Text & Blue Background
    SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    CEcoLab1Sink_printFirstHalfOfArray(startPtr, elem_count);
    // Gray Text & Black Background
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\nSecond half:");
    // White Text & Red Background
    SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    CEcoLab1Sink_printSecondHalfOfArray(startPtr, elem_count);
    // Gray Text & Black Background
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n");
    Sleep(SMALL_PAUSE);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnMinRunCalculated(struct IEcoLab1Events* me, int minRun) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (me == 0 ) {
        return -1;
    }

    printf("Calculated value of timsort's minRun: ");
    SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf(" %d \n\n", minRun);
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    Sleep(SMALL_PAUSE);
    return 0;
}



int16_t ECOCALLMETHOD CEcoLab1Sink_Advise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);

    if (result == 0 && pCPC != 0) {
        result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
        pCPC->pVTbl->Release(pCPC);
        pCPC = 0;
        if (result == 0 && pCP != 0) {

            result = pCP->pVTbl->Advise(pCP, (IEcoUnknown*)me, &me->m_cCookie);
            pCP->pVTbl->Release(pCP);
            pCP = 0;
        }
    }

    return result;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_Unadvise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint * pCP = 0;
    int16_t result = 0;

    if (me->m_cCookie) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);
        if (result == 0) {
            result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
            pCPC->pVTbl->Release(pCPC);
            pCPC = 0;
            if (result == 0) {
                result = pCP->pVTbl->Unadvise(pCP, me->m_cCookie);
                pCP->pVTbl->Release(pCP);
                pCP = 0;
            }
        }
    }
    return result;
}

/* Create Virtual Table IEcoLab1VTblEvents */
IEcoLab1VTblEvents g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents = {
    CEcoLab1Sink_QueryInterface,
    CEcoLab1Sink_AddRef,
    CEcoLab1Sink_Release,
    CEcoLab1Sink_OnInsertionSortCalled,
    CEcoLab1Sink_OnInsertionSortEnded,
    CEcoLab1Sink_OnInsertionSortIteration,
    CEcoLab1Sink_OnMergeSortCalled,
    CEcoLab1Sink_OnMergeSortEnded,
    CEcoLab1Sink_OnMinRunCalculated
};

int16_t ECOCALLMETHOD createCEcoLab1Sink(/* in */ IEcoMemoryAllocator1* pIMem, /* out */ IEcoLab1Events** ppIEcoLab1Events) {
    int16_t result = -1;
    CEcoLab1Sink* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1Events == 0 || pIMem == 0 ) {
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Sink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Sink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
    pCMe->m_pIMem->pVTbl->AddRef(pCMe->m_pIMem);

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoP2PEvents */
    pCMe->m_pVTblIEcoLab1Events = &g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents;

    *ppIEcoLab1Events = (IEcoLab1Events*)pCMe;

    return 0;
};

void ECOCALLMETHOD deleteCEcoLab1Sink(IEcoLab1Events* pIEcoLab1Events) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)pIEcoLab1Events;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1Events != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
};
