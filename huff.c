#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CODE_SIZE 3000

/*��尣�� ������ ���� ����ü*/
typedef struct TreeNode {
    int weight;
    char ch;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

/*���� ���� ������ �����ϱ� ���� ����ü*/
typedef struct {
    TreeNode* ptree;
    char ch;
    int key;
} element;

typedef struct {
    element heap[100];
    int heap_size;
} HeapType;

HeapType* create() { return (HeapType*)malloc(sizeof(HeapType)); }

/*�ʱ�ȭ �Լ�*/
void init(HeapType* h) { h->heap_size = 0; }

/*�ּ� �� ���� �Լ�*/
void insert_min_heap(HeapType* h, element item) {
    int i;
    i = ++(h->heap_size);

    while ((i != 1) && (item.key < h->heap[i / 2].key)) {
        h->heap[i] = h->heap[i / 2];
        i /= 2;
    }
    h->heap[i] = item;
}

/*�ּ� �� ���� �Լ�*/
element delete_min_heap(HeapType* h) {
    int parent, child;
    element item, temp;

    item = h->heap[1];
    temp = h->heap[(h->heap_size)--];
    parent = 1;
    child = 2;
    while (child <= h->heap_size) {
        if ((child < h->heap_size) &&
            (h->heap[child].key) > h->heap[child + 1].key) {
            child++;
        }
        if (temp.key < h->heap[child].key)
            break;

        h->heap[parent] = h->heap[child];
        parent = child;
        child *= 2;
    }
    h->heap[parent] = temp;
    return item;
}

/*���� Ʈ�� ���� �Լ�*/
TreeNode* make_tree(TreeNode* left, TreeNode* right) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->left = left;
    node->right = right;
    return node;
}

/*���� Ʈ�� ���� �Լ�*/
void destroy_tree(TreeNode* root) {
    if (root == NULL)
        return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

/*����Ʈ������ ���ܳ�带 �Ǵ��ϴ� �Լ�*/
int is_leaf(TreeNode* root) { return !(root->left) && !(root->right); }

void print_array(int codes[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d", codes[i]);
    }
    printf("\n");
}

//�迭�� �� ��Ұ� ���ڿ��� ����Ű�� ������
char* huffman_codes[26];

void print_codes(TreeNode* root, int codes[], int top) {
    /*���ʳ���̸� 1�� ����*/
    if (root->left) {
        codes[top] = 1;
        print_codes(root->left, codes, top + 1);
    }
    /*�����ʳ���̸� 0�� ����*/
    if (root->right) {
        codes[top] = 0;
        print_codes(root->right, codes, top + 1);
    }
    /*�ܸ�����̸� ������ ����� �� �ڵ带 ���*/
    if (is_leaf(root)) {
        // ex) huffman_codes[0] ��, 'a'�� ������ �ڵ� "101"�� �����ϴ� �ڵ��
        huffman_codes[root->ch - 'a'] = (char*)malloc(top + 1);
        for (int i = 0; i < top; i++) {
            huffman_codes[root->ch - 'a'][i] = codes[i] + '0';
        }
        huffman_codes[root->ch - 'a'][top] = '\0';
        printf("%c: ", root->ch);
        print_array(codes, top);
    }
}

/*������ �ڵ� ���� �Լ�*/
TreeNode* huffman_tree(int freq[], char ch_list[], int n) {
    int i;
    TreeNode* node, * x;
    HeapType* heap;
    element e, e1, e2;
    int codes[100];
    int top = 0;

    heap = create();
    init(heap);
    for (i = 0; i < n; i++) {
        node = make_tree(NULL, NULL);
        e.ch = node->ch = ch_list[i];
        e.key = node->weight = freq[i];
        e.ptree = node;
        insert_min_heap(heap, e);
    }
    for (i = 1; i < n; i++) {
        e1 = delete_min_heap(heap);
        e2 = delete_min_heap(heap);
        x = make_tree(e1.ptree, e2.ptree);
        e.key = x->weight = e1.key + e2.key;
        e.ptree = x;
        insert_min_heap(heap, e);
    }
    e = delete_min_heap(heap);
    print_codes(e.ptree, codes, top);
    free(heap);
    return e.ptree;
}
//�Է¹��ڿ�, ������ �ڵ�, ������� ���ڿ�
void encode_huffman(char* input, char* huffman_codes[], char* binary_output) {
    binary_output[0] = '\0';

    for (int i = 0; input[i] != '\0'; i++) {
        //������ �ڵ尡 ����� ��ġ�� ����Ͽ�, �ش� ��ġ�� ����� ������ �ڵ带
        //�������� �ڵ��
        char* huffman_code = huffman_codes[input[i] - 'a'];
        strcat(binary_output, huffman_code);
    }
}

void decode_huffman(TreeNode* root, const char* binary_input,
    char* decoded_output) {
    TreeNode* current = root;
    int decoded_index = 0;

    for (int i = 0; binary_input[i] != '\0'; i++) {
        if (binary_input[i] == '1') {
            current = current->left;
        }
        else if (binary_input[i] == '0') {
            current = current->right;
        }

        if (is_leaf(current)) {
            decoded_output[decoded_index++] = current->ch;
            //�ٽ� ���� ���ڿ��� ���ϱ����� current ��Ʈ���� �ʱ�ȭ
            current = root;
        }
    }
    decoded_output[decoded_index] = '\0';
}

int main(void) {
    char ch_list[CODE_SIZE];  /*�Է¹޴� �迭*/
    char ch_list2[CODE_SIZE]; /*�Է¹��� ���ĺ��� �ϳ��� �Է�*/
    int freq[26] = { 0 };  /*���ĺ� �󵵼�*/
    int cnt = 0;         /*�� ���δٸ� ���ĺ� ��*/

    FILE* file = fopen("huff.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "������ �� �� �����ϴ�.\n");
        return 1;
    }

    //���Ͽ� �������� ����� ���ڿ����� ch_list2 �迭�� �����ϴ� �ڵ�
    while (fgets(ch_list, sizeof(ch_list), file) != NULL) {
        for (int i = 0; ch_list[i] != '\0'; i++) {
            if (freq[tolower(ch_list[i]) - 'a'] == 0) {
                ch_list2[cnt++] = tolower(ch_list[i]);
            }
            freq[tolower(ch_list[i]) - 'a']++;
        }
        ch_list2[cnt] = '\0';
    }

    int freq_alpa[26];
    for (int i = 0; i < cnt; i++) {
        freq_alpa[i] = freq[ch_list2[i] - 'a'];
    }
    TreeNode* root = huffman_tree(freq_alpa, ch_list2, cnt); // Ʈ���� ��Ʈ�� �޾ƿ�
    fclose(file);

    char binary_output[CODE_SIZE]; // ���� ��� ���ڿ��� ������ ���� ����
    encode_huffman(ch_list, huffman_codes,
        binary_output); // �Է� ���ڿ��� ������ �ڵ�� ��ȯ�Ͽ� ����
    printf("���� ��� ���ڿ�: %s\n", binary_output);

    FILE* binary_file = fopen("binary_output.bin", "wb");
    if (binary_file == NULL) {
        fprintf(stderr, "������ �� �� �����ϴ�.\n");
        return 1;
    }
    //���̳ʸ� ���Ͽ� ���� ���� �ڵ�
    fwrite(binary_output, sizeof(char), strlen(binary_output), binary_file);

    // ���� ���ڿ��� �ٽ� ���� ���ڿ��� ���ڵ�
    char decoded_output[CODE_SIZE];
    decode_huffman(root, binary_output, decoded_output);
    printf("���ڵ��� ���ڿ�: %s\n", decoded_output);

    destroy_tree(root);

    return 0;
}