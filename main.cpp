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
    list_node* newElem = create_list_node(value);  // создание нового элемента
    newElem->next = elem->next; // следующий для newElem теперь тот, что стоял после elem
    newElem->prev = elem;  // предыдущий для newElem теперь elem, так newElem встал посередине
    if (elem->next) elem->next->prev = newElem; // предыдущим элементом для элемента после elem становится newElem, если конечно после elem что-то есть
    elem->next = newElem;  // соединяем elem с newElem
    return newElem; // возвращаем указатель на новый элемент
}

/*
удаляем элемент из списка
возвращает предыдущий, если его нет, то следующий
*/
list_node* remove(list_node* elem) {
    list_node *return_val = NULL;
    if (elem) {
        if (elem->prev) elem->prev->next = elem->next;  // изменяем предыдущий элемент при наличии
        if (elem->next) elem->next->prev = elem->prev;  // изменяем следующий элемент при наличии
        if (elem->prev) return_val = elem->prev;  // возвращаемое значение приравниваем к предыдущему от удаляемого, если нет, к следующему
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
    // меняем ссылки у элемента стоящего до elem1 и элемента после elem2
    // но это делаем, только если такие элементы имеются
    if (elem1->prev) elem1->prev->next = elem2;
    if (elem2->next) elem2->next->prev = elem1;
    // меняем ссылки для самих элементов
    elem2->prev = elem1->prev; 
    elem1->prev = elem2;
    elem1->next = elem2->next;
    elem2->next = elem1;
}

/*
сортировка пузырьком, возвращает указатель на первый элемент
т.к. первый элемент изменился и старый указатель, теперь указывает на элемент в отсортированом списке
*/
void bubble_sort(list_node *&head) {
    int size = size_list(head);
    for (int i = 0; i < size; i++) {  // пробегаемся по всем элементам
        list_node *cur = head;  // переписывает текущий указатель на начало
        for (int j = 0; j < size - 1 - i; j++) {  // пузырек всплывает
            if (cur->value > cur->next->value) {
                if (cur == head) head = cur->next;  //  сохраняем указатель на начало, если оно меняется
                swap(cur, cur->next);
                cur = cur->prev;  // т.к. элементы поменялись местами надо вернутся к тому же указателю
            }
            cur = cur->next;
        }
    }
}

void merge(list_node *left, list_node* middle, list_node *right) {
    // создадим два новых списка, которые будут содержать правую и левую часть оригинального списка
    list_node *left_sublist = create_list_node(left->value);
    list_node *right_sublist = create_list_node(middle->next->value);
    list_node *beg_left_sublist = left_sublist;
    list_node *beg_right_sublist = right_sublist;
    list_node *sublist_head = left_sublist;  // указатель, который бегает по подспискам
    // заполняем левый подсписок
    for (list_node *head = left->next; head != middle->next; head = head->next) {
        sublist_head = add(sublist_head, head->value);
    }
    // заполняем правый подсписок
    sublist_head = right_sublist;
    for (list_node *head = middle->next->next; head != right->next; head = head->next) {
        sublist_head = add(sublist_head, head->value);
    }
    // соединяем два подсписка в оригинальном списке
    // пока в одном из подсписков не закончатся элементы
    while (right_sublist && left_sublist) {
        if (left_sublist->value <= right_sublist->value) {
            left->value = left_sublist->value;
            left_sublist = left_sublist->next;        
        } else {
            left->value = right_sublist->value;
            right_sublist = right_sublist->next;
        }
        left = left->next;
    }
    // записываем остатки
    while (left_sublist) {
        left->value = left_sublist->value;
        left = left->next;
        left_sublist = left_sublist-> next;
    }
    while (right_sublist) {
        left->value = right_sublist->value;
        left = left->next;
        right_sublist = right_sublist-> next;
    }
    // чистим память которую выделил на подсписки
    destroy_list(beg_left_sublist);
    destroy_list(beg_right_sublist);
}

void merge_sort(list_node *left, list_node* right) {
    if (left != right) {
        int size = 0;
        list_node *middle = left;
        // определяем размер переданного участка списка от left до right
        list_node *head = left;
        for (; head->next != right; head = head->next, size++);
        size++;
        // при первом запуске определим right, как указатель на конец списка
        if (!right) right = head;
        for (int i = 0; i < size / 2; i++) middle = middle->next; // находим середину между left и right
        merge_sort(left, middle);
        merge_sort(middle->next, right);
        merge(left, middle, right);
    }
}

/*
запись списка из файла
*/
list_node *fill_list(int& option) {
    int a = 0;
    std::ifstream fin;
    fin.open("input.txt");  // открываем файл для чтения 
    fin >> option;
    fin >> a;
    list_node* head = create_list_node(a);  // создаем список
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
    std::ofstream fout("output.txt");  // открываем файл на запись
    fout << size_list(head) << " ";
    while (head) {
        fout << head->value << " ";  // записываем в файл значения через пробел
        head = head->next;
    }
    fout.close();
}

void fill_file(int option, int n) {
    std::ofstream fout("input.txt");  // открываем файл на запись
    fout << option << " ";
    for (int i = n; i != 0; i--) {
        fout << i << " ";  // записываем в файл значения через пробел
    }
    fout.close();
}

int main() {
    int option = 0;
    // для проверки работает на 5000 элементах
//    fill_file(option, 5000);
    list_node *head = fill_list(option);
    if (option == 0) merge_sort(head, NULL);  // слиянием
    if (option == 1) bubble_sort(head);  // пузырьком
    print_list(head);
    destroy_list(head);
    return 0;
}
