# OptimizationTest
Рассматривается многокритериальная задача оптимизации для размещения мусорных баков по микрорайону Нижней Ельцовки. В качестве целевой функции выбирается решение задачи коммивояжёра по расставленным бакам. Задача размещения должна удовлетворять следующим условиям:
1. Каждый подъезд должен быть обслужен
1. Если есть обслуживание, то надо ставить бак
1. Задано максимальное количество баков
1. Задано максимальное расстояние от бака до подъезда 

Задача коммивояжёра решается простейшей эвристикой ("иди в ближайший"). Задача размещения решается генетическим алгоритмом со следующими особенностями:
* Начальная популяция выбирается случайно
* Турнирная селекция для выбора родителя
* Вероятностный оператор мутации
* Алгоритм локального спуска с настриваемой n-окрестностью

В дальнейшем планируется:
1. Придумать простую эвристику для выбора начальной полпуляции
1. Заменить задачу коммивояжёра на задачу маршрутизации с применением параллельных алгоритмов
1. Модифицировать модель:
   1. Добавить интенсивность производства мусора и условие на равномерность заполнения баков 
   1. Штраф за переполнение баков
   1. Требование на вывоз мусора по заявке
   
# Данные
Карта с расстановкой баков представлена на картинке:
![N_E_Ekvat_marked](https://user-images.githubusercontent.com/22174649/126031709-0e6e49d5-5dd4-47dc-b585-16f8bd62c489.png)
где расцветка баков означает следующее:
* бордовые - баки стоят, вывоз по расписанию
* синие - баки стоят, вывоз по требованию
* зелёные - оборудованные места, баков нет

Координаты были получены с помощью сервиса 2GIS. Матрицы смежности расстояний были получения с помощью [2GIS api](https://docs.2gis.com/ru/api/navigation/distance-matrix/overview). Тело запроса можно найти в documents/Тестовый пример/body.json.

# Зависимости
Для десериализации json была использована библиотека [boost](https://nuwen.net/mingw.html)
