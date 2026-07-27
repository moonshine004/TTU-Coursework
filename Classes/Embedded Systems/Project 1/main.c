#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 30
#define NAME_ITEMS 30

//  struct item
struct list_item {
    char name[NAME_ITEMS];
    int quantity;
};

int main() {

    struct list_item items[MAX_ITEMS]; // storing items
    int count = 0;                     // keeping track
    int choice = 0;

    // loop until quit
    while (choice != 5) {

        // create the list
        if (count >= MAX_ITEMS) {
            printf("Option 2: Remove Item\n");
            printf("Option 3: View List\n");
            printf("Option 4: Clear the List\n");
            printf("Option 5: End\n");
        } else {
            printf("Option 1: Add Item\n");
            printf("Option 2: Remove Item\n");
            printf("Option 3: View List\n");
            printf("Option 4: Clear the List\n");
            printf("Option 5: End\n");
        }

        printf("Choose wisely: ");
        scanf(" %d", &choice);

        // option 1
        if (choice == 1) {

            // incase for full list
            if (count >= MAX_ITEMS) {
                printf("hey fyi the list is getting full, please remove or clear\n");
                continue;
            }

            // user input for item
            printf("Enter item name: ");
            scanf(" %s", items[count].name);

            // user input for how much of said item
            printf("How much?? ");
            scanf(" %d", &items[count].quantity);

            count++;
        }

        // option 2 | only can remove last item atm
        else if (choice == 2) {

            if (count > 0) {
                count--;
                printf("Last item removed.\n");
            } else {
                printf("List = Empty oops\n");
            }
        }

        // option 3
        else if (choice == 3) {

            if (count == 0) {
                printf("uhh theres nothing here\n");
            } else {
                for (int i = 0; i < count; i++) {
                    printf("%d. %s - %d\n",
                           i + 1,
                           items[i].name,
                           items[i].quantity);
                }
            }
        }

        // option 4
        else if (choice == 4) {

            count = 0;
            printf("ta da the list is gone\n");
        }

        // option 5
        else if (choice == 5) {
            printf("Program End. Stay warm!!\n");
        }

        else {
            printf("Invalid Choice..self destruct\n");
        }
    }

    return 0;
}