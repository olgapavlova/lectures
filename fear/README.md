<img width="1280" height="720" alt="2025-12-26-2" src="https://github.com/user-attachments/assets/e906b876-65f6-4007-8234-5e9f979f3b25" />

Youtube-запись от `2026-12-26`: https://youtu.be/AIKYXFIRIkI

# Разговор с теми, кто боится программирования
```mermaid
flowchart LR
dev([Программирование]):::red
art([Искусство и профессия]):::blue
app([Прикладной инструмент]):::green
p@{ shape: comment, label: "«Я могу закодить что угодно»" }
e@{ shape: comment, label: "«Я знаю, что нужно закодить»"}

dev ==> app
dev ==> art

app <-.-> e
art <-.-> p

classDef green fill:#9A9A00,stroke:#9A9A00,color:#F6F0D8;
classDef yellow fill:#CC9105,stroke:#CC9105,color:#F6F0D8;
classDef red fill:#D65D0E,stroke:#D65D0E,color:#F6F0D8;
classDef white fill:#FBFAF3,stroke:#F6F0D8,color:#272727;
classDef blue fill:#0077BB,stroke:#0077BB,color:#F6F0D8;

```
> [!TIP]
> 50+ лет программисты программировали *компьютеры*
> Ну и привыкли, конечно.

## А что мы программируем `сейчас`?
Уж точно не компьютеры. Не железки.

### 1 • Всё уже ~~украдено~~ закодили до нас
Осталось взять чужое и присобачть туда, где вы уже обитаете.

#### Система 3D-моделирования
> [!WARNING]
> FreeCAD — OpenSCAD — Autodesk Fusion 360

<img width="2184" height="1262" alt="Pasted image 20251226165019" src="https://github.com/user-attachments/assets/bf14ad28-88b6-4e61-95d8-fd5da02cbea4" />

> «Создай трёхмерный объект по чертежу и параметрам»
> • Python — *свой язык*

#### Редактор электронных схем
> [!WARNING]
> KiCAD

<img width="2394" height="1224" alt="Pasted image 20251226165437" src="https://github.com/user-attachments/assets/2d24c068-48ee-472e-93e3-520ee4f9eed4" />

> «Преврати эскиз в готовый результат по всем правилам»
> • Python

#### Музыкальный плеер
> [!WARNING]
> VLC

<img width="1988" height="1117" alt="Pasted image 20251226165710" src="https://github.com/user-attachments/assets/8e97b1e4-72ab-45b9-a0d6-cc6bdd57a2a1" />

> «Учти конкретные потребительские предпочтения пользователя»
> • Lua

#### Медиаредакторы
> [!WARNING]
> OBS — DaVinci

<img width="648" height="252" alt="Pasted image 20251226172439" src="https://github.com/user-attachments/assets/6048bbeb-0066-4c52-9544-3eb87ca831f3" />

> «Собери и оформи медиаконтент из источников данных»
> • Python — Lua

#### Специализированные базы знаний
> [!WARNING]
> Wolfram Mathematica — Anki

<img width="1902" height="692" alt="Pasted image 20251226172543" src="https://github.com/user-attachments/assets/07184f74-8fde-4886-a2cd-b9b05f73599d" />

> «Создай среду для экспериментов в предметной области»
> • *внутренний язык*

### 2 • Пощупаем настоящее программирование ~~за вымя~~

#### Виртуальная среда разработки
> [!INFO]
> Sketch — Processing — p5.js

<img width="2184" height="1262" alt="2025-12-26 17-34-15 verbs" src="https://github.com/user-attachments/assets/071c4beb-b19d-4317-8c77-2eba8722adf5" />

> «Дай возможность привыкнуть к абстрактным моделям и их **традиционному** поведению»
> • *внутренний язык* — — Wired — Java — Python — JavaScript — C

#### Электронный документ
> [!INFO]
> Google \* — Obsidian — Microsoft \* — vim — …

![Кусочек багтрекера | SobakaPav.ru](https://sobakapav.ru/_astro/5.DBDwrCPE_2m1TGG.webp)

> «Реализуй сложные зависимости внутри данных»
> • *пропиетарные и редкие языки*

#### Оболочка операционной системы
> [!INFO]
> macOS — iOS — Linux

<img width="2184" height="1262" alt="Pasted image 20251226173839" src="https://github.com/user-attachments/assets/e03e7381-847c-44ff-ba58-bb489dc305b9" />

> «Автоматизируй то, что обычно я делаю руками»
> • Automator — bash script

#### Физический мир
> [!INFO]
> Оборудование — yмный дом — сети и связь — голосовые помощники 

<img width="2184" height="1334" alt="Pasted image 20251226174246" src="https://github.com/user-attachments/assets/0828720d-c423-4f3a-b513-7b1ad89ab2e3" />

> «Преврати физическое поведение в цифры и обратно»
> • Arduino — C — ESP-IDF — MicroPython


### 3 • Очарование универсальных интерфейсов

#### Чат-бот
> [!TIP]
> Telegram

<img width="951" height="493" alt="Pasted image 20251226174606" src="https://github.com/user-attachments/assets/0cb78a2b-c069-485e-8a81-6ea428357489" />

> «Спрячь сложную систему за простым интерфейсом»
> • Python — JavaScript

#### Браузер
> [!TIP]
> Chrome — Safari — Chromium

<img width="2184" height="1262" alt="Pasted image 20251226174845" src="https://github.com/user-attachments/assets/813e2095-0815-4080-bf98-24ee5df69faf" />

> «Погоняй данные по сети, оформи их красивенько, дай пощупать»
> • JavaScript — DevTools — Python — Selenium


#### Electron-сборка
> [!TIP]
> Figma — Slack — Notion — VSCode — …

<img width="2184" height="1336" alt="Pasted image 20251226175303" src="https://github.com/user-attachments/assets/107c373f-9f87-406e-8a22-4514ada57a54" />

> «Автоматизируй то, у чего не дошли руки у разработчиков сборки»
> • TypeScript — React — JavaScript





## Но как же процессор, C и числа Фибоначчи?
> [!CAUTION]
> Давным-давно, когда мы…
> …не были обвешаны компьютерами с ног до головы,
> …с трудом вообще с ними взаимодействовали,
> …не могли освоить и технологии, и другую профессию…

> [!IMPORTANT]
> …выживание зависело от интереса к компьютерам
> И вот так он проявлялся — в исследовании их возможностей.
> С помощью освоения и создания технологий.
> И задач на вычисления ради вычислений.

```mermaid
flowchart TD

s([Программист-небожитель]):::red
p([Обычный программист]):::blue
e([Эксперт в теме]):::green

subgraph Технология
kernel([Для программистов]):::black
api([Для народа]):::yellow
end

s --> |создаёт| kernel
p --> |использует| kernel
p --> |реализует| api
e ==> |придумывает| p

api -.-> |пытается применить | e

classDef red fill:#D65D0E,stroke:#D65D0E,color:#F6F0D8;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#F6F0D8;
classDef yellow fill:#CC9105,stroke:#CC9105,color:#F6F0D8;
classDef blue fill:#0077BB,stroke:#0077BB,color:#F6F0D8;
classDef black fill:#272727,stroke:#272727,color:#FBFAF3;

```

> [!INFO]
> А теперь давайте займёмся своими делами
> Автоматизируем своё. Как можем.
> А программисты — своё.
> И все при деле.

```mermaid
flowchart TD

p([Программист]):::red
e([Эксперт в теме]):::green
tech([Технология]):::yellow

p --> |создаёт| tech
e <==> |эксплуатирует| tech

classDef red fill:#D65D0E,stroke:#D65D0E,color:#F6F0D8;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#F6F0D8;
classDef yellow fill:#CC9105,stroke:#CC9105,color:#F6F0D8;
classDef blue fill:#0077BB,stroke:#0077BB,color:#F6F0D8;
classDef black fill:#272727,stroke:#272727,color:#FBFAF3;

```


## И что нам нужно?
> [!INFO]
> Экспертиза в предметной области

> → **Находить** и формулировать задачи
> → Быстро-быстро-**быстро** итерировать
> → Распознавать **приемлемый** результат
> → Делать только нужное с • :LiEye: эксперта

> [!NOTE]
> Workflow автоматизации

> 90% времени занимает отладка.
> **Теперь** она на эксперте.

> [!TIP]
> Навыки освоения технологий

> [!CAUTION]
> Ну и так, по мелочи

 - Основы написания структурированного кода
 - Основы ООП
 - Основы функционального программирования
 - Работа с API
 - Работа со структурированными данными
 - Command Line Interface (CLI)
 - Ключевые архитектуры (высокоуровнево)
 - …




