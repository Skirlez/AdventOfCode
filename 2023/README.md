Timings on my CPU (Ryzen 7 3700X): 
|Day|Part 1 (μs)|Part 2 (μs)|Notes|
|-|-|-|-|
|01|91|137||
|02|14|15||
|03|33|18||
|04|117|119||
|05|37|43||
|06|0.041|0.033||
|07|375|376||
|08|26|183||
|09|280|280||
|10|68|265||
|11|61|61||
|12|950|56125|Part 2 solution relies on __int128_t|
|13|25|27||
|14|21|24103|Part 2 time varies a LOT depending on input|
|15|25|120||
|16|31|7873||
|17|47137|961989||
|18|1.8|3.6||
|19|192|190||

Give or take 10 μs for results above 200 μs. Might switch to a more consistent measurement method later.
See the README in the repository's root for information on how the measurements are done.