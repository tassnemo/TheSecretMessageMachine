#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MSG_MAX 128U
#define ALPHABET 26U

static char message[MSG_MAX];
static char scratch[MSG_MAX];
static uint8_t shiftKey;

static uint16_t textLength(const char *s);
static void readMessage(void);
static char shiftChar(char c, uint8_t key);
static void encode(void);
static void decode(void);
static void reverseText(char *s, uint16_t n);
static uint16_t countVowels(const char *s);
static uint8_t isPalindrome(const char *s);
static void letterHistogram(const char *s);
static void showMessage(void);
static void showMenu(void);
static void setKey(void);
static void dispatch(int choice);

static uint16_t textLength(const char *s)
{
    uint16_t length = 0U;
    while (s[length] != '\0' && length < MSG_MAX - 1U) {
        length++;
    }
    return length;
}

static void readMessage(void)
{
    uint16_t length;
    int discarded;

    printf("Enter a message: ");
    if (fgets(message, MSG_MAX, stdin) == NULL) {
        message[0] = '\0';
        return;
    }
    length = textLength(message);
    if (length > 0U && message[length - 1U] == '\n') {
        message[length - 1U] = '\0';
        return;
    }
    do {
        discarded = getchar();
    } while (discarded != '\n' && discarded != EOF);
}

static char shiftChar(char c, uint8_t key)
{
    if (c >= 'A' && c <= 'Z') {
        return (char)('A' + (c - 'A' + key) % ALPHABET);
    }
    if (c >= 'a' && c <= 'z') {
        return (char)('a' + (c - 'a' + key) % ALPHABET);
    }
    return c;
}

static void encode(void)
{
    uint16_t i;
    uint16_t length = textLength(message);
    for (i = 0U; i < length; i++) {
        scratch[i] = shiftChar(message[i], shiftKey);
    }
    scratch[length] = '\0';
    printf("Encoded: %s\n", scratch);
}

static void decode(void)
{
    uint16_t i;
    uint16_t length = textLength(message);
    uint8_t reverseKey = (uint8_t)((ALPHABET - shiftKey) % ALPHABET);
    for (i = 0U; i < length; i++) {
        scratch[i] = shiftChar(message[i], reverseKey);
    }
    scratch[length] = '\0';
    printf("Decoded: %s\n", scratch);
}

static void reverseText(char *s, uint16_t n)
{
    uint16_t left = 0U;
    uint16_t right = n == 0U ? 0U : n - 1U;
    while (left < right) {
        char temporary = s[left];
        s[left] = s[right];
        s[right] = temporary;
        left++;
        right--;
    }
}

static uint16_t countVowels(const char *s)
{
    uint16_t count = 0U;
    uint16_t i;
    for (i = 0U; s[i] != '\0'; i++) {
        if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' ||
            s[i] == 'o' || s[i] == 'u' || s[i] == 'A' ||
            s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U') {
            count++;
        }
    }
    return count;
}

static uint8_t isPalindrome(const char *s)
{
    uint16_t left = 0U;
    uint16_t right = textLength(s);
    if (right == 0U) {
        return 1U;
    }
    right--;
    while (left < right) {
        char leftChar;
        char rightChar;
        while (left < right && s[left] == ' ') {
            left++;
        }
        while (left < right && s[right] == ' ') {
            right--;
        }
        leftChar = s[left];
        rightChar = s[right];
        if (leftChar >= 'A' && leftChar <= 'Z') {
            leftChar = (char)(leftChar + ('a' - 'A'));
        }
        if (rightChar >= 'A' && rightChar <= 'Z') {
            rightChar = (char)(rightChar + ('a' - 'A'));
        }
        if (leftChar != rightChar) {
            return 0U;
        }
        left++;
        right--;
    }
    return 1U;
}

static void letterHistogram(const char *s)
{
    uint16_t counts[ALPHABET] = {0U};
    uint16_t i;
    for (i = 0U; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            counts[s[i] - 'A']++;
        } else if (s[i] >= 'a' && s[i] <= 'z') {
            counts[s[i] - 'a']++;
        }
    }
    printf("Letter histogram:\n");
    for (i = 0U; i < ALPHABET; i++) {
        uint16_t bar;
        if (counts[i] == 0U) {
            continue;
        }
        printf("%c: ", (char)('A' + i));
        for (bar = 0U; bar < counts[i]; bar++) {
            putchar('*');
        }
        printf(" (%u)\n", (unsigned)counts[i]);
    }
}

static void showMessage(void)
{
    printf("+----------------------------------------+\n");
    printf("| Message: %-29.29s |\n", message);
    printf("| Length: %-31u |\n", (unsigned)textLength(message));
    printf("| Key: %-34u |\n", (unsigned)shiftKey);
    printf("| Vowels: %-31u |\n", (unsigned)countVowels(message));
    printf("| Palindrome: %-27s |\n", isPalindrome(message) ? "yes" : "no");
    printf("+----------------------------------------+\n");
}

static void showMenu(void)
{
    printf("\n1. Read message\n2. Set key\n3. Encode\n4. Decode\n");
    printf("5. Reverse\n6. Vowels\n7. Palindrome\n8. Histogram\n9. Show\n0. Quit\nChoice: ");
}

static void setKey(void)
{
    int enteredKey;
    int discarded;

    printf("Enter key (0-26): ");
    if (scanf("%d", &enteredKey) == 1 && enteredKey >= 0 && enteredKey <= 26) {
        shiftKey = (uint8_t)(enteredKey % (int)ALPHABET);
    } else {
        printf("Invalid key.\n");
    }
    do {
        discarded = getchar();
    } while (discarded != '\n' && discarded != EOF);
}

static void dispatch(int choice)
{
    switch (choice) {
    case 1: readMessage(); break;
    case 2: setKey(); break;
    case 3: encode(); break;
    case 4: decode(); break;
    case 5: reverseText(message, textLength(message)); printf("Message reversed.\n"); break;
    case 6: printf("Vowels: %u\n", (unsigned)countVowels(message)); break;
    case 7: printf("Palindrome: %s\n", isPalindrome(message) ? "yes" : "no"); break;
    case 8: letterHistogram(message); break;
    case 9: showMessage(); break;
    case 0: printf("Goodbye.\n"); break;
    default: printf("Invalid choice.\n"); break;
    }
}

int main(void)
{
    int choice;
    do {
        int discarded;
        showMenu();
        if (scanf("%d", &choice) != 1) {
            do {
                discarded = getchar();
            } while (discarded != '\n' && discarded != EOF);
            choice = -1;
        } else {
            do {
                discarded = getchar();
            } while (discarded != '\n' && discarded != EOF);
        }
        dispatch(choice);
    } while (choice != 0);
    return 0;
}
