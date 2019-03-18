# Mochimo mini miner (working with mcmpool.pw)

Dependencies:
- CppREST: https://github.com/Microsoft/cpprestsdk
- Boost
- Mochimo source code

for linux: run
```
sudo apt-get update
sudo apt-get install libcpprest-dev
```

For windows: you can install it through Nuget, or download at Microsoft's github repository.


Notice: This miner is for educational/improvement purposes or researching about Mochimo Coin. It contains 95% mochimo original code (version 2.2). I do not have any rights about this, using/distributing it as your own risk.
This version is the first version I modified to work with http://mcmpool.pw

# How this work?

### Pool side: 
mcmpool.pw generates a root hash from transactions on mochimo network, then encode it (Base64) and send to client miners. The difficulty is adjusted as below function:
```
int CalcAdjustDiff(unsigned int Diff)
{
    unsigned int root = Diff >> 3;
    unsigned int last = Diff & 7;
    //modified
    root--;
    return (root << 3) | last;
}
```
That also means the difficult is subtracted by 8. 

### Miner side:
Miners will be received full information to mine from pool, so it can save a lot of time fetching block, communicating with peers and generating root. After that, miners try to generate nonces (32 bytes) then send back to Pool. A share is accepted by mcmpool if it's valid (for Trigg algorithm, haiku grammars,...), otherwise, failed shares mean non-valid shares or duplicated shares (someone send that nonce before) or in the time miner send that share, someone successfully finds a block.

### Connection method:
Currently, it uses GetWork. Stratum is sure, but it's future.

### Way to improve this miner:
- Cuda memory allocation is expensive. We can improve hash rate if we find the way to initialize cuda mem once.
- Pre-calculating SHA256 Nth round. If we can manage to predict the state of SHA256 before uploading it to GPU, we can save a lot of time.
- Multi-threading programming. The best way to achieve maximum power of GPU is controlling each GPU in different threads. Don't waste time at CPU to prepare data, we completely can do it in another thread.
- Early rejection in SHA256 algorithm: http://www.nicolascourtois.com/bitcoin/Optimising%20the%20SHA256%20Hashing%20Algorithm%20for%20Faster%20and%20More%20Efficient%20Bitcoin%20Mining_Rahul_Naik.pdf . We can use various heuristic methods to ignore invalid nonces before fully encrypt it.

# How to build
```
cd MiniMCM
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```
**Note: Miner wallet and url was hard-coded inside `miner.c`**
# Run it
```
./MCM_mini
```
