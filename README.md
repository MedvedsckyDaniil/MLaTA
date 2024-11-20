# Коллоквиум по МЛиТА
# Выводимость логических формул из аксиом
https://docs.google.com/document/d/1E9GDMKmo-WaZJKBVPZp200X0lrg41si4rYcLbqGdDao/edit?tab=t.0

## Постановка задачи

Известны аксиомы исчисления высказываний: <br/>
A1.   (A→(B→A)) <br/>
A2.   ((A→(B→C))→((A→B)→(A→C))) <br/>
A3.   ((-B→-A)→((-B→A)→B)) <br/>
Из этих аксиом и правила вывода Modus ponens можно вывести все тождественные формулы исчисления высказываний, например, закон Моргана и пр.
Modus ponens:   A, A→B ⊢  B <br/>
Конъюнкция и дизъюнкция выражаются через отрицание и импликацию.


## Структура проекта

#### Папки

`/results` – папка со скриншотами полученных в результате работы программы выводов теорем A4-A11 из аксиом A1-A3.

#### Файлы

* `/CMakeLists.txt` – сборка проекта;
* `/Makefile` – альтернативная сборка проекта;
* `/main.cpp` – создание и заполнение списка высказываний, полный циклический рекурсивный перебор применения правил вывода на парах высказываний;
* `/conclude.cpp` – вывод нового выражения по одному из заданных правил: "modus_ponens", "modus_tollens", "дизъюнктивный_силлогизм", "гипотетический_силлогизм", "разделительный_силлогизм";
* `/conclude.h` – соответсвующий заголовочный файл;
* `/converter.cpp` – преобразование выражений с дизъюнкциями, конъюнкциями, XOR-ами и эквивалентностями в выражения только с отрицаниями и импликациями;
* `/converter.h` – соответсвующий заголовочный файл;
* `/to_str.cpp` – получение строки логического выражения;
* `/to_str.h` – соответсвующий заголовочный файл, определение struct statement.

#### Функции `/main.cpp`

Таблица 1. Функции из главного файла `/main.cpp`

| Название функции                                                                                                                                                       | Что делает и что возвращает                                                                                                                                                                                                                                              |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `int add_size(const std::string& str)`                                                                                                                                 | Определяет, сколько вариантов строки функция `enumeration()` добавит в спискок `std::vector<struct statement> list`, возвращает полученное колличество.                                                                                                                  |
| `std::string list_pusher(std::string str, char sim, const std::string& var)`                                                                                           | Заменяет заданный символ в строке за заданную строку-замену, возвращает полученную строку.                                                                                                                                                                               |
| `std::vector<struct statement> enumeration(std::vector<struct statement> list, const std::string& str)`                                                                | Добавляет в список `std::vector<struct statement> list` все варианты нового высказывания, возвращает полученный список.                                                                                                                                                  |
| `int compare_str(const std::vector<struct statement>& list, size_t start_ind, const std::string& str)`                                                                 | Проверяет, есть ли в списке `std::vector<struct statement> list` с заданного индекса данное высказывание.                                                                                                                                                                |
| `std::string step(std::vector<struct statement> A, size_t start_i, const std::vector<std::string>& rulse, const std::string& str, size_t deep, const size_t max_deep)` | Циклический (все пары) рекурсивный (в глубину рекурсии создания новых элементов списка) перебор вывода необходимого высказывания из ранее полученных (изначально – только из аксиом и их подстановок), возвращает строку с полным выводом или "CAN'T FIND A CONCLUSION". |
| `int main()`                                                                                                                                                           | Создание и заполнение списка высказываний аксиомами и их подстановками, ввод высказывания, которое необходимо вывести, конвертирование (`str = converter(input);`) проверка на равенство с аксиомами и их подстановками, запуск циклического рекурсивного перебора.      |

## Детали ввода-вывода

Программа обрабатывает только выражения с тремя буквами английского алфавита: 'A', 'B' и 'C' и шестью операциями: '!', '|', '*', '>', '+', '='. <br/>
Любые выражения с бинарными операциями берутся в скобки. Отрицание же (единственная рассматриваемая унарная операция) не берётся в скобки.

#### Допустимые символы операций

Таблица 2. Символы допустимых математических операций при вводе и при выводе

| Математическая операция | Символ при вводе | Символ при выводе |
|-------------------------|------------------|-------------------|
| Отрицание               | !                | -                 |
| Дизъюнкция              | \|               | ∨                 |
| Конъюнкция              | *                | ∧                 |
| Импликация              | \>               | →                 |
| XOR                     | +                | ⊕                 | 
| Эквиваленция            | =                | ≡                 |

#### Конвертирование

A→B = -A∨B <br/>
then <br/>
A∨B = -A→B <br/>
(A∧B) = -(-A∨-B) = -(A→-B) <br/>
A⊕B = (-A∧B)∨(A∧-B) = -(A∨-B)∨-(-A∨B) = -(-A→-B)∨-(A→B) = (-A→-B)→-(A→B) <br/>
A≡B = -(A⊕B) = (A∧B)∨(-A∧-B) = -(-A∨-B)∨-(A∨B) = -(A→-B)∨-(-A→B) = (A→-B)→-(-A→B)

#### Конвертированные правила вывода

`if (rule == "дизъюнктивный_силлогизм")` <br/>
-P, P∨Q ⊢ Q <br/>
-P, -P→Q ⊢ Q <br/>
P, P→Q ⊢ Q <br/>
`if (rule == "разделительный_силлогизм")` <br/>
P, P ⊕ Q ⊢ -Q <br/>
P, (-P→-Q)→-(P→Q) ⊢ -Q <br/>

## Результаты

Таблица 3. Результаты выводов теорем

| **Номер теоремы**  | Теорема                   | Сконвертированная теорема       | "modus_ponens"                          | "modus_tollens"                        | "дизъюнктивный_силлогизм"               | "гипотетический_силлогизм"              | "разделительный_силлогизм"             | all of them                            |
|--------------------|---------------------------|---------------------------------|-----------------------------------------|----------------------------------------|-----------------------------------------|-----------------------------------------|----------------------------------------|----------------------------------------|
| **-**              | (A>A)                     | (A>A)                           | <span style='color: green;'> ✓ </span>  | <span style='color: red;'> ✗ </span>   | <span style='color: green;'> ✓ </span>  | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: green;'> ✓ </span> |
| **A4**             | ((A*B)>A)                 | (!(A>!B)>A)                     | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>  |
| **A5**             | ((A*B)>B)                 | (!(A>!B)>B)                     | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>  |
| **A6**             | (A>(B>(A*B)))             | (A>(B>!(A>!B)))                 | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    |
| **A7**             | (A>(A\|B))                | (A>(!A>B))                      | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>  |
| **A8**             | (B>(A\|B))                | (B>(!A>B))                      | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>  |
| **A9**             | ((A>C)>((B>C)>((A\|B)>C)) | ((A>C)>((B>C)>((!A>B)>C))       | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   |
| **A10**            | (!A>(A>B))                | (!A>(A>B))                      | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>    | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>  |
| **A11**            | (A\|!A)                   | (!A>!A)                         | <span style='color: green;'> ✓ </span>  | <span style='color: red;'> ✗ </span>   | <span style='color: green;'> ✓ </span>  | <span style='color: red;'> ✗ </span>   | <span style='color: red;'> ✗ </span>   | <span style='color: green;'> ✓ </span> |

<span style='color: green;'> ✓ </span> – используя это правило, программа смогла вывести выражение; <br/>
<span style='color: red;'> ✗ </span> – используя это правило, программа не смогла вывести выражение.

Формулировка теорем и сконвертированных теорем приведена в формате ввода в программу для того, чтобы можно было сразу их скопировать и проверить правдивость результатов.

#### "modus_ponens"

`(A>A)` <br/>
![(A>A).png](/results/modus_ponens/(A>A).png) <br/>
`(A|!A)` <br/>
![(A|!A).png](/results/modus_ponens/(A|!A).png)

#### "modus_tollens"

<span style='color: red;'> ✗ </span>

#### "дизъюнктивный_силлогизм"

`(A>A)` <br/>
![(A>A).png](/results/дизъюнктивный_силлогизм/(A>A).png) <br/>
`(A|!A)` <br/>
![(A|!A).png](/results/дизъюнктивный_силлогизм/(A|!A).png)

#### "гипотетический_силлогизм"

<span style='color: red;'> ✗ </span>

#### "разделительный_силлогизм"

<span style='color: red;'> ✗ </span>

#### all_of_them

`(A>A)` <br/>
![(A>A).png](/results/all_of_them/(A>A).png) <br/>
`(A|!A)` <br/>
![(A|!A).png](/results/all_of_them/(A|!A).png)

***

## Список использованных источников

1) Московский физико-технический институт, Факультет инноваций и высоких технологий, Математическая логика и теория алгоритмов, осень 2018, Лекции 5–7: исчисление высказываний и теорема о компактности. URL: <br/>
   http://ru.discrete-mathematics.org/fall2017/1/logic_pmi/2018/lecture-5-7-prop-calculus.pdf

