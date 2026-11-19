# MYT Explorer

MYT Explorer is based on the open-source
`onion-monero-blockchain-explorer` codebase and integrates with the MYT
network (a Monero-based fork).

## Upstream Attribution

- Original explorer base:
  https://github.com/moneroexamples/onion-monero-blockchain-explorer
- Monero C++ libraries and protocol stack are used as upstream base in MYT.

The original upstream license notices are retained. MYT-specific changes are
copyright (c) 2026, MYT.

Currently available Monero blockchain explorers have several limitations which are of
special importance to privacy-oriented users:

 - they use JavaScript,
 - have images which might be used for [cookieless tracking](http://lucb1e.com/rp/cookielesscookies/),
 - track users activities through google analytics,
 - are closed sourced,
 - are not available as hidden services,
 - do not support Monero testnet nor stagenet networks,
 - have limited JSON API.


In this example, these limitations are addressed by development of
an Onion Monero Blockchain Explorer. The example not only shows how to use
Monero C++ libraries, but also demonstrates how to use:

 - [crow](https://github.com/CrowCpp/Crow) - C++ micro web framework
 - [mstch](https://github.com/no1msd/mstch) - C++ {{mustache}} templates
 - [json](https://github.com/nlohmann/json) - JSON for Modern C++
 - [fmt](https://github.com/fmtlib/fmt) - Small, safe and fast string formatting library

## Explorer hosts

Clearnet versions:
 - [https://xmrchain.net/](https://xmrchain.net/) - HTTPS enabled, most popular, and very stable.
 - [https://monerohash.com/explorer/](https://monerohash.com/explorer/) - nice looking one, HTTPS enabled.
 - [http://monerochain.com/](http://monerochain.com/) - JSON API based, multiple nodes.   

Testnet version:

 - [https://testnet.xmrchain.com/](https://testnet.xmrchain.com/) - https enabled.

Stagenet version:
 
 - [https://stagenet.xmrchain.net/](https://stagenet.xmrchain.net/)

i2p users (main Monero network):

 - [http://7o4gezpkye6ekibhgpkg7v626ze4idsirapufzrefkdysa6zxhha.b32.i2p/](http://7o4gezpkye6ekibhgpkg7v626ze4idsirapufzrefkdysa6zxhha.b32.i2p/)

Tor versions:

 - [http://exploredv42tq2nowrll6f27nuymenndwupueqvyugaqzbrvmjhhceqd.onion/](http://exploredv42tq2nowrll6f27nuymenndwupueqvyugaqzbrvmjhhceqd.onion/) - Native v3 Onion, JSON API enabled, emission enabled, rawtx enabled.

Alternative block explorers:

- [https://localmonero.co/blocks](https://localmonero.co/blocks)
- [https://monerovision.com](https://monerovision.com)


## Onion Monero Blockchain Explorer features

The key features of the Onion Monero Blockchain Explorer are:

 - no cookies, no web analytics trackers, no images,
 - open sourced,
 - made fully in C++,
 - showing encrypted payments ID,
 - showing ring signatures,
 - showing transaction extra field,
 - showing public components of Monero addresses,
 - detailed information about ring members, such as their age, timescale, and ring sizes,
 - showing number of amount output indices,
 - support Monero testnet and stagnet networks,
 - tx checker and pusher for online pushing of transactions,
 - can provide total amount of all miner fees,


## Development branch

Current development branch:

 - https://github.com/moneroexamples/onion-monero-blockchain-explorer/tree/devel

Note: `devel` branch of the explorer follows `master` branch of the monero!

## Compilation on Ubuntu 24.04


#### Monero download and compilation

To download and compile recent Monero follow instructions
in the following link:

https://github.com/moneroexamples/monero-compilation/blob/master/README.md

##### Compile and run the explorer

Once the Monero compiles, the explorer can be downloaded and compiled
as follows:

```bash
# go to home folder if still in ~/monero
cd ~

# download the source code 
git clone https://github.com/moneroexamples/onion-monero-blockchain-explorer.git

# enter the downloaded sourced code folder
cd onion-monero-blockchain-explorer

# make a build folder and enter it
mkdir build && cd build

# create the makefile
cmake ..

# compile
make
```


To run it:
```
./xmrblocks
```

By default it will look for blockchain in its default location i.e., `~/.bitmonero/lmdb`.
You can use `-b` option if its in different location.

For example:

```bash
./xmrblocks -b /home/mwo/non-default-monero-location/lmdb/
```

Example output:

```bash
[mwo@arch onion-monero-blockchain-explorer]$ ./xmrblocks
2016-May-28 10:04:49.160280 Blockchain initialized. last block: 1056761, d0.h0.m12.s47 time ago, current difficulty: 1517857750
(2016-05-28 02:04:49) [INFO    ] Crow/0.1 server is running, local port 8081
```

Go to your browser: http://127.0.0.1:8081

## Compiling and running with Docker

The explorer can also be compiled using `docker build` as described below. By default it compiles
against latest release tag (i.e. `v0.18.4.0`) of monero:

```
# build using all CPU cores
docker build --no-cache -t xmrblocks .

# alternatively, specify number of cores to use (e.g. 2)
docker build --no-cache --build-arg NPROC=2  -t xmrblocks .

# to build against development branch of monero (i.e. master branch)
docker build --no-cache --build-arg NPROC=3 --build-arg MONERO_BRANCH=master  -t xmrblocks .
```

- The build needs 3 GB space.
- The final container image is 179MB.

To run it, mount the monero blockchain onto the container as volume.

```
# either run in foreground
docker run -it -v <path-to-monero-blockckain-on-the-host>:/home/monero/.bitmonero -p 127.0.0.1:8081:8081  xmrblocks

# or in background
docker run -it -d -v <path-to-monero-blockchain-on-the-host>:/home/monero/.bitmonero -p 127.0.0.1:8081:8081  xmrblocks
```

Example output:

```
docker run --rm -it -v /mnt/w7/bitmonero:/home/monero/.bitmonero -p 127.0.0.1:8081:8081 xmrblocks
Staring in non-ssl mode
(2020-04-20 16:20:00) [INFO    ] Crow/0.1 server is running at 0.0.0.0:8081 using 1 threads
```

### Docker Compose example

The explorer can also be built and run using Docker Compose, i.e.:

```yaml
version: '3'
services:
  monerod:
    image: ghcr.io/sethforprivacy/simple-monerod:latest
    restart: unless-stopped
    container_name: monerod
    volumes:
      - xmrdata:/home/monero/.bitmonero
    ports:
      - 18080:18080
      - 18089:18089
    depends_on:
        - explore
    command:
      - "--rpc-restricted-bind-ip=0.0.0.0"
      - "--rpc-restricted-bind-port=18089"
      - "--public-node"
      - "--no-igd"
      - "--enable-dns-blocklist"

  explore:
    image: xmrblocks:latest
    build: ./onion-monero-blockchain-explorer
    container_name: explore
    restart: unless-stopped
    volumes:
      - xmrdata:/home/monero/.bitmonero
    ports:
      - "127.0.0.1:8081:8081"
    command: ["./xmrblocks --bindaddr 0.0.0.0 --daemon-url=monerod:18089 --enable-json-api --enable-autorefresh-option --enable-emission-monitor --enable-pusher"]

volumes:
  xmrdata:
```

To build this image, run the following:

```bash
git clone https://github.com/moneroexamples/onion-monero-blockchain-explorer.git
cd onion-monero-blockchain-explorer
docker-compose build
```

Or build and run in one step via:

```bash
git clone https://github.com/moneroexamples/onion-monero-blockchain-explorer.git
cd onion-monero-blockchain-explorer
docker-compose up -d
```

When running via Docker, please use something like [Traefik](https://doc.traefik.io/traefik/) or [enable SSL](#enable-ssl-https) to secure communications.


## Deployment security

The HTTP process binds to loopback by default. Put it behind an HTTPS reverse
proxy with request timeouts, rate limits and a body limit. The server rejects
bodies above 4 MiB while reading them. In a container bind to all interfaces
inside the container and publish the host port on loopback.

Create and verify payment proofs in the local wallet with get_tx_proof and
check_tx_proof. The explorer accepts public blocks and transactions. Remove the
legacy wallet-inspection paths from access logs at the proxy, including logs
for rejected requests. Treat existing logs of those paths as sensitive data.
The old private-key checker flags are accepted for deployment compatibility
and produce no routes.

## The explorer's command line options

```
xmrblocks, Onion Monero Blockchain Explorer:
  -h [ --help ] [=arg(=1)] (=0)         produce help message
  -t [ --testnet ] [=arg(=1)] (=0)      use testnet blockchain
  -s [ --stagenet ] [=arg(=1)] (=0)     use stagenet blockchain
  --enable-pusher [=arg(=1)] (=0)       enable signed transaction pusher
  --enable-mixin-details [=arg(=1)] (=0)
                                        enable mixin details for key images,
                                        e.g., timescale, mixin of mixins, in tx
                                        context
  --enable-json-api [=arg(=1)] (=0)     enable JSON REST api
  --enable-as-hex [=arg(=1)] (=0)       enable links to provide hex
                                        represtations of a tx and a block
  --enable-autorefresh-option [=arg(=1)] (=0)
                                        enable users to have the index page on
                                        autorefresh
  --enable-emission-monitor [=arg(=1)] (=0)
                                        enable Monero total emission monitoring
                                        thread
  -p [ --port ] arg (=8081)             default explorer port
  -x [ --bindaddr ] arg (=127.0.0.1)      default bind address for the explorer
  --testnet-url arg                     you can specify testnet url, if you run
                                        it on mainnet or stagenet. link will
                                        show on front page to testnet explorer
  --stagenet-url arg                    you can specify stagenet url, if you
                                        run it on mainnet or testnet. link will
                                        show on front page to stagenet explorer
  --mainnet-url arg                     you can specify mainnet url, if you run
                                        it on testnet or stagenet. link will
                                        show on front page to mainnet explorer
  --no-blocks-on-index arg (=10)        number of last blocks to be shown on
                                        index page
  --mempool-info-timeout arg (=5000)    maximum time, in milliseconds, to wait
                                        for mempool data for the front page
  --mempool-refresh-time arg (=5)       time, in seconds, for each refresh of
                                        mempool state
  -c [ --concurrency ] arg (=0)         number of threads handling http
                                        queries. Default is 0 which means it is
                                        based you on the cpu
  -b [ --bc-path ] arg                  path to lmdb folder of the blockchain,
                                        e.g., ~/.bitmonero/lmdb
  --ssl-crt-file arg                    path to crt file for ssl (https)
                                        functionality
  --ssl-key-file arg                    path to key file for ssl (https)
                                        functionality
  -d [ --daemon-url ] arg (=http:://127.0.0.1:18081)
                                        Monero daemon url
  --daemon-login arg                    Specify username[:password] for daemon 
                                        RPC client
  --enable-mixin-guess [=arg(=1)] (=0)  enable guessing real outputs in key
```

Example usage, defined as bash aliases.

```bash
# for mainnet explorer
alias xmrblocksmainnet='~/onion-monero-blockchain-explorer/build/xmrblocks    --port 8081 --testnet-url "http://139.162.32.245:8082" --enable-pusher --enable-emission-monitor'

# for testnet explorer
alias xmrblockstestnet='~/onion-monero-blockchain-explorer/build/xmrblocks -t --port 8082 --mainnet-url "http://139.162.32.245:8081" --enable-pusher --enable-emission-monitor'
```

Example usage when running via Docker:

```bash
# Run in foreground
docker run -it -v <path-to-monero-blockckain-on-the-host>:/home/monero/.bitmonero -p 127.0.0.1:8081:8081  xmrblocks "./xmrblocks --bindaddr 0.0.0.0 --daemon-url=node.sethforprivacy.com:18089 --enable-json-api --enable-autorefresh-option --enable-emission-monitor --enable-pusher"

# Run in background
docker run -it -d -v <path-to-monero-blockchain-on-the-host>:/home/monero/.bitmonero -p 127.0.0.1:8081:8081  xmrblocks "./xmrblocks --bindaddr 0.0.0.0 --daemon-url=node.sethforprivacy.com:18089 --enable-json-api --enable-autorefresh-option --enable-emission-monitor --enable-pusher"
```

Make sure to always start the portion of command line flags with `./xmrblocks` and set any flags you would like after that, as shown above.

## Enable Monero emission

Obtaining current Monero emission amount is not straight forward. Thus, by default it is
disabled. To enable it use `--enable-emission-monitor` flag, e.g.,


```bash
xmrblocks --enable-emission-monitor
```

This flag will enable emission monitoring thread. When started, the thread
 will initially scan the entire blockchain, and calculate the cumulative emission based on each block.
Since it is a separate thread, the explorer will work as usual during this time.
Every 10000 blocks, the thread will save current emission in a file, by default,
 in `~/.bitmonero/lmdb/emission_amount.txt`. For testnet or stagenet networks,
 it is `~/.bitmonero/testnet/lmdb/emission_amount.txt` or `~/.bitmonero/stagenet/lmdb/emission_amount.txt`. This file is used so that we don't
 need to rescan entire blockchain whenever the explorer is restarted. When the
 explorer restarts, the thread will first check if `~/.bitmonero/lmdb/emission_amount.txt`
 is present, read its values, and continue from there if possible. Subsequently, only the initial
 use of the thread is time consuming. Once the thread scans the entire blockchain, it updates
 the emission amount using new blocks as they come. Since the explorer writes this file, there can
 be only one instance of it running for mainnet, testnet and stagenet. Thus, for example, you can't have
 two explorers for mainnet
 running at the same time, as they will be trying to write and read the same file at the same time,
 leading to unexpected results. Off course having one instance for mainnet and one instance for testnet
 is fine, as they write to different files.

 When the emission monitor is enabled, information about current emission of coinbase and fees is
 displayed on the front page, e.g., :

```
Monero emission (fees) is 14485540.430 (52545.373) as of 1313448 block
```

The values given, can be checked using Monero daemon's  `print_coinbase_tx_sum` command.
For example, for the above example: `print_coinbase_tx_sum 0 1313449`.

To disable the monitor, simply restart the explorer without `--enable-emission-monitor` flag.

## Enable SSL (https)

By default, the explorer does not use ssl. But it has such a functionality.

As an example, you can generate your own ssl certificates as follows:

```bash
cd /tmp # example folder
openssl genrsa -out server.key 1024
openssl req -new -key server.key -out server.csr
openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt
```

Having the `crt` and `key` files, run `xmrblocks` in the following way:

```bash
./xmrblocks --ssl-crt-file=/tmp/server.crt --ssl-key-file=/tmp/server.key
```

Note: Because we generated our own certificate, modern browsers will complain
about it as they can't verify the signatures against any third party. So probably
for any practical use need to have properly issued ssl certificates.

## JSON API

The explorer has JSON api. For the API, it uses conventions defined by [JSend](https://labs.omniti.com/labs/jsend).
By default the api is disabled. To enable it, use `--enable-json-api` flag, e.g.,

```
./xmrblocks --enable-json-api
```

#### api/transaction/<tx_hash>

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/transaction/6093260dbe79fd6277694d14789dc8718f1bd54457df8bab338c2efa3bb0f03d"
```

Partial results shown:

```json
{
  "data": {
    "block_height": 1268252,
    "coinbase": false,
    "confirmations": 1057855,
    "current_height": 2326107,
    "extra": "01be23e277aed6b5f41f66b05244bf994c13108347366ec678ae16657f0fc3a22b",
    "inputs": [
      {
        "amount": 0,
        "key_image": "67838fd0ffd79f13e735830d3ec60412aed59e53e1f997feb6f73d088b949611",
        "mixins": [
          {
            "block_no": 1238623,
            "public_key": "0a5b853c55303c10e1326acfb085b9e246e088b1ccac7e37f7a810d46a28a914",
            "tx_hash": "686555fb053dd53f6f9eb79449e2bdcd377221f823f508158d70d4a1966fe955"
          },
          {
            "block_no": 1246942,
            "public_key": "527cf86f5abbfb006c970f7c6eb40493786d4751306f8985c6a43f98a88c0dff",
            "tx_hash": "4fa1999f9e0d2ad031dbe5594f2e8336651b6cad19dd3cee7980a01c47600f91"
          }
        ]
      }
    ],
    "mixin": 9,
    "outputs": [
      {
        "amount": 0,
        "public_key": "525779873776e4a42f517fd79b72e7c31c3ba03e730fc32287f6414fb702c1d7"
      },
      {
        "amount": 0,
        "public_key": "e25f00fceb77af841d780b68647618812695b4ca6ebe338faba6e077f758ac30"
      }
    ],
    "payment_id": "",
    "payment_id8": "",
    "rct_type": 1,
    "timestamp": 1489753456,
    "timestamp_utc": "2017-03-17 12:24:16",
    "tx_fee": 12517785574,
    "tx_hash": "6093260dbe79fd6277694d14789dc8718f1bd54457df8bab338c2efa3bb0f03d",
    "tx_size": 13323,
    "tx_version": 2,
    "xmr_inputs": 0,
    "xmr_outputs": 0
  },
  "status": "success"
}

```

#### api/transactions

Transactions in last 25 blocks


```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/transactions"
```

Partial results shown:

```json
{
  "data": {
    "blocks": [
      {
        "age": "33:16:49:53",
        "height": 1268252,
        "size": 105390000000000000,
        "timestamp": 1489753456,
        "timestamp_utc": "2017-03-17 12:24:16",
        "txs": [
          {
            "coinbase": true,
            "mixin": 0,
            "outputs": 8491554678365,
            "rct_type": 0,
            "tx_fee": 0,
            "tx_hash": "7c4286f64544568265bb5418df84ae69afaa3567749210e46f8340c247f4803f",
            "tx_size": 151000000000000,
            "tx_version": 2
          },
          {
            "coinbase": false,
            "mixin": 5,
            "outputs": 0,
            "rct_type": 2,
            "tx_fee": 17882516700,
            "tx_hash": "2bfbccb918ee5f050808dd040ce03943b7315b81788e9cdee59cf86b557ba48c",
            "tx_size": 19586000000000000,
            "tx_version": 2
          }
        ]
      }
    ],
    "limit": 25,
    "page": 0
  },
  "status": "success"
}
```

#### api/transactions?page=<page_no>&limit=<tx_per_page>


```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/transactions?page=2&limit=10"
```

Result analogical to the one above.

#### api/block/<block_number|block_hash>


```bash
curl  -w "\n" -X GET "http://139.162.32.245:8081/api/block/1293257"
```

Partial results shown:

```json
{
  "data": {
    "block_height": 1293257,
    "block_reward": 0,
    "current_height": 1293264,
    "hash": "9ef6bb8f9b8bd253fc6390e5c2cdc45c8ee99fad16447437108bf301fe6bd6e1",
    "size": 141244,
    "timestamp": 1492761974,
    "timestamp_utc": "2017-04-21 08:06:14",
    "txs": [
      {
        "coinbase": true,
        "extra": "018ae9560eb85d5ebd22d3beaed55c21d469eab430c5e3cac61b3fe2f5ad156770020800000001a9030800",
        "mixin": 0,
        "payment_id": "",
        "payment_id8": "",
        "rct_type": 0,
        "tx_fee": 0,
        "tx_hash": "3ff71b65bec34c9261e01a856e6a03594cf0472acf6b77db3f17ebd18eaa30bf",
        "tx_size": 95,
        "tx_version": 2,
        "xmr_inputs": 0,
        "xmr_outputs": 8025365394426
      }
    ]
  },
  "status": "success"
}
```

#### api/mempool

Return all txs in the mempool.

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/mempool"
```

Partial results shown:

```json
{
  "data": {
    "limit": 100000000,
    "page": 0,
    "total_page_no": 0,
    "txs": [
      {
        "coinbase": false,
        "extra": "022100325f677d96f94155a4840a84d8e0c905f7a4697a25744633bcb438feb1e51fb2012eda81bf552c53c2168f4130dbe0265c3a7898f3a7eee7c1fed955a778167b5d",
        "mixin": 3,
        "payment_id": "325f677d96f94155a4840a84d8e0c905f7a4697a25744633bcb438feb1e51fb2",
        "payment_id8": "",
        "rct_type": 2,
        "timestamp": 1494470894,
        "timestamp_utc": "2017-05-11 02:48:14",
        "tx_fee": 15894840000,
        "tx_hash": "9f3374f8ac67febaab153eab297937a3d0d2c706601e496bf5028146da0c9aef",
        "tx_size": 13291,
        "tx_version": 2,
        "xmr_inputs": 0,
        "xmr_outputs": 0
      }
    ],
    "txs_no": 7
  },
  "status": "success"
}
```

Limit of 100000000 is just default value above to ensure that all mempool txs are fetched
if no specific limit given.

#### api/mempool?limit=<no_of_top_txs>

Return number of newest mempool txs, e.g., only 10.

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/mempool?limit=10"
```

Result analogical to the one above.

#### api/search/<block_number|tx_hash|block_hash>

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/search/1293669"
```

Partial results shown:

```json
{
  "data": {
    "block_height": 1293669,
    "current_height": 1293670,
    "hash": "5d55b8fabf85b0b4c959d66ad509eb92ddfe5c2b0e84e1760abcb090195c1913",
    "size": 118026,
    "timestamp": 1492815321,
    "timestamp_utc": "2017-04-21 22:55:21",
    "title": "block",
    "txs": [
      {
        "coinbase": true,
        "extra": "01cb7fda09033a5fa06dc601b9295ef3790397cf3c645e958e34cf7ab699d2f5230208000000027f030200",
        "mixin": 0,
        "payment_id": "",
        "payment_id8": "",
        "rct_type": 0,
        "tx_fee": 0,
        "tx_hash": "479ba432f5c88736b438dd4446a11a13046a752d469f7828151f5c5b86be4e9a",
        "tx_size": 95,
        "tx_version": 2,
        "xmr_inputs": 0,
        "xmr_outputs": 7992697599717
      }
    ]
  },
  "status": "success"
}
```


#### api/networkinfo

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/networkinfo"
```

```json
{
  "data": {
    "alt_blocks_count": 0,
    "block_size_limit": 600000,
    "cumulative_difficulty": 2091549555696348,
    "difficulty": 7941560081,
    "fee_per_kb": 303970000,
    "grey_peerlist_size": 4991,
    "hash_rate": 66179667,
    "height": 1310423,
    "incoming_connections_count": 0,
    "outgoing_connections_count": 5,
    "start_time": 1494822692,
    "status": "OK",
    "target": 120,
    "target_height": 0,
    "testnet": false,
    "top_block_hash": "76f9e85d62415312758bc09e0b9b48fd2b005231ad1eee435a8081e551203f82",
    "tx_count": 1219048,
    "tx_pool_size": 2,
    "white_peerlist_size": 1000
  },
  "status": "success"
}
```

#### api/emission

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/emission"
```

```json
{
  "data": {
    "blk_no": 1313969,
    "coinbase": 14489473877253413000,
    "fee": 52601974988641130
  },
  "status": "success"
}
```

Emission only works when the emission monitoring thread is enabled.

#### api/version

```bash
curl  -w "\n" -X GET "http://127.0.0.1:8081/api/version"
```

```json
{
  "data": {
    "api": 65536,
    "blockchain_height": 1357031,
    "git_branch_name": "update_to_current_monero",
    "last_git_commit_date": "2017-07-25",
    "last_git_commit_hash": "a549f25",
    "monero_version_full": "0.10.3.1-ab594cfe"
  },
  "status": "success"
}
```

api number is store as `uint32_t`. In this case `65536` represents
major version 1 and minor version 0.
In JavaScript to get these numbers, one can do as follows:

```javascript
var api_major = response.data.api >> 16;
var api_minor = response.data.api & 0xffff;
```

#### api/rawblock/<block_number|block_hash>

Return raw json block data, as represented in Monero.

```bash
curl  -w "\n" -X GET "http://139.162.32.245:8081/api/rawblock/1293257"
```

Example result not shown.

#### api/rawtransaction/<tx_hash>

Return raw json tx data, as represented in Monero.

```bash
curl  -w "\n" -X GET "http://139.162.32.245:8081/api/rawtransaction/6093260dbe79fd6277694d14789dc8718f1bd54457df8bab338c2efa3bb0f03d"
```

Example result not shown.

## Other monero examples

Other examples can be found on  [github](https://github.com/moneroexamples?tab=repositories).
Please know that some of the examples/repositories are not
finished and may not work as intended.

## How can you help?

Constructive criticism, code and website edits are always good. They can be made through github.

### Legacy wallet-route denylist at the reverse proxy

Before switching the binary and templates together, add this block to each
existing Nginx explorer server (Mainnet and Testnet), before any general
regex proxy locations:

    location ~ ^/(?:myoutputs|prove|rawkeyimgs|checkrawkeyimgs|rawoutputkeys|checkrawoutputkeys|api/outputs|api/outputsblocks)(?:/|$) {
        access_log off;
        return 404;
    }

Check that no competing exact or ^~ location takes precedence. Keep the
explorer bound to 127.0.0.1; publish container ports on loopback as above.
Run nginx -t before an authorized reload. Test each exact path and a child
path with GET and POST; all must return 404 without an access-log entry.
Do not use real keys in these requests. Review any additional load-balancer
or CDN logging separately; an Nginx rule cannot suppress their logs.

This is a deployment procedure, not a request to expose or migrate production
services automatically. Existing logs of legacy wallet routes are sensitive
and must not be attached to bug reports or pull requests.
