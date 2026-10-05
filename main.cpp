// =======================================================
// GREEN CART SYSTEM - C++ MAIN SOURCE CODE
// =======================================================

#include <iostream>
#include <mysql/mysql.h>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Structure to hold individual items added to shopping cart
struct CartItem {
    string name;
    int quantity;
    double price;
    double carbon;
};

// Function to establish a secure link with local MySQL Server
MYSQL* get_db_connection() {
    MYSQL* conn = mysql_init(NULL);
    if (conn == NULL) {
        cout << "❌ MySQL initialization failed!" << endl;
        return NULL;
    }

    // NOTE: Change "password" to your own local MySQL server root password
    if (mysql_real_connect(conn, "localhost", "root", "password", "greencart_db", 0, NULL, 0) == NULL) {
        cout << "❌ Database Connection Error: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

// Function to display the eco-products catalog from MySQL
void show_products(MYSQL* conn) {
    if (mysql_query(conn, "SELECT * FROM products")) {
        cout << "❌ Query Error: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == NULL) return;

    MYSQL_ROW row;
    cout << "\n" << string(58, '=') << endl;
    cout << left << setw(5) << "ID" << " | " << setw(25) << "Product Name" << " | " << setw(10) << "Price (Rs)" << " | " << setw(10) << "CO2 (g)" << endl;
    cout << string(58, '=') << endl;

    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(5) << row[0] 
             << " | " << setw(25) << row[1] 
             << " | " << setw(10) << row[2] 
             << " | " << setw(10) << row[3] << endl;
    }
    cout << string(58, '=') << endl;
    mysql_free_result(res);
}

// Logic function to compute environmental performance rating
string get_eco_rating(double carbon) {
    if (carbon <= 50.0) return "Green Warrior (Excellent)";
    if (carbon <= 150.0) return "Eco Conscious (Good)";
    return "High Carbon Alert (Poor)";
}

// Function to create itemized bill and submit transaction metrics
void new_billing(MYSQL* conn) {
    string customer_name;
    cout << "\nEnter Customer Name: ";
    cin.ignore();
    getline(cin, customer_name);

    vector<CartItem> cart;
    string p_id;

    while (true) {
        show_products(conn);
        cout << "Enter Product ID to add to cart (or press 0 to Finish): ";
        cin >> p_id;
        if (p_id == "0") break;

        string query = "SELECT name, price, carbon_factor FROM products WHERE id = " + p_id;
        if (mysql_query(conn, query.c_str())) {
            cout << "❌ Invalid Product ID! Try again." << endl;
            continue;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            int qty;
            cout << "Enter quantity for " << row[0] << ": ";
            cin >> qty;

            double total_price = stod(row[1]) * qty;
            double total_carbon = stod(row[2]) * qty;

            cart.push_back({row[0], qty, total_price, total_carbon});
            cout << "✅ Added " << row[0] << " x " << qty << " to cart.\n" << endl;
        } else {
            cout << "❌ Product not found!" << endl;
        }
        mysql_free_result(res);
    }

    if (cart.empty()) {
        cout << "🛒 Cart is empty. Billing cancelled." << endl;
        return;
    }

    double final_amount = 0, final_carbon = 0;
    for (const auto& item : cart) {
        final_amount += item.price;
        final_carbon += item.carbon;
    }
    string rating = get_eco_rating(final_carbon);

    // Print Receipt Console Invoice Layout
    cout << "\n🌿🌿🌿🌿🌿🌿 GREEN CART INVOICE 🌿🌿🌿🌿🌿🌿" << endl;
    cout << "Customer Name: " << customer_name << endl;
    cout << string(50, '-') << endl;
    cout << left << setw(25) << "Item" << " | " << setw(5) << "Qty" << " | " << setw(10) << "Price (Rs)" << endl;
    cout << string(50, '-') << endl;
    for (const auto& item : cart) {
        cout << left << setw(25) << item.name << " | " << setw(5) << item.quantity << " | " << setw(10) << fixed << setprecision(2) << item.price << endl;
    }
    cout << string(50, '-') << endl;
    cout << "Total Bill Amount     : Rs " << final_amount << endl;
    cout << "Total Carbon Footprint: " << final_carbon << " grams of CO2" << endl;
    cout << "Environmental Rating  : " << rating << endl;
    cout << string(50, '-') << endl;

    // Direct insertion of data logs inside remote SQL Schema
    string insert_query = "INSERT INTO sales (customer_name, total_amount, total_carbon, eco_rating) VALUES ('" 
                         + customer_name + "', " + to_string(final_amount) + ", " + to_string(final_carbon) + ", '" + rating + "')";
    
    if (mysql_query(conn, insert_query.c_str()) == 0) {
        cout << "\n✅ Transaction successfully saved to MySQL database." << endl;
    } else {
        cout << "\n❌ Failed to save transaction: " << mysql_error(conn) << endl;
    }
}

int main() {
    MYSQL* conn = get_db_connection();
    if (!conn) return 1;

    int choice;
    while (true) {
        cout << "\n===== 🌿 GREEN CART SYSTEM MENU (C++) =====" << endl;
        cout << "1. View Eco-Products Catalogue" << endl;
        cout << "2. Create New Bill (Check Carbon Footprint)" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        if (choice == 1) {
            show_products(conn);
        } else if (choice == 2) {
            new_billing(conn);
        } else if (choice == 3) {
            cout << "Thank you for using GreenCart C++! Keep saving the environment. 🌱" << endl;
            break;
        } else {
            cout << "❌ Invalid Choice! Try again." << endl;
        }
    }

    mysql_close(conn);
    return 0;
}
