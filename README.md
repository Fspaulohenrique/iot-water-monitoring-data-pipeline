# 💧 IoT Water Monitoring Data Pipeline

This project simulates an IoT-based water monitoring system, designed to collect, process, and analyze environmental data such as water levels and temperature.

Originally developed as an engineering project, it has been evolved into a data-focused solution applying concepts of data pipelines and backend processing.

---

## 🚀 Project Overview

The goal of this project is to demonstrate how data from sensors can be:

- Collected from IoT devices (Arduino)
- Stored in a structured format (CSV)
- Processed and transformed using Python
- Used for analysis and monitoring

---

## 🧠 Technologies Used

- Arduino (IoT data collection)
- Python (data processing)
- Pandas (data manipulation)
- CSV (data storage)

---

## 📊 Sample Data

The project includes sample data representing sensor readings such as:

- Water level
- Temperature
- Timestamped records

---

## 🏗️ Architecture

This project simulates a complete data flow from IoT devices to data processing:

1. **Data Collection (Arduino)**
   - Water flow sensor collects real-time data
   - Calculates flow rate, total volume, and cost

2. **Data Storage (CSV)**
   - Sensor data is structured and stored as tabular data

3. **Data Processing (Python)**
   - Data is loaded and transformed using Python (Pandas)
   - Additional metrics are calculated (m³, cost)
   - Aggregations are performed

4. **Output**
   - Processed data is saved for further analysis

---

## ⚙️ How to Run

1. Clone the repository
2. Navigate to the project folder
3. Run the pipeline script:

```bash
python src/data_pipeline.py
