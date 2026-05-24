//Student info:
//Hajar Maisarah Binti Ashari 24000225
//Syaheda Nadia Binti Azman 24000997
//Lew Wei Cheng 24000270
//Mohammad Danish Ikmal Bin Asmawi 22010206


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// Constants for the heat index formula
const double c1 = -42.379;
const double c2 = 2.04901523;
const double c3 = 10.14333127;
const double c4 = -0.22475541;
const double c5 = -0.00683783;
const double c6 = -0.05481717;
const double c7 = 0.00122874;
const double c8 = 0.00085282;
const double c9 = -0.00000199;

// Struct to store data 
struct WeatherDataHeatIndex {
    string state;
    string district;
    int year;
    double avgTemperature; 
    double humidity;       
};

//structure to store humidity
struct WeatherData {
    string state;
    string district;
    int districtNumber;
    int year;
    int humidity;
    string humidityLevel;
};

// Structure to store temperature records
struct TempRecord {
    int year;
    double avgTemp;
};

// Structure to store rainfall data
struct RainfallRecord {
    int year;
    double rainfall;
};

// Function to display menu
void displayMenu() {
    cout << "\n===== Weather Analysis System =====" << endl;
    cout << "T - Temperature Analysis" << endl;
    cout << "R - Rainfall Analysis" << endl;
    cout << "C - Climate Change Impact" << endl;
    cout << "H - Humidity Level" << endl;
    cout << "S- Heat Index" << endl;
    cout << "Q - Quit" << endl;
}

// Function to safely extract a number from a string
bool isNumber(const string &str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

// Function to extract the average temperature from a range like "24-30"
double extractAvgTemperature(const string &tempRange) {
    size_t dashPos = tempRange.find('-');
    if (dashPos != string::npos) {
        string minTempStr = tempRange.substr(0, dashPos);
        string maxTempStr = tempRange.substr(dashPos + 1);

        try {
            double minTemp = stod(minTempStr);
            double maxTemp = stod(maxTempStr);
            return (minTemp + maxTemp) / 2.0; // Compute average temperature
        } catch (...) {
            return -999; // Return an invalid value if conversion fails
        }
    }
    return -999; // Return an invalid value if the format is incorrect
}

// Function to determine humidity level
string determineHumidityLevel(int humidity) {
    if (humidity >= 70) return "Very High";
    else if (humidity >= 60 && humidity < 70) return "High";
    else if (humidity >= 30 && humidity < 60) return "Mid";
    else if (humidity >= 25 && humidity < 30) return "Low";
    else return "Very Low";
}

// Function to analyze climate change trends and display results in a table
void ClimateChangeTrend() {
    ifstream inputFile("Weather_Statistics.txt");//not at specific path so can open in any laptop 

    if (!inputFile) {
        cout << "Error: Unable to open Weather_Statistics.txt!" << endl;
        return;
    }

    map<string, vector<TempRecord>> districtData;  // Stores yearly temperature data for each district
    string line;
    
    //Skip the first line (Header)
    getline(inputFile, line);

    // Read the file and store temperature data
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string state, district, fullDistrict, yearStr, tempRange;
        int year;
        double avgTemp;

        if (!getline(ss, state, ',') || !getline(ss, district, ',')) continue;
        fullDistrict = state + " " + district;

        if (!getline(ss, yearStr, ',')) continue;
        yearStr.erase(remove(yearStr.begin(), yearStr.end(), ' '), yearStr.end());

        if (!isNumber(yearStr)) continue;
        year = stoi(yearStr);

        if (!getline(ss, tempRange, ',')) continue;
        avgTemp = extractAvgTemperature(tempRange);

        if (avgTemp == -999) continue;

        districtData[fullDistrict].push_back({year, avgTemp});
    }
    
    inputFile.close();

    //Display results in table format
    cout << "\n===================================================================================" << endl;
    cout << "                           CLIMATE CHANGE IMPACT ANALYSIS                          " << endl;
    cout << "===================================================================================" << endl;
    cout << left << setw(30) << "State & District" 
         << setw(12) << "Year 1" 
         << setw(12) << "Year 2" 
         << setw(15) << "Temp Change(C)" 
         << setw(12) << "Trend" 
         << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;

    bool hasResults = false;

    // Analyze temperature trends
    for (auto& entry : districtData) {
        string district = entry.first;
        vector<TempRecord>& records = entry.second;

        // Sort by year in case the data is unordered
        sort(records.begin(), records.end(), [](const TempRecord &a, const TempRecord &b) {
            return a.year < b.year;
        });

        // Check year-to-year temperature changes
        for (size_t i = 1; i < records.size(); i++) {
            double change = records[i].avgTemp - records[i - 1].avgTemp;
            string trend = (change > 0) ? "Increase" : "Decrease";

            if (abs(change) >= 2.0) {
                hasResults = true;
                cout << left << setw(30) << district 
                     << setw(12) << records[i - 1].year 
                     << setw(12) << records[i].year 
                     << setw(15) << fixed << setprecision(2) << abs(change)
                     << setw(12) << trend
                     << endl;
            }
        }
    }

    if (!hasResults) {
        cout << "No districts found with a temperature change of 2°C or more.\n";
    }

    cout << "===================================================================================" << endl;
}

// Function to process temperature trends
void TemperatureTrend() {
    ifstream weatherFile("Weather_Statistics.txt"); //not at specific path so can open in any laptop 
    ofstream highTempFile("High_Temp.txt");
    ofstream lowTempFile("Low_Temp.txt");
    
    if (!weatherFile || !highTempFile || !lowTempFile) {
        cerr << "Error opening file!" << endl;
        return;
    }
    
    string line;
    getline(weatherFile, line); // Skip header
    highTempFile << line << endl;
    lowTempFile << line << endl;
    
    while (getline(weatherFile, line)) {
        stringstream ss(line);
        string state, district, year, tempRange, rainfall, humidity;
        getline(ss, state, ',');
        getline(ss, district, ',');
        getline(ss, year, ',');
        getline(ss, tempRange, ',');
        getline(ss, rainfall, ',');
        getline(ss, humidity, ',');

        tempRange.erase(0, tempRange.find_first_not_of(" \t\r\n"));
        tempRange.erase(tempRange.find_last_not_of(" \t\r\n") + 1);
        
        size_t dashPos = tempRange.find('-');
        if (dashPos == string::npos) {
            cerr << "Invalid temperature format: " << tempRange << endl;
            continue; 
        }
        
        string minTempStr = tempRange.substr(0, dashPos);
        string maxTempStr = tempRange.substr(dashPos + 1);
        
        double minTemp = stod(minTempStr);
        double maxTemp = stod(maxTempStr);
        double avgTemp = (minTemp + maxTemp) / 2.0;
        
        if (avgTemp > 30.0) {
            highTempFile << line << endl;
        } else {
            lowTempFile << line << endl;
        }
    }
        
    weatherFile.close();
    highTempFile.close();
    lowTempFile.close();
    
    cout << "Data successfully split into High_Temp.txt and Low_Temp.txt." << endl;
}

// Function to analyze and display rainfall trends in a table format
void RainfallAnalysis() {
    ifstream inputFile("Weather_Statistics.txt"); //not at specific path so can open in any laptop 

    if (!inputFile) {
        cout << "Error: Unable to open Weather_Statistics.txt!" << endl;
        return;
    }

    //Store total rainfall + count years per district
    map<string, map<int, double>> districtRainfallData; // {District -> {Year -> Rainfall}}
    map<string, int> districtYearCount; // {District -> Count of years}
    string line;

    //Skip the first line (Header)
    getline(inputFile, line);

    //Read and process the file
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string state, district, fullDistrict, yearStr, tempRange, rainfallStr, humidity;
        int year;
        double rainfall;

        // Read data from the correct columns
        if (!getline(ss, state, ',')) continue;
        if (!getline(ss, district, ',')) continue;
        fullDistrict = state + " " + district;

        if (!getline(ss, yearStr, ',')) continue;
        if (!getline(ss, tempRange, ',')) continue; // Skip temperature range column
        if (!getline(ss, rainfallStr, ',')) continue; // Get rainfall column
        if (!getline(ss, humidity, ',')) continue; // Skip humidity column

        // Convert year to integer
        try {
            year = stoi(yearStr);
        } catch (...) {
            cout << "Invalid year format in line -> " << line << endl;
            continue;
        }

        // Convert rainfall to double
        try {
            rainfall = stod(rainfallStr);
        } catch (...) {
            cout << "Invalid rainfall format in line -> " << line << endl;
            continue;
        }

        //Store total rainfall per district AND per year
        districtRainfallData[fullDistrict][year] += rainfall;
        districtYearCount[fullDistrict]++; // Count how many years exist for this district
    }
    
    inputFile.close();

    //Table Header for Rainfall Analysis
    cout << "\n===================================================================================" << endl;
    cout << "                                RAINFALL ANALYSIS                                  " << endl;
    cout << "===================================================================================" << endl;
    cout << left << setw(30) << "State & District" 
         << setw(10) << "Year" 
         << setw(20) << "Total Rainfall (mm)"
         << setw(20) << "Average Rainfall (mm)" 
         << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;

    map<string, double> annualRainfallAbove1000mm; // Stores districts with >1000mm annual rainfall

    // Process rainfall data and display in table format
    for (auto& districtEntry : districtRainfallData) {
        string district = districtEntry.first;
        double totalDistrictRainfall = 0.0;
        int yearsCount = districtYearCount[district];

        for (auto& yearEntry : districtEntry.second) {
            int year = yearEntry.first;
            double totalRainfall = yearEntry.second;
            totalDistrictRainfall += totalRainfall;

            double averageRainfall = totalRainfall / yearsCount;

            cout << left << setw(30) << district 
                 << setw(10) << year 
                 << setw(20) << fixed << setprecision(2) << totalRainfall 
                 << setw(20) << fixed << setprecision(2) << averageRainfall
                 << endl;

            if (totalRainfall > 1000.0) {
                annualRainfallAbove1000mm[district] = totalRainfall;
            }
        }
    }

    cout << "===================================================================================" << endl;

    //Table for Districts with Rainfall > 1000mm Annually
    if (!annualRainfallAbove1000mm.empty()) {
        cout << "\nDistricts with Annual Rainfall Above 1000mm" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << left << setw(30) << "District" 
             << setw(20) << "Annual Rainfall (mm)" 
             << endl;
        cout << "---------------------------------------------------" << endl;

        for (const auto& entry : annualRainfallAbove1000mm) {
            cout << left << setw(30) << entry.first 
                 << setw(20) << fixed << setprecision(2) << entry.second 
                 << endl;
        }
        cout << "---------------------------------------------------" << endl;
    } else {
        cout << "\nNo districts found with total annual rainfall > 1000mm!\n";
    }
}


void HumidityLevel() {
    ifstream InputFile("Weather_Statistics.txt");
    if (!InputFile) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    vector<WeatherData> weatherRecords;
    string line;
    getline(InputFile, line); // Skip header line

    while (getline(InputFile, line)) {
        stringstream ss(line);
        string state, district, yearStr, tempRange, rainfallStr, humidityStr;
        int year, humidity;

        if (!getline(ss, state, ',')) continue;
        if (!getline(ss, district, ',')) continue;
        if (!getline(ss, yearStr, ',')) continue;
        if (!getline(ss, tempRange, ',')) continue;
        if (!getline(ss, rainfallStr, ',')) continue;
        if (!getline(ss, humidityStr, ',')) continue;

        try {
            year = stoi(yearStr);
            humidity = stoi(humidityStr);
        } catch (exception& e) {
            continue; // Skip invalid entries
        }

        // Extract district number directly
        int districtNumber = 0;
        for (char c : district) {
            if (isdigit(c)) {
                districtNumber = districtNumber * 10 + (c - '0');
            }
        }

        weatherRecords.push_back({state, district, districtNumber, year, humidity, determineHumidityLevel(humidity)});
    }
    InputFile.close();

    // Sorting by state, district number, year
    sort(weatherRecords.begin(), weatherRecords.end(), [](const WeatherData &a, const WeatherData &b) {
        if (a.state != b.state) return a.state < b.state;
        if (a.districtNumber != b.districtNumber) return a.districtNumber < b.districtNumber;
        return a.year < b.year;
    });

    cout << "====================================================================================\n";
    cout << "                                HUMIDITY LEVEL REPORT                               \n";
    cout << "====================================================================================\n";
    cout << left << setw(20) << "State" 
         << setw(20) << "District" 
         << setw(10) << "Year" 
         << setw(15) << "Humidity (%)" 
         << "Level" << "\n";
    cout << "------------------------------------------------------------------------------------\n";

    for (const auto &record : weatherRecords) {
        cout << left << setw(20) << record.state
             << setw(20) << record.district
             << setw(10) << record.year
             << setw(15) << record.humidity
             << record.humidityLevel << "\n";
    }

    cout << "===================================================================================" << endl;
}

// Heat Index Calculation Fuctions
double calculateHeatIndex(double celsius, double humidity) {
    // Convert to Celsius to Fahrenheit
    double fahrenheit = (celsius * 9 / 5) + 32;

    // Heat Index formula
    double hi = c1 + c2 * fahrenheit + c3 * humidity + c4 * fahrenheit * humidity
        + c5 * pow(fahrenheit, 2) + c6 * pow(humidity, 2)
        + c7 * pow(fahrenheit, 2) * humidity + c8 * fahrenheit * pow(humidity, 2)
        + c9 * pow(fahrenheit, 2) * pow(humidity, 2);

    // Convert back to Celsius
    return (hi - 32) * 5 / 9;
}

// File Reading Function
void readFile(const string& line, WeatherDataHeatIndex& entry) {
    istringstream stream(line);
    string tempRange;

    // Differentiate strict and district via a comma, the store data in respective variable
    getline(stream, entry.state, ',');
    getline(stream, entry.district, ',');
    stream >> entry.year;
    stream.ignore();

    // Calculate average temperature by differentiating max and min temp range, then calculating average with (max + min)/2
    getline(stream, tempRange, ',');
    size_t dashPos = tempRange.find('-');
    double temp1 = stod(tempRange.substr(0, dashPos));
    double temp2 = stod(tempRange.substr(dashPos + 1));
    entry.avgTemperature = (temp1 + temp2) / 2.0;

    // Store data in humidity
    stream.ignore(numeric_limits<streamsize>::max(), ',');
    stream >> entry.humidity;
}

// Main process function that uses all the other previous functions
void HeatIndex() {
    ifstream inputFile("Weather_Statistics.txt");
    ofstream outputFile("Heat_Index.txt");

    string line;

    // Skip header line
    getline(inputFile, line);

    // Output file header
    outputFile << "State,District,Year,AvgTemperature(C),Humidity(%),HeatIndex(C)\n";

    // Read each line in Weather_Statistics.txt, then make the corresponding line of Heat Index data in the Heat_Index.txt file
    while (getline(inputFile, line)) {
        WeatherDataHeatIndex entry;
        readFile(line, entry);
        double hi = calculateHeatIndex(entry.avgTemperature, entry.humidity);
        outputFile << entry.state << "," << entry.district << ","
            << entry.year << "," << fixed << setprecision(2)
            << entry.avgTemperature << "," << entry.humidity
            << "," << hi << "\n";
    }

    // Close both file being read and file being write
    inputFile.close();
    outputFile.close();
    
    cout << "====================================================================" << endl;
    cout << "Heat Index data has been successfully written to 'Heat_Index.txt'" << endl;
}

// Main function with menu-driven program
int main() {
    char userSelection;
    while (true) {
        displayMenu();
        cout << "Enter choice: ";
        cin >> userSelection;
        userSelection = toupper(userSelection);

        switch (userSelection) {
            case 'T':
                TemperatureTrend();
                break;
            case 'R':
                RainfallAnalysis();
                break;
            case 'C':
                ClimateChangeTrend();
                break;
            case 'H':
            	HumidityLevel();
            	break;
            case 'S':
            	HeatIndex();
            	break;
            case 'Q':
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid input. Try again.\n";
        }
    }
    return 0;
}
