# OptimizationTest
Рассматривается многокритериальная задача оптимизации для размещения мусорных баков по микрорайону Нижней Ельцовки. В качестве целевой функции выбирается решение задачи коммивояжёра по расставленным бакам. Задача размещения должна удовлетворять следующим условиям:

1. Каждый подъезд должен быть обслужен
1. Если есть обслуживание, то надо ставить бак
1. Задано максимальное количество баков p
1. Задано максимальное расстояние от бака до подъезда 

Для решения данной задачи представлен алгоритм полного перебора. Задача коммивояжёра решается простейшей эвристикой ("иди в ближайший"). В дальнейшем планируется:

1. Применить генетический алгоритм для задачи размещения
1. Заменить задачу коммивояжёра на задачу маршрутизации с применением параллельных алгоритмов
1. Модифицировать модель:
   1. Добавить интенсивность производства мусора и как условие на равномерность заполнения баков 
   1. Штраф за переполнение баков
   1. Требование на вывоз мусора по заявке
   
# Данные
Карта с расстановкой баков представлена на картинке OptimizationTest/documents/N_E_Ekvat_marked.png, где расцветка баков означает следующее:

* бордовые - баки стоят, вывоз по расписанию
* синие - баки стоят, вывоз по требованию
* зелёные - оборудованные места, баков нет

Координаты были получены с помощью сервиса 2GIS. Матрицы смежности расстояний были получения с помощью [2GIS api](https://docs.2gis.com/ru/api/navigation/distance-matrix/overview). Тело запроса можно найти в body.json.
