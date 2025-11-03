#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "miniproj.h"

// ---------------------- GLOBAL VARIABLES ----------------------

static struct Order *front = NULL; // Front pointer of the priority queue
struct SalesRecord salesHistory[MAX_HISTORY]; // Array to store sales history
int salesCount = 0; // Counter for sales records

// ---------------------- BST IMPLEMENTATION ----------------------

// Inserts a new product node into the Binary Search Tree
struct Product* insertBST(struct Product* root, int id, char name[], int stock, float price) {
    if (root == NULL) {
        struct Product* newNode = (struct Product*)malloc(sizeof(struct Product));
        newNode->id = id;
        strcpy(newNode->name, name);
        newNode->stock = stock;
        newNode->price = price;
        newNode->lowStockFlag = (stock < LOW_STOCK_THRESHOLD) ? 1 : 0;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    if (id == root->id) {
        printf("Product ID %d already exists!\n", id);
        return root;
    }

    if (id < root->id)
        root->left = insertBST(root->left, id, name, stock, price);
    else
        root->right = insertBST(root->right, id, name, stock, price);

    return root;
}

// Searches for a product in the BST using recursive algorithm
struct Product* searchBST(struct Product* root, int id) {
    if (root == NULL || root->id == id)
        return root;
    if (id < root->id)
        return searchBST(root->left, id);
    else
        return searchBST(root->right, id);
}

// Performs in-order traversal to display products in sorted order
void inorderBST(struct Product* root) {
    if (root == NULL)
        return; // just return, no print here

    inorderBST(root->left);
    printf("ID: %4d | Name: %-20s | Stock: %4d | Price: $%7.2f | %s\n",
           root->id, root->name, root->stock, root->price,
           root->lowStockFlag ? "LOW STOCK" : "        ");
    inorderBST(root->right);
}
// Finds the product with the minimum ID in the BST
struct Product* findMin(struct Product* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

// Deletes a product node from the BST using recursive algorithm
struct Product* deleteProductBST(struct Product* root, int id) {
    if (root == NULL) return root;

    if (id < root->id)
        root->left = deleteProductBST(root->left, id);
    else if (id > root->id)
        root->right = deleteProductBST(root->right, id);
    else {
        if (root->left == NULL) {
            struct Product* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Product* temp = root->left;
            free(root);
            return temp;
        }

        struct Product* temp = findMin(root->right);
        root->id = temp->id;
        strcpy(root->name, temp->name);
        root->stock = temp->stock;
        root->price = temp->price;
        root->lowStockFlag = temp->lowStockFlag;
        root->right = deleteProductBST(root->right, temp->id);
    }
    return root;
}

// Counts the total number of products in the BST
int countProducts(struct Product* root) {
    if (root == NULL) return 0;
    return 1 + countProducts(root->left) + countProducts(root->right);
}

// Displays only products that are below low stock threshold
void displayLowStock(struct Product* root) {
    if (root == NULL) return;
    
    displayLowStock(root->left);
    if (root->lowStockFlag) {
        printf("ID: %4d | Name: %-20s | Stock: %4d | Price: $%7.2f\n",
               root->id, root->name, root->stock, root->price);
    }
    displayLowStock(root->right);
}

// ---------------------- PRIORITY QUEUE USING LINKED LIST ----------------------

// Inserts a new order into the priority queue maintaining priority order
void insertPQ(int id, int quantity, int priority, char customerName[]) {
    struct Order *newOrder = (struct Order*)malloc(sizeof(struct Order));
    newOrder->productId = id;
    newOrder->quantity = quantity;
    newOrder->priority = priority;
    strcpy(newOrder->customerName, customerName);
    newOrder->next = NULL;

    if (front == NULL || priority > front->priority) {
        newOrder->next = front;
        front = newOrder;
    } else {
        struct Order *temp = front;
        while (temp->next != NULL && temp->next->priority >= priority)
            temp = temp->next;
        newOrder->next = temp->next;
        temp->next = newOrder;
    }

    printf("Order added successfully!\n");
    printf("Customer: %s | Product ID: %d | Quantity: %d | Priority: %d\n", 
           customerName, id, quantity, priority);
}

// Removes and returns the order with highest priority (front of queue)
struct Order* deleteMax() {
    if (front == NULL)
        return NULL;

    struct Order *temp = front;
    front = front->next;
    return temp;
}

// Displays all pending orders in priority order
void displayOrders() {
    if (front == NULL) {
        printf("No pending orders.\n");
        return;
    }

    printf("\n=== PENDING ORDERS (by priority) ===\n");
    struct Order *temp = front;
    int count = 1;
    while (temp != NULL) {
        printf("%d. Customer: %-15s | Product ID: %4d | Quantity: %3d | Priority: %2d\n",
               count++, temp->customerName, temp->productId, temp->quantity, temp->priority);
        temp = temp->next;
    }
    printf("Total Orders: %d\n", count-1);
}

// Counts the number of orders currently in the priority queue
int countPendingOrders() {
    int count = 0;
    struct Order *temp = front;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Clears all orders from the priority queue
void clearAllOrders() {
    if (front == NULL) {
        printf("No orders to clear.\n");
        return;
    }
    
    printf("Are you sure you want to clear all %d pending orders? (1=Yes, 0=No): ", countPendingOrders());
    int confirm;
    scanf("%d", &confirm);
    
    if (confirm) {
        struct Order *temp;
        while (front != NULL) {
            temp = front;
            front = front->next;
            free(temp);
        }
        printf("All orders cleared successfully!\n");
    } else {
        printf("Operation cancelled.\n");
    }
}

// ---------------------- SALES HISTORY FUNCTIONS ----------------------

// Adds a completed sale to the sales history array
void addSalesRecord(int id, char name[], int quantity, float amount, char date[]) {
    if (salesCount < MAX_HISTORY) {
        salesHistory[salesCount].productId = id;
        strcpy(salesHistory[salesCount].productName, name);
        salesHistory[salesCount].quantitySold = quantity;
        salesHistory[salesCount].totalAmount = amount;
        strcpy(salesHistory[salesCount].date, date);
        salesCount++;
    } else {
        printf("Sales history full. Cannot add more records.\n");
    }
}

// Displays comprehensive sales report with totals
void displaySalesReport() {
    if (salesCount == 0) {
        printf("No sales records available.\n");
        return;
    }
    
    printf("\n=== SALES REPORT ===\n");
    float totalRevenue = 0;
    for (int i = 0; i < salesCount; i++) {
        printf("Date: %s | Product: %-20s | Qty: %3d | Amount: $%7.2f\n",
               salesHistory[i].date, salesHistory[i].productName,
               salesHistory[i].quantitySold, salesHistory[i].totalAmount);
        totalRevenue += salesHistory[i].totalAmount;
    }
    printf("Total Sales: %d transactions | Total Revenue: $%.2f\n", salesCount, totalRevenue);
}

// Calculates total revenue from all sales records
float calculateTotalRevenue() {
    float total = 0;
    for (int i = 0; i < salesCount; i++) {
        total += salesHistory[i].totalAmount;
    }
    return total;
}