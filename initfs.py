#!/usr/bin/python3

import os
import sys

# Словарь для хранения всех переменных
variables = {}


def collect_variables(directory, parent_var="root"):
    dir_name = os.path.basename(directory)
    var_name = dir_name.replace(".", "_")
    if var_name not in variables:
        variables[var_name] = f"/{os.path.relpath(directory, start_directory)}"
    for item in os.listdir(directory):
        item_path = os.path.join(directory, item)
        if os.path.isdir(item_path):
            collect_variables(item_path, var_name)

def xor_text(text: str, key: str) -> bytes:
    text_bytes = text.encode('ascii')  # Преобразуем текст в байты
    key_bytes = key.encode('ascii')    # Преобразуем ключ в байты
    result = bytearray()
    key_length = len(key_bytes)
    for i, byte in enumerate(text_bytes):
        result.append(byte ^ key_bytes[i % key_length])
    return bytes(result)

def bytes_to_c_string(data: bytes) -> str:
    return ''.join(f'\\x{byte:02x}' for byte in data) + "\\xff"


def generate_code(directory, parent_var="root", indent=0):
    dir_name = os.path.basename(directory)
    var_name = dir_name.replace(".", "_")
    full_path = os.path.relpath(directory, start_directory)

    code = ""
    if full_path != ".":
        code += f"{'    ' * indent}{var_name} = make_dir({parent_var}, \"/{dir_name}\");\n"

    for item in os.listdir(directory):
        item_path = os.path.join(directory, item)
        rel_path = os.path.relpath(item_path, start_directory)

        if os.path.isfile(item_path):
            with open(item_path, "r", encoding="utf-8", errors="ignore") as file:
                content = file.read().strip().replace('"', '\\"').replace('\n', '\\n')
                if ".enc." in item:
                    passwd = item.split(".enc.")[1]
                    content = bytes_to_c_string(xor_text(content, passwd))
                    rel_path = rel_path.replace(f".{passwd}", "")
                code += f"{'    ' * (indent + 1)}touch({var_name}, \"{item}\", \"{content}\");\n"

    for item in os.listdir(directory):
        item_path = os.path.join(directory, item)
        if os.path.isdir(item_path):
            code += generate_code(item_path, var_name, indent + 1)

    return code


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <directory>")
        sys.exit(1)

    start_directory = sys.argv[1]

    if not os.path.isdir(start_directory):
        print(f"Error: '{start_directory}' is not a valid directory.")
        sys.exit(1)

    # Сбор всех переменных
    collect_variables(start_directory)

    # Генерация начала функции
    output_code = "#include \"fs.h\"\n\n"
    output_code += "void init_fs() {\n"

    # Объявление всех переменных в начале функции
    for var in variables:
        output_code += f"    int {var};\n"

    output_code += f"\n    {start_directory} = make_dir(-1, \"/\");\n"

    # Генерация кода для каталогов и файлов
    output_code += generate_code(start_directory)

    # Завершение функции
    output_code += "}\n"

    print(output_code)
