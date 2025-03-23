#!/bin/bash

INTERFACE="wlp1s0"  # Zmień na odpowiedni interfejs sieciowy
TOTAL_RX=0
TOTAL_TX=0
COUNT=0
HISTORY_SIZE=10  # Długość historii dla wykresu
RX_HISTORY=()
TX_HISTORY=()

clear
while true; do
    # Prędkość wysyłania i odbierania danych z sieci
    RX1=$(cat /proc/net/dev | grep $INTERFACE | awk '{print $2}')
    TX1=$(cat /proc/net/dev | grep $INTERFACE | awk '{print $10}')
    sleep 1
    RX2=$(cat /proc/net/dev | grep $INTERFACE | awk '{print $2}')
    TX2=$(cat /proc/net/dev | grep $INTERFACE | awk '{print $10}')
    RX_RATE=$((RX2 - RX1))
    TX_RATE=$((TX2 - TX1))
    
    if [ $RX_RATE -lt 0 ]; then RX_RATE=0; fi
    if [ $TX_RATE -lt 0 ]; then TX_RATE=0; fi

    RX_RATE_HUMAN=$(numfmt --to=iec $RX_RATE)
    TX_RATE_HUMAN=$(numfmt --to=iec $TX_RATE)

    # Aktualizacja sumarycznych wartości i liczby pomiarów
    TOTAL_RX=$((TOTAL_RX + RX_RATE))
    TOTAL_TX=$((TOTAL_TX + TX_RATE))
    COUNT=$((COUNT + 1))

    # Obliczanie średniej prędkości
    AVG_RX=$((TOTAL_RX / COUNT))
    AVG_TX=$((TOTAL_TX / COUNT))
    AVG_RX_HUMAN=$(numfmt --to=iec $AVG_RX)
    AVG_TX_HUMAN=$(numfmt --to=iec $AVG_TX)

    # Aktualizacja historii pomiarów
    RX_HISTORY=("${RX_HISTORY[@]}" "$RX_RATE")
    TX_HISTORY=("${TX_HISTORY[@]}" "$TX_RATE")
    if [ ${#RX_HISTORY[@]} -gt $HISTORY_SIZE ]; then
        RX_HISTORY=("${RX_HISTORY[@]:1}")
        TX_HISTORY=("${TX_HISTORY[@]:1}")
    fi

    # Przesunięcie kursora na początek terminala
    tput cup 0 0
    clear


    # Wykorzystanie rdzeni procesora
    CPU_USAGE=$(grep 'cpu' /proc/stat | awk 'NR>1 {usage=($2+$4)*100/($2+$4+$5); printf "CPU%d: %.2f%%\n", NR-2, usage}')
    echo "$CPU_USAGE"

    # Częstotliwość pracy rdzeni procesora
    for cpu in /sys/devices/system/cpu/cpu[0-9]*; do
        freq=$(cat $cpu/cpufreq/scaling_cur_freq 2>/dev/null)
        if [ -n "$freq" ]; then
            printf "%s: %4d MHz\n" "$(basename $cpu)" "$((freq / 1000))"
        fi
    done

    # Czas działania systemu
    UPTIME=$(awk '{print int($1/86400)"d "int(($1%86400)/3600)"h "int(($1%3600)/60)"m "int($1%60)"s"}' /proc/uptime)
    echo "Czas działania systemu: $UPTIME"

    # Stan baterii
    if [ -f /sys/class/power_supply/BAT0/uevent ]; then
        BATTERY=$(grep 'POWER_SUPPLY_CAPACITY=' /sys/class/power_supply/BAT0/uevent | cut -d= -f2)
        echo "Stan baterii: $BATTERY%"
    fi

    # Obciążenie systemu
    LOADAVG=$(cat /proc/loadavg | awk '{print "1 min: "$1", 5 min: "$2", 15 min: "$3}')
    echo "Obciążenie systemu: $LOADAVG"

    # Wykorzystanie pamięci bez buffers i cached
    MEMINFO=$(awk '/MemTotal/ {total=$2} /MemFree/ {free=$2} END {used=total-free; print "Pamięć całkowita: " total/1024 " MB, Pamięć używana: " used/1024 " MB, Pamięć wolna: " free/1024 " MB"}' /proc/meminfo)
    echo "$MEMINFO"

    # Użycie printf do formatowania wyjścia
    printf "Prędkość odbierania: %6s/s, Prędkość wysyłania: %6s/s\n" "$RX_RATE_HUMAN" "$TX_RATE_HUMAN"
    printf "Średnia prędkość odbierania: %6s/s, Średnia prędkość wysyłania: %6s/s\n" "$AVG_RX_HUMAN" "$AVG_TX_HUMAN"
    # Wykres słupkowy historii prędkości
    echo -e "Historia prędkości odbierania (KB/s):"
    for rate in "${RX_HISTORY[@]}"; do
        printf "%.0f|" "$((rate / 1024))"
        for ((i=0; i<rate/1024/10; i++)); do printf "#"; done
        echo
    done

    echo -e "Historia prędkości wysyłania (KB/s):"
    for rate in "${TX_HISTORY[@]}"; do
        printf "%.0f|" "$((rate / 1024))"
        for ((i=0; i<rate/1024/10; i++)); do printf "#"; done
        echo
    done
    sleep 1
done
