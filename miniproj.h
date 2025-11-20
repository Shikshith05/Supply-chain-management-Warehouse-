#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <stddef.h>

// Runtime-configurable settings (set via command-line args)
extern int LOW_STOCK_THRESHOLD; // Threshold for low stock alerts
extern int MAX_HISTORY;         // Maximum sales records to store

// ---------------------- STRUCT DEFINITIONS ----------------------

// Represents a product in the warehouse inventory
struct Product {
    int id;                     // Unique product identifier
    char name[50];              // Product name
    int stock;                  // Current stock quantity
    float price;                // Product price
    int lowStockFlag;           // Flag indicating low stock (1 = low, 0 = normal)
    struct Product *left;       // Pointer to left child in BST
    struct Product *right;      // Pointer to right child in BST    
    char supplier[50];          // Supplier name
};

// Represents a customer order in the priority queue
struct Order {
    int productId;              // ID of the product being ordered
    int quantity;               // Quantity requested
    int priority;               // Order priority (1-10, higher = more urgent)
    char customerName[50];      // Name of the customer who placed the order
    struct Order *next;         // Pointer to next order in the queue
};

// Stores sales transaction history
struct SalesRecord {
    int productId;              // ID of the sold product
    char productName[50];       // Name of the sold product
    int quantitySold;           // Quantity sold in this transaction
    float totalAmount;          // Total sale amount (quantity * price)
    char date[20];              // Date of the sale transaction
};

// ---------------------- BST FUNCTION DECLARATIONS ----------------------

struct Product* insertBST(struct Product*, int, char[], int, float,char []);
struct Product* searchBST(struct Product*, int);
void inorderBST(struct Product*);
struct Product* findMin(struct Product*);
struct Product* deleteProductBST(struct Product*, int);
int countProducts(struct Product*);
void displayLowStock(struct Product*);

// ---------------------- PRIORITY QUEUE FUNCTION DECLARATIONS ----------------------

void insertPQ(int id, int quantity, int priority, char customerName[]);
struct Order* deleteMax();
void displayOrders();
int countPendingOrders();
void clearAllOrders();

// ---------------------- SALES HISTORY FUNCTIONS ----------------------

void addSalesRecord(int id, char name[], int quantity, float amount, char date[]);
void displaySalesReport();
float calculateTotalRevenue();

// ---------------------- CORE FUNCTION DECLARATIONS ----------------------

void addProduct();
void searchProduct();
void updateProductMenu();
void deleteProduct();
void displayInventoryStats();
void ordersPlaced();
void updateStockAfterDispatch(int id, int quantity);
void restockProduct();
void generateReports();

// Initialize runtime configuration (call early from main)
void initConfig(int lowStockThreshold, int maxHistory);

// ---------------------- INPUT HELPERS ----------------------
int safeIntInput();
float safeNonNegativeFloatInput();
int safePositiveIntInput();

#endif