# Stock Prediction using Alpha Vantage API

## Overview
This C++ program fetches stock market data from the Alpha Vantage API, processes it, and predicts future stock prices using a moving average approach. The program also normalizes stock prices for better analysis.

## Features
- Fetches real-time stock data using **Alpha Vantage API**.
- Parses and processes JSON data.
- Normalizes stock prices for better analysis.
- Computes **moving average** to predict future stock prices.
- Reverse normalizes predicted values to get actual stock prices.

## Dependencies
To run this program, ensure you have the following installed:
- **C++ Compiler (g++)**
- **cURL** (for API requests)
- **nlohmann/json.hpp** (for JSON parsing)

## Installation

### 1. Install Dependencies
#### On Ubuntu (Linux):
```sh
sudo apt update
sudo apt install g++ libcurl4-openssl-dev
```
#### On macOS:
```sh
brew install curl
```

### 2. Download Required Libraries
- Install `nlohmann/json` for JSON parsing:
```sh
wget https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp -O json.hpp
```

## Usage

### 1. Compile the Program
```sh
g++ -o stock_predict stock_predict.cpp -lcurl
```

### 2. Run the Program
```sh
./stock_predict
```

### 3. Input Details
When prompted, enter:
- **Stock Symbol** (e.g., `TSLA` for Tesla, `AAPL` for Apple).
- **Alpha Vantage API Key** (Get it from [Alpha Vantage](https://www.alphavantage.co)).

### 4. Output Example
```sh
Enter stock symbol: TSLA
Enter your Alpha Vantage API key: xxxxxxxx
Predicted stock price for TSLA: 712.35
```

## How It Works
1. **Fetch Data:** Uses cURL to request stock data from Alpha Vantage API.
2. **Parse JSON:** Extracts and processes stock data.
3. **Normalize Data:** Transforms stock prices between 0 and 1.
4. **Calculate Moving Average:** Uses recent data points to estimate the next stock price.
5. **Reverse Normalize:** Converts the normalized value back to actual price.

## Limitations
- This is a simple moving average model and may not accurately predict stock prices.
- Requires a valid **Alpha Vantage API key**.
- API rate limits apply (5 free API calls per minute).

## Future Improvements
- Implement advanced **Machine Learning models** for better predictions.
- Integrate with a GUI for better visualization.
- Fetch data from multiple sources for better accuracy.

## License
This project is open-source and can be modified freely.

## Author
Dixansh

