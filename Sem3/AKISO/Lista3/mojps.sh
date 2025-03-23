#!/bin/bash
max_ppid=4
max_pid=3
max_comm=8
max_state=5
max_tty=3
max_rss=3
max_pgid=4
max_sid=3
max_open_files=10
for pid in $(ls /proc | grep -E '^[0-9]+$' | sort -n); do
    pid=$(basename "$pid")
    if [ -f "/proc/$pid/status" ] && [ -f "/proc/$pid/stat" ]; then
        stat_info=$(cat /proc/$pid/stat)
        comm=$(echo "$stat_info" | awk '{print $2}' | tr -d '()')
        ppid=$(echo "$stat_info" | awk '{print $4}')
        state=$(echo "$stat_info" | awk '{print $3}')
        pgid=$(echo "$stat_info" | awk '{print $5}')
        sid=$(echo "$stat_info" | awk '{print $6}')
        rss=$(echo "$stat_info" | awk '{print $24}')
        tty=$(ls -l /proc/$pid/fd/ | grep "/dev/" | head -n 1 | awk '{print $NF}' | xargs basename 2>/dev/null)
        [ -z "$tty" ] && tty="?"
        open_files=$(ls -1 /proc/$pid/fd/ 2>/dev/null | wc -l)
        max_ppid=$(( ${#ppid} > max_ppid ? ${#ppid} : max_ppid ))
        max_pid=$(( ${#pid} > max_pid ? ${#pid} : max_pid ))
        max_comm=$(( ${#comm} > max_comm ? ${#comm} : max_comm ))
        max_state=$(( ${#state} > max_state ? ${#state} : max_state ))
        max_tty=$(( ${#tty} > max_tty ? ${#tty} : max_tty ))
        max_rss=$(( ${#rss} > max_rss ? ${#rss} : max_rss ))
        max_pgid=$(( ${#pgid} > max_pgid ? ${#pgid} : max_pgid ))
        max_sid=$(( ${#sid} > max_sid ? ${#sid} : max_sid ))
        max_open_files=$(( ${#open_files} > max_open_files ? ${#open_files} : max_open_files ))
    fi
done
format="%-${max_ppid}s %-${max_pid}s %-${max_sid}s %-${max_state}s %-${max_tty}s %-${max_rss}s %-${max_pgid}s %-${max_open_files}s %-${max_comm}s\n"
printf "$format" "PPID" "PID" "SID" "STATE" "TTY" "RSS" "PGID" "OPEN_FILES" "COMMANDS"
for pid in $(ls /proc | grep -E '^[0-9]+$' | sort -n); do
    pid=$(basename "$pid")
    if [ -f "/proc/$pid/status" ] && [ -f "/proc/$pid/stat" ]; then
        stat_info=$(cat /proc/$pid/stat)
        comm=$(echo "$stat_info" | awk '{print $2}' | tr -d '()')
        ppid=$(echo "$stat_info" | awk '{print $4}')
        state=$(echo "$stat_info" | awk '{print $3}')
        pgid=$(echo "$stat_info" | awk '{print $5}')
        sid=$(echo "$stat_info" | awk '{print $6}')
        rss=$(echo "$stat_info" | awk '{print $24}')
        tty=$(ls -l /proc/$pid/fd/ | grep "/dev/" | head -n 1 | awk '{print $NF}' | xargs basename 2>/dev/null)
        [ -z "$tty" ] && tty="?"
 if [ -z "$tty" ] || [ "$tty" == "null" ]; then
            tty="?"
        fi
        open_files=$(ls -1 /proc/$pid/fd/ 2>/dev/null | wc -l)
        printf "$format" "$ppid" "$pid" "$sid" "$state" "$tty" "$rss" "$pgid" "$open_files" "$comm"
    fi
done
