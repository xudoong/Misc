import pandas as pd

csv_file = 'example.csv'
parquet_file = 'output.parquet'

# read csv
df = pd.read_csv(csv_file)
print(df)

# drop column
df = df.drop(columns=['C'])

# convert format
df['A'] = (df['A'] * 10).astype('float64')

# reorder
df = df[['B', 'A', 'D']]

# save parquet
df.to_parquet(parquet_file, engine='pyarrow')

df = pd.read_parquet(parquet_file)
print(df)