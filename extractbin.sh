#!/bin/bash

# Проверяем, что пользователь указал входной файл
if [ "$#" -ne 2 ]; then
    echo "Использование: $0 <входной_файл> <выходной файл>"
    exit 1
fi

INPUT_FILE="$1"
OUTPUT_FILE="$2"

# Ищем позицию начала и конца фрагмента
END_BYTES="de ad be ef"

# Находим начальную позицию (в байтах)
START_OFFSET=536

# Находим конечную позицию (в байтах)
END_OFFSET=$(hexdump -v -e '1/1 "%02x "' "$INPUT_FILE" | tr -d '\n' | grep -oba "${END_BYTES}" | cut -d':' -f1)
END_OFFSET=$((${END_OFFSET} / 3))

# Если не найден ни один из маркеров, выходим с ошибкой
if [ -z "$START_OFFSET" ] || [ -z "$END_OFFSET" ]; then
    echo "Не удалось найти начало или конец фрагмента в файле."
    exit 1
fi

# Вычисляем длину фрагмента
FRAGMENT_LENGTH=$((END_OFFSET - START_OFFSET + 8)) # +8 байт, так как END_BYTES занимает 4 байта

# Выделяем фрагмент и сохраняем его в выходной файл
dd if="$INPUT_FILE" of="$OUTPUT_FILE" bs=1 skip="$START_OFFSET" count="$FRAGMENT_LENGTH" 2>/dev/null
