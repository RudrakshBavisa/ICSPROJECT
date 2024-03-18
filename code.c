#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Product
{
    int ID;
    char ProductName[50];
    int Quantity;
    char Date[12];
};
struct Customer
{
    int ID;
    char password[20];
};

FILE *fp;

void addproduct();
void displayproduct();
void updateproduct();
void buy();
void productsSoldToday();
void createCustomerAccount();

int main()
{
    int ch;
    char userType[10], password[20], correctPassword[] = "password", managerPassword[] = "manager123";
    printf("Are you a customer or manager? (customer/manager): ");
    scanf("%s", userType);

    if (strcmp(userType, "manager") == 0)
    {
        manager:
            printf("Enter the password: ");
            scanf("%s", password);
            if (strcmp(password, correctPassword) == 0)
            {
                system("cls");
                printf("Access granted!\n");
                while (1)
                {
                    printf("<=MANAGER MENU=>\n");
                    printf("1.Products sold today\n");
                    printf("2.Add product\n");
                    printf("3.Display products\n");
                    printf("4.Update product\n");
                    printf("5.Switch to Menu\n");
                    printf("0.Exit\n\n");
                    printf("enter your choice: ");
                    scanf("%d", &ch);

                    switch (ch)
                    {
                    case 0:
                        exit(0);
                    case 1:
                        printf("Enter manager password: ");
                        scanf("%s", password);
                        if (strcmp(password, managerPassword) == 0)
                        {
                            productsSoldToday();
                        }
                        else
                        {
                            printf("Incorrect manager password!\n");
                        }
                        break;
                    case 2:
                        addproduct();
                        break;
                    case 3:
                        displayproduct();
                        break;
                    case 4:
                        updateproduct();
                        break;
                    case 5:
                        goto customer;
                    default:
                        printf("Invalid choice\n\n");
                        break;
                    }
                }
        }
        else
        {
            printf("Incorrect password! Access denied.\n");
        }
    }
    else if (strcmp(userType, "customer") == 0)
    {
        customer:
            while (1)
            {
                system("cls");
                printf("<=CUSTOMER MENU=>\n");
                printf("1.Buy product\n");
                printf("2.Goto manager menu\n");
                printf("0.Exit\n\n");
                printf("enter your choice: ");
                scanf("%d", &ch);

                switch (ch)
                {
                case 0:
                    exit(0);

                case 1:
                    buy();
                    break;
                case 2:
                    goto manager;
                    break;
                default:
                    printf("invalid choice\n\n");
                    break;
                }
        }
    }
    else
    {
        printf("Invalid user type!\n");
    }

    return 0;
}

void addproduct()
{
    struct Product p;
    char mydate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(mydate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(p.Date, mydate);

    fp = fopen("product.txt", "at");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("enter product id: ");
    scanf("%d", &p.ID);

    printf("enter product name: ");
    fflush(stdin);
    fgets(p.ProductName, 50, stdin);
    p.ProductName[strcspn(p.ProductName, "\n")] = 0; // remove trailing newline

    printf("enter product quantity: ");
    scanf("%d", &p.Quantity);

    printf("\nproduct added successfully...\n");

    fprintf(fp, "%d, %s, %d, %s\n", p.ID, p.ProductName, p.Quantity, p.Date);
    fclose(fp);
}

void displayproduct()
{
    struct Product p;
    system("cls");
    printf("<==Product list ==>\n\n");
    printf("%-10s %-30s %-30s %s\n", "ID", "product name", "Quantity", "Date");
    printf("--------------------------------------------------\n");

    fp = fopen("product.txt", "rt");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    while (fscanf(fp, "%d, %[^,], %d, %s\n", &p.ID, p.ProductName, &p.Quantity, p.Date) == 4)
    {
        printf("%-10d %-30s %-30d %s\n", p.ID, p.ProductName, p.Quantity, p.Date);
    }
    fclose(fp);
}

void updateproduct()
{
    int ID, f = 0;
    struct Product p;

    system("cls");
    displayproduct();
    printf("<==update products==>\n\n");
    printf("enter product id to update: ");
    scanf("%d", &ID);

    fp = fopen("product.txt", "r+t");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    FILE *ft = fopen("temp.txt", "wt");
    if (ft == NULL)
    {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d, %[^,], %d, %s\n", &p.ID, p.ProductName, &p.Quantity, p.Date) == 4)
    {
        if (ID == p.ID)
        {
            f = 1;
            printf("enter new product name: ");
            fflush(stdin);
            fgets(p.ProductName, 50, stdin);
            p.ProductName[strcspn(p.ProductName, "\n")] = 0; // remove trailing newline

            printf("enter new product quantity: ");
            fflush(stdin);
            scanf("%d", &p.Quantity);
        }
        fprintf(ft, "%d, %s, %d, %s\n", p.ID, p.ProductName, p.Quantity, p.Date);
    }
    fclose(fp);
    fclose(ft);

    remove("product.txt");
    rename("temp.txt", "product.txt");

    if (f == 1)
    {
        printf("\nproduct updated...\n");
    }
    else
    {
        printf("\nproduct not found!\n");
    }
}

void buy()
{   
    int id, quantity, f = 0;
    struct Product p;

    system("cls");
    printf("<==buy products==>\n\n");
    displayproduct();
    printf("Enter product ID to buy: ");
    scanf("%d", &id);

    printf("Enter quantity to buy: ");
    scanf("%d", &quantity);

    fp = fopen("product.txt", "r+t");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    FILE *ft = fopen("temp.txt", "wt");
    if (ft == NULL)
    {
        printf("Error creating temp file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d, %[^,], %d, %s\n", &p.ID, p.ProductName, &p.Quantity, p.Date) == 4)
    {
        if (id == p.ID)
        {
            f = 1;
            if (p.Quantity >= quantity)
            {
                p.Quantity -= quantity;
                printf("\nProducts bought successfully...\n");
                int a=0;
                scanf("%d",&a);
            }
            else
            {
                printf("\nInsufficient stock!\n");
                int b=1;
                scanf("%d",&b);
                buy();                
            }
        }
        fprintf(ft, "%d, %s, %d, %s\n", p.ID, p.ProductName, p.Quantity, p.Date);
    }
    fclose(fp);
    fclose(ft);

    remove("product.txt");
    rename("temp.txt", "product.txt");

    if (f == 0)
    {
        printf("\nProduct not found!\n");
    }
    displayproduct();
    
}

void productsSoldToday()
{
    struct Product p;
    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);
    char todayDate[12];
    sprintf(todayDate, "%02d/%02d/%d", currentTime->tm_mday, currentTime->tm_mon + 1, currentTime->tm_year + 1900);

    int soldCount = 0;

    printf("Products sold today (%s):\n", todayDate);
    printf("%-10s %-30s %-30s %s\n", "ID", "Product Name", "Date", "Quantity Sold");
    printf("--------------------------------------------------\n");

    fp = fopen("product.txt", "rt");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    while (fscanf(fp, "%d, %[^,], %d, %s\n", &p.ID, p.ProductName, &p.Quantity, p.Date) == 4)
    {
        if (strcmp(p.Date, todayDate) == 0 && p.Quantity < 0) {
            printf("%-10d %-30s %-30s %d\n", p.ID, p.ProductName, p.Date, abs(p.Quantity));
            soldCount += abs(p.Quantity);
        }
    }
    fclose(fp);

    printf("\nTotal products sold today: %d\n", soldCount);
}
void createCustomerAccount()
{
    struct Customer c;

    printf("Enter customer ID: ");
    scanf("%d", &c.ID);

    printf("Enter password: ");
    scanf("%s", c.password);

    FILE *customerFile = fopen("customers.txt", "at");
    if (customerFile == NULL)
    {
        printf("Error opening customer file.\n");
        return;
    }
    fprintf(customerFile, "%d, %s\n", c.ID, c.password);
    fclose(customerFile);

    printf("Customer account created successfully.\n");
}
