<img width="1280" height="720" alt="2026-03-13" src="https://github.com/user-attachments/assets/fc4c6eeb-bc42-4ca8-bed4-cc219db99d74" />

Youtube-запись от `2026-03-13`: https://youtu.be/fu_Bp8TxIzE

# SPI — не так уж много проводочков

![IMG_0745](https://github.com/user-attachments/assets/0b76f9bd-6fe7-4428-862c-826add4df591)


## Как всегда, две микросхемы должны договориться

### Хотим двухполосное движение 
```mermaid
flowchart LR

main(Центр):::red
second(Устройство):::green

main ==> |данные туда| second
second -.-> |данные обратно| main

classDef red fill:#D65D0E,stroke:#D65D0E,color:#FDF9EC;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#FDF9EC;

```
> [!NOTE]
> **туда** == `Main Out → Second In` == `MOSI`

> [!NOTE]
> **обратно** == `Second Out → Main In` == `MISO`


### Устройство — дальтоник с СДВГ
- Не отличает одну единицу от нескольких подряд — *ну а как?*
- Не может постоянно смотреть на вход и ловить нули и единички — *отвлекается*

Поэтому нужно «щёлкать пальцами», чтобы привлечь внимание устройства:
```mermaid
flowchart LR

main(Центр):::red
second(Устройство):::green

main --> |MISO| second
main ==> |щёлк-щёлк-щёлк| second
second -.-> |MOSI| main

classDef red fill:#D65D0E,stroke:#D65D0E,color:#FDF9EC;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#FDF9EC;

```
> [!NOTE]
> **щёлк-щёлк-щёлк** == `Serial Clock` == `SCK`

### И он ещё поспать любит!
Постучись, разбуди, потом уже разговаривай. *Ну, в первом приближении.*
```mermaid
flowchart LR

main(Центр):::red
second(Устройство):::green

main --> |MISO| second
main --> |SCK| second
main ==> |есть кто дома?| second
second -.-> |MOSI| main

classDef red fill:#D65D0E,stroke:#D65D0E,color:#FDF9EC;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#FDF9EC;

```
> [!NOTE]
> **есть кто дома?** == `Chip Select` == `CS`

### Select как бы намекает…
Каждому устройству подавай отдельную линию-будильник.

```mermaid
flowchart LR

main(Центр):::red

bus((•)):::black

subgraph Устройства
d1(1):::green
d2(2):::green
end

main --> |MISO| bus
main --> |SCL| bus

main x-.-x d1
main ==> |подъём!| d2
bus o--o |о, это мне| d2

bus -.-> |MOSI| main

classDef red fill:#D65D0E,stroke:#D65D0E,color:#FDF9EC;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#FDF9EC;
classDef black fill:#282828,stroke:#282828,color:#FFFFFF;

```
- Конечно, можно размножать «будильники» и не на ножках микроконтроллера. Например, у нас уже был сдвиговый регистр для таких целей.
- Есть устройства (чипы), умеющие будить друг друга по цепочке. Но это экзотика.

> [!WARNING]
> Итого: быстро и просто.
> Немного сбивает с толку разнообразие вариантов и обозначений.

Тут-то мы и вспомним про внешнюю flash-память. Она же как раз на SPI!
Ссылка на каталог JEDEC ID: https://antenna-dvb-t2.ru/spi_nor_flash.php
