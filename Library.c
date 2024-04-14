#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100

typedef struct {
    char isbn[20];
    char title[50];
    char author[50];
    int publicationYear;
    int isBorrowed;
} Book;

typedef struct {
    Book books[MAX_BOOKS];
    int count;
} Library;

Library library;

void displayMenu() {
    printf("----- LIBRARY MANAGEMENT SYSTEM -----\n");
    printf("1. Add Book\n");
    printf("2. Delete Book\n");
    printf("3. Update Book\n");
    printf("4. Filter and Sort Books\n");
    printf("5. Reverse Library\n");
    printf("6. Search Book\n");
    printf("7. Borrow Book\n");
    printf("8. Return Book\n");
    printf("9. Save Library to File\n");
    printf("10. Load Library from File\n");
    printf("11. Exit\n");
    printf("Enter your choice: ");
}

void addBook(const char *isbn, const char *title, const char *author, int publicationYear, int isBorrowed) {
    if (library.count >= MAX_BOOKS) {
        printf("Library is full. Book could not be added.\n");
        return;
    }

    strcpy(library.books[library.count].isbn, isbn);
    strcpy(library.books[library.count].title, title);
    strcpy(library.books[library.count].author, author);
    library.books[library.count].publicationYear = publicationYear;
    library.books[library.count].isBorrowed = isBorrowed;

    library.count++;

    printf("Book added successfully.\n");

    // Add book information to file
    FILE *file = fopen("books.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s|%s|%s|%d|%d\n", isbn, title, author, publicationYear, isBorrowed);
        fclose(file);
    } else {
        printf("Failed to open file.\n");
    }
}

void deleteBook(const char *isbn) {
    int found = 0;
    int index = -1;

    for (int i = 0; i < library.count; i++) {
        if (strcmp(library.books[i].isbn, isbn) == 0) {
            index = i;
            found = 1;
            break;
        }
    }

    if (found) {
        for (int i = index; i < library.count - 1; i++) {
            library.books[i] = library.books[i + 1];
        }
        library.count--;
        printf("Book deleted successfully.\n");

        // Remove book from file
        FILE *file = fopen("books.txt", "w");
        if (file != NULL) {
            for (int i = 0; i < library.count; i++) {
                fprintf(file, "%s|%s|%s|%d|%d\n",
                        library.books[i].isbn,
                        library.books[i].title,
                        library.books[i].author,
                        library.books[i].publicationYear,
                        library.books[i].isBorrowed);
            }
            fclose(file);
        } else {
            printf("Failed to open file.\n");
        }
    } else {
        printf("Book not found.\n");
    }
}

void updateBook(const char *isbn, const char *property, const char *value) {
    int found = 0;

    for (int i = 0; i < library.count; i++) {
        if (strcmp(library.books[i].isbn, isbn) == 0) {
            if (strcmp(property, "isbn") == 0) {
                strcpy(library.books[i].isbn, value);
            } else if (strcmp(property, "title") == 0) {
                strcpy(library.books[i].title, value);
            } else if (strcmp(property, "author") == 0) {
                strcpy(library.books[i].author, value);
            } else if (strcmp(property, "publicationYear") == 0) {
                library.books[i].publicationYear = atoi(value);
            } else {
                printf("Invalid property name.\n");
                return;
            }

            found = 1;
            break;
        }
    }

    if (found) {
        printf("Book updated successfully.\n");
    } else {
        printf("Book not found.\n");
    }
}

void displayBookList(const Book *bookList, int count) {
    if (count == 0) {
        printf("No books found in the library.\n");
        return;
    }

    printf("%-15s%-30s%-30s%-10s\n", "ISBN", "Title", "Author", "Year");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-15s%-30s%-30s%-10d\n",
               bookList[i].isbn,
               bookList[i].title,
               bookList[i].author,
               bookList[i].publicationYear);
    }
}

void displayBookInfo(const Book *book) {
    printf("ISBN: %s\n", book->isbn);
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Publication Year: %d\n", book->publicationYear);
    printf("Status: %s\n", book->isBorrowed ? "Borrowed" : "Available");
}

void filterAndSortBooks(int filterOption, const char *keyword, int sortOption) {
    Book filteredList[MAX_BOOKS];
    int count = 0;

    for (int i = 0; i < library.count; i++) {
        if (filterOption == 1 && strstr(library.books[i].title, keyword) != NULL) {
            filteredList[count] = library.books[i];
            count++;
        } else if (filterOption == 2 && strstr(library.books[i].author, keyword) != NULL) {
            filteredList[count] = library.books[i];
            count++;
        } else if (filterOption == 3 && library.books[i].publicationYear == atoi(keyword)) {
            filteredList[count] = library.books[i];
            count++;
        }
    }

    switch (sortOption) {
        case 1:
            for (int i = 0; i < count - 1; i++) {
                for (int j = 0; j < count - i - 1; j++) {
                    if (strcmp(filteredList[j].title, filteredList[j + 1].title) > 0) {
                        Book temp = filteredList[j];
                        filteredList[j] = filteredList[j + 1];
                        filteredList[j + 1] = temp;
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < count - 1; i++) {
                for (int j = 0; j < count - i - 1; j++) {
                    if (strcmp(filteredList[j].author, filteredList[j + 1].author) > 0) {
                        Book temp = filteredList[j];
                        filteredList[j] = filteredList[j + 1];
                        filteredList[j + 1] = temp;
                    }
                }
            }
            break;
        case 3:
            for (int i = 0; i < count - 1; i++) {
                for (int j = 0; j < count - i - 1; j++) {
                    if (filteredList[j].publicationYear > filteredList[j + 1].publicationYear) {
                        Book temp = filteredList[j];
                        filteredList[j] = filteredList[j + 1];
                        filteredList[j + 1] = temp;
                    }
                }
            }
            break;
    }

    displayBookList(filteredList, count);
}

void borrowBook(const char *isbn) {
    int found = 0;

    for (int i = 0; i < library.count; i++) {
        if (strcmp(library.books[i].isbn, isbn) == 0) {
            if (library.books[i].isBorrowed) {
                printf("This book is already borrowed.\n");
            } else {
                library.books[i].isBorrowed = 1;
                printf("Book borrowed successfully.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }
}

void returnBook(const char *isbn) {
    int found = 0;

    for (int i = 0; i < library.count; i++) {
        if (strcmp(library.books[i].isbn, isbn) == 0) {
            if (library.books[i].isBorrowed) {
                library.books[i].isBorrowed = 0;
                printf("Book returned successfully.\n");
            } else {
                printf("This book is already in the library.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }
}

void saveLibraryToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    for (int i = 0; i < library.count; i++) {
        fprintf(file, "%s;%s;%s;%d;%d\n",
                library.books[i].isbn,
                library.books[i].title,
                library.books[i].author,
                library.books[i].publicationYear,
                library.books[i].isBorrowed);
    }

    fclose(file);
    printf("Library saved successfully to file.\n");
}

void loadLibraryFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    library.count = 0;
    while (fscanf(file, "%[^;];%[^;];%[^;];%d;%d\n",
                  library.books[library.count].isbn,
                  library.books[library.count].title,
                  library.books[library.count].author,
                  &library.books[library.count].publicationYear,
                  &library.books[library.count].isBorrowed) == 5) {
        library.count++;
    }

    fclose(file);
    printf("Library loaded successfully from file.\n");
}

int main() {
    library.count = 0;

    int choice;
    char isbn[20], title[50], author[50], keyword[50], value[50];
    int publicationYear, filterOption, sortOption;

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("ISBN: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strlen(isbn) - 1] = '\0';

                printf("Title: ");
                fgets(title, sizeof(title), stdin);
                title[strlen(title) - 1] = '\0';

                printf("Author: ");
                fgets(author, sizeof(author), stdin);
                author[strlen(author) - 1] = '\0';

                printf("Publication Year: ");
                scanf("%d", &publicationYear);
                getchar();

                addBook(isbn, title, author, publicationYear, 0);
                break;
            case 2:
                printf("ISBN: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strlen(isbn) - 1] = '\0';

                deleteBook(isbn);
                break;
            case 3:
                printf("ISBN: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strlen(isbn) - 1] = '\0';

                printf("Property: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strlen(keyword) - 1] = '\0';

                printf("Value: ");
                fgets(value, sizeof(value), stdin);
                value[strlen(value) - 1] = '\0';

                updateBook(isbn, keyword, value);
                break;
            case 4:
                printf("Filter Options:\n");
                printf("1. Title\n");
                printf("2. Author\n");
                printf("3. Publication Year\n");
                printf("Enter your choice: ");
                scanf("%d", &filterOption);
                getchar();

                printf("Search Keyword: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strlen(keyword) - 1] = '\0';

                printf("Sort Options:\n");
                printf("1. Title\n");
                printf("2. Author\n");
                printf("3. Publication Year\n");
                printf("Enter your choice: ");
                scanf("%d", &sortOption);
                getchar();

                filterAndSortBooks(filterOption, keyword, sortOption);
                break;
            case 5:
                for (int i = 0, j = library.count - 1; i < j; i++, j--) {
                    Book temp = library.books[i];
                    library.books[i] = library.books[j];
                    library.books[j] = temp;
                }
                printf("Library reversed successfully.\n");
                break;
            case 6:
                printf("ISBN: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strlen(keyword) - 1] = '\0';

                for (int i = 0; i < library.count; i++) {
                    if (strcmp(library.books[i].isbn, keyword) == 0) {
                        displayBookInfo(&library.books[i]);
                        break;
                    }
                }
                break;
            case 7:
                printf("Enter the ISBN of the book you want to borrow: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strlen(isbn) - 1] = '\0';

                borrowBook(isbn);
                break;
            case 8:
                printf("Enter the ISBN of the book you want to return: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strlen(isbn) - 1] = '\0';

                returnBook(isbn);
                break;
            case 9:
                printf("Enter the file name: ");
                fgets(value, sizeof(value), stdin);
                value[strlen(value) - 1] = '\0';

                saveLibraryToFile(value);
                break;
            case 10:
                printf("Enter the file name: ");
                fgets(value, sizeof(value), stdin);
                value[strlen(value) - 1] = '\0';

                loadLibraryFromFile(value);
                break;
            case 11:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }

        printf("\n");
    } while (choice != 11);

    return 0;
}
