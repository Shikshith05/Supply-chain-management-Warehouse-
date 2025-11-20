#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miniproj.h"

// Global variables
struct Product* root = NULL;

int safeIntInput() {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1) {
            printf("Invalid input! Please enter a non-negative integer: ");
            while (getchar() != '\n'); // clear buffer
            continue;
        }
        if (value < 0) {
            printf("Negative values are not allowed. Please enter a non-negative integer: ");
            continue;
        }
        return value;
    }
}

// Read a non-negative float from stdin, re-prompting on invalid/negative input
float safeNonNegativeFloatInput() {
    float value;
    while (1) {
        if (scanf("%f", &value) != 1) {
            printf("Invalid input! Please enter a non-negative number: ");
            while (getchar() != '\n');
            continue;
        }
        if (value < 0.0f) {
            printf("Negative values are not allowed. Please enter a non-negative number: ");
            continue;
        }
        return value;
    }
}

// Read a positive integer (>0) from stdin, re-prompting on invalid/<=0 input
int safePositiveIntInput() {
    int value;
    while (1) {
        if (scanf("%d", &value) != 1) {
            printf("Invalid input! Please enter a positive integer: ");
            while (getchar() != '\n');
            continue;
        }
        if (value <= 0) {
            printf("Only positive integers are allowed. Please enter a positive integer: ");
            continue;
        }
        return value;
    }
}

// Helper function to count low stock products
int countLowStockProducts(struct Product* root) {
    if (root == NULL) return 0;
    int count = 0;
    count += countLowStockProducts(root->left);
    if (root->lowStockFlag) count++;
    count += countLowStockProducts(root->right);
    return count;
}

// ---------------------- CORE FUNCTION IMPLEMENTATIONS ----------------------
//creating a node and inserting into BST
void addProduct() {
    int id, stock;
    float price;
        char name[50], supplier[50];

    printf("Enter Product ID: ");
    id = safeIntInput();

    struct Product *exists = searchBST(root, id);
    if (exists != NULL) {
        printf("Product ID %d already exists! Cannot add duplicate.\n", id);
        return;
    }

    printf("Enter Product Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Stock Quantity: ");
        stock = safePositiveIntInput();
    printf("Enter Price: ");
    price = safeNonNegativeFloatInput();
        printf("Enter Supplier Name: ");
        scanf(" %[^\n]", supplier);

    root = insertBST(root, id, name, stock, price,supplier);
    printf("Product added successfully!\n");
    
    struct Product *p = searchBST(root, id);
    if (p && p->stock < LOW_STOCK_THRESHOLD) {
        p->lowStockFlag = 1;
        printf("Low stock alert for new product!\n");
    }
}

void searchProduct() {
    int id;
    printf("Enter Product ID to search: ");
    id = safeIntInput();

    struct Product *p = searchBST(root, id);
    if (p) {
        printf("\n=== PRODUCT DETAILS ===\n");
        printf("ID: %d\n", p->id);
        printf("Name: %s\n", p->name);
        printf("Stock: %d\n", p->stock);
        printf("Price: $%.2f\n", p->price);
        printf("Status: %s\n", p->lowStockFlag ? "LOW STOCK" : "In Stock");
        printf("=======================\n");
    } else {
        printf("Product not found!\n");
    }
}

void updateProductMenu() {
    int id, choice, newStock;
    float newPrice;
    char newName[50];
    char newSupplier[50];
    
    printf("Enter Product ID to update: ");
    id = safeIntInput();
    
    struct Product *p = searchBST(root, id);
    if (!p) {
        printf("Product not found!\n");
        return;
    }
    
    printf("\nCurrent Details:\n");
    printf("Name: %s | Stock: %d | Price: $%.2f |Supplier Name: %s \n ", p->name, p->stock, p->price,p->supplier);
    
    printf("\nWhat would you like to update?\n");
    printf("1. Update Name\n");
    printf("2. Update Stock\n");
    printf("3. Update Price\n");
    printf("4. Update All\n");
    printf("5. Update Supplier Name\n");
    printf("Enter choice: ");
    choice = safeIntInput();
    
    switch(choice) {
        case 1:
            printf("Enter new name: ");
            scanf(" %[^\n]", newName);
            strcpy(p->name, newName);
            break;
        case 2:
            printf("Enter new stock quantity: ");
                newStock = safePositiveIntInput();
            p->stock = newStock;
            p->lowStockFlag = (newStock < LOW_STOCK_THRESHOLD) ? 1 : 0;
            break;
        case 3:
            printf("Enter new price: ");
            newPrice = safeNonNegativeFloatInput();
            p->price = newPrice;
            break;
        case 4:
            printf("Enter new name: ");
            scanf(" %[^\n]", newName);
            printf("Enter new stock quantity: ");
                newStock = safePositiveIntInput();
            printf("Enter new price: ");
            newPrice = safeNonNegativeFloatInput();
            strcpy(p->name, newName);
            p->stock = newStock;
            p->price = newPrice;
            p->lowStockFlag = (newStock < LOW_STOCK_THRESHOLD) ? 1 : 0;
            break;
        case 5:
                printf("Enter new supplier name: ");
                scanf(" %[^\n]", newSupplier);
                strcpy(p->supplier, newSupplier);
            break;
        default:
            printf("Invalid choice!\n");
            return;
    }
    printf("Product updated successfully!\n");
}

void deleteProduct() {
    int id;
    printf("Enter Product ID to delete: ");
    id = safeIntInput();
    
    struct Product *p = searchBST(root, id);
    if (!p) {
        printf("Product not found!\n");
        return;
    }
    
    printf("Are you sure you want to delete '%s' (ID: %d)? (1=Yes, 0=No): ", p->name, p->id);
    int confirm = safeIntInput();
    
    if (confirm) {
        root = deleteProductBST(root, id);
        printf("Product deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

void displayInventoryStats() {
    int totalProducts = countProducts(root);
    int lowStockCount = countLowStockProducts(root);
    
    printf("\n=== INVENTORY STATISTICS ===\n");
    printf("Total Products: %d\n", totalProducts);
    printf("Low Stock Items: %d\n", lowStockCount);
    printf("Pending Orders: %d\n", countPendingOrders());
    printf("Total Revenue: $%.2f\n", calculateTotalRevenue());
    printf("============================\n");
}

void restockProduct() {
    int id, quantity;
    printf("Enter Product ID to restock: ");
    id = safeIntInput();
    
    struct Product *p = searchBST(root, id);
    if (!p) {
        printf("Product not found!\n");
        return;
    }
    
    printf("Current stock: %d\n", p->stock);
    printf("Enter quantity to add: ");
    quantity = safePositiveIntInput();
    
    if (quantity <= 0) {
        printf("Invalid quantity!\n");
        return;
    }
    
    p->stock += quantity;
    p->lowStockFlag = (p->stock < LOW_STOCK_THRESHOLD) ? 1 : 0;
    
    printf("Restocked successfully! New stock: %d\n", p->stock);
}

void updateStockAfterDispatch(int id, int quantity) {
    struct Product *p = searchBST(root, id);
    if (p) {
        if (p->stock >= quantity) {
            p->stock -= quantity;
            
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            char date[20];
            sprintf(date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
            
            addSalesRecord(id, p->name, quantity, quantity * p->price, date);
            
            printf("Updated stock for %s (ID: %d): %d units left.\n",
                   p->name, p->id, p->stock);
            
            p->lowStockFlag = (p->stock < LOW_STOCK_THRESHOLD) ? 1 : 0;
            
            if (p->lowStockFlag)
                printf("Low stock alert for product %s (ID: %d)\n", p->name, p->id);
        } else {
            printf("Insufficient stock for %s (ID: %d). Available: %d, Required: %d\n", 
                   p->name, p->id, p->stock, quantity);
        }
    } else {
        printf("Product not found in inventory!\n");
    }
}

void generateReports() {
    int choice;
    do {
        printf("\n=== REPORTS MENU ===\n");
        printf("1. Sales Report\n");
        printf("2. Low Stock Report\n");
        printf("3. Inventory Report\n");
        printf("4. Financial Summary\n");
        printf("5. Back to Main Menu\n");
        printf("Enter choice: ");
        choice = safeIntInput();
        
        switch(choice) {
            case 1:
                displaySalesReport();
                break;
            case 2:
                printf("\n=== LOW STOCK REPORT ===\n");
                displayLowStock(root);
                break;
            case 3:
                printf("\n=== COMPLETE INVENTORY ===\n");
                inorderBST(root);
                break;
            case 4:
                printf("\n=== FINANCIAL SUMMARY ===\n");
                printf("Total Revenue: $%.2f\n", calculateTotalRevenue());
                printf("Total Products: %d\n", countProducts(root));
                printf("Pending Orders: %d\n", countPendingOrders());
                break;
            case 5:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while(choice != 5);
}

void ordersPlaced() {
    int choice, pid, prio, quantity;
    char customerName[50];

    do {
        printf("Current inventory details:\n");
        inorderBST(root);
        printf("\n--- Order Management System ---\n");
        printf("1. New Order\n");
        printf("2. Dispatch Highest Priority Order\n");
        printf("3. View Pending Orders\n");
        printf("4. Order Statistics\n");
        printf("5. Clear All Orders\n");
        printf("6. Exit to Main Menu\n");
        printf("Enter choice: ");
        choice = safeIntInput();

        switch (choice) {
            case 1:
                printf("Enter Customer Name: ");
                scanf(" %[^\n]", customerName);
                printf("Enter Product ID: ");
                pid = safeIntInput();
                printf("Enter Quantity: ");
                quantity = safePositiveIntInput();
                printf("Enter Priority (1-10, higher = more urgent): ");
                prio = safeIntInput();
                
                if (prio < 1 || prio > 10) {
                    printf("Priority must be between 1-10!\n");
                    break;
                }
                
                if (searchBST(root, pid)) {
                    insertPQ(pid, quantity, prio, customerName);
                } else {
                    printf("Invalid Product ID. Product not found in inventory.\n");
                }
                break;

            case 2: {
                struct Order *dispatched = deleteMax();
                if (dispatched == NULL) {
                    printf("No orders to dispatch.\n");
                } else {
                    printf("\n=== DISPATCHING ORDER ===\n");
                    printf("Customer: %s\n", dispatched->customerName);
                    printf("Product ID: %d | Quantity: %d | Priority: %d\n",
                           dispatched->productId, dispatched->quantity, dispatched->priority);
                    printf("==========================\n");
                    updateStockAfterDispatch(dispatched->productId, dispatched->quantity);
                    free(dispatched);
                }
                break;
            }

            case 3:
                displayOrders();
                break;
                
            case 4:
                printf("\n=== ORDER STATISTICS ===\n");
                printf("Pending Orders: %d\n", countPendingOrders());
                break;
                
            case 5:
                clearAllOrders();
                break;

            case 6:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid option.\n");
        }
    } while (choice != 6);
}

int main(int argc, char* argv[]) {
    int choice;
    int cfgLow = 5;
    int cfgMaxHistory = 100;

    // Parse command-line arguments: program <low_stock_threshold> <max_history>
    if (argc >= 2) {
        int v = atoi(argv[1]);
        if (v > 0) cfgLow = v;
    }
    if (argc >= 3) {
        int v = atoi(argv[2]);
        if (v > 0) cfgMaxHistory = v;
    }

    // Initialize runtime configuration (allocates sales history)
    initConfig(cfgLow, cfgMaxHistory);

    printf("====== SUPPLY CHAIN MANAGEMENT SYSTEM ======\n");
    printf("           Warehouse Management v2.0        \n\n");

    do {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Add Product\n");
        printf("2. Search Product\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Display All Products\n");
        printf("6. Manage Orders\n");
        printf("7. Restock Product\n");
        printf("8. Generate Reports\n");
        printf("9. Inventory Statistics\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        choice = safeIntInput();

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                searchProduct();
                break;
            case 3:
                updateProductMenu();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                printf("\n--- PRODUCT INVENTORY ---\n");
                if (root == NULL)
                    printf("No products in inventory.\n");
                else
                    inorderBST(root);
                break;
            case 6:
                ordersPlaced();
                break;
            case 7:
                restockProduct();
                break;
            case 8:
                generateReports();
                break;
            case 9:
                displayInventoryStats();
                break;
            case 10:
                printf("Exiting system... Thank you!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 10);

    return 0;
}
