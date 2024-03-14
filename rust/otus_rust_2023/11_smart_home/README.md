### Крейт thiserror

Цель:
Научиться пользоваться библиотекой thiserror.
Результатом является:
Рефакторинг библиотеки "умный дом".


Описание/Пошаговая инструкция выполнения домашнего задания:
Заменить ручную реализацию обработки ошибок в библиотеке "умный дом" на использование крейта thiserror.


Критерии оценки:
Статус "Принято" ставится, если:

Библиотека компилируется, тесты выполняются.
Реализации трейтов Error, Display и From заменены на макросы из thiserror, где это возможно.
Утилита cargo clippy не выдаёт предупреждений.
Команда cargo fmt --check не выдаёт предупреждений.