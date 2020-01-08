# PDADD (Parallel disk aware discord discovery)

#### Описание
Алгоритм поиска диссонансов во временном ряде, параллельный для вычислительных кластеров с узлами, имеющими многоядерные ускорители<br>
[Модульная структура](https://github.com/AlexandrGrents/PDADD/blob/master/images/%D0%9C%D0%BE%D0%B4%D1%83%D0%BB%D1%8C%D0%BD%D0%B0%D1%8F%20%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%82%D1%83%D1%80%D0%B0.jpg)<br>
[Файловая структура](https://github.com/AlexandrGrents/PDADD/blob/master/images/%D0%A4%D0%B0%D0%B9%D0%BB%D0%BE%D0%B2%D0%B0%D1%8F%20%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%82%D1%83%D1%80%D0%B0.jpg)<br>
[Структуры данных](https://github.com/AlexandrGrents/PDADD/blob/master/images/%D0%A1%D1%82%D1%80%D1%83%D0%BA%D1%82%D1%83%D1%80%D1%8B%20%D0%B4%D0%B0%D0%BD%D0%BD%D1%8B%D1%85.jpg)<br>

Реализация состоит из четырёх частей:
1. [Алгоритм для 1 узла и 1 ядра](https://github.com/AlexandrGrents/PDADD/tree/master/PDADD-linear) Реализован, протестирован✅<br>
2. [Алгоритм для 1 узла и нескольких ядер](https://github.com/AlexandrGrents/PDADD/tree/master/PDADD-omp) Реализован, протестирован✅<br>
3. [Алгоритм для нескольких узлов и 1 ядра](#) В проекте.<br>
4. [Алгоритм для нескольких узлов и нескольких ядер](#) В проекте.<br>
