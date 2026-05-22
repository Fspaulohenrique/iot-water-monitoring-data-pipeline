import pandas as pd

# Load sample data
df = pd.read_csv('../data/sample_data.csv')

# Convert timestamp to datetime
df['timestamp'] = pd.to_datetime(df['timestamp'])

# Calculate additional metrics
df['water_level_m3'] = df['water_level'] / 1000
df['cost'] = df['water_level_m3'] * 15.14

# Aggregate data
summary = df.agg({
    'water_level': 'sum',
    'water_level_m3': 'sum',
    'cost': 'sum'
})

print("=== Data Summary ===")
print(summary)

# Save processed data
df.to_csv('../data/processed_data.csv', index=False)

print("\nProcessed data saved successfully.")
