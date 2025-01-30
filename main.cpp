#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

// Structure to hold stock data
struct StockData
{
    string date;
    double open, high, low, close, volume;
};

// Callback function for cURL to write data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// Function to fetch stock data from Alpha Vantage
void fetchStockData(const string &symbol, const string &apiKey, const string &filename)
{
    CURL *curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + ".BSE&outputsiz=full&apikey=" + apiKey;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            std::ofstream outFile(filename);
            outFile << readBuffer;
            outFile.close();
        }
        else
        {
            cerr << "Error fetching data: " << curl_easy_strerror(res) << std::endl;
        }
    }
}

// Function to parse JSON stock data
void parseStockData(const string &filename, vector<StockData> &stockData, double &minClose, double &maxClose)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    json jsonData;
    inFile >> jsonData;

    // Check if 'Time Series (Daily)' exists and is not null
    if (jsonData.contains("Time Series (Daily)") && !jsonData["Time Series (Daily)"].is_null())
    {
        auto dailyData = jsonData["Time Series (Daily)"];
        for (auto &[date, data] : dailyData.items())
        {
            StockData entry;
            entry.date = date;
            entry.open = data.contains("1. open") ? stod(data["1. open"].get<string>()) : 0.0;
            entry.high = data.contains("2. high") ? stod(data["2. high"].get<string>()) : 0.0;
            entry.low = data.contains("3. low") ? stod(data["3. low"].get<string>()) : 0.0;
            entry.close = data.contains("4. close") ? stod(data["4. close"].get<string>()) : 0.0;
            entry.volume = data.contains("6. volume") ? stod(data["6. volume"].get<string>()) : 0.0;

            // Update min and max close prices
            minClose = min(minClose, entry.close);
            maxClose = max(maxClose, entry.close);

            stockData.push_back(entry);
        }
    }
    else
    {
        cerr << "Error: 'Time Series (Daily)' data is missing or null." << endl;
    }
}

// Function to normalize stock prices and return min/max closing prices
void normalizeData(vector<StockData> &stockData, double minClose, double maxClose)
{
    for (auto &entry : stockData)
    {
        entry.close = (entry.close - minClose) / (maxClose - minClose);
    }
}

// Function to calculate moving average for prediction
double movingAverage(const vector<StockData> &stockData, int window)
{
    double sum = 0;
    int n = stockData.size();
    for (int i = n - window; i < n; ++i)
    {
        sum += stockData[i].close;
    }
    return sum / window;
}

// Function to reverse normalize and get the actual stock price
double reverseNormalize(double normalizedPrice, double minClose, double maxClose)
{
    return normalizedPrice * (maxClose - minClose) + minClose;
}

int main()
{
    // Input stock symbol and API key
    string symbol, apiKey;
    cout << "Enter stock symbol: ";
    cin >> symbol;
    cout << "Enter your Alpha Vantage API key: ";
    cin >> apiKey;

    // Fetch and parse stock data
    const string filename = symbol + ".json";
    fetchStockData(symbol, apiKey, filename);

    vector<StockData> stockData;
    double minClose = numeric_limits<double>::max();
    double maxClose = numeric_limits<double>::min();

    parseStockData(filename, stockData, minClose, maxClose);

    if (stockData.empty())
    {
        cerr << "No stock data parsed. Exiting." << endl;
        return 1;
    }

    // Normalize data
    normalizeData(stockData, minClose, maxClose);

    // Predict stock price using moving average
    int window = 7; // Increased moving average window size
    double predictedNormalizedPrice = movingAverage(stockData, window);

    // Convert predicted normalized price back to actual price
    double predictedPrice = reverseNormalize(predictedNormalizedPrice, minClose, maxClose);

    cout << "Predicted stock price for " << symbol << ": " << predictedPrice << endl;

    return 0;
}
