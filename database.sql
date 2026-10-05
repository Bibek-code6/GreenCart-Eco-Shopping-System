-- =======================================================
-- GREEN CART SYSTEM - DATABASE SETUP
-- =======================================================

-- 1. Create and Use Database
CREATE DATABASE IF NOT EXISTS greencart_db;
USE greencart_db;

-- 2. Create Products Table with Carbon Factors
CREATE TABLE IF NOT EXISTS products (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    carbon_factor DECIMAL(5,2) NOT NULL -- CO2 emission in grams per unit
);

-- 3. Insert Eco-friendly and Non-Eco-friendly Sample Items
INSERT INTO products (name, price, carbon_factor) VALUES
('Organic Cloth Bag', 50.00, 5.0),
('Plastic Shopping Bag', 5.00, 50.0),
('LED Energy Saving Bulb', 150.00, 12.0),
('Incandescent Bulb (Old)', 20.00, 95.0),
('Recycled Paper Notebook', 60.00, 15.0),
('Regular Plastic Notebook', 45.00, 70.0),
('Stainless Steel Bottle', 350.00, 25.0),
('Single-use Plastic Bottle', 20.00, 120.0);

-- 4. Create Sales Table to log transaction history
CREATE TABLE IF NOT EXISTS sales (
    bill_no INT AUTO_INCREMENT PRIMARY KEY,
    customer_name VARCHAR(50) NOT NULL,
    total_amount DECIMAL(10,2) NOT NULL,
    total_carbon DECIMAL(10,2) NOT NULL,
    eco_rating VARCHAR(30) NOT NULL
);
