#include <stdio.h>

#define REG_SIZE 100

//operations I/O
#define READ 10
#define WRITE 11

//operations load/store
#define LOAD 20
#define STORE 21

//operations math
#define ADD 30
#define SUBSTRACT 31
#define DIVIDE 32
#define MULTIPLY 33

//operation delegation lead
#define BRANCH 40 //if number in mem is positive
#define BRANCHNEG 41 //if number in mem is negative
#define BRANCHZERO 42 //if number in mem is zero
#define HALT 43

void load_prog(int mem[], int mem_size)
{
    printf("\n\n");
    int word;
    for(int i = 0; i < mem_size; i++) {
        printf("%.2d? ", i);
        scanf("%d", &word);
        if(word == -99999)
            break;
        if(word < -9999 || word > 9999) {
            printf("ОШИБКА: значение выходит за диапазон [-9999, 9999]\n");
            printf("Введите корректное значение\n");
            i--;
        }
        mem[i] = word;
    }
}

void print_dump(int mem[], int mem_size)
{
    printf("%8d", 0);
    for(int i = 1; i < 10; i++)
        printf("%6d", i);
    printf("\n");

    for(int dump_string = 0; dump_string < 10; dump_string++) {
        if(dump_string == 0)
            printf("%d  ", dump_string*10);
        else printf("%d ", dump_string*10);
        for(int i = dump_string * 10; i < dump_string * 10 + 10; i++) {
            if(mem[i] == 0 || (mem[i] < 1000 && mem[i] > -1000))
                printf("%c%04d ", mem[i] >= 0 ? '+' : '-', mem[i]);
            else printf("%c%d ", mem[i] >= 0 ? '+' : '-', mem[i]);
        } 
        printf("\n");
    }

    printf("\n\n");
}

void doing_instructions(int mem[], const int mem_size)
{
    int accumulator = 0;
    int instruction_counter = -1;
    int instruction_reg = 0;
    int operation_code = 0;
    int operand = 0;

    while(operation_code != 43 && instruction_counter <= 100) {
        instruction_counter++;
        instruction_reg = mem[instruction_counter];
        operation_code = instruction_reg/100;
        operand = instruction_reg%100;

        int error_flag = 0;
        switch(operation_code) {
            case READ:
                scanf("%d", &mem[operand]);
                break;
            case WRITE:
                printf("Вывод: %d\n", mem[operand]);
                break;
            case LOAD:
                accumulator = mem[operand];
                break;
            case STORE:
                mem[operand] = accumulator;
                break;
            case ADD:
                accumulator += mem[operand];
                if(accumulator > 9999) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: переполнение аккумулятора\n");
                    error_flag = 1;
                }
                break;
            case SUBSTRACT:
                accumulator -= mem[operand];
                if(accumulator < -9999) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: переполнение аккумулятора\n");
                    error_flag = 1;
                }
                break;
            case DIVIDE:
                if (mem[operand] == 0) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: деление на ноль\n");
                    error_flag = 1;
                }
                accumulator /= mem[operand];
                break;
            case MULTIPLY:
                accumulator *= mem[operand];
                if(accumulator > 9999) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: переполнение аккумулятора\n");
                    error_flag = 1;
                }
                break;
            case BRANCH:
                instruction_counter = operand-1;
                if(instruction_counter > 100) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: выход за пределы памяти\n");
                    error_flag = 1;
                }
                break;
            case BRANCHNEG:
                if(accumulator < 0)
                    instruction_counter = operand-1;
                if(instruction_counter > 100) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: выход за пределы памяти\n");
                    error_flag = 1;
                }
                break;
            case BRANCHZERO:
                if(accumulator == 0)
                    instruction_counter = operand-1;
                if(instruction_counter > 100) {
                    printf("ФАТАЛЬНАЯ ОШИБКА: выход за пределы памяти\n");
                    error_flag = 1;
                }
                break;
        }
        if(error_flag == 1) {
            printf("Симплетрон аварийно завершил выполнение программы\n");
            print_dump(mem, mem_size);
            break;
        }
    } 
    printf("*** Симплетрон закончил свои вычисления ***\n");
    print_dump(mem, mem_size);
}

int main()
{
    printf("*** Симплетрон приветствует вас! ***\n");
    printf("*** Введите вашу команду по одной командe ***\n");
    printf("*** или слову данных за раз. Я буду выводить в качестве ***\n");
    printf("*** подсказки текущий адрес и знак вопроса (?). Введённое ***\n");
    printf("*** слово будет размещено по указанному адресу. Для ***");
    printf("*** прекращения ввода программы введите число -99999. ***\n");

    int memory[REG_SIZE];
    for(int i = 0; i < REG_SIZE; i++)
       memory[i] = 0; 
    load_prog(memory, REG_SIZE);
    printf("*** Загрузка программы завершена ***\n");
    printf("*** Начинаю выполнение программы ***\n\n");
    doing_instructions(memory, REG_SIZE);
    
    return 0;
}
