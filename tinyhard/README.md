
![2025-04-18](https://github.com/user-attachments/assets/77757a24-8650-4bd9-93c6-d63a84d39e1e)

Youtube-запись от `2025-04-18`: https://youtu.be/J0HMeGvl6ew

# Шина, протокол, микроконтроллер — наглядное первое знакомство

```mermaid
flowchart LR
kernel(ядро)
driver(драйвер)
mc(микроконтроллер)
hard(железка)
kernel <==> driver <==> mc <==> hard:::h

classDef h fill:#000,color:#fff;
```

- Автоматизируют подключение устройств
- Протокол + шина

```mermaid
flowchart LR
kernel(ядро)
driver(драйвер)
protocol(протокол)
bus(шина)
mc(микроконтроллер)
hard(железка)
mc <==> hard:::h

bus:::n <==> protocol:::n

protocol <==> mc

kernel <==> bus

classDef n fill:#B8BA46;
classDef h fill:#000,color:#fff;
```

### Практика 0

- Получить `Hello World!` с устройства в компьютер
- …стандартными POSIX-средствами

### Практика 1

`Serial` `RX/TX` `GPIO` `UART`

```bash
echo ON | sudo tee /dev/... # пусть там будет лампочка
```

### Практика 2

- Вращаем настоящим моторчиком
