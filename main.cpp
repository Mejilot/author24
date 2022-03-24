#include <iostream>
#include <fstream>

/* 
Структура элемента/узла двусвязного списка(Node значит узел)
*/
struct list_node {
    int value;  // значение текущего элемента
    list_node* next;  // ссылка на следующий элемент
    list_node* prev;  // ссылка на предыдущий элемент
};

/* 
Создание элемента списка по переданному значению value
*/
list_node* create_list_node(int value) {
    list_node* elem;
    elem = (list_node*)malloc(sizeof(list_node));  //выделение памяти
    elem->value = value;  // записываем переданное значение
    elem->next = NULL;  // пока что элемент никуда не указывает
    elem->prev = NULL;  // аналогично
    return elem;
}

/*
Добавления нового элемента со значением vаlue 
после переданного первым аргументом элемента списка elem
*/
list_node* add(list_node* elem, int value) {
    // создание нового элемента
    list_node* newElem = create_list_node(value);
    // ставим newElem между elem и elem->next
    // соединяем newElem c elem и elem->next
    newElem->next = elem->next; // следующий для newElem теперь тот, что стоял после elem
    newElem->prev = elem; // предыдущий для newElem теперь elem, так newElem встал посередине
    // соединяем elem с newElem
    elem->next = newElem;
    // соединяем elem->next c newElem
    elem->next->prev = newElem; // предыдущим элементом элемента после elem становится новый элемент newElem
    return newElem; // возвращаем указатель на этот новый элемент
}

/*
удаляем элемент из списка
возвращает предыдущий, если его нет, то следующий
*/
list_node* remove(list_node* elem) {
    list_node *return_val = NULL;
    if (elem) {
        if (elem->prev) elem->prev->next = elem->next;
        if (elem->next) elem->next->prev = elem->prev;
        if (elem->prev) return_val = elem->prev;
        else return_val = elem->next;
        delete elem;
    }
    return return_val;
}

/*
уничтожаем список, последовательно удаляя все элементы
*/
void destroy_list(list_node *head) {
    while (head) {
        head = remove(head);   
    }
}

/*
получаем размер списка, передаем указатель на начало
*/
int size_list(list_node* head) {
    int size = 0;
    // двигаемся по списку, пока head не указывает на NULL - конец списка
    while (head) {
        head = head->next;
        size++;
    }
    return size;
}

/*
меняем местами elem1 и elem2, если elem1 стоит после elem2
*/
void swap(list_node* elem1, list_node* elem2) {
    // меняем ссылки у элемента стоящего до elem1 и у того что стоит после elem2
    // но это делаем, только если такие элементы имеются
    if (elem1->prev) elem1->prev->next = elem2;
    if (elem2->next) elem2->next->prev = elem1;
    // меняем ссылки для самих элементов
    list_node *temp;
    temp = elem1->prev;
    elem1->prev = elem2;
    elem2->prev = temp;
    temp = elem2->next;
    elem2->next = elem1;
    elem1->next = temp;
      
}

/*
сортировка пузырьком, возвращает указатель на первый элемент
т.к. первый элемент изменился и старый указатель, теперь указывает на элемент в отсортированом списке
*/
void bubble_sort(list_node *&head) {
    int size = size_list(head);
    for (int i = 0; i < size; i++) {
        list_node *cur = head;
        for (int j = 0; j < size - 1 - i; j++) {
            if (cur->value > cur->next->value) {
                if (cur == head) head = cur->next;  //  сохраняем указатель на начало, если оно меняется
                swap(cur, cur->next);
                cur = cur->prev;          
            }
            cur = cur->next;
        }
    }
}

/*
запись списка из файла
*/
list_node *fill_list(int& option) {
    int a = 0;
    std::ifstream fin;
    fin.open("input.txt");
    fin >> option;
    fin >> a;
    list_node* head = create_list_node(a);
    list_node* last = head;  // создаем указатель на конец списка
    fin >> a; 
    while (!fin.eof()) {
        last = add(last, a);  // перезаписываем указатель на только что добавленный элемент 
        fin >> a;  
    }
    fin.close();
    return head;
}

/*
печать списка в файл
*/
void print_list(list_node* head) {
    std::ofstream fout("output.txt");
    fout << size_list(head) << " ";
    while (head) {
        fout << head->value << " ";
        head = head->next;
    }
    fout.close();
}

int main() {
    int option = 0;
    list_node *head = fill_list(option);
//    if (option == 0) merge_sort(head);  // слиянием
    if (option == 1) bubble_sort(head);  // пузырьком
    print_list(head);
    destroy_list(head);
    return 0;
}
