
![](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

# EcoOS-Lab-1

## Постановка задачи 

Необходимо создать новый компонент, реализующий алгоритм сортировки **timsort**. Новый компонент должен реализовать новый интерфейс с методом (функцией) согласно задаче. Интерфейс должен содержать методы (функции) с разными типами данных (``int``, ``char``, ``float``, ``double``).

## Результаты тестировния

Было проведено тестирование алгоритма с различными входными данными: случайные числа типа ``int``, случайные числа типа ``float``, случайные числа типа ``double``. Размеры сортируемых массивов варьировались от ``10.000`` элементов до ``1.000.000`` элементов.

### Случайные числа типа integer

![image](https://github.com/Sparou/Eco.Lab1/assets/106825713/2e5dce93-79ad-415a-8fc8-147c2690019f)

### Случайные числа типа float

![image](https://github.com/Sparou/Eco.Lab1/assets/106825713/fb559b7d-e844-4318-8c66-3cf306f11b63)

### Случайные числа типа double

![image](https://github.com/Sparou/Eco.Lab1/assets/106825713/c8254b6a-d575-4853-89ab-3d56f9932931)

## Случайные значения типа char

![image](https://github.com/Sparou/Eco.Lab1/assets/106825713/0d1f1496-15fc-4d93-be52-63fd07cc0821)

## Случайные значения типа string

![image](https://github.com/Sparou/Eco.Lab1/assets/106825713/7446f6cd-dbf0-4902-8649-993d4004ab5f)

## Выводы

Из приведенных выше данных можно сделать вывод, что время работы сортировки **напрямую зависит от входных данных**. В целом, моя реализация алгоритма **Timsort** уступает в скорости стандартной функции сортировки ``qsort`` из ``stdlib`` примерно ``в 3 – 4 раза``, однако все ровно показывает хорошие результаты на всех типах входных данных. Худший результат обе функции показали на входных данных типа ``string``, тогда как лучший достигнут при тестировании на типе данных ``char``. Это связано с тем, что тип ``string`` занимает наибольшее количество байт памяти, в то время как тип ``char`` ― наименьшее.
