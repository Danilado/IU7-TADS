import random

def generate_unique_numbers(n, min_value, max_value):
    if n > (max_value - min_value + 1):
        raise ValueError("Невозможно сгенерировать указанное количество уникальных чисел в заданном диапазоне.")

    unique_numbers = random.sample(range(min_value, max_value + 1), n)
    return unique_numbers

def write_to_file(filename, numbers):
    with open(filename, 'w') as file:
        for number in numbers:
            file.write(str(number) + '\n')

if __name__ == "__main__":
    n = int(input("Введите количество случайных чисел: "))
    min_value = int(input("Введите минимальное значение: "))
    max_value = int(input("Введите максимальное значение: "))
    output_file = "res.txt"

    try:
        unique_numbers = generate_unique_numbers(n, min_value, max_value)
        write_to_file(output_file, unique_numbers)
        print(f"Сгенерированные уникальные числа сохранены в файл {output_file}.")
    except ValueError as e:
        print(f"Ошибка: {e}")
