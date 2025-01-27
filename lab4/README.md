# LAB 1

## Цели работы

Задание: разработать шаблонные классы для хранения разреженных вектора и
2D матрицы для чего:
- Предложить структуры данных для хранения значений исходных векторов и матриц. (подсказка: использование хеш-таблиц — хорошая идея), рассмотрите необходимость поддержки собственных итераторов;
- Реализовать набор унарных и бинарных операций для вектора и матриц; (транспонирование, сложение, произведение векторов (вспоминаем линейную алгебру), обращение матрицы, возведение матрицы в степень — предусмотреть два варианта с целочисленным и вещественным показателем, подумать на тему относительно того, как решить вопрос с возведением в степень — т. е. в степень можно возводить только квадратные матрицы, но не создавать же отдельный класс для квадратных и для прямоугольных матриц, или создавать?), произведение вектора и матрицы.
- Реализовать поэлементные операции для элементов векторов и матриц — арифметические операции со скалярной величиной, поэлементное возведение в степень. 
- Провести сравнение скоростей обработки предложенного вами способа хранения и обработки разреженных матриц и выполнение тех же действий над векторами и матрицами, хранящимся с помощью стандартного контейнера vector. 
- Результаты работы оформить в виде отчета или файла readme если код будет на Git.

## Выполнение

В ходе выполнения работы были разработаны классы матрицы и вектора. 
Данные в векторе хранятся с помощью std::unordered_map, где в качестве ключа выступает индекс. Хранятся только значения отличные от нуля.
Данные в матрице также хранятся с помощью std::unordered_map, но в отличие от вектора, в качестве ключа выступает пара значений: индекс строки и индекс колонки.

## Выводы по работе

В ходе выполнения работы, были разработана программа на языке C++, изучены принципы работы с разреженными структурами