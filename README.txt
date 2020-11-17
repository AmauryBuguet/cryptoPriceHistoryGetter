PriceHistoryGetter, a tool to get kline price history of a USDT-Ⓜ Futures trading pair from Binance.
It can be very useful to backtest trading strategies if you know how to use it.

Just select a start date, a timeframe, a pair and an end date and click start, the program will poll data from Binance and store it at the location specified in the code.
The data is stored in a txt file, as a JSON-formatted array of klines arrays (more info on these in the Binance API documentation)

NB : most of the Futures were created in early 2020 so it is useless (and not recommended as it can lead to errors) to select start dates earlier than that for many altcoins
A good practice would be to check creation dates before choosing it.

Future improvements : 
- Add support for all the existing pairs from Binance, including spot
- auto correct start date if its prior to pair creation on Binance