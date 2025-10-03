
<img width="1280" height="720" alt="2025-10-03" src="https://github.com/user-attachments/assets/876f863e-c5f6-4ba3-ba80-3ed0e57a6955" />

Youtube-запись от `2025-10-03`: https://youtu.be/lAGk64EB0yA

# Расшифровываем WiFi-логи

```
ts_us,channel,len,rssi,mac_hash,eapol,head
502333,1,78,-80,ed1615538a1a3ab6,0,0802000001000ccccccd52823d9fb568
```

* WiFi кадры == «пакеты»
* Одна строка — один кадр
* **Записываем не всё!**

`1` миллисекунды с начала записи

`2` канал (их несколько)

`3` длина кадра в байтах

`4` уровень сигнала в dBm (rssi, отрицательное)

`5` MAC-адрес источника (**зашифрованный**, первые 16 символов)

`6` маркер EAPOL-протокола (подключение или попытка)

`7` первые 16 (или меньше) байт кадра, там много чего зашито

##### Сколько записей в логе?
```bash
wc -l [имя файла]
```

##### Сколько времени мы писали лог?
```bash
awk -F',' 'NR==2 {first=$1} {last=$1} END { printf("%0.0f минут\n", (last - first) / 60000000); }' long.csv
```

##### Какие каналы мы мониторили?
```bash
cut -d',' -f2 long.csv | sort -n | uniq
```

##### А если не смотреть на первую строку?
```bash
tail -n +2 long.csv | ...
```

```bash
sed '1d' long.csv | ...
```

```bash
awk 'NR>1' long.csv | ...
```

##### Сколько пакетов пришло по какому каналу?
```bash
awk -F',' 'NR>1 { count[$2]++ } 
END {
  for (c in count) {
    printf("%-7s %d\n", c, count[c]);
  }
}' long.csv | sort -k2 -nr
```

##### Какие устройства были самыми активными?
```bash
cut -d',' -f5 long.csv | sort -n | uniq -c | sort -nr | head -n +10
```

##### Какие устройства подключались активней других?
```bash
cut -d',' -f5,6 long.csv | grep ',1' | sort | uniq -c | sort -nr | cut -d',' -f1 | head -n +20
```

##### На сладкое
- Frame Control — первые 2 байта кадра **(тут считаем слева направо)**

- Первый байт (fc0) кодирует:
	- Type = биты 2–3
    - Subtype = биты 4–7
    **(а тут — справа налево!)**

Type = 0 → Management (управляющие кадры)
- Subtype 8 → Beacon (маячки, которые рассылает точка доступа с SSID)
- Subtype 4 → Probe Request (клиент ищет сети)
- Subtype 5 → Probe Response (AP отвечает клиенту)
- Subtype 0/1 → Association Request/Response (подключение)
- Subtype 10/12 → Disassociation / Deauthentication (разрыв соединения)

Type = 1 → Control (служебные кадры)
- Subtype 13 → ACK (подтверждение)
- Subtype 11 → RTS (Request To Send)
- Subtype 12 → CTS (Clear To Send)

Type = 2 → Data (данные)
- Subtype 0 → Data (обычный кадр с полезной нагрузкой, может быть EAPOL, IP и т.д.)

```bash
awk -F',' 'NR>1 {
  fc0 = strtonum("0x" substr($7,1,2))
  
  type = int(fc0 / 4) % 4
  subtype = int(fc0 / 16) % 16

  if (type==0 && subtype==8) name="Beacon"
  else if (type==0 && subtype==4) name="Probe Request"
  else if (type==0 && subtype==5) name="Probe Response"
  else if (type==0) name="Mgmt"
  else if (type==1 && subtype==13) name="ACK"
  else if (type==1) name="Ctrl"
  else if (type==2) name="Data"
  else name="Other"

  count[name]++
}
END {
  for (n in count) printf("%-15s %d\n", n, count[n])
}' long.csv | sort -k2 -nr
```

