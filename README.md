Yoda
====
Общее описание
--------------
Yoda (Йода; от имени персонажа фильмов Звездные войны; или шуточный акроним Yet Other Developments Are) - реализация постфиксного конкатенативного языка программирования Forth, отчасти вдохновлённая Диалоговой Системой Структурированного Программирования (ДССП), разработанной в ВЦ ВМиК МГУ группой учёных, разработавших троичные компьютеры "Сетунь" и "Сетунь-70". Наибольшее влияние выражено в поддержке трёхзначной логики из коробки.

Однако, в отличие от ДССП, которая является прямым продолжением архитектуры компьютера "Сетунь-70", Yoda стремится по мере возможности уйти от понятия машинного слова, которое в сущности является единственным типом данных классического Forth, в пользу потоков тритов (троичных цифр: -1 (*NO*, false), 0 (*YO*, Yes/No, undefined, ...), +1 (*YES*, true)), чтобы
1) предложить наилучшую (наиболее гибкую и продвинутую) работу с логическими значениями в языке Форт;
2) попробовать "достичь" ещё более низкоуровневых операций, нежели это предлагают распространенные Форт-системы (и посмотреть, что из этого получится)
3) Возможно, в крайне отдалённой перспективе придти к более совершенным (энергоэффективным) моделям компьютера

Таким образом, данный подход крайне экспериментальный, ориентирован на гибкую работу с логическим типом данных. Использование чисел произвольной длины (в совокупности с использованием троичной логики и арифметики) дают возможность работы с "управляемой" арифметикой плавающей точки, гарантирующей правильное число значащих цифр для решения инженерных задач. Корректная обработка именно этих типов данных (логического и чисел с плавающей точкой) особо актуальна для промышленной автоматизации.

Также, возможно, данный эксперимент (в качестве побочного эффекта) опишет новую архитектуру троичного компьютера, который, скорее всего, будет не очень производительным, но наверняка будет энергоэффективным. В то же время, Форт-подобные языки (Yoda является Фортом) позволяют решить практическую задачу в условиях недостатка оперативной памяти/регистрового файла, что опять-таки гармонирует с ожидаемой энергоэффективностью: данный гипотетический компьютер можно будет использовать как контроллер "умной вещи".

Перспективы - зачем этот язык был сделан
----------------------------------------
Гибкость Forth-языков (в частности, возможность писать на естественном языке), дающая возможность описывать технологию производства словами (как и действия всех управляемых машин и приборов), в том числе "сшивая" автоматизированные участки с ещё не автоматизированными без обновления документации (!), делает Forth очень интересным языком для дискретного машиностроения.

Совмещение этих возможностей с низкоуровневой реализацией "длинной арифметики", ставшей возможной благодаря трёхзначной арифметике, а также трёхзначной логики, делают язык Yoda вдвойне интересным проектом для дискретного машиностроения, поскольку интегрируют существующие прямо сейчас методы работы с автоматикой (математическая логика + программная обработка вещественных чисел).

Особенности работы
------------------
Система основана на стеках тритов, снабжённых специальными "метками". Эти метки позволяют разграничить разнородные данные. Можно поставить сколько угодно меток на один и тот же трит, так как на данный момент они просто хранятся в другом массиве (организованном с использованием машинных слов; разумеется, в планах минимизировать их роль либо придумать их (хотя бы теоретическую) реализацию в железе.

Метка _завершает_ поток тритов. Трит, на который она ставится, считается последним. Отсутствие элементов в стеке также трактуется как законченный поток (то есть, выражаясь условно, на "-1"ом трите тоже как бы есть метка-разделитель).

Система состоит из трёх участков последовательной памяти, организованных стекоподобно.
1. Стек данных, предназначенный для передачи аргументов конкатенативным функциям, и таким образом хранящий состояние данных в программе.
2. Стек возвратов, предназначенный для возвращения из вложенных функций; НА ДАННЫЙ МОМЕНТ работает по составному адресу возврата, составленному 2 значениями: индексу машинного слова и реальному битовому адресу внутри данному машинного слова. (ВОЗМОЖНО, данная дисгармония в системе будет устранена позднее - на данный момент это сделано ради небольшой экономии тактов процессора, но скорее всего зря)
3. Шитый код по сути тоже является тритовым стеком и на данный момент различает три типа инструкций:
	1. составные. являются ссылкой на такой же шитый код. сверху (над самим адресом, без разделителей) помечаются тритом YO
	2. литерал Forth - инструкция "поместить значение N на стек данных" (в языке Yoda, соответственно, это означает "скопировать тритовую строку на стек данных"). Уточнение: копирование на стек данных производится без разделителей, для постановки разделителя необходимо вызвать соответствующую инструкцию. Данный тип инструкций помечается тритом NO.
	3. машинные. данный тип наиболее простых и универсальных функций реализован на уровне машинного кода и сверху помечается тритом YES. под тритом типа расположена строка-указатель на функцию (проект выполнен на C, поэтому целесообразно указать тип: void (\*ptr)(void) )

Интерпретатор, получив адрес некоторой функции в шитом коде, будет "видеть" шитый код как стек и последовательно "снимать" с него значения (локально, только для самого себя), интерпретируя каждую последовательность по верхнему триту (для машинной инструкции - преобразование в адрес и исполнение, для литерала - непосредственное копирование на стек данных без преобразования в машинное слово, для 

Также __планируется__ (но пока не реализована, см.ниже) "текстовая" часть словаря. Она будет реализовывать ассоциативный массив на шитый код.

Система может работать в двух режимах (на данный момент эти режимы не переключаются и достижимы лишь перекомпиляцией исходных кодов в разных режимах): терминал и автономное выполнение. Первый режим ориентирован на общение человек-машина H2M, а второй на межмашинное взаимодействие M2M. Разумеется, первый режим сложнее реализовать в рамках стандарта языка C (который используется и будет использоваться до тех пор, пока не получится осуществить самокомпиляцию), и при этом сделать решение действительно платформонезависимым и безопасным.

При запуске в режиме M2M, формируется или подгружается существующий шитый код и стек возвратов, после чего осуществляется операция возврата из функции (то есть, берётся адрес из стека возвратов), после чего осуществляется интерпретация.

При запуске в режиме H2M реализуется в целом типичная диалоговая среда языка Форт. (пока это ПЛАНЫ)

Прочие особенности
------------------
Реализация в целом ориентируется на стандарт MISRA C 2012, однако очевидно, что задача такого рода (реализация Форт-подобного языка программирования) предполагает опосредованное нарушение множества правил MISRA - ведь MISRA стремится запретить или хотя бы минимизировать количество "неоднозначных", потенциальных всех источников ошибок в коде на языке программирования C, но реализация иного языка программирования (тем более такого, требующего самоконтроля, как Forth) в каком-то смысле "выключает" эти правила, оставляя РЕАЛИЗАЦИЮ безопасной, но не защищая от небезопасного кода, написанного уже на самом Yoda.

См. также
---------
Устройство Форт-систем довольно просто, как и трёхзначная логика, однако стороннему разработчику может быть сложно с ними разобраться. Мне в этом деле помогли широко распространенные книги и/или интернет-публикации:
* Лео Броуди. Начальный курс программирования на языке Форт [отличное учебное пособие по основам Форта]
* Брусенцов. Микрокомпьютеры [описание ДССП], Начала информатики [описание трёхзначной логики]
* Jon Purdy. Why Concatenative Programming Matters [публикация, разъясняющая суть самого конкатенативного программирования, языком программирования которого является в том числе Форт]
* Баранов, Ноздрунов. Язык программирования Форт [углублённая книга, возможно лучшая отечественная книга по Форт-тематике]
* Chuck Moore. Ранние годы ФОРТа [Публикация-пост от автора оригинального языка Форт о том, в какой последовательности данный язык формировался]
* Крис Касперски. Два слова о трёхзначной логике [преимущества трёхзначной логики], Языки которые мы потеряли [о значении метапрограммирования в решении классов задач, либо мейнстримном (и неудачном, по мнению автора) пути - проектировании систем в духе "C++ ООП", а также о достоинствах языков Форт и Лисп в метапрограммировании]
* Советов Пётр. Программирование мультикомпьютеров на кристалле семейства SEAforth [Возможно единственная публикация на русском языке, объясняющая (притом достойно) работу и смысл существования гомогенных мультикомпьютерных форт-систем, производимых Чаком Муром и его компанией GreenArrays]

Последний автор (@true-grue) дал очень много ценных рекомендаций, и в какой-то момент фактически соориентировал меня в области Форта. Благодарю его за помощь как фактически учителя/руководителя, и надеюсь на дальнейшее с ним сотрудничество в том числе на основе данного проекта.

Планы
=====
Глобальные
----------
Пока есть определенные тенденции, которые хочется соблюдать:
* придерживаться конкатенативности
** в частности, планируется попробовать реализовать подсистему супероптимизации (интерпретатор, который не просто последовательно выполняет код, но умеет его упрощать - с точки зрения конкатенативного программирования упрощение программы и её выполнение суть одно и то же)
* По возможности, уходить от чисел в пользу тритов, там где это возможно (увы, по всей видимости это возможно не везде)
* Применить Yoda со стороны программиста-автоматизатора (т.н. низкий уровень автоматизации)
* Применить Yoda со стороны технолога (т.н. высокий уровень автоматизации) при описании технологического процесса; посмотреть, возможно ли использование метапрограммирования.

Локальные
---------
Прямо сейчас в Yoda, к сожалению, всё ещё остаются нереализованными множество традиционных возможностей Форт-систем:
1. ассоциация шитого кода с названиями функций
2. пока не реализованы никакие функции чтения с терминала
3. пока не реализованы функции вывода на терминал (кроме отладочных)
4. также не реализованы высокоуровневые управляющие конструкции и операции, такие как циклы, ветвления, а также "высокоуровневые" (для потоков тритов неопределенной длины) арифметические и прочие числовые операции
5. реализация литерала и вызова составного слова
6. реализовать своевременную демонстрацию аварийных сообщений (для расширенного, H2M-варианта) языка.